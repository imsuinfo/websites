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
 * Implements hook_rules_event_info() on behalf of the node module.
 */
function quail_api_node_rules_event_info() {
  $items = array(
    'quail_api_node_after_validating' => array(
      'label' => t("After Accessibility Validating Content"),
      'group' => t("Node"),
      'variables' => rules_events_node_variables(t("Accessibility Validated Content"), FALSE),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  $items['quail_api_node_after_validating']['variables']['results'] = array(
    'type' => 'unknown',
    'label' => t("Accessibility Validation Results"),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function quail_api_node_rules_action_info() {
  $items = array(
    'quail_api_node_perform_validation' => array(
      'label' => t("Accessibility Validate Content"),
      'group' => t("Node"),
      'base' => 'quail_api_node_perform_validation_action',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t("Content that will be accessibility validated"),
        ),
      ),
      'provides' => array(
        'results' => array(
          'type' => 'unknown',
          'label' => t("Accessibility Validation Results"),
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
 *   An array containing the node object stored in the key called 'node'.
 */
function quail_api_node_perform_validation_action($node) {
  if (!is_object($node)){
    cf_error_invalid_object(__FUNCTION__, 'node', WATCHDOG_ERROR);
    return array('node' => $node);
  }

  // @fixme: there seems to be some sort of bug in the rules module that causes actions to be executed twice.
  static $execute_once;

  if ($execute_once){
    return;
  }
  $execute_once = TRUE;

  $results = quail_api_node_validate(array($node), NULL, NULL, array(__FUNCTION__));

  if (isset($results[$node->nid]['report'])){
    $results = $results[$node->nid]['report'];
    $methods = quail_api_get_validation_methods(NULL, array(__FUNCTION__));
    $database = isset($methods[$node->accessibility['method']]['database']) ? $methods[$node->accessibility['method']]['database'] : FALSE;

    if ($database && !empty($results)){
      $no_failures = TRUE;

      foreach($results as $severity => $severity_results){
        if (isset($severity_results['total']) && $severity_results['total'] > 0){
          $no_failures = FALSE;
          break;
        }
      }

      if ($no_failures){
        quail_api_node_delete_node_tests($node->nid, $node->vid);
      }
      else {
        quail_api_node_save_node_tests($node->nid, $node->vid, $results);
      }
    }
  }

  return array('node' => $node, 'results' => $results);
}
