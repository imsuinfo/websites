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

   $items['content_is_live'] = array(
    'group' => t("Node"),
    'label' => t("Content is Live"),
    'base' => 'workbench_moderation_rules_condition_content_is_live',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
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
    'label' => t("After Unpublishing Live Content"),
    'group' => t("Node"),
    'variables' => rules_events_node_variables(t("Unpublished Content"), FALSE),
    'access callback' => 'rules_node_integration_access',
  );

  $items['workbench_moderation_after_unpublishing_live_content']['variables']['live_content'] = array(
    'type' => 'node',
    'label' => t("Live Content"),
  );

  $items['workbench_moderation_after_changing_state'] = array(
    'label' => t("After Changing Moderation State"),
    'group' => t("Node"),
    'variables' => rules_events_node_variables(t("Content"), FALSE),
    'access callback' => 'rules_node_integration_access',
  );

  $items['workbench_moderation_after_changing_state']['variables']['new_revision'] = array(
    'type' => 'unknown',
    'label' => t("New Revision"),
  );

  return $items;
}

/**
 * TODO: Implements hook_rules_action_info() on behalf of the node module.
 *
function workbench_moderation_rules_action_info() {
  $items = array();

  $items['workbench_moderation_set_state' = array(
    'label' => t("Set Moderation State"),
    'group' => t("Node"),
    'base' => 'workbench_moderation_set_state_action',
    'parameter' => killfile_rules_actions_parameters(t("Content")),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}
//*/

/**
 * Condition: Check if the content is live
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the content is live.
 */
function workbench_moderation_rules_condition_content_is_live($node) {
  if (!is_object($node)){
    return FALSE;
  }

  return workbench_moderation_node_is_current($node);
}
