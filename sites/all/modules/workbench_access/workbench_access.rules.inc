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

   $items['contents_current_access_section'] = array(
     'group' => t("Node"),
     'label' => t("Contents current access control section"),
     'base' => 'workbench_access_rules_condition_contents_current_access_section',
     'parameter' => array(
       'node' => array('type' => 'node', 'label' => t("Content")),
       'access_section' => array(
         'type' => 'text',
         'label' => t("Workbench access section"),
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

  $items['workbench_access_assign_access_section'] = array(
    'label' => t("Assign access control section (presave)"),
    'group' => t("Node"),
    'base' => 'workbench_access_assign_access_section_presave_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
      'access_section' => array(
        'type' => 'text',
        'label' => t("Workbench access control section"),
        'options list' => 'workbench_access_active_options',
        'restriction' => 'input',
      ),
    ),
  );

  $items['workbench_access_unassign_access_section'] = array(
    'label' => t("Unassign access control section (presave)"),
    'group' => t("Node"),
    'base' => 'workbench_access_unassign_access_section_presave_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
      'access_section' => array(
        'type' => 'text',
        'label' => t("Workbench access control section"),
        'options list' => 'workbench_access_active_options',
        'restriction' => 'input',
      ),
    ),
  );

  $items['workbench_access_load_access_section'] = array(
    'label' => t("Load access control section"),
    'group' => t("Node"),
    'base' => 'workbench_access_load_access_section_rules_action',

    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t("Content"),
      ),
    ),
    'provides' => array(
      'workbench_access_sections' => array(
        'type' => 'list<taxonomy_term>',
        'label' => t("Workbench Access Sections"),
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
 * Condition: Check if workbench access section matched selected section.
 *
 * @param $node
 *   A node object
 * @param $access_section
 *   A node object
 *
 * @return
 *  TRUE/FALSE depending on if the nodes current section matches selected section.
 */
function workbench_access_rules_condition_contents_current_access_section($node, $access_section) {
  if (!is_object($node) || !property_exists($node, 'workbench_access_id')) {
    return FALSE;
  }

  if (is_array($node->workbench_access_id)){
   return in_array($access_section, $node->workbench_access_id);
  }

  return $node->workbench_access_id == $access_section;
}

/**
 * Action: Assign an access section to a given node.
 *
 * $param $node
 *   A node object
 *
 * $param $access_section
 *   The desired access section to assign.
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function workbench_access_assign_access_section_presave_rules_action($node, $access_section) {
  if (is_object($node) && !empty($access_section)){
    actions_do('workbench_access_assign_section_presave_action', $node, array('section' => $access_section));
  }

  return array('node' => $node);
}

/**
 * Action: Unassign an access section from a given node.
 *
 * $param $node
 *   A node object
 *
 * $param $access_section
 *   The desired access section to assign.
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function workbench_access_unassign_access_section_presave_rules_action($node, $access_section) {
  if (is_object($node) && !empty($access_section)){
    actions_do('workbench_access_unassign_section_presave_action', $node, array('section' => $access_section));
  }

  return array('node' => $node);
}

/**
 * Action: Loads the workbench access section into a variable.
 *
 * $param $node
 *   A node object
 *
 * @return
 *   An array containing the node object stored in the key called 'node' and an array containing the access control section for the given node.
 */
function workbench_access_load_access_section_rules_action($node) {
  $sections = array();

  if (is_object($node)) {
    if (property_exists($node, 'workbench_access_id')) {
      if (is_array($node->workbench_access_id)) {
        $sections = $node->workbench_access_id;
      }
      else {
        $sections = array($node->workbench_access_id);
      }
    }
    else if (property_exists($node, 'workbench_access')) {
      if (is_array($node->workbench_access)) {
        $sections = $node->workbench_access;
      }
      else {
        $sections = array($node->workbench_access);
      }
    }
  }

  return array('node' => $node, 'workbench_access_sections' => $sections);
}

