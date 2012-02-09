<?php

/**
 * @file
 * Rules integration file for node accessibility.
 */

/**
 * Implements hook_rules_file_info().
 */
function workbench_menu_rules_file_info() {
  $items = array();
  $items[] = 'workbench_menu.rules';

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function workbench_menu_rules_action_info() {
  $items = array(
    'workbench_menu_clear_cache_at_path' => array(
      'label' => t("Clear workbench menu cache at path"),
      'group' => t("Workbench"),
      'base' => 'workbench_menu_clear_cache_at_path_rules_action',
      'parameter' => array(
        'path' => array(
          'type' => 'unknown',
          'label' => t("The url path whose workbench menu is to be cleared."),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Action: Clear cache of a workbench menu at a given path.
 *
 * $param string $path
 *   A url path to clear.
 */
function workbench_menu_clear_cache_at_path_rules_action($path) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  workbench_menu_clear_cache_at_path($path, $function_history);
}
