<?php

/**
 * @file
 * The PHP documentation parser that generates content for api.module.
 */

/**
 * Reads in a file and calls a callback function to parse and save it.
 *
 * @param $callback
 *   Name of the function to call to parse and save the file contents, such as
 *   api_parse_php_file(), api_parse_text_file(), or api_parse_html_file()
 *   (pass the function name as a string).
 * @param $file_path
 *   Full path to the file to read in.
 * @param $branch
 *   Object representing the branch to assign the file contents to.
 * @param $file_name
 *   File name to store in the database for this file.
 */
function api_parse_file($callback, $file_path, $branch, $file_name) {
  $basename = strpos($file_name, '/') ? substr($file_name, strrpos($file_name, '/') + 1) : $file_name;
  $docblock = array(
    'object_name' => $file_name,
    'branch' => $branch,
    'object_type' => 'file',
    'file_name' => $file_name,
    'title' => $basename,
    'basename' => $basename,
    'documentation' => '',
    'references' => array(),
    'modified' => filemtime($file_path),
    'queued' => 0,
    'source' => str_replace(array("\r\n", "\r"), array("\n", "\n"), file_get_contents($file_path)),
    'content' => '',
    'class' => '',
  );

  $callback($docblock);
}

/**
 * Saves contents of a file as a single piece of text documentation.
 *
 * Callback for api_parse_file().
 *
 * @param $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_text_file($docblock) {
  $docblock['documentation'] = api_format_documentation($docblock['source'], FALSE);
  // We don't actually want to keep track of references for text files.
  $references = array();
  $docblock['code'] = api_format_php($docblock['source'], $references, TRUE, FALSE);

  api_save_documentation(array($docblock));
}

/**
 * Saves contents of a file as a single piece of HTML documentation.
 *
 * Escapes any HTML special characters in the text, so that it can be
 * displayed to show the HTML tags.
 *
 * Callback for api_parse_file().
 *
 * @param $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_html_file($docblock) {
  $docblock['code'] = '<pre>' . htmlspecialchars($docblock['source']) . '</pre>';
  $title_match = array();
  if (preg_match('!<title>([^<]+)</title>!is', $docblock['source'], $title_match)) {
    $docblock['title'] = trim($title_match[1]);
    $docblock['summary'] = $docblock['title'];
  }
  $documentation_match = array();
  if (preg_match('!<body>(.*?</h1>)?(.*)</body>!is', $docblock['source'], $documentation_match)) {
    $docblock['documentation'] = $documentation_match[2];
  }

  api_save_documentation(array($docblock));
}

/**
 * Parses a PHP file and saves the file and its contents as documentation.
 *
 * PHP functions, classes, global variables, constants, etc. in the file
 * are saved as documentation, if they have docblocks.
 *
 * Callback for api_parse_file().
 *
 * @param $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_php_file($docblock) {
  if (!api_libraries_loaded()) {
    return;
  }

  // Build grammar statements.
  $editor = PGPEditor::getInstance();
  $reader = $editor->getReader();
  $reader->setSnippet($docblock['source']);
  $reader->addTokenNames();
  $reader->buildGrammar();

  // Retrieve items of interest.
  $statements = $reader->getStatements();
  if (!$statements) {
    // This is a text file or template file with no declarations of functions,
    // constants, etc. But we do want to track references in it.
    $docblock['code'] = api_format_php($docblock['source'], $docblock['references'], TRUE);

    api_save_documentation(array($docblock));
    // Free up memory.
    $reader->reset();
    return;
  }

  // Reserve the first array slot for the file documentation block. Note that
  // we might or might not keep the references we parsed out.
  $references = array();
  $docblock['code'] = api_format_php($docblock['source'], $references, TRUE);
  $docblocks = array($docblock);

  // Set default documentation block array for items other than the file.
  $default_block = array(
    'branch' => $docblock['branch'],
    'file_name' => $docblock['file_name'],
    'class' => '',
    'object_type' => '',
    'documentation' => '',
    'references' => array(),
    'see' => '',
  );

  api_documentation_loop($statements, $default_block, $docblocks);

  // Free up memory.
  $reader->reset();

  // If this file only contains a docblock for itself, plus maybe one @file
  // block, then any references we found above should be saved for the file.
  // This is usually the case for theme templates.
  if (count($references) > 0 &&
    (count($docblocks) == 1 ||
      (count($docblocks) == 2 && $docblocks[1]['object_type'] == '' && strpos($docblocks[1]['content'], '@file') !== FALSE))) {
    $docblocks[0]['references'] = $references;
  }

  api_save_documentation($docblocks);
}

/**
 * Builds a list of documentation items.
 *
 * @param PGPBody $statements
 *   A PGPBody object containing body statements.
 * @param array $default_block
 *   The default documentation block to use to build documentation items.
 * @param array $docblocks
 *   The array of documentation block items. Documentation items
 *   are added to the end of this array.
 */
function api_documentation_loop($statements, $default_block, &$docblocks) {
  if (!api_libraries_loaded()) {
    return;
  }

  static $object_types = array(
    T_ASSIGNMENT => 'property',
    T_CONST => 'constant',
  );
  $editor = PGPEditor::getInstance();

  // Traverse statement list to gather documentation items.
  $current = $statements->first();
  while ($current != NULL) {
    $statement = $current->data;
    $type = is_object($statement) ? $statement->type : $statement['type'];

    // Common processing.
    switch ($type) {
      case T_ASSIGNMENT: // Class property
        if (empty($default_block['class'])) {
          break;
        } // Explicit fallthrough.
      case T_INTERFACE:
      case T_CLASS:
      case T_FUNCTION:
      case T_DEFINE:
      case T_CONST:
      case T_GLOBAL:
        $docblock = $default_block;
        $docblock['object_type'] = isset($object_types[(int) $type]) ? $object_types[(int) $type] : $editor->statementTypeToString($statement);

        $class_prefix = empty($default_block['class']) ? '' : $default_block['class'] . '::';
        if ($type == T_GLOBAL || $type == T_ASSIGNMENT) {
          $docblock['member_name'] = preg_replace('/^\$/', '', $editor->statementOperandToText($statement));
        }
        else {
          $docblock['member_name'] = $editor->statementOperandToText($statement);
        }
        $docblock['object_name'] = $class_prefix . $docblock['member_name'];
        $docblock['title'] = $class_prefix . $editor->statementOperandToText($statement);
        $docblock['start_line'] = $current->line;
        $docblock['content'] = $editor->commentToString($statement->comment);
        $statement->comment = '';
        $references = array();
        $docblock['code'] = api_format_php("<?php\n" . $statement->toString() . "\n?" . ">", $references, FALSE, ($statement->type == T_FUNCTION));
        if ($statement->type == T_FUNCTION) {
          $docblock['signature'] = $editor->functionGetSignature($statement);
          $docblock['references'] = $references;
        }
        elseif ($statement->type == T_CLASS || $statement->type == T_INTERFACE) {
          $docblock['extends'] = $statement->extends;
          $docblock['implements'] = $statement->implements;
        }
        $docblocks[] = $docblock;
        break;

      case T_DOC_COMMENT:
        $docblock = $default_block;
        $docblock['content'] = $editor->commentToString($statement);
        $docblock['start_line'] = $current->line;
        $docblocks[] = $docblock;
        break;
    }

    // Additional recursive processing on statements with bodies.
    switch ($type) {
      case T_INTERFACE:
      case T_CLASS:
        if (!empty($statement->body)) {
          api_documentation_loop($statement->body, array_merge($default_block, array('class' => $docblock['object_name'])), $docblocks);
        }
        break;
    }
    $current = $current->next;
  }
}

/**
 * Saves documentation information to the database.
 *
 * @param $docblocks
 *   An array containing information about API documentation items. The first
 *   array element should be for the file as a whole, and subsequent elements
 *   should have information for items declared in that file (if any).
 */
function api_save_documentation($docblocks) {
  $nested_groups = array();

  // Make a list of the documentation IDs that were contained in this file
  // the last time through. We'll remove any items that are not in this file
  // any more after we get through the $docblocks array.
  $old_dids = db_select('api_documentation', 'd')
    ->fields('d', array('did'))
    ->condition('branch_id', $docblocks[0]['branch']->branch_id)
    ->condition('file_name', $docblocks[0]['file_name'])
    ->execute()
    ->fetchCol();

  $dids = array();
  $class_dids = array();
  // Look for @file block first so $docblocks[0] gets filled in before it is processed.
  foreach ($docblocks as $docblock) {
    if (preg_match('/' . API_RE_TAG_START . 'file/', $docblock['content'])) {
      $docblocks[0]['content'] = str_replace('@file', '', $docblock['content']);
      break;
    }
  }

  foreach ($docblocks as $docblock) {
    if (preg_match('/' . API_RE_TAG_START . 'mainpage/', $docblock['content'])) {
      preg_match('/' . API_RE_TAG_START . 'mainpage (.*?)\n/', $docblock['content'], $mainpage_matches);
      $docblock['title'] = $mainpage_matches[1];
      $docblock['content'] = preg_replace('/' . API_RE_TAG_START . 'mainpage.*?\n/', '', $docblock['content']);
      $docblock['object_type'] = 'mainpage';
      $docblock['object_name'] = $docblocks[0]['branch']->branch_name;
    }
    elseif (preg_match('/' . API_RE_TAG_START . 'defgroup/', $docblock['content'])) {
      if (preg_match('/' . API_RE_TAG_START . 'defgroup ([a-zA-Z0-9_.-]+) +(.*?)\n/', $docblock['content'], $group_matches)) {
        $group_name = $group_matches[1];
        // See if the group already exists in another file in this branch, and
        // if so, treat this as an @addtogroup.
        $did = db_select('api_documentation', 'd')
          ->condition('object_name', $group_name)
          ->condition('file_name', $docblock['file_name'], '<>')
          ->condition('branch_id', $docblocks[0]['branch']->branch_id)
          ->fields('d', array('did'))
          ->execute()
          ->fetchField();
        if ($did > 0) {
          $docblock['content'] = str_replace('defgroup', 'addtogroup', $docblock['content']);
          watchdog('api', 'Duplicate @defgroup in %file at line %line treated as @addtogroup.', array('%file' => $docblocks[0]['file_name'], '%line' => $docblock['start_line']), WATCHDOG_WARNING);

        }
        else {
          $docblock['object_name'] = $group_name;
          $docblock['title'] = $group_matches[2];
          $docblock['content'] = preg_replace('/' . API_RE_TAG_START . 'defgroup.*?\n/', '', $docblock['content']);
          $docblock['object_type'] = 'group';
        }
      }
      else {
        watchdog('api', 'Malformed @defgroup in %file at line %line.', array('%file' => $docblocks[0]['file_name'], '%line' => $docblock['start_line']), WATCHDOG_WARNING);
      }
    }

    // Determine group membership.
    $match = array();
    if (preg_match_all('/' . API_RE_TAG_START . '(ingroup|addtogroup) ([a-zA-Z0-9_.-]+)/', $docblock['content'], $match)) {
      $docblock['groups'] = $match[2];
      $docblock['content'] = preg_replace('/' . API_RE_TAG_START . '(ingroup|addtogroup).*?\n/', '', $docblock['content']);
    }

    // Handle nested groups.
    if (!isset($nested_groups[$docblock['class']])) {
      $nested_groups[$docblock['class']] = array();
    }
    foreach ($nested_groups[$docblock['class']] as $group_id) {
      if (!empty($group_id)) {
        $docblock['groups'][] = $group_id;
      }
    }
    if (preg_match('/' . API_RE_TAG_START . '{/', $docblock['content'])) {
      if ($docblock['object_type'] === 'group') {
        array_push($nested_groups[$docblock['class']], $docblock['object_name']);
      }
      elseif (isset($docblock['groups'])) {
        array_push($nested_groups[$docblock['class']], reset($docblock['groups']));
      }
      else {
        array_push($nested_groups[$docblock['class']], '');
      }
    }
    if (preg_match('/' . API_RE_TAG_START . '}/', $docblock['content'])) {
      array_pop($nested_groups[$docblock['class']]);
    }

    if (empty($docblock['object_type'])) {
      continue;
    }

    if (!empty($docblock['content'])) {
      // Find parameter definitions.
      $offset = 0;
      $params = '';
      while (preg_match('/' . API_RE_TAG_START . 'param\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', substr($docblock['content'], $offset), $param_match, PREG_OFFSET_CAPTURE)) {
        $docblock['content'] = str_replace($param_match[0][0], '', $docblock['content']);
        // Add some formatting to the parameter -- strong tag for everything
        // that was on the @param line, and a colon after. Note that tags
        // are stripped out below, so we use [strong] and then fix it later.
        $this_param = $param_match[1][0];
        $this_param = preg_replace('|^([^\n]+)|', '[strong]$1[/strong]:', $this_param);
        $params .= "\n\n" . $this_param;
        $offset = $param_match[0][1];
      }
      // Format and then replace our fake tags with real ones.
      $params = api_format_documentation($params);
      $params = str_replace('[strong]', '<strong>', $params);
      $params = str_replace('[/strong]', '</strong>', $params);
      $docblock['parameters'] = $params;

      // Find return value definitions.
      $docblock['return_value'] = '';
      preg_match_all('/' . API_RE_TAG_START . 'return\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $return_matches, PREG_SET_ORDER);
      foreach ($return_matches as $return_match) {
        $docblock['content'] = str_replace($return_match[0], '', $docblock['content']);
        $docblock['return_value'] .= "\n\n" . $return_match[1];
      }
      $docblock['return_value'] = api_format_documentation($docblock['return_value']);

      // Find @see lines.
      $offset = 0;
      $docblock['see'] = '';
      while (preg_match('/' . API_RE_TAG_START . 'see\s(.*?)\n/s', substr($docblock['content'], $offset), $match, PREG_OFFSET_CAPTURE)) {
        $docblock['content'] = str_replace($match[0][0], '', $docblock['content']);
        $docblock['see'] .= "\n\n" . $match[1][0];
        $offset = $match[0][1];
      }
      $docblock['see'] = api_format_documentation($docblock['see']);

      // Find @var, a class or variable type name.
      $docblock['var'] = '';
      if (preg_match('/' . API_RE_TAG_START . 'var\s(.*?)\n/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['var'] = trim($match[1]);
      }

      // Find @throws, a paragraph
      $docblock['throws'] = '';
      if (preg_match('/' . API_RE_TAG_START . 'throws\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['throws'] = api_format_documentation($match[1]);
      }

      // Format everything remaining as the main documentation.
      $docblock['documentation'] = api_format_documentation($docblock['content']);
    }
    // Grab the first line as a summary, unless already provided.
    if (!isset($docblock['summary'])) {
      $docblock['summary'] = api_documentation_summary($docblock['documentation']);
    }

    if (!empty($docblock['class'])) {
      $docblock['class_did'] = $class_dids[$docblock['class']];
    }

    // See if this docblock already existed, and get its ID.
    $docblock['did'] = db_select('api_documentation', 'd')
      ->fields('d', array('did'))
      ->condition('object_name', $docblock['object_name'])
      ->condition('file_name', $docblock['file_name'])
      ->condition('object_type', $docblock['object_type'])
      ->condition('branch_id', $docblocks[0]['branch']->branch_id)
      ->execute()
      ->fetchField();

    if ($docblock['did'] > 0) {
      _api_add_text_defaults($docblock, 'api_documentation');
      drupal_write_record('api_documentation', $docblock, 'did');
    }
    else {
      $nid = api_new_documentation_node();
      $docblock['did'] = $nid;
      $docblock['branch_id'] = $docblocks[0]['branch']->branch_id;
      _api_add_text_defaults($docblock, 'api_documentation');
      drupal_write_record('api_documentation', $docblock);
    }

    // Keep track of class membership
    if ($docblock['object_type'] === 'class' || $docblock['object_type'] === 'interface') {
      $class_dids[$docblock['object_name']] = $docblock['did'];
    }

    // Delete all overrides information we had for this item. We will compute
    // it again in api_shutdown().
    db_delete('api_overrides')
      ->condition(db_or()
        ->condition('did', $docblock['did'])
        ->condition('overrides_did', $docblock['did'])
      )
      ->execute();

    switch ($docblock['object_type']) {
      case 'function':
        db_delete('api_function')
          ->condition('did', $docblock['did'])
          ->execute();
        _api_add_text_defaults($docblock, 'api_function');
        drupal_write_record('api_function', $docblock);

        api_replace_references($docblocks[0]['branch'], $docblock['did'], $docblock['object_name'], $docblock['references']);
        break;

      case 'file':
        // Note that this will set the 'queued' field back to zero, since
        // $docblock['queued'] is set to 0 up at the top of api_parse_file().
        drupal_write_record('api_file', $docblock, 'did');
        api_replace_references($docblock['branch'], $docblock['did'], $docblock['object_name'], $docblock['references']);
        break;

      case 'interface':
      case 'class':
        db_delete('api_reference_storage')
          ->condition('branch_id', $docblocks[0]['branch']->branch_id)
          ->condition('object_type', array('class', 'interface'))
          ->condition('from_did', $docblock['did'])
          ->execute();
        foreach ($docblock['extends'] as $extend) {
          api_reference($docblocks[0]['branch'], 'class', $extend, $docblock['did']);
        }
        foreach ($docblock['implements'] as $implement) {
          api_reference($docblocks[0]['branch'], 'interface', $implement, $docblock['did']);
        }
        break;
    }

    db_delete('api_reference_storage')
      ->condition('branch_id', $docblocks[0]['branch']->branch_id)
      ->condition('object_type', 'group')
      ->condition('from_did', $docblock['did'])
      ->execute();

    if (isset($docblock['groups'])) {
      foreach ($docblock['groups'] as $group_name) {
        api_reference($docblocks[0]['branch'], 'group', $group_name, $docblock['did']);
      }
    }

    $dids[] = $docblock['did'];
  }

  // Clean out all of the doc objects from this file that no longer exist.
  $old_dids = array_diff($old_dids, $dids);
  if (count($old_dids) > 0) {
    db_delete('api_documentation')
      ->condition('did', $old_dids)
      ->execute();
    db_delete('api_file')
      ->condition('did', $old_dids)
      ->execute();
    db_delete('api_function')
      ->condition('did', $old_dids)
      ->execute();
    db_delete('api_reference_storage')
      ->condition('from_did', $old_dids)
      ->execute();
    db_delete('api_overrides')
      ->condition(db_or()
        ->condition('did', $old_dids)
        ->condition('overrides_did', $old_dids)
      )
      ->execute();
  }

  api_schedule_shutdown();
}

/**
 * Saves PHP branch documentation information to the database.
 *
 * @param $docblocks
 *   An array containing information about functions found in the branch.
 */
function api_save_php_documentation($docblocks) {
  // Make a list of the PHP functions we found last time. We'll remove any that
  // were not here this time at the end.
  $old_dids = db_select('api_php_documentation', 'd')
    ->fields('d', array('did'))
    ->condition('branch_id', $docblocks[0]['branch_id'])
    ->execute()
    ->fetchCol();

  $dids = array();

  foreach ($docblocks as $docblock) {
    // See if this docblock already existed, and get its ID.
    $docblock['did'] = db_select('api_php_documentation', 'd')
      ->fields('d', array('did'))
      ->condition('object_name', $docblock['object_name'])
      ->condition('object_type', $docblock['object_type'])
      ->condition('branch_id', $docblock['branch_id'])
      ->execute()
      ->fetchField();
    if ($docblock['did'] > 0) {
      _api_add_text_defaults($docblock, 'api_php_documentation');
      drupal_write_record('api_php_documentation', $docblock, 'did');
    }
    else {
      $docblock['did'] = api_new_php_doc_id();
      _api_add_text_defaults($docblock, 'api_php_documentation');
      drupal_write_record('api_php_documentation', $docblock);
    }

    $dids[] = $docblock['did'];
  }

  // Clean out functions we didn't find this time.
  $old_dids = array_diff($old_dids, $dids);
  if (count($old_dids) > 0) {
    db_delete('api_php_documentation')
      ->condition('did', $old_dids)
      ->execute();
  }
}

/**
 * Returns a new ID for a PHP doc item.
 */
function api_new_php_doc_id() {
  $select = db_select('api_php_documentation', 'p');
  $select->addExpression('MAX(did)', 'a');
  $ids = $select->groupBy('branch_id')
    ->execute()
    ->fetchCol();

  if (count($ids)) {
    return max($ids) + 1;
  }

  return 1;
}

/**
 * Replaces any existing references for function/hook calls with new ones.
 *
 * @param $branch
 *   Branch object for the item making the calls.
 * @param $did
 *   Documentation ID of the item making the calls.
 * @param $name
 *   Name of the item making the calls.
 * @param $references
 *   Array of references set up by api_format_php().
 */
function api_replace_references($branch, $did, $name, $references) {
  // Find the name without class prefix, if any.
  $shortname = $name;
  if (($pos = strpos($name, '::')) !== FALSE && $pos > 1) {
    $shortname = substr($name, $pos + 2);
  }

  // Remove any existing references.
  db_delete('api_reference_storage')
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', array('function', 'potential hook', 'potential theme', 'potential alter', 'potential callback', 'member-parent', 'member-self', 'member', 'member-class'))
    ->condition('from_did', $did)
    ->execute();

  // Add the new references.
  foreach ($references as $type => $list) {
    // Don't save a reference to the item itself.
    if ($type == 'function' || $type == 'potential callback' || $type == 'member-self') {
      unset($list[$shortname]);
    }
    foreach ($list as $call) {
      api_reference($branch, $type, $call, $did);
    }
  }
}

/**
 * Formats a documentation block as HTML.
 *
 * First escapes all HTML tags. Then processes links and code blocks, and
 * converts newlines into paragraphs.
 *
 * @param $documentation
 *   Documentation string to format.
 * @param $auto_p
 *   TRUE (default) to convert to paragraphs. FALSE to skip this conversion and
 *   put the documentation in PRE tags.
 *
 * @return
 *   Formatted documentation.
 */
function api_format_documentation($documentation, $auto_p = TRUE) {
  // Don't do processing on empty text (so we don't end up with empty paragraphs).
  if (empty($documentation)) {
    return '';
  }

  $documentation = check_plain($documentation);

  // @link full URLs.
  $documentation = preg_replace('/' . API_RE_TAG_START . 'link ((http:\/\/|https:\/\/|ftp:\/\/|mailto:|smb:\/\/|afp:\/\/|file:\/\/|gopher:\/\/|news:\/\/|ssl:\/\/|sslv2:\/\/|sslv3:\/\/|tls:\/\/|tcp:\/\/|udp:\/\/)([a-zA-Z0-9@:%_+*~#?&=.,\/;-]*[a-zA-Z0-9@:%_+*~#&=\/;-])) (.*?) ' . API_RE_TAG_START . 'endlink/', '<a href="$1">$4</a>', $documentation);
  // Site URLs.
  $documentation = preg_replace('/' . API_RE_TAG_START . 'link \/([a-zA-Z0-9_\/-]+) (.*?) ' . API_RE_TAG_START . 'endlink/', str_replace('%24', '$', l('$2', '$1')), $documentation);

  // Replace left over curly braces
  $documentation = preg_replace('/' . API_RE_TAG_START . '[{}]/', '', $documentation);

  // Process the @code @endcode tags.
  $documentation = preg_replace_callback('/' . API_RE_TAG_START . 'code(.+?)' . API_RE_TAG_START . 'endcode/s', 'api_format_embedded_php', $documentation);

  // Convert newlines into paragraphs.
  if ($auto_p) {
    $documentation = api_autop($documentation);
  }
  else {
    $documentation = '<pre class="api-text">' . $documentation . '</pre>';
  }

  return $documentation;
}

/**
 * Converts newlines into paragraphs.
 *
 * Like _filter_autop(), but does not add <br /> tags.
 *
 * @param $text
 *   Text to convert.
 *
 * @return
 *   Converted text.
 */
function api_autop($text) {
  // All block level tags
  $block = '(?:table|thead|tfoot|caption|colgroup|tbody|tr|td|th|div|dl|dd|dt|ul|ol|li|pre|select|form|blockquote|address|p|h[1-6]|hr)';

  // Split at <pre>, <script>, <style> and </pre>, </script>, </style> tags.
  // We don't apply any processing to the contents of these tags to avoid messing
  // up code. We look for matched pairs and allow basic nesting. For example:
  // "processed <pre> ignored <script> ignored </script> ignored </pre> processed"
  $chunks = preg_split('@(</?(?:pre|script|style|object)[^>]*>)@i', $text, -1, PREG_SPLIT_DELIM_CAPTURE);
  // Note: PHP ensures the array consists of alternating delimiters and literals
  // and begins and ends with a literal (inserting NULL as required).
  $ignore = FALSE;
  $ignoretag = '';
  $output = '';
  foreach ($chunks as $i => $chunk) {
    if ($i % 2) {
      // Opening or closing tag?
      $open = ($chunk[1] != '/');
      list($tag) = preg_split('/[ >]/', substr($chunk, 2 - $open), 2);
      if (!$ignore) {
        if ($open) {
          $ignore = TRUE;
          $ignoretag = $tag;
        }
      }
      // Only allow a matching tag to close it.
      elseif (!$open && $ignoretag == $tag) {
        $ignore = FALSE;
        $ignoretag = '';
      }
    }
    elseif (!$ignore) {
      $chunk = api_format_documentation_lists($chunk);
      $chunk = preg_replace('|\n*$|', '', $chunk) . "\n\n"; // just to make things a little easier, pad the end
      $chunk = preg_replace('|<br />\s*<br />|', "\n\n", $chunk);
      $chunk = preg_replace('!(<' . $block . '[^>]*>)!', "\n$1", $chunk); // Space things out a little
      $chunk = preg_replace('!(</' . $block . '>)!', "$1\n\n", $chunk); // Space things out a little
      $chunk = preg_replace("/\n\n+/", "\n\n", $chunk); // take care of duplicates
      $chunk = preg_replace('/\n?(.+?)(?:\n\s*\n|\z)/s', "<p>$1</p>\n", $chunk); // make paragraphs, including one at the end
      $chunk = preg_replace('|<p>\s*</p>\n|', '', $chunk); // under certain strange conditions it could create a P of entirely whitespace
      $chunk = preg_replace("|<p>(<li.+?)</p>|", "$1", $chunk); // problem with nested lists
      $chunk = preg_replace('|<p><blockquote([^>]*)>|i', "<blockquote$1><p>", $chunk);
      $chunk = str_replace('</blockquote></p>', '</p></blockquote>', $chunk);
      $chunk = preg_replace('!<p>\s*(</?' . $block . '[^>]*>)!', "$1", $chunk);
      $chunk = preg_replace('!(</?' . $block . '[^>]*>)\s*</p>!', "$1", $chunk);
      $chunk = preg_replace('/&([^#])(?![A-Za-z0-9]{1,8};)/', '&amp;$1', $chunk);
    }
    $output .= $chunk;
  }
  return $output;
}

/**
 * Regular expression callback for \@code tags in api_format_documentation().
 */
function api_format_embedded_php($matches) {
  $references = array();
  return "\n" . api_format_php("<?php" . decode_entities($matches[1]) . "?" . ">", $references, FALSE, FALSE) . "\n";
}

/**
 * Formats documentation lists as HTML lists.
 *
 * Parses a block of text for lists that uses hyphens or asterisks as bullets,
 * and format the lists as proper HTML lists.
 *
 * @param $documentation
 *   Documentation string to format.
 *
 * @return
 *   $documentation with lists formatted.
 */
function api_format_documentation_lists($documentation) {
  $lines = explode("\n", $documentation);
  $output = '';
  $bullet_indents = array(-1);

  foreach ($lines as $line) {
    preg_match('!^( *)([*-] )?(.*)$!', $line, $matches);
    $indent = strlen($matches[1]);
    $bullet_exists = $matches[2];
    $is_start = FALSE;

    if ($indent < $bullet_indents[0]) {
      // First close off any lists that have completed.
      while ($indent < $bullet_indents[0]) {
        array_shift($bullet_indents);
        $output .= '</li></ul>';
      }
    }

    if ($indent == $bullet_indents[0]) {
      if ($bullet_exists) {
        // A new bullet at the same indent means a new list item.
        $output .= '</li><li>';
        $is_start = TRUE;
      }
      else {
        // If the indent is the same, but there is no bullet, that also
        // signifies the end of the list.
        array_shift($bullet_indents);
        $output .= '</li></ul>';
      }
    }

    if ($indent > $bullet_indents[0] && $bullet_exists) {
      // A new list at a lower level.
      array_unshift($bullet_indents, $indent);
      $output .= '<ul><li>';
      $is_start = TRUE;
    }

    // At the start of a bullet, if there is a ":" followed by a space, put
    // everything before the : in bold.
    if ($is_start && (($p = strpos($matches[3], ': ')) > 0)) {
      $matches[3] = '<strong>' . substr($matches[3], 0, $p) . '</strong>' .
        substr($matches[3], $p);
    }
    $output .= $matches[3] . "\n";
  }

  // Clean up any unclosed lists.
  array_pop($bullet_indents);
  foreach ($bullet_indents as $indent) {
    $output .= '</li></ul>';
  }

  // To make sure that api_autop() doesn't get confused, remove newlines
  // immediately before </li> tags.
  $output = str_replace("\n</li>", "</li>", $output);

  return $output;
}

/**
 * Retrieves a summary from a documentation block.
 *
 * @param $documentation
 *   Documentation block to find the summary of. Should be pre-formatted into
 *   paragraphs.
 *
 * @return
 *   First paragraph of the documentation block, stripped of tags, and
 *   truncated to 255 characers.
 */
function api_documentation_summary($documentation) {
  $pos = strpos($documentation, '</p>');
  if ($pos !== FALSE) {
    $documentation = substr($documentation, 0, $pos);
  }
  $documentation = trim(strip_tags($documentation));

  if (strlen($documentation) > 255) {
    return substr($documentation, 0, strrpos(substr($documentation, 0, 252), ' ')) . '…';
  }
  else {
    return $documentation;
  }
}

/**
 * Colorizes and formats PHP code, and finds references in it.
 *
 * @param $code
 *   PHP code to format. Note that any actual PHP code needs to be enclosed in
 *   PHP start/end tags to be formatted correctly.
 * @param $references
 *   Array to return references in, if $track_references is TRUE. References
 *   are hook invocations, function calls, etc. $references is an associative
 *   array where the keys are the types of references ('function',
 *   'potential hook', etc.), and the values are arrays of the names of this
 *   type that were found.
 * @param $number
 *   FALSE to not number the lines (default). TRUE to number the lines, or an
 *   integer to start numbering at that line number.
 * @param $track_references
 *   TRUE (default) to track references, FALSE to skip reference tracking.
 *
 * @return
 *   HTML-formatted code, with spans enclosing various PHP elements.
 */
function api_format_php($code, &$references, $number = FALSE, $track_references = TRUE) {
  $output = '';

  if (!defined('T_ML_COMMENT')) {
    define('T_ML_COMMENT', T_COMMENT);
  }
  if (!defined('T_DOC_COMMENT')) {
    define('T_DOC_COMMENT', T_COMMENT);
  }

  // Set up to track references, if we are tracking them.
  if ($track_references) {
    $references = array(
      'function' => array(),
      'potential hook' => array(),
      'potential theme' => array(),
      'potential alter' => array(),
      'potential callback' => array(),
      'member-parent' => array(),
      'member-self' => array(),
      'member' => array(),
      'member-class' => array(),
    );
  }

  // Keep track of:
  // - Are we in a double-quoted string?
  // - Are we in the parameters for a hook or theme invocation?
  // - How many PHP start/end tags we found (will remove at end if there is
  //   only one set).
  // - Class members: $foo->bar or Foo::bar.

  // Double-quoted strings.
  $in_string = FALSE;

  // Hook/theme invokes.
  $invoke_type = '';
  $paren_count = 0;
  $target_paren_count = 0;

  // PHP open/close tag counts and string size.
  $open_count = 0;
  $close_count = 0;
  $open_size = 0;
  $close_size = 0;

  // Class members.
  $in_member = FALSE;
  $member_of = '';

  // Split the code into tokens.
  $tokens = token_get_all($code);
  foreach ($tokens as $array_index => $token) {
    $close_size = 0;

    // Take care of double-quoted strings.
    if ($in_string) {
      $in_member = FALSE;
      $member_of = '';
      if ($token == '"') {
        $output .= '"</span>';
        $in_string = FALSE;
      }
      else {
        $output .= is_array($token) ? htmlspecialchars($token[1]) : htmlspecialchars($token);
      }
      continue;
    }
    elseif ($token == '"') {
      $output .= '<span class="php-string">"';
      $in_string = TRUE;
      $in_member = FALSE;
      $member_of = '';
      continue;
    }

    if (is_array($token)) {
      $type = $token[0];
      $value = htmlspecialchars($token[1]);

      switch ($type) {
        case T_OPEN_TAG:
          $in_member = FALSE;
          $member_of = '';
          $to_add = '<span class="php-boundry">' . $value . '</span>';
          $open_count++;
          if (strlen($output) == 0) {
            // This is at the start of $output, so we tentatively want
            // to remove it.
            $open_size = strlen($to_add);
          }

          $output .= $to_add;
          break;

        case T_CLOSE_TAG:
          $in_member = FALSE;
          $member_of = '';
          $to_add = '<span class="php-boundry">' . $value . '</span>';
          $close_count++;
          // Save the size of the closing tag here. If we didn't encounter
          // any more tokens after this, it will remain (this is reset to zero
          // each time through the loop).
          $close_size = strlen($to_add);
          $output .= $to_add;
          break;

        case T_COMMENT:
        case T_ML_COMMENT:
        case T_DOC_COMMENT:
          $in_member = FALSE;
          $member_of = '';
          $output .= '<span class="php-comment">' . $value . '</span>';
          break;

        case T_VARIABLE:
          $in_member = FALSE;
          $member_of = $value;
          $output .= '<span class="php-variable">' . $value . '</span>';
          break;

        case T_CONSTANT_ENCAPSED_STRING:
          $in_member = FALSE;
          $member_of = '';
          // This is a string like 'foo'. If we're in a call of some sort,
          // then this is a potential function, hook, etc. name.
          $class = 'php-string';
          if ($invoke_type != '' && preg_match("|^'" . API_RE_HOOK_NAME . "'$|", $value)) {
            $class .= ' potential-' . $invoke_type;
            if ($track_references) {
              $newname = str_replace("'", '', $value);
              $references['potential ' . $invoke_type][$newname] = $newname;
            }
          }
          if ($track_references && preg_match("|^'" . DRUPAL_PHP_FUNCTION_PATTERN . "'$|", $value)) {
            $newname = str_replace("'", '', $value);
            $references['potential callback'][$newname] = $newname;
          }

          $output .= '<span class="' . $class . '">' . $value . '</span>';
          break;

        case T_INLINE_HTML:
          $in_member = FALSE;
          $member_of = '';
          $output .= '<span class="php-string">' . $value . '</span>';
          break;

        case T_STRING:
          // This is a function, constant, etc. being declared or used.
          $class = 'php-function-or-constant';
          $reference_type = 'function';
          $reference = $value;

          // See if this is a function call or not, by looking ahead to the
          // next token and seeing if it is an open paren.
          $is_function = (isset($tokens[$array_index + 1]) && !is_array($tokens[$array_index + 1]) && $tokens[$array_index + 1] == '(');

          if ($in_member) {
            if ($is_function) {
              $class .= ' function';
            }
            else {
              $class .= ' property';
            }
            if ($member_of == 'parent') {
              $class .= ' member-of-parent';
              $reference_type = 'member-parent';
            }
            elseif ($member_of == '$this' || $member_of == 'self') {
              $class .= ' member-of-self';
              $reference_type = 'member-self';
            }
            elseif (strpos($member_of, '$') === 0) {
              $class .= ' member-of-variable';
              $reference_type = 'member';
            }
            elseif (strlen($member_of) > 0) {
              $class .= ' member-of-class-' . $member_of;
              $reference_type = 'member-class';
              $reference = $member_of . '::' . $value;
            }
            else {
              $class .= ' member-of-variable';
              $reference_type = 'member';
            }
          }

          $output .= '<span class="' . $class . '">' . $value . '</span>';

          // Save references -- only for functions.
          if ($track_references && $is_function) {
            $references[$reference_type][$value] = $reference;
          }

          // Update $in_member/$member_of status.
          if ($in_member) {
            // If we were already in a member chain, clear $member_of, because
            // we won't know what the next type is if -> comes next.
            $member_of = '';
          }
          else {
            // We were not already in a member chain. So save this keyword
            // in case it is the start of a chain.
            $member_of = $value;
          }
          $in_member = FALSE;

          // Save $invoke_type state.
          if ($value == 'theme') {
            $invoke_type = 'theme';
            $target_paren_count = $paren_count + 1;
          }
          elseif (in_array($value, array('module_invoke', 'module_invoke_all', 'module_implements', 'module_hook', '_field_invoke', '_field_invoke_multiple', '_field_invoke_default', '_field_invoke_multiple_default', 'node_invoke', 'user_module_invoke'))) {
            $invoke_type = 'hook';
            $target_paren_count = $paren_count + 1;
          }
          elseif ($value == 'drupal_alter') {
            $invoke_type = 'alter';
            $target_paren_count = $paren_count + 1;
          }

          break;

        case T_LNUMBER:
        case T_DNUMBER:
          $in_member = FALSE;
          $member_of = '';
          $output .= '<span class="php-constant">' . $value . '</span>';
          break;

        case T_ARRAY_CAST:
        case T_ARRAY:
        case T_AS:
        case T_BOOL_CAST:
        case T_BREAK:
        case T_CASE:
        case T_CLASS:
        case T_CONST:
        case T_CONTINUE:
        case T_DECLARE:
        case T_DEFAULT:
        case T_DO:
        case T_DOUBLE_CAST:
        case T_ECHO:
        case T_ELSE:
        case T_ELSEIF:
        case T_EMPTY:
        case T_ENDDECLARE:
        case T_ENDFOR:
        case T_ENDFOREACH:
        case T_ENDIF:
        case T_ENDSWITCH:
        case T_ENDWHILE:
        case T_EVAL:
        case T_EXIT:
        case T_EXTENDS:
        case T_FOR:
        case T_FOREACH:
        case T_FUNCTION:
        case T_GLOBAL:
        case T_IF:
        case T_INCLUDE_ONCE:
        case T_INCLUDE:
        case T_INT_CAST:
        case T_ISSET:
        case T_LIST:
        case T_NEW:
        case T_OBJECT_CAST:
        case T_PRINT:
        case T_REQUIRE_ONCE:
        case T_REQUIRE:
        case T_RETURN:
        case T_STATIC:
        case T_STRING_CAST:
        case T_SWITCH:
        case T_UNSET_CAST:
        case T_UNSET:
        case T_USE:
        case T_VAR:
        case T_WHILE:
          $in_member = FALSE;
          $member_of = '';
          $output .= '<span class="php-keyword">' . $value . '</span>';
          break;

        case T_OBJECT_OPERATOR:
        case T_DOUBLE_COLON:
          $output .= $value;
          $in_member = TRUE;
          break;

        default:
          $in_member = FALSE;
          $member_of = '';
          $output .= $value;
      }
    }
    else {
      $in_member = FALSE;
      $member_of = '';

      // Keep track of parentheses for whether we are in a hook/theme/alter
      // call.
      if ($token == '(') {
        $paren_count++;
      }
      elseif ($token == ')') {
        $paren_count--;
        // See if we've reached the end of this function call.
        if ($paren_count < $target_paren_count) {
          $invoke_type = '';
        }
      }

      $output .= $token;
    }
  }

  // If this block of code started and ended with PHP open/close tags and
  // didn't have any additional open/close inside, remove the open/close tags.
  if ($open_size > 0 && $close_size > 0 && $open_count == 1 && $close_count == 1) {
    $output = substr($output, $open_size, strlen($output) - $open_size - $close_size);
  }

  // Add line numbering, if requested.
  if ($number !== FALSE) {
    $start = (is_int($number)) ? $number : 1;
    $lines = explode("\n", $output);
    $output = '<ol class="code-lines" start="' . $number . '">';
    foreach ($lines as $line) {
      $output .= '<li>' . $line . "\n</li>";
    }
    $output .= '</ol>';
  }

  $output = '<pre class="php"><code>' . $output . '</code></pre>';
  return $output;
}

/**
 * Adds a reference to the {api_reference_storage} table.
 *
 * Since we may parse a file containing a reference before we have parsed the
 * file containing the referenced object, we keep track of the references
 * using a scratch table.
 *
 * @param $branch
 *   Object representing the branch the reference is in.
 * @param $to_type
 *   Type of object being referenced.
 * @param $to_name
 *   Name of object being referenced.
 * @param $from_did
 *   Documentation ID of the object that references this object.
 */
function api_reference($branch, $to_type, $to_name, $from_did) {
  static $is_php_function = array();

  // Don't make references to built-in PHP functions.
  if ($to_type == 'function' && !isset($is_php_function[$to_name])) {
    $query = db_select('api_php_documentation', 'd')
      ->fields('d', array('did'));
    $query->condition('d.object_name', $to_name);
    $is_php_function[$to_name] = $query->execute()->fetchField();
  }

  if ($to_type != 'function' || !$is_php_function[$to_name]) {
    $id = db_insert('api_reference_storage')
      ->fields(array(
          'object_name' => $to_name,
          'branch_id' => $branch->branch_id,
          'object_type' => $to_type,
          'from_did' => $from_did,
        ))
      ->execute();
  }
}

/**
 * Registers a shutdown function for cron, making sure to do it just once.
 *
 * @see api_shutdown()
 */
function api_schedule_shutdown() {
  static $scheduled = FALSE;

  if (!$scheduled) {
    register_shutdown_function('api_shutdown');
    $scheduled = TRUE;
  }
}

/**
 * Cleans up at the end of the cron job.
 *
 * Updates the collected references, updates the JSON object list, and clears
 * the cache.
 */
function api_shutdown() {
  // We will be re-computing this below.
  db_delete('api_reference_storage')
    ->condition('object_type', 'computed-member')
    ->execute();

  // Figure out which class member documentation objects do not have entries in
  // the {api_overrides} table. These entries are deleted during
  // api_save_documentation(), so that they can be recomputed here for any
  // class members that were updated.
  $query = db_select('api_documentation', 'ad')
    ->fields('ad', array('did', 'object_type', 'member_name', 'class_did', 'documentation', 'throws', 'var', 'code'));
  $ao = $query->leftJoin('api_overrides', 'ao', 'ao.did = ad.did');
  $af = $query->leftJoin('api_function', 'af', 'af.did = ad.did');
  $query->fields('af', array('parameters', 'return_value'))
    ->condition('ad.class_did', 0, '<>')
    ->isNull('ao.did');
  $result = $query->execute();

  // Compute overrides (i.e., if a class member is an override/implementation of
  // another class/interface's member) for each of the resulting objects, and
  // keep track of which classes were changed along the way.
  $changed_classes = array();
  foreach ($result as $object) {
    $changed_classes[$object->class_did] = TRUE;
    $override = array(
      'did' => $object->did,
      'overrides_did' => 0,
      'documented_did' => api_has_documentation($object) ? $object->did : 0,
    );
    $overrides_did = 0;
    // Go up the parent stack and see if this member overrides a parent member.
    $classes_to_check = array($object->class_did);
    while ($class_did = array_shift($classes_to_check)) {
      // If we have al the information we need here, stop this loop.
      if ($override['overrides_did'] > 0 && $override['documented_did'] > 0) {
        break;
      }

      // Find members of the same name/type as $object on $class_did's
      // parent classes or interfaces.
      $query = db_select('api_reference_storage', 'ars');
      $parent_d = $query->innerJoin('api_documentation', 'pd', 'ars.object_name = pd.object_name AND ars.branch_id = pd.branch_id AND ars.object_type = pd.object_type');
      $query
        ->condition('ars.object_type', array('class', 'interface'))
        ->condition('pd.object_type', array('class', 'interface'))
        ->condition('ars.from_did', $class_did);
      $member_d = $query->leftJoin('api_documentation', 'md', 'md.class_did = pd.did');
      $query
        ->fields($member_d, array('did', 'summary', 'documentation', 'see', 'throws', 'var', 'class_did'))
        ->condition('md.object_type', $object->object_type)
        ->condition('md.member_name', $object->member_name);
      $func = $query->leftJoin('api_function', 'af', 'af.did = md.did');
      $query->fields($func, array('parameters', 'return_value'));

      $result_parents = $query->execute();
      foreach ($result_parents as $parent_member) {
        $classes_to_check[] = $parent_member->class_did;
        if (!is_null($parent_member->did)) {
          // This parent class/interface has a matching member name.
          if ($override['overrides_did'] === 0) {
            // This is the first matching member name we've encountered, so
            // this is the member that $object is overriding.
            $override['overrides_did'] = $parent_member->did;
          }
          if ($override['documented_did'] === 0 && api_has_documentation($parent_member)) {
            // $object didn't have documentation, but $parent_member does, so
            // inherit it.
            $override['documented_did'] = $parent_member->did;
            $object->summary = $parent_member->summary;
            drupal_write_record('api_documentation', $object, 'did');
          }
        }
      }
    }

    drupal_write_record('api_overrides', $override);
  }

  // Compute and save member list for each changed class. Members can be on
  // the class itself, or can be inherited from a parent class.
  while (list($class_did, ) = each($changed_classes)) {
    // Add current class's child classes to the list, because they need to
    // be recomputed too.
    $query = db_select('api_reference_storage', 'ars')
      ->fields('ars', array('from_did'));
    // @todo Use namespaces here rather than just blindly matching on the
    // object name.
    $doc = $query->innerJoin('api_documentation', 'ad', 'ars.object_name = ad.object_name AND ars.object_type = ad.object_type AND ars.branch_id = ad.branch_id');
    $result_children = $query
      ->condition('ad.did', $class_did)
      ->condition('ars.from_did', 0, '<>')
      ->condition('ars.object_type', array('class', 'interface'))
      ->execute()
      ->fetchCol();
    foreach ($result_children as $new_class) {
      $changed_classes[$new_class] = TRUE;
    }

    // Walk up the tree to find members. Since methods, properties, and
    // constants can share names, do each separately.
    $all_members = array(
      'function' => array(''),
      'property' => array(''),
      'constant' => array(''),
    );
    $parents = array($class_did);
    while ($parent = array_shift($parents)) {
      // Add this class's parents to the list.
      // @todo Use namespaces here. Right now we're just matching on the class
      // name.
      $query = db_select('api_reference_storage', 'ars');
      $doc = $query->innerJoin('api_documentation', 'ad', 'ars.object_name = ad.object_name AND ars.object_type = ad.object_type AND ars.branch_id = ad.branch_id');
      $result_parents = $query
        ->condition('ars.object_type', array('class', 'interface'))
        ->fields($doc, array('did'))
        ->condition('from_did', $parent)
        ->execute()
        ->fetchCol();
      foreach ($result_parents as $new_class) {
        $parents[] = $new_class;
      }

      // Find members of each type on this parent that haven't yet been added
      // to the list.
      foreach ($all_members as $type => $members) {
        $result_members = db_select('api_documentation', 'd')
          ->fields('d', array('did', 'member_name'))
          ->condition('class_did', $parent)
          ->condition('object_type', $type)
          ->condition('member_name', array_keys($members), 'NOT IN')
          ->execute();
        foreach ($result_members as $member) {
          $members[$member->member_name] = $member->did;
        }
        $all_members[$type] = $members;
      }
    }

    // Save the computed members.
    db_delete('api_members')
      ->condition('class_did', $class_did)
      ->execute();
    foreach ($all_members as $type => $members) {
      array_shift($members);
      foreach ($members as $did) {
        $api_member = array(
          'class_did' => $class_did,
          'did' => $did,
        );
        drupal_write_record('api_members', $api_member);
      }
    }
  }

  // Figure out the member-self references in reference storage. Basically,
  // ChildClass::foo() calls $this->bar(). We need to figure out the full name
  // of $this->bar(), which could be ChildClass::bar() or
  // SomeParentClass::bar(). The {api_methods} table keeps track of this.

  $select = db_select('api_reference_storage', 'r');
  $d = $select->innerJoin('api_documentation', 'd', 'r.from_did = %alias.did');

  $select2 = db_select('api_members', 'm');
  $memberd = $select2->innerJoin('api_documentation', 'memberd', 'm.did = %alias.did');
  $m_name = $select2->addField($memberd, 'member_name', 'm_name');
  $m_cdid = $select2->addField('m', 'class_did', 'm_cdid');
  $m_objtype = $select2->addField($memberd, 'object_type', 'm_objtype');
  $m_objname = $select2->addField($memberd, 'object_name', 'm_objname');
  $mm = $select->innerJoin($select2, 'mm', "%alias.$m_name = r.object_name AND %alias.$m_cdid = $d.class_did");

  $select->addField($mm, $m_objname, 'object_name');
  $select->addField('r', 'branch_id', 'branch_id');
  $select->addField('r', 'from_did', 'from_did');
  // Note: SelectQuery adds expressions to the query at the end of the field
  // list.
  $select->addExpression("'computed-member'", 'object_type');
  $select->condition('r.object_type', 'member-self')
    ->condition($mm . '.' . $m_objtype, 'function');

  db_insert('api_reference_storage')
    ->fields(array('object_name', 'branch_id', 'from_did', 'object_type'))
    ->from($select)
    ->execute();

  // Do the same for member-parent references, where ChildClass::foo() calls
  // parent::bar() -- figure out the full name of the parent member.
  // @todo Use namespaces here instead of just matching parent class names.

  $select = db_select('api_reference_storage', 'r')
    ->condition('r.object_type', 'member-parent');
  $calling_d = $select->innerJoin('api_documentation', 'cd', 'r.from_did = cd.did');
  $parent_class_r = $select->innerJoin('api_reference_storage', 'pr', "pr.from_did = cd.class_did AND pr.branch_id = cd.branch_id AND pr.object_type = 'class'" );
  $parent_class_d = $select->innerJoin('api_documentation', 'pd', 'pd.object_name = pr.object_name AND pd.branch_id = pr.branch_id AND pd.object_type = pr.object_type');
  $parent_m = $select->innerJoin('api_members', 'pm', 'pm.class_did = pd.did');
  $member_d = $select->innerJoin('api_documentation', 'md', "md.did = pm.did AND md.member_name = r.object_name AND md.object_type = 'function'");

  $select->addField($member_d, 'object_name', 'object_name');
  $select->addField('r', 'branch_id', 'branch_id');
  $select->addField('r', 'from_did', 'from_did');
  // Note: SelectQuery adds expressions to the query at the end of the field
  // list.
  $select->addExpression("'computed-member'", 'object_type');

  db_insert('api_reference_storage')
    ->fields(array('object_name', 'branch_id', 'from_did', 'object_type'))
    ->from($select)
    ->execute();

  // Save JSON autocomplete cache.
  $date = gmdate('U');
  foreach (api_get_branch_names() as $branch_name) {
    $new_json = api_autocomplete($branch_name, FALSE);

    $old_file_path = variable_get('api_autocomplete_path_' . $branch_name, FALSE);
    if ($old_file_path !== FALSE) {
      if (md5($new_json) === md5(file_get_contents($old_file_path))) {
        continue; // No changes, no file write.
      }
      // Delete in the future, help avoid race conditions.
      $queue = DrupalQueue::get('api_delete');
      $queue->createItem(array('path' => $old_file_path));
    }

    $file_name = 'api-' . $branch_name . '-' . $date . '.json';
    $directory = 'public://api';
    file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
    $saved = file_unmanaged_save_data($new_json, $directory . '/' . $file_name, FILE_EXISTS_REPLACE);
    if ($saved) {
      variable_set('api_autocomplete_path_' . $branch_name, $saved);
    }
  }

  cache_clear_all();
}

/**
 * Determines whether an object contains any documentation.
 *
 * @param $object
 *   An object to test to see if it contains documentation.
 *
 * @return
 *   TRUE if the object has any of the following non-empty properties:
 *   - documentation
 *   - parameters
 *   - return_value
 *   - see
 *   - throws
 *   - var
 */
function api_has_documentation($object) {
  foreach (array('documentation', 'parameters', 'return_value', 'see', 'throws', 'var') as $member) {
    if (!empty($object->$member)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Updates all branches, by calling their update functions.
 *
 * @see api_update_branch_php()
 * @see api_update_branch_files()
 */
function api_update_all_branches() {
  foreach (api_get_branches() as $branch) {
    $function = 'api_update_branch_' . $branch->type;
    if ($function($branch)) {
      watchdog('api', 'Updated %project branch %branch_name.', array('%branch_name' => $branch->branch_name, '%project' => $branch->project));
    }
  }
}

/**
 * Updates a PHP branch.
 *
 * Queries the branch URL to get an updated list of functions, and saves each
 * function in the database.
 *
 * @param $branch
 *   Object representing a PHP branch to update.
 * @param $force
 *   TRUE to force an update no matter what, and FALSE to update only if the
 *   time threshold has been exceeded.
 *
 * @return
 *   TRUE if the branch was updated, and FALSE if it was skipped due to not
 *   needing to be run this time.
 *
 * @see api_update_all_branches()
 */
function api_update_branch_php($branch, $force = FALSE) {
  // See if we should really update this branch at all.
  if (!$force) {
    $last_run = variable_get('api_last_updated_' . $branch->branch_id, 0);
    $now = time();
    $add = 0;
    if (isset($branch->update_time)) {
      $add = $branch->update_time;
    }
    if ($last_run + $add > $now) {
      // We shouldn't run this time.
      return FALSE;
    }
  }

  $response = drupal_http_request($branch->summary);
  if ($response->code === '200') {
    $docblocks = array();
    preg_match_all('!^[a-zA-Z0-9_]* (' . DRUPAL_PHP_FUNCTION_PATTERN . ')\(.*\n.*$!m', $response->data, $function_matches, PREG_SET_ORDER);
    foreach ($function_matches as $function_match) {
      $docblocks[] = array(
        'branch_id' => $branch->branch_id,
        'object_type' => 'function',
        'object_name' => $function_match[1],
        'documentation' => $function_match[0],
        'member_name' => '',
      );
    }

    if (count($docblocks)) {
      api_save_php_documentation($docblocks);
    }

    variable_set('api_last_updated_' . $branch->branch_id, time());
  }

  return TRUE;
}

/**
 * Returns the list of parsing functions for file extensions.
 *
 * @return
 *   An associative array whose keys are file extensions, and whose values are
 *   the functions used to parse files with that extension.
 */
function api_parse_functions() {
  // @todo This could use a hook to let other modules extend this array?
  return array(
    'php' => 'api_parse_php_file',
    'module' => 'api_parse_php_file',
    'inc' => 'api_parse_php_file',
    'install' => 'api_parse_php_file',
    'engine' => 'api_parse_php_file',
    'theme' => 'api_parse_php_file',
    'profile' => 'api_parse_php_file',
    'test' => 'api_parse_php_file',

    'txt' => 'api_parse_text_file',
    'info' => 'api_parse_text_file',

    'htm' => 'api_parse_html_file',
    'html' => 'api_parse_html_file',
  );
}

/**
 * Updates a files branch.
 *
 * Checks the current directories included in the branch to make an updated
 * list of files. Removes documentation from files that no longer exist, adds
 * documentation from new files, and updates documentation for any files that
 * have changed.
 *
 * @param $branch
 *   Object representing a files branch to update.
 *
 * @return
 *   TRUE to indicate that the branch was updated.
 *
 * @see api_update_all_branches()
 */
function api_update_branch_files($branch) {
  $parse_functions = api_parse_functions();

  // List all documented files for the branch.
  $files = array();
  $query = db_select('api_documentation', 'd')
    ->fields('d', array('object_name'));
  $f = $query->innerJoin('api_file', 'f', 'd.did = f.did');
  $query->fields($f, array('did', 'modified', 'queued'))
    ->condition('d.branch_id', $branch->branch_id)
    ->condition('d.object_type', 'file');
  $result = $query->execute();
  foreach ($result as $file) {
    $files[$file->object_name] = $file;
  }

  foreach (api_scan_directories($branch->directories, $branch->excluded_directories) as $path => $file_name) {
    preg_match('!\.([a-z]*)$!', $file_name, $matches);
    if (isset($matches[1]) && isset($parse_functions[$matches[1]])) {
      $new = FALSE;
      if (isset($files[$file_name])) {
        $parse = (filemtime($path) > $files[$file_name]->modified) && !$files[$file_name]->queued;
        $file = $files[$file_name];
        unset($files[$file_name]); // All remaining files will be removed.
      }
      else { // New file.
        $parse = TRUE;
        $new = TRUE;
      }
      if ($parse) {
        $queue = DrupalQueue::get('api_parse');
        $queue->createItem(array('parser' => $parse_functions[$matches[1]], 'path' => $path, 'branch' => $branch, 'file' => $file_name, 'branch_name' => $branch->branch_name));

        // Make sure this doesn't get added to the parse queue again.

        if ($new) {
          // Add stub to the {api_documentation} and {api_file} table for this
          // file so it doesn't get added to the parse queue again.
          $nid = api_new_documentation_node();
          $doc = array(
            'object_name' => $file_name,
            'file_name' => $file_name,
            'title' => strpos($file_name, '/') ? substr($file_name, strrpos($file_name, '/') + 1) : $file_name,
            'object_type' => 'file',
            'branch_id' => $branch->branch_id,
            'queued' => time(),
            'did' => $nid,
          );
          _api_add_text_defaults($doc, 'api_documentation');
          drupal_write_record('api_documentation', $doc);
          drupal_write_record('api_file', $doc);
        }
        else {
          // Update the 'queued' field in the {api_file} table so this does not
          // get queued again.
          $file->queued = time();
          drupal_write_record('api_file', $file, 'did');
        }
      }
    }
  }

  // Remove outdated files.
  foreach (array_keys($files) as $file_name) {
    watchdog('api', 'Removing %file.', array('%file' => $file_name));
    $dids = db_select('api_documentation', 'ad')
      ->fields('ad', array('did'))
      ->condition('file_name', $file_name)
      ->condition('branch_id', $branch->branch_id)
      ->execute()
      ->fetchCol();
    foreach ($dids as $did) {
      db_delete('api_documentation')
        ->condition('did', $did)
        ->execute();
      db_delete('api_file')
        ->condition('did', $did)
        ->execute();
      db_delete('api_function')
        ->condition('did', $did)
        ->execute();
      db_delete('api_reference_storage')
        ->condition('from_did', $did)
        ->execute();
    }
    api_schedule_shutdown();
  }

  return TRUE;
}

/**
 * Finds all the files in the directories specified for a branch.
 *
 * @param $directories
 *   List of directories to scan, as text (separated by newlines).
 * @param $excluded_directories
 *   List of directories to exclude, as text (separated by newlines).
 *
 * @return
 *   Associative array of files, where the keys are the full paths to the
 *   files and the values are the file names. File names exclude the part of
 *   the full path that is shared by all directories in the scan list. Only
 *   files that can be parsed are included (see api_parse_functions() for a
 *   list). Hidden directories (names starting with .) below top-level entries
 *   in $directories are excluded, as well as directories called CVS.
 */
function api_scan_directories($directories, $excluded_directories) {
  $directory_array = explode("\n", $directories);
  $excluded_array = explode("\n", $excluded_directories);
  $extensions = array_keys(api_parse_functions());

  // Figure out the shared part of the file name that is in all the directories.
  if (count($directory_array) > 1) {
    $directories_components = array();
    foreach ($directory_array as $directory) {
      $directory_components = array();
      $parts = explode(DIRECTORY_SEPARATOR, $directory);
      $starts_with_sep = (strpos($directory, DIRECTORY_SEPARATOR) === 0);
      foreach ($parts as $part) {
        if (strlen($part)) {
          $previous = reset($directory_components);
          if (strlen($previous) > 0 || $starts_with_sep) {
            $previous .= DIRECTORY_SEPARATOR;
          }
          array_unshift($directory_components, $previous . $part);
        }
      }
      $directories_components[] = $directory_components;
    }

    $common_ancestor_components = call_user_func_array('array_intersect', $directories_components);
    $common_ancestor = reset($common_ancestor_components);
  }
  else {
    $common_ancestor = $directories;
  }

  // Scan the directories. Instead of letting file_scan_directory() do its own
  // recursion, we recurse ourselves. The reason is that we can exclude things
  // as we go, saving time and memory scanning .git, CVS, and the excluded
  // directories.
  $source_files = array();
  $todo = $directory_array;
  while (count($todo) > 0) {
    $directory = array_shift($todo);
    $files = file_scan_directory($directory, '/.*/', array('recurse' => FALSE));
    foreach ($files as $path => $file) {
      if (is_dir($path)) {
        // See if we want to scan this path, and if so, add to To Do list.
        // Don't keep any directory that starts with ".", or anything in the
        // excluded list. Note that $file->filename includes $directory, so
        // strip that off before checking the name for ".".
        $dir_name = substr($file->filename, strlen($directory));
        if ((strpos($dir_name, '.') !== 0) &&
          !in_array($path, $excluded_array)) {
          $todo[] = $path;
        }
      }
      else {
        // This is a regular file. See if it has an extension we recognize,
        // and save it in our return list if so.
        $ext = pathinfo($path, PATHINFO_EXTENSION);
        if (in_array($ext, $extensions)) {
          $file_name = substr($path, strlen($common_ancestor) + 1);
          $source_files[$path] = $file_name;
        }
      }
    }
  }

  return $source_files;
}

/**
 * Adds defaults for TEXT fields to a database record.
 *
 * These cannot come from the schema, because TEXT fields have no defaults.
 *
 * @param $record
 *   Record about to be written to the database with drupal_write_record().
 * @param $table
 *   Table $record is going into.
 */
function _api_add_text_defaults(&$record, $table) {
  switch ($table) {
    case 'api_documentation':
      $record += array(
        'summary' => '',
        'documentation' => '',
        'code' => '',
        'see' => '',
        'var' => '',
        'throws' => '',
      );
      break;

    case 'api_php_documentation':
      $record += array(
        'documentation' => '',
      );
      break;

    case 'api_function':
      $record += array(
        'parameters' => '',
        'return_value' => '',
      );
      break;
  }
}

/**
 * Creates a new documentation object node (for comments).
 *
 * @return
 *   Node ID of the created node.
 */
function api_new_documentation_node() {
  $node = new stdClass();
  $node->type = 'api';
  $node->uid = 0;
  if (module_exists('comment')) {
    $node->comment =  variable_get('comment_api', COMMENT_NODE_OPEN);
  }
  node_save($node);

  return $node->nid;
}
