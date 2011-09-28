<?php

/**
 * Implements hook_drush_command().
 */
function quail_api_node_drush_command() {
  $items = array();

  $items['acc_validate_node'] = array(
    'callback' => 'quail_api_node_drush_callback_validate',
    'description' => "Perform accessibility validation on nodes. This is only useful if the content type has save to database support enabled.",
    'arguments' => array(
      'nids' => "A list of space-separated node IDs to validate.",
    ),
    'options' => array(
      '--all' => "Perform validation on every single node in the system (this may take a while).",
      '--type' => "Perform validation on every single node in the system of the specified content type (use the machine name of the content type).",
    ),
    'examples' => array(
      'drush acc_validate_node 45 46 47' => "Validate nodes with IDs of 45, 46, and 47.",
      'drush acc_validate_node --all' => "Validate all nodes in the system.",
      'drush acc_validate_node --type page' => "Validate all nodes in the system of the content type 'page'.",
    ),
  );

  // Add aliases for usability.
  quail_api_node_drush_command_add_alias($items, 'acc_validate_node', 'acc_validate_node');

  return $items;
}

/**
 * A function to help alias commands as other commands.
 */
function quail_api_node_drush_command_add_alias(&$items, $command, $alias) {
  // Create a property on the command for adding aliases, if not there.
  if (!isset($items[$command]['quail_api_node command aliases'])) {
    $items[$command]['quail_api_node command aliases'] = array();
  }

  // Record the alias into that property.
  $items[$command]['quail_api_node command aliases'][] = $alias;
  // Create the alias as a new command.
  $items[$alias] = $items[$command];
  // Indicate what this new command is an alias for.
  $items[$alias]['quail_api_node alias for'] = $command;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function quail_api_node_drush_help($section) {
  // This is to prevent duplication of information from hook_drush_command().
  $commands = quail_api_node_drush_command();

  foreach ($commands as $command => $command_info) {
    if ($section == 'drush:' . $command) {
      $out = $command_info['description'];

      if (isset($command_info['quail_api_node alias for'])) {
        $output .= "\nThis command is an alias for ";
        $output .= $command_info['quail_api_node alias for'] . ".";
      }

      if (isset($command_info['quail_api_node command aliases'])) {
        if (count($command_info['quail_api_node command aliases']) == 1) {
          $output .= "\nThis command can be called by it's alias; ";
          $output .= $command_info['quail_api_node command aliases'] . ".";
        }
        else {
          $last_alias = array_pop($command_info['quail_api_node command aliases']);
          $output .= "\nThis command can be called by it's aliases; ";
          $output .= implode(", ", $command_info['quail_api_node command aliases']);
          $output .= ", or " . $last_alias . ".";
        }
      }

      $info = array();
      $info['arguments'] = "Arguments";
      $info['options'] = "Options";
      $info['examples'] = "Examples";

      foreach ($info as $key => $value){
        $out .= "\n\n$value:";

        if (isset($command_info[$key])) {
          foreach ($command_info[$key] as $k => $v) {
            $out .= "\n  " . $k . " : " . $v;
          }
        }
      }

      return dt($out);
    }
  }
}

/**
 * Drush command callback.
 *
 * Import nodes from data.
 */
function quail_api_node_drush_callback_validate() {
  $uid = drush_get_option('uid');
  $nids = array();
  $rows = array();

  if ($uid != 0) {
    if (is_null($uid)) $uid = 1;

    global $user;
    $user = user_load($uid);
  }

  if (drush_get_option('all')){
    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $rows = $query->execute()->fetchAllAssoc('nid');
  }
  else {
    $node_type = drush_get_option('type');

    if (empty($node_type)){
      $nids = array_filter(func_get_args(), 'is_numeric');
    }
    else {
      $query = db_select('node', 'n');
      $query->fields('n', array('nid'));
      $query->condition('type', check_plain($node_type), '=');
      $rows = $query->execute()->fetchAllAssoc('nid');
    }
  }

  foreach ($rows as $row){
    $nids[] = $row->nid;
  }

  $result = quail_api_node_operation_validate($nids);

  if ($result === FALSE) {
    drush_set_error('DRUSH_NOT_COMPLETED', "Failed to perform accessibility validation.");
  }
  else {
    drush_print("Accessibility Validation Operation Complete");
  }
}
