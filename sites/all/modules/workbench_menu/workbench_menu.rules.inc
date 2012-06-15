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
 * Implements hook_rules_condition_info() on behalf of the node module.
 */
function workbench_menu_rules_condition_info() {
  $items = array();

  $items['workbench_menu_has_path'] = array(
    'group' => t("Workbench"),
    'label' => t("Workbench menu has path"),
    'base' => 'workbench_menu_has_path_rules_condition',
    'parameter' => array(
      'access_id' => array('type' => 'text', 'label' => t("Access ID")),
      'path' => array('type' => 'text', 'label' => t("Path"), 'optional' => TRUE),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function workbench_menu_rules_action_info() {
  $items = array();

  $items['workbench_menu_clear_cache'] = array(
    'label' => t("Clear workbench menu cache"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_clear_cache_rules_action',
    'parameter' => array(
      'menu' => array(
        'type' => 'workbench_menu',
        'label' => t("Menu"),
      ),
    ),
  );

  $items['workbench_menu_clear_cache_at_path'] = array(
    'label' => t("Clear workbench menu cache at path"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_clear_cache_at_path_rules_action',
    'parameter' => array(
      'path' => array(
        'type' => 'text',
        'label' => t("Url Path"),
      ),
    ),
  );

  $items['workbench_menu_load_menu'] = array(
    'label' => t("Load workbench menu"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_load_menu_rules_action',
    'parameter' => array(
      'access_id' => array('type' => 'text', 'label' => t("Access ID")),
    ),
    'provides' => array(
      'menu' => array('type' => 'workbench_menu', 'label' => t("Menu")),
    ),
  );

  $items['workbench_menu_load_menus_by_path'] = array(
    'label' => t("Load workbench menus (using menu path)"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_load_menus_by_path_rules_action',
    'parameter' => array(
      'access_id' => array('type' => 'text', 'label' => t("Path"), 'optional' => TRUE),
    ),
    'provides' => array(
      'menus' => array('type' => 'list<workbench_menu>', 'label' => t("Menus")),
    ),
  );

  return $items;
}

/**
 * Condition: Check if workbench menu has a given path.
 *
 * @param $access_id
 *   A menus unique access id.
 * @param $path
 *   (optional) A path to compare the menu path against.
 *
 * @return
 *   TRUE/FALSE depending on if the menu has a given path.
 */
function workbench_menu_has_path_rules_condition($access_id, $path = '') {
  if (!is_numeric($access_id)) {
    return FALSE;
  }

  if (!is_string($path)) {
    if (module_exists('cf_error')) {
      cf_error::not_string('path');
    }
    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) return FALSE;

  reset($menus);
  $menu = current($menus);

  if (!is_object($menu)) return FALSE;

  return $menu->path == $path;
}

/**
 * Condition: Check if workbench menu has a given path.
 *
 * @param $menu_id
 *   A menus unique menu id.
 * @param $path
 *   (optional) A path to compare the menu path against.
 *
 * @return
 *   TRUE/FALSE depending on if the menu has a given path.
 */
function workbench_menu_has_path_by_menu_id_rules_condition($menu_id, $path = '') {
  if (!is_numeric($menu_id)) {
    if (module_exists('cf_error')) {
      cf_error::not_numeric('menu_id');
    }

    return FALSE;
  }

  if (!is_string($path)) {
    if (module_exists('cf_error')) {
      cf_error::not_string('path');
    }

    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('id' => $menu_id, 'fast' => NULL), 'id');

  if (empty($menus)) return FALSE;

  reset($menus);
  $menu = current($menus);

  if (!is_object($menu)) return FALSE;

  return $menu->path == $path;
}


/**
 * Action: Clear cache of a workbench menu for a given menu..
 *
 * $param object $menu
 *   A workbench menu object.
 */
function workbench_menu_clear_cache_rules_action($menu) {
  if (!is_object($menu)) {
    if (module_exists('cf_error')) {
      cf_error::invalid_object('menu');
    }

    return;
  }

  workbench_menu_clear_cache_at_path($menu->path);
}

/**
 * Action: Clear cache of a workbench menu at a given path.
 *
 * $param string $path
 *   A url path to clear.
 */
function workbench_menu_clear_cache_at_path_rules_action($path) {
  workbench_menu_clear_cache_at_path($path);
}

/**
 * Action: Load a single workbench menu using the access id.
 *
 * @param $access_id
 *   A menus unique access id.
 *
 * @return
 *   FALSE on any failure and an array containing the menu object otherwise.
 */
function workbench_menu_load_menu_rules_action($access_id) {
  if (!is_numeric($access_id)) {
    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) return FALSE;

  reset($menus);
  $menu = current($menus);

  if (!is_object($menu)) return FALSE;

  return array('menu' => $menu);
}

/**
 * Action: Load multiple workbench menus found at a given path.
 *
 * @param $path
 *   A path where the menus are located.
 *
 * @return
 *   An array of menu objects and an empty array on any problem.
 */
function workbench_menu_load_menus_by_path_rules_action($path) {
  if (!is_string($path)) {
    if (module_exists('cf_error')) {
      cf_error::not_string('path');
    }

    return array('menus' => array());
  }

  $menus = (array) workbench_menu_get_menus(array('path' => $path, 'fast' => NULL), 'id');

  return array('menus' => &$menus);
}
