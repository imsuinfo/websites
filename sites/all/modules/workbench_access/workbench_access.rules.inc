<?php

/**
 * Implements hook_rules_file_info().
 */
function workbench_access_rules_file_info() {
  $items = array();
  $items[] = 'workbench_access.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function workbench_access_rules_condition_info() {
   $items = array();

   $items['content_is_using_workbench_access'] = array(
    'group' => t("Node"),
    'label' => t("Content is using workbench access control"),
    'base' => 'workbench_access_rules_condition_content_is_using_workbench_access',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

   $items['contents_current_access_state'] = array(
     'group' => t("Node"),
     'label' => t("Contents current access control state"),
     'base' => 'workbench_access_rules_condition_contents_current_access_state',
     'parameter' => array(
       'node' => array('type' => 'node', 'label' => t("Content")),
       'access_state' => array(
         'type' => 'text',
         'label' => t("Workbench access state"),
         'options list' => 'workbench_access_active_options',
         'restriction' => 'input',
         'save' => TRUE,
       ),
     ),
     'access callback' => 'rules_node_integration_access',
   );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the workbench_access module.
 */
function workbench_access_rules_action_info() {
  $items = array();

  $items['workbench_access_assign_access_state'] = array(
    'label' => t("Assign access control state (presave)"),
    'group' => t("Node"),
    'base' => 'workbench_access_assign_access_state_presave_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
      'access_state' => array(
        'type' => 'text',
        'label' => t("Workbench access control state"),
        'options list' => 'workbench_access_active_options',
        'restriction' => 'input',
      ),
    ),
  );

  $items['workbench_access_unassign_access_state'] = array(
    'label' => t("Unassign access control state (presave)"),
    'group' => t("Node"),
    'base' => 'workbench_access_unassign_access_state_presave_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
      'access_state' => array(
        'type' => 'text',
        'label' => t("Workbench access control state"),
        'options list' => 'workbench_access_active_options',
        'restriction' => 'input',
      ),
    ),
  );

  $items['workbench_access_load_access_states'] = array(
    'label' => t("Load access control states"),
    'group' => t("Node"),
    'base' => 'workbench_access_load_access_states_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
    ),
    'provides' => array(
      'workbench_access_states' => array(
        'type' => 'unknown',
        'label' => t("Workbench Access States"),
      ),
    ),
  );

  return $items;
}

/**
 * Condition: Check if the content is using workbench access control.
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the content is using workbench access control.
 */
function workbench_access_rules_condition_content_is_using_workbench_access($node) {
  if (!is_object($node)) {
    return FALSE;
  }

  return variable_get('workbench_access_node_type_' . $node->type, 1) == 1;
}

/**
 * Condition: Check if workbench moderation state matched selected state.
 *
 * @param $node
 *   A node object
 * @param $access_state
 *   A node object
 *
 * @return
 *  TRUE/FALSE depending on if the nodes current state matches selected state.
 */
function workbench_access_rules_condition_contents_current_access_state($node, $access_state) {
  if (!is_object($node)) {
    return FALSE;
  }

  if (is_array($node->workbench_access_id)){
   return in_array($access_state, $node->workbench_access_id);
  }

  return $node->workbench_access_id == $access_state;
}

/**
 * Action: Assign an access state to a given node.
 *
 * $param $node
 *   A node object
 *
 * $param $access_state
 *   The desired access state to assign.
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function workbench_access_assign_access_state_presave_rules_action($node, $access_state) {
  if (is_object($node) && !empty($access_state)){
    actions_do('workbench_access_assign_section_presave_action', $node, array('section' => $access_state));
  }

  return array('node' => $node);
}

/**
 * Action: Unassign an access state from a given node.
 *
 * $param $node
 *   A node object
 *
 * $param $access_state
 *   The desired access state to assign.
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function workbench_access_unassign_access_state_presave_rules_action($node, $access_state) {
  if (is_object($node) && !empty($access_state)){
    actions_do('workbench_access_unassign_section_presave_action', $node, array('section' => $access_state));
  }

  return array('node' => $node);
}

/**
 * Action: Loads the workbench access states into a variable.
 *
 * $param $node
 *   A node object
 *
 * @return
 *   An array containing the node object stored in the key called 'node' and an array containing the access control states for the given node.
 */
function workbench_access_load_access_states_rules_action($node) {
  $states = array();

  if (is_object($node) && property_exists($node, 'workbench_access_id')){
    $states = $node->workbench_access_id;
  }

  return array('node' => $node, 'workbench_access_states' => $states);
}

