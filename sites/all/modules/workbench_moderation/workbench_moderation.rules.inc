<?php

/**
 * Implements hook_rules_file_info().
 */
function workbench_moderation_rules_file_info() {
  $items = array();
  $items[] = 'workbench_moderation.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function workbench_moderation_rules_condition_info() {
   $items = array();

   $items['content_is_live_revision'] = array(
    'group' => t("Node"),
    'label' => t("Content is live revision"),
    'base' => 'workbench_moderation_rules_condition_content_is_live_revision',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

   $items['contents_current_state'] = array(
     'group' => t('Node'),
     'label' => t('Contents current moderation state'),
     'base' => 'workbench_moderation_rules_condition_contents_current_state',
     'parameter' => array(
       'node' => array('type' => 'node', 'label' => t('Content')),
       'moderation_state' => array(
         'type' => 'text',
         'label' => t('Workbench moderation state'),
         'options list' => 'workbench_moderation_state_labels',
         'restriction' => 'input',
         'save' => TRUE,
       ),
     ),
     'access callback' => 'rules_node_integration_access',
   );

  return $items;
}

/**
 * Implements hook_rules_event_info().
 */
function workbench_moderation_rules_event_info() {
  $items = array();

  $items['workbench_moderation_after_unpublishing_live_content'] = array(
    'label' => t("After unpublishing live content"),
    'group' => t("Node"),
    'variables' => rules_events_node_variables(t("Unpublished content"), FALSE),
    'access callback' => 'rules_node_integration_access',
  );

  $items['workbench_moderation_after_unpublishing_live_content']['variables']['live_content'] = array(
    'type' => 'node',
    'label' => t("Live workbench content"),
  );

  $items['workbench_moderation_after_changing_state'] = array(
    'label' => t("After changing workbench moderation state"),
    'group' => t("Node"),
    'variables' => rules_events_node_variables(t("Content"), FALSE),
    'access callback' => 'rules_node_integration_access',
  );

  $items['workbench_moderation_after_changing_state']['variables']['new_revision'] = array(
    'type' => 'unknown',
    'label' => t("New revision"),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the workbench_moderation module.
 */
function workbench_moderation_rules_action_info() {
  $items = array();

  $items['workbench_moderation_set_state'] = array(
    'label' => t("Set moderation state"),
    'group' => t("Node"),
    'base' => 'workbench_moderation_set_state_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
      'moderation_state' => array(
        'type' => 'text',
        'label' => t('Workbench moderation state'),
        'options list' => 'workbench_moderation_state_labels',
        'restriction' => 'input',
      ),
    ),
  );

  return $items;
}

/**
 * Condition: Check if the content is live revision
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the content is live revision.
 */
function workbench_moderation_rules_condition_content_is_live_revision($node) {
  if (!is_object($node)) {
    return FALSE;
  }

  return workbench_moderation_node_is_current($node);
}

/**
 * Condition: Check if workbench moderation state matched selected state.
 *
 * @param $node
 *   A node object
 *
 * @return
 *  TRUE/FALSE depending on if the nodes current state matches selected state.
 */
function workbench_moderation_rules_condition_contents_current_state($node, $moderation_state) {
  if (!is_object($node)) {
    return FALSE;
  }

  if ($node->workbench_moderation['current']->state != $moderation_state) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Action: Change the moderation state of a given node.
 *
 * $param $node
 *   A node object
 *
 * $param $moderation_state
 *   A machine-name string of the desired moderation state to assign.
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function workbench_moderation_set_state_rules_action($node, $moderation_state) {
  if (is_object($node) && !empty($moderation_state)){
    actions_do('workbench_moderation_set_state_action', $node, array('state' => $moderation_state));
  }

  return array('node' => $node);
}

