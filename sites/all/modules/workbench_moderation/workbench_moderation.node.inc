<?php

/**
 * @file
 * Provides node functions for Workbench Moderation.
 */

/**
 * Redirects the user to the current revision of the node.
 *
 * This page will deliver either the 'draft' page or the regular node view page.
 *
 * @param $node
 *   The node being acted upon.
 */
function workbench_moderation_node_current_view($node) {
  if (_workbench_moderation_access_current_draft($node)) {
    drupal_goto('node/' . $node->nid . '/draft');
  }
  drupal_goto('node/' . $node->nid);
}

/**
 * Get the menu router item for nodes.
 *
 * @param $node
 *   The node being acted upon.
 * @return
 *   A fully themed node page.
 */
function workbench_moderation_router_item_page_callback($node) {
  $router_item = menu_get_item('node/' . $node->nid);
  if ($router_item['include_file']) {
    require_once DRUPAL_ROOT . '/' . $router_item['include_file'];
  }

  // Replace the node in the router arguments with the draft node.
  foreach ($router_item['page_arguments'] as $index => $arg) {
    if (is_object($arg) && isset($arg->nid) && $arg->nid == $node->nid) {
      $router_item['page_arguments'][$index] = $node;
    }
  }

  // Call whatever function is assigned to the main node path but pass the
  // current node as an argument. This approach allows for the reuse of of Panel
  // definition acting on node/%node.
  return call_user_func_array($router_item['page_callback'], $router_item['page_arguments']);

}
/**
 * Displays the current draft the node, if it is not published.
 *
 * @param $node
 *   The node being acted upon.
 *
 * @return
 *   A fully themed node page.
 */
function workbench_moderation_node_view_draft($node) {
  $current_node = workbench_moderation_node_current_load($node);
  return workbench_moderation_router_item_page_callback($current_node);
}

/**
 * Displays a specific revisison of the node.
 *
 * @param $node
 *   The node being acted upon.
 *
 * @return
 *   A fully themed node page.
 */
function workbench_moderation_node_view_revision($node) {
  return workbench_moderation_router_item_page_callback($node);
}

/**
 * Display a node's moderation history.
 *
 * @param $node
 *   The node being acted upon.
 *
 * @return
 *   A themed table of data and links for node moderation actions.
 */
function workbench_moderation_node_history_view($node) {
  global $user;

  // Page title.
  drupal_set_title(t('History of %title', array('%title' => $node->title)), PASS_THROUGH);

  // Get all of the node revisions, each with its most recent moderation.
  $query = db_select('node', 'n')->extend('PagerDefault');
  $query->innerJoin('node_revision', 'r', 'n.nid = r.nid');
  $query->leftJoin('users', 'u', 'r.uid = u.uid');
  $query->addField('n', 'vid', 'live_revision');
  $query->condition('n.nid', $node->nid)
    ->orderBy('r.vid', 'DESC')
    ->fields('r', array('nid', 'vid', 'title', 'log', 'uid', 'timestamp'))
    ->fields('u', array('name'))
    ->limit(30);

  $revisions = $query->execute()
    ->fetchAllAssoc('vid');

  // Build the table rows.
  $rows = array();
  foreach ($revisions as $revision) {
    $row = array(
      'data' => array(
        'vid' => '',
        'info' => '',
        'revision' => '',
        'moderation' => '',
      ),
      'class' => array('revision'),
    );

    // Add row classes.
    if ($revision->vid == $node->workbench_moderation['current']->vid) {
      $row['class'][] = 'current-revision';
    }
    if (isset($node->workbench_moderation['published']) && $revision->vid == $node->workbench_moderation['published']->vid) {
      $row['class'][] = 'published-revision';
    }
    else {
      $row['class'][] = 'unpublished-revision';
    }

    // Add row data.
    $row['data']['vid'] = $revision->vid;
    $row['data']['info'] .= '<div class="revision-log description">' . str_replace("\n", '<br>', trim(filter_xss($revision->log))) . '</div>';

    // Revision operations.
    $revision_operations = array();

    // Loading the node at the specific revision using node_load() is too slow
    // when there are many revisions, thus we fake it by cloning the original
    // node and changing the 'vid' and the 'my_revision' elements required for
    // granting access to the revision operations (view, update/revert, delete).
    $node_revision = clone $node;
    $node_revision->vid = $revision->vid;
    $node_revision->workbench_moderation['my_revision'] = $revision;

    // View operation.
    if (_workbench_moderation_revision_access($node_revision, 'view')) {
      // Link to the node page if this is the published revision.
      if (isset($node->workbench_moderation['published']) && $revision->vid == $node->workbench_moderation['published']->vid) {
        $url = "node/{$revision->nid}";
      }
      // The special case "current-revision" link handles routing for the
      // current unpublished revision.
      elseif ($revision->vid == $node->workbench_moderation['current']->vid) {
        $url = "node/{$revision->nid}/current-revision";
      }
      // Otherwise, link to the normal revision view page.
      else {
        $url = "node/{$revision->nid}/revisions/{$revision->vid}/view";
      }
      $revision_operations['view'] = l(t('View'), $url);
    }

    if (function_exists('node_accessibility_is_enabled') && node_accessibility_is_enabled($node->type)) {
      $revision_operations['accessibility'] = l(t("accessibility"), 'node/' . $node->nid . '/accessibility/' . $revision->vid . '/revision');
    }

    // Revert operation.
    if (_workbench_moderation_revision_access($node_revision, 'update')) {
      $revision_operations['revert'] = l(t('Revert'), "node/{$revision->nid}/revisions/{$revision->vid}/revert");
    }

    // Provide a courtesy edit operation if this is the current revision.
    if ($revision->vid == $node->workbench_moderation['current']->vid) {
      // The edit operation's default link title, "Edit draft", matches
      // the logic tree in workbench_moderation_edit_tab_title().
      $edit_operation_title = t('Edit draft');

      // Modify the edit operation's link title to "New draft", matching the
      // logic tree in workbench_moderation_edit_tab_title() when the current
      // revision is the published node.
      if (isset($node->workbench_moderation['published']) && $revision->vid == $node->workbench_moderation['published']->vid) {
        $edit_operation_title = t('New draft');
      }
      $revision_operations['edit'] = l($edit_operation_title, "node/{$revision->nid}/edit", array('query' => array('destination' => "node/{$revision->nid}/moderation")));
    }

    // Delete operation.
    if (_workbench_moderation_revision_access($node_revision, 'delete')) {
      $revision_operations['delete'] = l(t('Delete'), "node/{$revision->nid}/revisions/{$revision->vid}/delete");
    }

    // Get the moderation history of this revision.
    $query = db_select('workbench_moderation_node_history', 'm');
    $query->leftJoin('users', 'u', 'm.uid = u.uid');
    $query->condition('m.vid', $revision->vid)
      ->orderBy('m.stamp', 'DESC')
      ->orderBy('m.hid', 'DESC')
      ->fields('m')
      ->fields('u', array('name'));

    $moderations = $query->execute()
      ->fetchAllAssoc('hid');

    // Make sure it's an array.
    if (empty($moderations)) {
      $moderations = array();
      if ($revision->vid == $node->workbench_moderation['current']->vid) {
        $moderations = array($node->workbench_moderation['current']);
      }
    }

    // If this is the current revision, provide moderation links.
    if (!empty($node->workbench_moderation['published']) && $revision->vid == $node->workbench_moderation['published']->vid) {
      $row['data']['moderation'] = '<div class="moderation-state"><strong>' . t('This is the published revision.') . '</strong></div>';
      // Provide an unpublish link.
      $next_states = workbench_moderation_states_next(workbench_moderation_state_published(), $user, $node);
      if (!empty($next_states)) {
        $attributes = array();
        $attributes['class'] = array('moderation-action-unpublish');

        if (_workbench_moderation_access('unpublish', $node)) {
          $row['data']['revision'] .= '<div class="moderation-actions">' . l(t('Unpublish'), "node/{$revision->nid}/moderation/{$revision->live_revision}/unpublish", array('attributes' => $attributes)) . '</div>';
        }
      }
    }
    elseif ($revision->vid == $node->workbench_moderation['current']->vid) {
      $node_current = workbench_moderation_node_current_load($node);
      $row['data']['moderation'] = '<div class="moderation-state"><strong>This is the current revision.<br></strong>' . t('The current state is: <strong>%state</strong>.', array('%state' => workbench_moderation_state_label($node->workbench_moderation['current']->state))) . '<br></div><br>' . $row['data']['revision'];
    }

    $row['data']['revision'] .= '<div class="revision-actions">' . implode(' | ', array_filter($revision_operations)) . '</div>';

    // Build a table of the moderation history.
    $moderation_rows = array();
    $items = array();
    foreach ($moderations as $moderation) {
      if ($moderation->from_state) {
        $items[] = t('From <strong>%from_state</strong> &rarr; <strong>%to_state</strong><br> on %date<br> by !user',
          array(
            '%from_state' => workbench_moderation_state_label($moderation->from_state),
            '%to_state' => workbench_moderation_state_label($moderation->state),
            '%date' => format_date($moderation->stamp, 'short'),
            '!user' => theme('username', array('account' => $moderation)),
          )
        );
      }
      else {
        $items[] = t('Created as %to_state on %date by !user',
          array(
            '%to_state' => workbench_moderation_state_label($moderation->state),
            '%date' => format_date($moderation->stamp, 'short'),
            '!user' => theme('username', array('account' => $moderation)),
          )
        );
      }
    }

    if (empty($items)) {
      $row['data']['moderation'] = format_date($revision->timestamp, 'short');
    }
    else {
      $row['data']['moderation'] .= theme('item_list', array('items' => $items));
    }

    $rows[] = $row;
  }

  // If this page is being re-loaded as a result of the revision form being
  // submitted, it is not until this point that we can accurately check
  // if the moderation state is published or not.
  // Alert if there is no live node
  if (!isset($node->workbench_moderation['published'])) {
    drupal_set_message(t('Currently there is no published revision of this node.'), 'warning');
  }

  // Set the table header.
  $header = array(t('Revision'), t('Workflow Notes'), t('Actions'), t('Workflow Log'));

  // Return properly styled output.
  $build['pager_table'] = array(
    '#caption' => t("Workflow Table"),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'workbench_moderation') . '/css/workbench_moderation.css',
      ),
    ),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'workbench_moderation-workflow_table', 'class' => array('workflow_table'), 'summary' => 'This table provides a history of the workflow changes and conditionally grants actions to perform on specific revisions.'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Form to unpublish the live revision.
 *
 * @param $node
 *   The node being acted upon.
 *
 * @return
 *   A Drupal confirmation form to unpublish the live revision.
 */
function workbench_moderation_node_unpublish_form($form, &$form_state, $node) {
  global $user;

  // Ensure that this is the live revision.
  if (isset($node->workbench_moderation['published']->vid) && $node->vid != $node->workbench_moderation['published']->vid) {
    drupal_set_message(t('This is not the live revision of this node.'), 'error');
    drupal_goto("node/{$node->nid}/moderation");
  }

  $form = array();
  $form['node'] = array(
    '#value' => $node,
    '#type' => 'value',
  );
  $form['message'] = array(
    '#markup' =>  '<p>' . t('Are you sure that you want to unpublish the live revision of this content?') . '</p>',
  );

  $current_state = $node->workbench_moderation['my_revision']->state;
  if ($next_states = workbench_moderation_states_next($current_state, $user, $node)) {
    $form['state'] = array(
      '#title' => t('Set moderation state'),
      '#type' => 'select',
      '#options' => $next_states,
      '#default_value' => _workbench_moderation_default_next_state($current_state, $next_states),
    );
  }
  else {
    $form['message']['#markup'] .= '<p>' . t('The current live revision will be set to moderation state %state.', array('%state' => workbench_moderation_state_label(workbench_moderation_state_none()))) . '</p>';
    $form['state'] = array(
      '#type' => 'value',
      '#value' => workbench_moderation_state_none(),
    );
  }

  return confirm_form($form, t('Unpublish %title?', array('%title' => $node->title)), "node/{$node->nid}/moderation", '', t('Unpublish'));
}

/**
 * Submit handler for unpublishing a live revision of a node.
 */
function workbench_moderation_node_unpublish_form_submit($form, &$form_state) {
  global $user;
  $node = $form['node']['#value'];

  // Moderate the revision. This will do the heavy lifting.
  workbench_moderation_moderate($node, $form_state['values']['state']);

  drupal_set_message(t('The live revision of this content has been unpublished.'));
  $form_state['redirect'] ="node/{$node->nid}/moderation";
}
