<?php

/**
 * Implements hook_rules_file_info().
 */
function quail_api_node_rules_file_info() {
  $items = array();
  $items[] = 'quail_api_node.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function quail_api_node_rules_condition_info() {
   $items = array();

   $items['content_is_using_quail_api_node'] = array(
    'group' => t("Node"),
    'label' => t("Content is using accessibility validation"),
    'base' => 'quail_api_node_rules_condition_content_is_using_quail_api_node',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  $items['content_is_requiring_quail_api_validation'] = array(
    'group' => t("Node"),
    'label' => t("Content is requiring accessibility validation"),
    'base' => 'quail_api_node_rules_condition_content_is_requiring_quail_api_node',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}

/**
 * Implements hook_rules_event_info() on behalf of the node module.
 */
function quail_api_node_rules_event_info() {
  $items = array(
    'quail_api_node_after_validating' => array(
      'label' => t("After accessibility validating content"),
      'group' => t("Node"),
      'variables' => rules_events_node_variables(t("Accessibility validated content"), FALSE),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  $items['quail_api_node_after_validating']['variables']['validation_results'] = array(
    'type' => 'unknown',
    'label' => t("Validation results"),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function quail_api_node_rules_action_info() {
  $items = array(
    'quail_api_node_validate_action' => array(
      'label' => t("Accessibility validate content"),
      'group' => t("Node"),
      'base' => 'quail_api_node_validate_rules_action',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t("Content that will be accessibility validated"),
        ),
      ),
      'provides' => array(
        'validation_results' => array(
          'type' => 'unknown',
          'label' => t("Validation results"),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Action: Perform accessibility validation on a node.
 *
 * $param $node
 *   A node object
 *
 * @return
 *   An array containing the node object stored in the key called 'node' and the validation results (if any) stored in the key called 'results'.
 */
function quail_api_node_validate_rules_action($node) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_object($node)){
    cf_error_invalid_object($function_history, 'node');
    return array('node' => $node);
  }

  $results = actions_do('quail_api_node_validate_action', $node);

  if (!isset($results['quail_api_node_validate_action'])){
    return array('node' => $node, 'validation_results' => $results['quail_api_node_validate_action']);
  }

  return array('node' => $node);
}

/**
 * Condition: Check if the content is using accessibility validation.
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the content is using quail api node validation.
 */
function quail_api_node_rules_condition_content_is_using_quail_api_node($node) {
  if (!is_object($node)) {
    return FALSE;
  }

  return quail_api_node_is_enabled($node->type);
}

/**
 * Condition: Check if the content is requiring accessibility validation.
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the content is requiring quail api node validation.
 */
function quail_api_node_rules_condition_content_is_requiring_quail_api_node($node) {
  if (!is_object($node)) {
    return FALSE;
  }

  return quail_api_node_is_required($node->type);
}
