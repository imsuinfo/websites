<?php

/**
 * @file
 * Workbench Menu admin file.
 */

/**
 * Display the workbench menu edit page for menus.
 *
 * @param string|null $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int|null $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return array
 *   An editing form or a list of section menus.
 *
 * @see workbench_menu_edit_settings_form()
 */
function workbench_menu_edit_settings($access_type = NULL, $access_id = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if (!(is_null($access_type) && is_null($access_id))) {
    return drupal_get_form('workbench_menu_edit_settings_form', $access_type, $access_id);
  }

  $access = workbench_access_get_access_tree();
  $active = workbench_access_get_active_tree();

  $output = '<h2>' . t("Menus") . '</h2>';
  $output .= '<p>' . t("The following menus are currently available for editing.");

  if ($active === FALSE) {
    $output .= '<br>' . workbench_access_sections_needed_message();

    $active = array();
    $active['tree'] = array();
  }

  $output .= '</p>';
  $sorted = array();

  foreach ($active['tree'] as $access_id => &$section) {
    if (!is_numeric($access_id)) continue;

    $sorted[$access_id] = &$section['name'];
  }

  asort($sorted);

  $menus = (array) workbench_menu_get_menus(array('access_id' => array_keys($sorted), 'fast' => NULL), 'access_id');

  $rows = array();
  foreach ($sorted as $access_id => &$section_name) {
    if (!array_key_exists($access_id, $access)) {
      continue;
    }

    if (!isset($active['active'][$access_id])) {
      continue;
    }

    $row = array();
    $row[] = l($section_name, 'admin/workbench/menus/' . $active['access_scheme']['access_type'] . '/' . $access_id);

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'path')) {
      $default_path = $menus[$access_id]->path;

      // be user-friendly and show normal spaces instead of %20.
      $row[] = preg_replace('/%20/', ' ', $default_path);
    }
    else {
      $row[] = "";
    }

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'id')) {
      $row[]  = intval($menus[$access_id]->id);
    }
    else {
      $row[] = "";
    }

    $row[] = intval($access_id);

    $rows[] = $row;
  }

  $header = array(t("Menu Name"), t("Menu Path"), t("Menu ID"), t("Access ID"));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("There are no menus available for you to edit.")));
  $build['content']['#markup'] = $output;
  return $build;
}

/**
 * Generate a user overview form for a section.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param string $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return array
 *   The built form array.
 */
function workbench_menu_edit_settings_form($form, &$form_state, $access_type, $access_id) {
  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $form = array();
  $access = workbench_access_get_access_tree();
  $active = workbench_access_get_active_tree();
  $active['access_scheme']['access_id'] = $access_id;

  if ($active['access_scheme']['access_type'] != $access_type || !isset($active['active'][$access_id])) {
    drupal_access_denied();
    drupal_exit();
  }

  $default_path = '';
  $default_weight = 0;
  $default_show_parents = TRUE;
  $default_hidden = FALSE;
  $default_breadcrumb_name = '';

  $term = taxonomy_term_load($access_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/workbench/menus';
  $items = array();

  // the menu edit form should always pull the latest data from the database
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id), NULL);

  if (!empty($menus)) {
    $menu = array_pop($menus);

    if (property_exists($menu, 'path')) {
      $default_path = &$menu->path;

      // be user-friendly and show normal spaces instead of %20.
      $default_path = preg_replace('/%20/', ' ', $default_path);
    }

    if (property_exists($menu, 'weight')) {
      $default_weight = &$menu->weight;
    }

    if (property_exists($menu, 'show_parents')) {
      $default_show_parents = &$menu->show_parents;
    }

    if (property_exists($menu, 'hidden')) {
      $default_hidden = &$menu->hidden;
    }

    if (property_exists($menu, 'breadcrumb_name')) {
      $default_breadcrumb_name = &$menu->breadcrumb_name;
    }

    if (property_exists($menu, 'items')) {
      $items = &$menu->items;
    }
  }
  else {
    drupal_not_found();
    drupal_exit();
  }

  drupal_set_title(t("Menu Settings for %menu Menu (ID: %id)", array('%menu' => $term->name, '%id' => $menu->id)), PASS_THROUGH);

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Path"),
    '#default_value' => $default_path,
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t("Specify a relative url path in which this menu will be displayed on.") . '<br>' .
      t("Must not begin with a /.") . '<br>' .
      t("Changing this auto-resets the menu cache for this menu."),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t("Advanced Settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $form['advanced']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Weight"),
    '#required' => TRUE,
    '#description' =>
      t("The menu weight is a number used to define where an item is positioned when there are more than one menu using the same menu path."),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $default_weight,
  );

  $form['advanced']['show_parents'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show Parents"),
    '#default_value' => $default_show_parents,
    '#description' => t("Specify whether or not parent menus should be displayed or not.") . '<br>' .
      t("A parent menu is a menu that appears in a url path above the current path such that if the current path was '/a/b', the parent menus for '/a/b' would be at '/a'."),
  );

  $form['advanced']['hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t("Hidden"),
    '#default_value' => $default_hidden,
    '#description' => t("Specify whether or not this menu should be displayed or not.") . '<br>' .
      t("A hidden menu may only be displayed if used as a menu item menu via another menu."),
  );

  $form['advanced']['breadcrumb_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Breadcrumb Name"),
    '#size' => 64,
    '#maxlength' => 128,
    '#default_value' => $default_breadcrumb_name,
    '#description' => t("Specify whether or not a custom breadcrumb name should be used.") . '<br>' .
      t("If left empty, then the taxonomy name (aka: group name) is used instead.") . '<br>' .
      t("Changing this auto-resets the menu cache for this menu."),
  );

  if (!(user_access('change workbench menu path') || user_access('administer workbench menu'))) {
    $form['path']['#disabled'] = TRUE;
  }

  if (!(user_access('change workbench menu parent') || user_access('administer workbench menu'))) {
    // completely hide field(s) from unprivileged users.
    unset($form['advanced']['show_parents']);
  }

  if (!(user_access('change workbench menu hidden') || user_access('administer workbench menu'))) {
    // completely hide field(s) from unprivileged users.
    unset($form['advanced']['hidden']);
  }

  if (!(user_access('change workbench menu hidden') || user_access('administer workbench menu'))) {
    // completely hide field(s) from unprivileged users.
    unset($form['advanced']['hidden']);
  }

  if (!(user_access('change workbench menu breadcrumb') || user_access('administer workbench menu'))) {
    // completely hide field(s) from unprivileged users.
    unset($form['advanced']['breadcrumb_name']);
  }

  workbench_menu_build_item_editing_form($form, 'admin/workbench/menus/taxonomy/' . $access_id, $items);

  $form['workbench_access'] = array(
    '#type' => 'value',
    '#value' => $active['access_scheme'],
  );

  $form['access_id']= array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['menu_id']= array(
    '#type' => 'value',
    '#value' => $menu->id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination . '/taxonomy/' . $access_id,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Update Menu"),
  );

  if (!empty($default_path)) {
    $form['actions']['reset_cache'] = array(
      '#type' => 'submit',
      '#value' => t("Reset Cache"),
      '#weight' => 10,
      '#submit' => array('workbench_menu_reset_cache_form'),
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for workbench_menu_edit_settings_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_settings_form_validate()
 */
function workbench_menu_edit_settings_form_validate($form, &$form_state) {
  workbench_menu_admin_settings_form_validate($form, $form_state);
}

/**
 * Submit process for workbench_menu_edit_settings_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_settings_form_submit()
 */
function workbench_menu_edit_settings_form_submit($form, &$form_state) {
  workbench_menu_admin_settings_form_submit($form, $form_state);
}

/**
 * Theme function for presenting a drag and drop table.
 *
 * @param array $element
 *   A form element array to be renderred.
 *
 * @see theme_workbench_menu_admin_settings_form()
 */
function theme_workbench_menu_edit_settings_form($element) {
  return theme_workbench_menu_admin_settings_form($element);
}

/**
 * Display the workbench menu administration page for menu items.
 *
 * @param string $access_type
 *   The type of access requested (e.g. taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $operation (optional)
 *   A string representing the operation to perform, which can be:
 *   - add: Add a new item.
 *   - edit: Edit an existing item, this requires the $item parameter
 *   to be defined.
 *   - delete: Delete an existing item, this requires the $item
 *   parameter to be defined.
 * @param $item (optional)
 *   A conditionally optional parameter that is the item id for a
 *   given item.
 *
 * @return
 *   An edit existing or add new form for section menus.
 *
 * @see workbench_menu_admin_item_settings_add_form()
 * @see workbench_menu_admin_item_settings_edit_form()
 * @see workbench_menu_admin_item_settings_delete_form()
 */
function workbench_menu_edit_item_settings($access_type, $access_id, $operation = NULL, $item = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $return_destination = 'admin/workbench/menus/' . $access_type . '/' . $access_id;

  if ($operation === 'add') {
    return drupal_get_form('workbench_menu_admin_item_settings_add_form', $access_type, $access_id, $return_destination);
  }
  elseif ($operation === 'edit' && !is_null($item)) {
    return drupal_get_form('workbench_menu_admin_item_settings_edit_form', $access_type, $access_id, $item, $return_destination);
  }
  elseif ($operation === 'delete' && !is_null($item)) {
    return drupal_get_form('workbench_menu_admin_item_settings_delete_form', $access_type, $access_id, $item, $return_destination);
  }

  drupal_not_found();
  drupal_exit();
}

/**
 * Display the workbench menu administration page for menus.
 *
 * @param string|null $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int|null $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return array
 *   An editing form or a list of section menus.
 *
 * @see workbench_menu_admin_settings_form()
 */
function workbench_menu_admin_settings($access_type = NULL, $access_id = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if (!(is_null($access_type) && is_null($access_id))) {
    return drupal_get_form('workbench_menu_admin_settings_form', $access_type, $access_id);
  }

  $active = workbench_access_get_active_tree();

  $output = '<h2>' . t("Menus") . '</h2>';
  $output .= '<p>' . t("The following menus are currently available for editing.");

  if ($active === FALSE) {
    $output .= '<br>' . "\n" . workbench_access_sections_needed_message();

    $active = array();
    $active['tree'] = array();
  }
  else if (user_access('administer workbench access')) {
    $output .= '<br>' . "\n" . t("You may <a href=\"!url\">enable or disable sections</a>.", array('!url' => url('admin/config/workbench/access/sections')));
  }

  $output .= '</p>';

  $menus = (array) workbench_menu_get_menus(array('access_id' => array_keys($active['tree']), 'fast' => NULL), 'access_id');

  $rows = array();
  foreach ($active['tree'] as $access_id => &$section) {
    if (!isset($active['active'][$access_id])) {
      continue;
    }

    $row = array();

    if (is_numeric($access_id) && !empty($active['access_scheme']['access_type'])) {
      $row[] = str_repeat('- ', $section['depth'] ) . ' ' . l($section['name'], 'admin/config/workbench/access/menus/' . $active['access_scheme']['access_type'] . '/' . $access_id);
    }
    else {
      $row[] = str_repeat('- ', $section['depth'] ) . ' ' . $section['name'];
    }

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'path')) {
      $default_path = $menus[$access_id]->path;

      // be user-friendly and show normal spaces instead of %20.
      $row[] = preg_replace('/%20/', ' ', $default_path);
    }
    else {
      $row[] = "";
    }

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'id')) {
      $row[]  = intval($menus[$access_id]->id);
    }
    else {
      $row[] = "";
    }

    $row[] = intval($access_id);

    $rows[] = $row;
  }

  $header = array(t("Menu Name"), t("Menu Path"), t("Menu ID"), t("Access ID"));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("There are no menus available for you to edit.")));
  $build['content']['#markup'] = $output;
  return $build;
}

/**
 * Generate a user overview form for a section.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param string $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return
 *   The built form array.
 */
function workbench_menu_admin_settings_form($form, &$form_state, $access_type, $access_id) {
  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $form = array();
  $active = workbench_access_get_active_tree();
  $active['access_scheme']['access_id'] = $access_id;

  if ($active['access_scheme']['access_type'] != $access_type || !isset($active['active'][$access_id])) {
    drupal_access_denied();
    drupal_exit();
  }

  $default_path = '';
  $default_weight = 0;
  $default_show_parents = TRUE;
  $default_hidden = FALSE;
  $default_breadcrumb_name = '';

  $term = taxonomy_term_load($access_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/workbench/access/menus';
  $items = array();

  // the menu edit form should always pull the latest data from the database
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id), NULL);

  if (!empty($menus)) {
    $menu = array_pop($menus);

    if (property_exists($menu, 'path')) {
      $default_path = &$menu->path;

      // be user-friendly and show normal spaces instead of %20.
      $default_path = preg_replace('/%20/', ' ', $default_path);
    }

    if (property_exists($menu, 'weight')) {
      $default_weight = &$menu->weight;
    }

    if (property_exists($menu, 'show_parents')) {
      $default_show_parents = &$menu->show_parents;
    }

    if (property_exists($menu, 'hidden')) {
      $default_hidden = &$menu->hidden;
    }

    if (property_exists($menu, 'breadcrumb_name')) {
      $default_breadcrumb_name = &$menu->breadcrumb_name;
    }

    if (property_exists($menu, 'items')) {
      $items = &$menu->items;
    }
  }
  else {
    drupal_not_found();
    drupal_exit();
  }

  drupal_set_title(t("Menu Settings for %menu Menu (ID: %id)", array('%menu' => $term->name, '%id' => $menu->id)), PASS_THROUGH);

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Path"),
    '#default_value' => $default_path,
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t("Specify a relative url path in which this menu will be displayed on.") . '<br>' .
      t("Must not begin with a /.") . '<br>' .
      t("Changing this auto-resets the menu cache for this menu."),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t("Advanced Settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $form['advanced']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Weight"),
    '#required' => TRUE,
    '#description' =>
      t("The menu weight is a number used to define where an item is positioned when there are more than one menu using the same menu path."),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $default_weight,
  );

  $form['advanced']['show_parents'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show Parents"),
    '#default_value' => $default_show_parents,
    '#description' => t("Specify whether or not parent menus should be displayed or not.") . '<br>' .
      t("A parent menu is a menu that appears in a url path above the current path such that if the current path was '/a/b', the parent menus for '/a/b' would be at '/a'."),
  );

  $form['advanced']['hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t("Hidden"),
    '#default_value' => $default_hidden,
    '#description' => t("Specify whether or not this menu should be displayed or not.") . '<br>' .
      t("A hidden menu may only be displayed if used as a menu item menu via another menu."),
  );

  $form['advanced']['breadcrumb_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Breadcrumb Name"),
    '#size' => 64,
    '#maxlength' => 128,
    '#default_value' => $default_breadcrumb_name,
    '#description' => t("Specify whether or not a custom breadcrumb name should be used.") . '<br>' .
      t("If left empty, then the taxonomy name (aka: group name) is used instead.") . '<br>' .
      t("Changing this auto-resets the menu cache for this menu."),
  );

  if (!(user_access('change workbench menu path') || user_access('administer workbench menu'))) {
    $form['path']['#disabled'] = TRUE;
  }

  if (!(user_access('change workbench menu parent') || user_access('administer workbench menu'))) {
    $form['advanced']['show_parents']['#disabled'] = TRUE;
  }

  if (!(user_access('change workbench menu hidden') || user_access('administer workbench menu'))) {
    $form['advanced']['hidden']['#disabled'] = TRUE;
  }

  if (!(user_access('change workbench menu breadcrumb') || user_access('administer workbench menu'))) {
    $form['advanced']['breadcrumb_name']['#disabled'] = TRUE;
  }

  workbench_menu_build_item_editing_form($form, 'admin/config/workbench/access/menus/taxonomy/' . $access_id, $items);

  $form['workbench_access'] = array(
    '#type' => 'value',
    '#value' => $active['access_scheme'],
  );

  $form['access_id']= array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['menu_id']= array(
    '#type' => 'value',
    '#value' => $menu->id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination . '/taxonomy/' . $access_id,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Update Menu"),
  );

  if (!empty($default_path)) {
    $form['actions']['reset_cache'] = array(
      '#type' => 'submit',
      '#value' => t("Reset Cache"),
      '#weight' => 10,
      '#submit' => array('workbench_menu_reset_cache_form'),
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for workbench_menu_admin_settings_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_settings_form_validate($form, &$form_state) {
  if (workbench_menu_menu_exists($form['menu_id']['#value']) !== TRUE) {
    form_set_error('menu_id', t("An invalid menu id has been specified."));
  }

  if (workbench_menu_menu_exists($form['access_id']['#value'], TRUE) !== TRUE) {
    form_set_error('access_id', t("An invalid access id has been specified."));
  }

  if (!empty($form_state['values']['path'])) {
    // be user-friendly and allow normal whitespaces on insert.
    $form_state['values']['path'] = preg_replace('/ /', '%20', $form_state['values']['path']);

    if (!valid_url($form_state['values']['path'], FALSE)) {
      form_set_error('path', t("The specified path is not a valid url path."));
    }

    if (preg_match('@^/@i', $form_state['values']['path']) !== 0) {
      form_set_error('path', t("The path must not begin with a /."));
    }
  }

  if (!is_numeric($form_state['values']['weight'])) {
    form_set_error('weight', t("The menu weight can only be a number."));
  }
}

/**
 * Submit process for workbench_menu_admin_settings_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_settings_form_submit($form, &$form_state) {
  $menu = array();
  $menu['access_id'] = &$form['workbench_access']['#value']['access_id'];
  $menu['path'] = &$form_state['values']['path'];
  $menu['weight'] = &$form_state['values']['weight'];
  $menu['show_parents'] = &$form_state['values']['show_parents'];
  $menu['hidden'] = &$form_state['values']['hidden'];
  $menu['breadcrumb_name'] = &$form_state['values']['breadcrumb_name'];
  $menu['items'] = array();

  $results = workbench_menu_save_menus(array($menu));

  if (!empty($form_state['values']['items'])) {
    foreach ($form_state['values']['items'] as $id => &$item) {
      if (!is_numeric($id)) continue;

      $primary_keys = array('id');

      $record = array();
      $record['id'] = $id;
      $record['parent_id'] = $item['parent'];
      $record['weight'] = $item['weight'];

      drupal_write_record('workbench_menu_items', $record, $primary_keys);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }

  if ($form['path']['#default_value'] != $form_state['values']['path']) {
    workbench_menu_clear_cache_at_path($form['path']['#default_value']);
    workbench_menu_clear_cache_at_path($form_state['values']['path']);
  }
  elseif (isset($form['advanced']['breadcrumb_name']['#default_value']) && $form['advanced']['breadcrumb_name']['#default_value'] != $form_state['values']['breadcrumb_name']) {
    workbench_menu_clear_cache_at_path($form['path']['#default_value']);
    workbench_menu_clear_cache_at_path($form_state['values']['path']);
  }
}

/**
 * Builds the editing form array.
 *
 * Provides the ability for users and administrators to sort, add,
 * and remove menu items.
 *
 * This function will not load the items.
 * Use the items parameter to pass the list of items to show.
 *
 * @param array $form
 *   The form array to append the item editing array to.
 * @param string $path_prefix
 *   A string that defines where the editing form links will begin.
 *   Such as: admin/config/workbench/access/menus/taxonomy/182.
 *   Do not include the leading /.
 *   Do not include the trailing /.
 * @param array $items
 *   (optional) An array containing any existing items to populate
 *   the array with.
 */
function workbench_menu_build_item_editing_form(&$form, $path_prefix, $items = array()) {
  $form['path_prefix'] = array(
    '#type' => 'value',
    '#value' => $path_prefix,
  );

  $form['items'] = array(
    '#tree' => TRUE,
  );

  $data['fields'] = &$form['items'];

  foreach ($items['nested'] as &$nested_item) {
    workbench_menu_build_item_editing_form_recursive_nest($nested_item, $data);
  }
}

/**
 * Build edit menu options for menu item children.
 *
 * This function is recursive.
 *
 * @param object $item
 *   The menu item object.
 * @param array $data
 *   An array of data globally shared between the different
 *   recursive calls.
 */
function workbench_menu_build_item_editing_form_recursive_nest(&$item, &$data) {
  $id = $item['item']->id;

  if (!isset($data['depth'])) {
    $data['depth'] = 0;
  }

  $data['fields'][$id] = array(
    '#tree' => TRUE,
    '#depth' =>  $data['depth'],
  );

  $data['fields'][$id]['weight'] = array(
    '#type' => 'textfield',
    '#title' => t("Weight"),
    '#default_value' => $item['item']->weight,
    '#size' => 6,
    '#attributes' => array('class' => array('workbench_menu-menu_items_table-weight')),
  );

  $data['fields'][$id]['parent'] = array(
    '#type' => 'textfield',
    '#title' => t("Parent"),
    '#default_value' => $item['item']->parent_id,
    '#size' => 6,
    '#attributes' => array('class' => array('workbench_menu-menu_items_table-parent')),
  );

  $data['fields'][$id]['item'] = array(
    '#type' => 'hidden',
    '#value' => $id,
    '#attributes' => array('class' => array('workbench_menu-menu_items_table-item')),
  );

  $data['fields'][$id]['object'] = array(
    '#type' => 'value',
    '#value' => $item,
  );

  if (!empty($item['children'])) {
    $data['depth']++;

    foreach ($item['children'] as &$child) {
      workbench_menu_build_item_editing_form_recursive_nest($child, $data);
    }

    $data['depth']--;
  }
}

/**
 * Theme function for presenting a drag and drop menu.
 *
 * @param array $element
 *   A form element array to be renderred.
 *
 * @see workbench_menu_build_item_editing_form()
 */
function theme_workbench_menu_admin_settings_form($element) {
  $form = &$element['form'];

  $table_id = 'workbench_menu-menu_items_table';
  $header = array(t("Item Name"), t("Order"), t("Item Type"), t("Operations"), t("ID"));

  drupal_add_tabledrag($table_id, 'match', 'parent', 'workbench_menu-menu_items_table-parent', 'workbench_menu-menu_items_table-parent', 'workbench_menu-menu_items_table-item');
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'workbench_menu-menu_items_table-weight');

  $rows = array();
  $path_prefix = &$form['path_prefix']['#value'];

  foreach ($form['items'] as $id => &$nested_item) {
    if (!is_numeric($id)) continue;
    $item = &$nested_item['object']['#value'];
    $depth = &$nested_item['#depth'];

    $row = array('data' => array(), 'class' => array('draggable'));
    $name = '';
    $type = '';
    $operations = '';

    $operations .= l(t("edit"), $path_prefix . '/items/edit/' . $id);
    $operations .= ' ';
    $operations .= l(t("delete"), $path_prefix . '/items/delete/' . $id);

    if (property_exists($item['item'], 'human_name')) {
      $name = theme('indentation', array('size' => $depth)) . $item['item']->human_name;
    }

    if (!empty($item['item']->item_type)) {
      $value = array_pop($item['item']->item_type);
      $type = $value->human_name;

      if ($value->machine_name == 'item_menu' || $value->machine_name == 'item_menu_item') {
        $row['class'][] = 'tabledrag-leaf';
      }

      if ($value->machine_name == 'item_menu') {
        $row['class'][] = 'tabledrag-leaf';
      }
    }

    $row['data'][] = $name;
    $row['data'][] = drupal_render($nested_item['weight']) . drupal_render($nested_item['parent']) . drupal_render($nested_item['item']);
    $row['data'][] = $type;
    $row['data'][] = $operations;
    $row['data'][] = $id;

    $rows[] = $row;
  }

  // ensure that the actions are presented below the table.
  $actions = array(&$form['actions']);
  unset($form['actions']);

  $variables = array();
  $variables['table'] =  array('header' => $header, 'rows' => $rows, 'empty' => t("This menu has no items."), 'attributes' => array('id' => $table_id, 'class' => array('menu_settings-table')));
  $variables['add_path'] = $path_prefix . '/items/add';

  $output = '';
  $output .= drupal_render_children($form);
  $output .= theme('workbench_menu_settings_table', $variables);
  $output .= drupal_render_children($actions);

  return $output;
}

/**
 * Display the workbench menu administration page for menu items.
 *
 * @param string $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param string|null $operation
 *   (optional) A string representing the operation to perform,
 *   which can be:
 *   - add: Add a new item.
 *   - edit: Edit an existing item, this requires the $item parameter
 *   to be defined.
 *   - delete: Delete an existing item, this requires the $item
 *   parameter to be defined.
 * @param string|null $item
 *   (optional) A conditionally optional parameter that is the item
 *   id for a given item.
 *
 * @return array
 *   An edit existing or add new form for section menus.
 *
 * @see workbench_menu_admin_item_settings_add_form()
 * @see workbench_menu_admin_item_settings_edit_form()
 * @see workbench_menu_admin_item_settings_delete_form()
 */
function workbench_menu_admin_item_settings($access_type, $access_id, $operation = NULL, $item = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $return_destination = 'admin/config/workbench/access/menus/' . $access_type . '/' . $access_id;

  if ($operation === 'add') {
    return drupal_get_form('workbench_menu_admin_item_settings_add_form', $access_type, $access_id, $return_destination);
  }
  elseif ($operation === 'edit' && !is_null($item)) {
    return drupal_get_form('workbench_menu_admin_item_settings_edit_form', $access_type, $access_id, $item, $return_destination);
  }
  elseif ($operation === 'delete' && !is_null($item)) {
    return drupal_get_form('workbench_menu_admin_item_settings_delete_form', $access_type, $access_id, $item, $return_destination);
  }

  drupal_not_found();
  drupal_exit();
}

/**
 * Creates the form settings array for an individual menu item.
 *
 * @param object $item
 *   A menu item object.
 *   If there is no existing item, then NULL may be passed instead.
 * @param object $menu
 *   A menu object.
 *
 * @return array|false
 *   A form array, or FALSE on error.
 */
function workbench_menu_admin_item_settings_form_settings($item, $menu) {
  if (!is_null($item) && !is_object($item)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('item', "Must be either a valid menu item object or NULL.");
    }

    return FALSE;
  }

  if (!is_object($menu)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('menu');
    }

    return FALSE;
  }

  $item_id = NULL;

  $defaults = array();
  $defaults['human_name'] = '';
  $defaults['parent_id'] = '0';
  $defaults['weight'] = '0';
  $defaults['active_path'] = '';
  $defaults['item_type'] = 'item_node';
  $defaults['expand'] = 'expand_auto';
  $defaults['properties'] = array();
  $defaults['properties']['url'] = '';
  $defaults['properties']['node_id'] = '';
  $defaults['properties']['disabled'] = FALSE;
  $defaults['properties']['clickable'] = FALSE;
  $defaults['properties']['file'] = '';
  $defaults['properties']['file_type'] = 'file_type_file';
  $defaults['properties']['menu'] = NULL;
  $defaults['properties']['menu_item'] = NULL;

  if (is_object($item)) {
    $item_id = &$item->id;

    if (property_exists($item, 'human_name') && is_string($item->human_name)) {
      $defaults['human_name'] = $item->human_name;
    }

    if (property_exists($item, 'parent_id') && is_numeric($item->parent_id)) {
      $defaults['parent_id'] = $item->parent_id;
    }

    if (property_exists($item, 'weight') && is_numeric($item->weight)) {
      $defaults['weight'] = $item->weight;
    }

    if (property_exists($item, 'active_path') && is_string($item->active_path)) {
      $defaults['active_path'] = $item->active_path;

      // be user-friendly and show normal spaces instead of %20.
      $defaults['active_path'] = preg_replace('/%20/', ' ', $defaults['active_path']);
    }

    if (property_exists($item, 'item_type') && is_array($item->item_type)) {
      foreach ($item->item_type as &$type) {
        $defaults['item_type'] = $type->machine_name;
      }
    }

    if (property_exists($item, 'expand') && is_array($item->expand)) {
      foreach ($item->expand as &$expand) {
        $defaults['expand'] = $expand->machine_name;
      }
    }

    if (property_exists($item, 'properties') && is_array($item->properties)) {
      foreach ($item->properties as $key => &$value) {
        $defaults['properties'][$key] = $value;
      }
    }

    if (!empty($defaults['properties']['url'])) {
      // be user-friendly and show normal spaces instead of %20.
      $defaults['properties']['url'] = preg_replace('/%20/', ' ', $defaults['properties']['url']);
    }

    if (!empty($defaults['properties']['file'])) {
      // be user-friendly and show normal spaces instead of %20.
      $defaults['properties']['file'] = preg_replace('/%20/', ' ', $defaults['properties']['file']);
    }

    if (!empty($defaults['properties']['node_id'])) {
      $defaults['properties']['node_id'] = $defaults['properties']['node_id'];
    }
  }

  $available = array();
  $available['parents'] = workbench_menu_get_load_menu_items_select_list_options(array('menu_id' => $menu->id), 'id');
  $available['item_types'] = cf_db_options_get_options_list('workbench_menu', 'item_type');
  $available['expand'] = cf_db_options_get_options_list('workbench_menu', 'expand');

  $available['file_types'] = array(
    'audio' => t("Audio"),
    'base' => t("Base"),
    'excel' => t("Excel"),
    'external' => t("External File"),
    'file' => t("Generic File"),
    'flash' => t("Flash"),
    'image' => t("Image"),
    'impress' => t("Impress"),
    'mail' => t("Mail"),
    'math' => t("Math"),
    'pdf' => t("PDF"),
    'powerpoint' => t("Powerpoint"),
    'webdocument' => t("Web Document"),
    'word' => t("Word"),
    'writer' => t("Writer"),
  );

  $form = array();

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Name"),
    '#required' => TRUE,
    '#description' => t("This is the menu items name and is also the text presented in the menu for most menu item types."),
    '#size' => 64,
    '#maxlength' => 255,
    '#default_value' => $defaults['human_name'],
  );

  $form['settings']['parent_id'] = array(
    '#type' => 'select',
    '#title' => t("Parent Item"),
    '#options' => $available['parents'],
    '#default_value' => $defaults['parent_id'],
    '#description' => t("Allows this item to be nested under another item defined here."),
  );

  $form['settings']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t("Weight"),
    '#required' => TRUE,
    '#description' =>
      t("The items weight is used to define where an item is positioned.") . '<br>' .
      t("All items of the same weight are sorted alphabetically."),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $defaults['weight'],
  );

  $form['settings']['active_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Active Path"),
    '#description' =>
      t("The active path defines how this item relates to the url path.") . '<br>' .
      t("For example, with a url of http://my.site.com/group/first/second/, the active path for a menu item could be either first or second.") . '<br>' .
      t("Paths that do not begin with / will have their parent menu item's path prefixed onto them.") . '<br>' .
      t("Paths that do begin with / will only have their group's path prefixed onto them.") . '<br>' .
      t("Use &lt;top&gt; if this menu item needs to use the groups path, which would be group in the example."),
    '#size' => 64,
    '#maxlength' => 255,
    '#default_value' => $defaults['active_path'],
  );

  $form['settings']['item_type'] = array(
    '#type' => 'select',
    '#title' => t("Item Type"),
    '#options' => $available['item_types'],
    '#default_value' => $defaults['item_type'],
    '#description' => t("Each menu item works in a different way based on its item type."),
  );

  $form['settings']['url'] = array(
    '#type' => 'textfield',
    '#title' => t("URL"),
    '#default_value' => $defaults['properties']['url'],
    '#description' =>
      t("Specify the url this link points to.") . '<br>' .
      t("Relative paths that do not begin with / will have their group's path prefixed onto them.") . '<br>' .
      t("Use &lt;top&gt; if this menu item needs to link to the group path."),
    '#dependency' => array('edit-settings-item-type' => array('item_link')),
    '#size' => 64,
  );

  $form['settings']['node_id'] = array(
    '#type' => 'textfield',
    '#title' => t("Node ID"),
    '#default_value' => $defaults['properties']['node_id'],
    '#description' =>
      t("Specify the numeric node id of your desired page.") . '<br>' .
      t("You can search for a specific node id by typing in the title in this field.") . '<br>' .
      t("Both ? and # are allowed to be appended onto the node id."),
    '#dependency' => array('edit-settings-item-type' => array('item_node')),
    '#size' => 64,
    '#autocomplete_path' => 'workbench_menu/autocomplete_nid',
  );

  $form['settings']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Disabled"),
    '#default_value' => $defaults['properties']['disabled'],
    '#description' => t("Specify that this label represents a disabled or otherwise not available link."),
    '#dependency' => array('edit-settings-item-type' => array('item_label')),
  );

  $form['settings']['clickable'] = array(
    '#type' => 'checkbox',
    '#title' => t("Clickable"),
    '#default_value' => $defaults['properties']['clickable'],
    '#description' => t("Specify that this label acts like a link and when clicked on may even expand to show hidden child items."),
    '#dependency' => array('edit-settings-item-type' => array('item_label')),
  );

  $form['settings']['file'] = array(
    '#type' => 'textfield',
    '#title' => t("File"),
    '#default_value' => $defaults['properties']['file'],
    '#description' => t("Select specify a file id or file to use as a file as menu item"),
    '#dependency' => array('edit-settings-item-type' => array('item_file')),
    '#size' => 64,
    '#maxlength' => 1024,
  );

  if (module_exists('imce')) {
    $imce_javascript = 'window.open(\'/imce?app=workbench_menu_item_file|url@edit-settings-file\', \'\', \'width=760,height=560,resizable=1\');';
    $form['settings']['file']['#description'] =  t(
      "!select a file id or file to use as the menu item.",
      array(
        '!select' => '<a class="filefield-sources-imce-browse" href="#" onclick="' . $imce_javascript . '" title="' . t("Select a file using the IMCE wysiwyg") . '">' . t("Select") . '</a>',
      )
    );
  }
  else {
    $form['settings']['file']['#autocomplete_path'] = 'workbench_menu/autocomplete_fid';
  }

  $form['settings']['menu'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu"),
    '#default_value' => $defaults['properties']['menu'],
    '#description' =>
      t("Load another menu and attach at this location.") . '<br>' .
      t("You can search for a specific menu id by typing in the title in this field."),
    '#dependency' => array('edit-settings-item-type' => array('item_menu')),
    '#size' => 64,
    '#autocomplete_path' => 'workbench_menu/autocomplete_menu_id',
  );

  $form['settings']['menu_item'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Item"),
    '#default_value' => $defaults['properties']['menu_item'],
    '#description' =>
      t("Specify a menu item id from another menu and attach as this menu.") . '<br>' .
      t("You can search for a specific menu id by typing in the title in this field."),
    '#dependency' => array('edit-settings-item-type' => array('item_menu_item')),
    '#size' => 64,
    '#autocomplete_path' => 'workbench_menu/autocomplete_menu_item_id',
  );

  $form['settings']['file_type'] = array(
    '#type' => 'select',
    '#title' => t("File Type"),
    '#options' => $available['file_types'],
    '#default_value' => $defaults['properties']['file_type'],
    '#description' => t("Select the appropriate file type that defines the file, if unsure select generic file."),
    '#dependency' => array('edit-settings-item-type' => array('item_file')),
  );

  $form['settings']['expand'] = array(
    '#type' => 'select',
    '#title' => t("When to Expand"),
    '#options' => $available['expand'],
    '#default_value' => $defaults['expand'],
    '#description' => t("Define the default behavior for when the item should have its child items shown."),
  );

  if (!(user_access('change workbench menu item active path') || user_access('administer workbench menu'))) {
    $form['settings']['active_path']['#type'] = 'value';
  }

  return $form;
}

/**
 * Add a new menu item to the specified menu.
 *
 * @param string $access_type
 *   The type of access requested (e.g. taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param string $return_destination
 *   A url path string that designates where this form should return
 *   to on submit or cancel.
 *
 * @return array
 *   A form array for adding a new menu item.
 */
function workbench_menu_admin_item_settings_add_form($form, &$form_state, $access_type, $access_id, $return_destination) {
  $term = taxonomy_term_load($access_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) {
    drupal_not_found();
    drupal_exit();
  }

  $menu = array_pop($menus);

  drupal_set_title(t("Add New Item to %menu Menu (ID: %id)", array('%menu' => $term->name, '%id' => $menu->id)), PASS_THROUGH);

  $form['settings'] = (array) workbench_menu_admin_item_settings_form_settings(NULL, $menu);

  $form['access_id']= array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['menu_id'] = array(
    '#type' => 'value',
    '#value' => $menu->id,
  );

  $form['menu_human_name'] = array(
    '#type' => 'value',
    '#value' => $term->name,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Add Menu Item"),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for workbench_menu_admin_item_settings_add_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_item_settings_edit_form_validate()
 */
function workbench_menu_admin_item_settings_add_form_validate($form, &$form_state) {
  return workbench_menu_admin_item_settings_edit_form_validate($form, $form_state);
}

/**
 * Submit process for workbench_menu_admin_item_settings_add_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_item_settings_edit_form_submit()
 */
function workbench_menu_admin_item_settings_add_form_submit($form, &$form_state) {
  return workbench_menu_admin_item_settings_edit_form_submit($form, $form_state);
}

/**
 * Edit an existing menu item from the specified menu.
 *
 * @param array $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param int $item_id
 *   The numerical id for a menu item.
 * @param string $return_destination
 *   A url path string that designates where this form should return
 *   to on submit or cancel.
 *
 * @return array|false
 *   A form array for adding a new menu item, or FALSE on error.
 */
function workbench_menu_admin_item_settings_edit_form($form, &$form_state, $access_type, $access_id, $item_id, $return_destination) {
  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($item_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('item_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $term = taxonomy_term_load($access_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id), 'id');

  if (empty($menus)) {
    drupal_not_found();
    drupal_exit();
  }

  $menu = array_pop($menus);

  // do not allow operating on items that do not exist in the selected menu.
  if (!array_key_exists($item_id, $menu->items['flat'])) {
    drupal_not_found();
    drupal_exit();
  }

  $item = &$menu->items['flat'][$item_id]['item'];

  drupal_set_title(t("Menu Item %item_name (ID: %item_id) from %menu Menu (ID: %menu_id)", array('%item_name' => $item->human_name, '%menu' => $term->name, '%item_id' => $item->id, '%menu_id' => $menu->id)), PASS_THROUGH);

  $form['settings'] = (array) workbench_menu_admin_item_settings_form_settings($item, $menu);

  $form['item_id'] = array(
    '#type' => 'value',
    '#value' => $item->id,
  );

  $form['access_id']= array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['menu_id'] = array(
    '#type' => 'value',
    '#value' => $menu->id,
  );

  $form['item_human_name'] = array(
    '#type' => 'value',
    '#value' => $item->human_name,
  );

  $form['menu_human_name'] = array(
    '#type' => 'value',
    '#value' => $term->name,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save Menu Item"),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for workbench_menu_admin_item_settings_edit_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_edit_form_validate($form, &$form_state) {
  global $base_path;

  if (workbench_menu_menu_exists($form['menu_id']['#value']) !== TRUE) {
    form_set_error('menu_id', t("An invalid menu id has been specified."));
  }

  if (isset($form['item_id']['#value'])) {
    if (workbench_menu_menu_item_exists($form['item_id']['#value'], $form['menu_id']['#value']) !== TRUE) {
      form_set_error('menu_id', t("An invalid item id has been specified."));
    }

    if (isset($form['access_id']['#value'])) {
      if (workbench_menu_menu_exists($form['access_id']['#value'], TRUE) !== TRUE) {
        form_set_error('access_id', t("An invalid access id has been specified."));
      }
    }
    else {
      form_set_error('', t("The specified menu associated with this menu item does not exist."));
    }
  }

  $item_type = &$form_state['values']['settings']['item_type'];
  $item_type_name = &$form['settings']['settings']['item_type']['#options'][$item_type];

  if (isset($form_state['values']['settings']['parent_id'])) {
    if (isset($form['item_id']['#value'])) {
      if ($form['item_id']['#value'] === $form_state['values']['settings']['parent_id']) {
        form_set_error('settings][parent_id', t("The menu item cannot be its own parent."));
      }
    }

    if ($form_state['values']['settings']['parent_id'] != 0) {
      $items = (array) workbench_menu_get_load_menu_items(array('id' => $form_state['values']['settings']['parent_id']), 'id');

      if (empty($items)) {
        form_set_error('settings][parent_id', t("The specified item parent is invalid."));
      }
      else {
        if (!isset($items['flat'][$form_state['values']['settings']['parent_id']])) {
          form_set_error('settings][parent_id', t("The specified item parent is invalid."));
        }
        else {
          $item = &$items['flat'][$form_state['values']['settings']['parent_id']];
          $type = '';

          foreach ($item['item']->item_type as &$t) {
            $type = $t->machine_name;
            break;
          }

          if (empty($type)) {
            form_set_error('settings][parent_id', t("The specified item parent is invalid."));
          }
          else {
            if ($type == 'item_menu' || $type == 'item_menu_item') {
              form_set_error('settings][parent_id', t("The items of the type %type cannot be used as item parents.", array('%type' => $form['settings']['settings']['item_type']['#options'][$type])));
            }
          }
        }
      }
    }
  }

  if (!empty($form_state['values']['settings']['active_path'])) {
    if ($form_state['values']['settings']['active_path'] !== '<top>') {
      // be user-friendly and allow normal whitespaces on insert.
      $form_state['values']['settings']['active_path'] = preg_replace('/ /', '%20', $form_state['values']['settings']['active_path']);

      if (valid_url($form_state['values']['settings']['active_path'], TRUE)) {
        form_set_error('settings][active_path', t("The active path cannot be a full URL."));
      }
      elseif (!valid_url($form_state['values']['settings']['active_path'])) {
        form_set_error('settings][active_path', t("'%path' is not a valid path.", array('%path' => $form_state['values']['settings']['active_path'])));
      }
    }
  }

  if (!is_numeric($form_state['values']['settings']['weight'])) {
    form_set_error('settings][weight', t("The items weight can only be a number."));
  }

  switch ($item_type) {
    case 'item_node':
      $field = & $form_state['values']['settings']['node_id'];

      if (empty($field)) {
        form_set_error('settings][node_id', t("The node id is required for an item type of %type.", array('%type' => $item_type_name)));
      }
      else {
        $matches = array();
        $node_id = $field;
        $link_extras = '';
        $matched = preg_match('@^(\d+)(/.*|$|\?.*|#.*)@', $node_id, $matches);

        if ($matched == 0) {
          $matched = preg_match('@\[nid: (\d+)\]\s*($|\?.*|#.*)@', $field, $matches);
        }

        if ($matched > 0) {
          $node_id = $matches[1];

          if (isset($matches[2])) {
            $link_extras = $matches[2];
          }
        }

        if (cf_is_integer($node_id)) {
          $query = db_select('node', 'n');
          $query->fields('n', array('nid', 'title'));
          $query->condition('n.nid', $node_id);

          $results = (array) $query->execute()->fetchAll();

          if (empty($results)) {
            form_set_error('settings][node_id', t("There is no node with an id of %nid .", array('%nid' => $node_id)));
          }
          else {
            $result = array_pop($results);
            $field = $result->title . ' [nid: ' . $result->nid . ']';

            if (!empty($link_extras)) {
              $field .= ' ' . $link_extras;
            }
          }
        }
        else {
          form_set_error('settings][node_id', t("%nid is not a valid node id, the node id can only be a positive integer.", array('%nid' => $field)));
        }
      }
      break;

    case 'item_link':
      if (empty($form_state['values']['settings']['url'])) {
        form_set_error('settings][url', t("The URL is required for an item type of %type.", array('%type' => $item_type_name)));
      }
      elseif ($form_state['values']['settings']['url'] !== '<top>') {
        // be user-friendly and allow normal whitespaces on insert.
        $form_state['values']['settings']['url'] = preg_replace('/ /', '%20', $form_state['values']['settings']['url']);

        if (!valid_url($form_state['values']['settings']['url'], TRUE)) {
          if (!valid_url($form_state['values']['settings']['url'])) {
            form_set_error('settings][url', t("'%url' is not a valid URL.", array('%url' => $form_state['values']['settings']['url'])));
          }
        }
      }
      break;

    case 'item_file':
      $field = &$form_state['values']['settings']['file'];

      if (empty($field)) {
        form_set_error('settings][file', t("The file is required for an item type of %item.", array('%item' => $item_type_name)));
      }
      else {
        $wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_READ | STREAM_WRAPPERS_VISIBLE);
        if (empty($wrappers)) {
          form_set_error('settings][file', t("There are no valid file wrappers enabled."));
        }
        else {
          $invalid_msg = t("'%file' is neither a valid file URL nor a valid file ID.", array('%file' => $field));

          // Alwaus re-validate and update filename for [fid: XX] entries.
          $matches = array();
          $matched = preg_match('@\[fid: (\d+)\]\s*$@i', $field, $matches);

          if ($matched > 0) {
            $match = array_pop($matches);

            if (cf_is_integer($match)) {
              $field = $match;
            }
          }

          try {
            $query = db_select('file_managed', 'fm');
            $query->fields('fm', array('fid', 'filename'));
            $query->condition('fm.status', 1);

            if (cf_is_integer($field)) {
              $or = db_or();
              foreach ($wrappers as $w => &$wrapper) {
                $or->condition('fm.uri', db_like($w) . '://%', 'LIKE');
              }

              $query->condition($or);
              $query->condition('fm.fid', $field);

              $results = $query->execute()->fetchAll();

              if (empty($results)) {
                form_set_error('settings][file', $invalid_msg);
              }
              else {
                $result = array_pop($results);
                $field = $result->filename . ' [fid: ' . $result->fid . ']';
              }
            }
            else {
              // be user-friendly and allow normal whitespaces on insert.
              $field = preg_replace('/%20/', ' ', $field);

              $matched = FALSE;
              $matched_uri = '';
              foreach ($wrappers as $w => &$wrapper) {
                if (preg_match('@^' . $w . '://@i', $field) > 0) {
                  $matched = TRUE;
                  break;
                }

                $instance = file_stream_wrapper_get_instance_by_scheme($w);
                $directory = $instance->getDirectoryPath();

                if (empty($directory)) continue;

                if (preg_match('@' . $base_path . $directory . '@i', $field) > 0) {
                  $matched = TRUE;
                  $field = $w . '://' . preg_replace('@' . $base_path . $directory . '/@i', '', $field);
                  break;
                }
              }

              if ($matched) {
                // @todo: auto-convert files that are in the database via an update script
                $query->condition('fm.uri', $field);

                $results = $query->execute()->fetchAll();

                if (empty($results)) {
                  form_set_error('settings][file', $invalid_msg);
                }

                $result = array_pop($results);
                $field = $result->filename . ' [fid: ' . $result->fid . ']';
              }
              else {
                if (!valid_url($field, TRUE)) {
                  if (!valid_url($field)) {
                    form_set_error('settings][file', $invalid_msg);
                  }
                }
              }
            }
          } catch (Exception $e) {
            if (class_exists('cf_error')) {
              cf_error::on_query_execution($e);
            }

            form_set_error('settings][file', $invalid_msg);
          }
        }
      }
      break;

    case 'item_menu':
      if (empty($form_state['values']['settings']['menu'])) {
        form_set_error('settings][menu', t("The menu is required for an item type of %type.", array('%type' => $item_type_name)));
      }

      // if the current it has children, do not allow it to be changed into an item type that is not allowed to have children.
      $items = (array) workbench_menu_get_load_menu_items(array('parent_id' => $form['item_id']['#value']));

      if (!empty($items)) {
        form_set_error('settings][item_type', t("This item cannot be changed to an item type of %type because that item type does not support child items. You must remove all child items from this item to use this item type.", array('%type' => $item_type_name)));
      }
      break;

    case 'item_menu_item':
      if (empty($form_state['values']['settings']['menu_item'])) {
        form_set_error('settings][menu_item', t("The menu item is required for an item type of %type.", array('%type' => $item_type_name)));
      }
      elseif (!is_numeric($form_state['values']['settings']['menu_item'])) {
        form_set_error('settings][menu_item', t("The menu item '%value' is not and must be a number.", array('%value' => $form_state['values']['settings']['menu_item'], '%type' => $item_type_name)));
      }
      else {
        $query = db_select('workbench_menu_items', 'wmi');

        $query->fields('wmi');
        $query->orderBy('wmi.id', 'ASC');

        $query->condition('wmi.id', $form_state['values']['settings']['menu_item']);

        $records = (array) $query->execute()->fetchAll();

        if (empty($records)) {
          form_set_error('settings][menu_item', t("There exists no item with the id of '%value'.", array('%value' => $form_state['values']['settings']['menu_item'])));
        }
        else {
          $record = array_pop($records);

          $form_state['values']['settings']['menu'] = $record->menu_id;
        }
      }

      // if the current it has children, do not allow it to be changed into an item type that is not allowed to have children.
      $items = (array) workbench_menu_get_load_menu_items(array('parent_id' => $form['item_id']['#value']));

      if (!empty($items)) {
        form_set_error('settings][item_type', t("This item cannot be changed to an item type of %type because that item type does not support child items. You must remove all child items from this item to use this item type.", array('%type' => $item_type_name)));
      }
      break;

    case 'item_label':
      if ($form_state['values']['settings']['disabled'] && $form_state['values']['settings']['clickable']) {
        form_set_error('settings][menu', t("The menu item %type cannot be both disabled and clickable, choose one or the other.", array('%type' => $item_type_name)));
      }
      break;
  }
}

/**
 * Submit process for workbench_menu_admin_item_settings_edit_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_edit_form_submit($form, &$form_state) {
  $transaction = db_transaction();

  if (isset($form['item_id']['#value'])) {
    $item_id = $form['item_id']['#value'];
  }
  else {
    $item_id = NULL;
  }

  if (isset($form_state['value']['settings']['item_type'])) {
    $item_type = &$form_state['value']['settings']['item_type'];
  }
  else {
    $item_type = NULL;
  }

  $user = cf_current_user();

  if (is_null($item_id)) {
    watchdog('Workbench Menu', "The user %user has added a new item %item to the menu %menu (%menu_id)", array('%user' => $user->name, '%item' => $form_state['values']['settings']['human_name'], '%menu' => $form['menu_human_name']['#value'], '%menu_id' => $form['menu_id']['#value']), WATCHDOG_INFO);
  }
  else {
    watchdog('Workbench Menu', "The user %user has changed the item %item (%item_id) from the menu %menu (%menu_id)", array('%user' => $user->name, '%item' => $form_state['values']['settings']['human_name'], '%item_id' => $form['item_id']['#value'], '%menu' => $form['menu_human_name']['#value'], '%menu_id' => $form['menu_id']['#value']), WATCHDOG_INFO);
  }

  $properties = array();

  foreach (array('url', 'node_id', 'disabled', 'clickable', 'file', 'file_type', 'menu', 'menu_item') as $property) {
    if (isset($form_state['values']['settings'][$property])) {
      $properties[$property] = &$form_state['values']['settings'][$property];
    }
  }

  try {
    if (is_null($item_id)) {
      $existing = NULL;
    }
    else {
      $query = db_select('workbench_menu_items', 'wmi');
      $query->fields('wmi');
      $query->condition('id', $item_id);
      $existing = $query->execute()->fetchAll();
    }

    $columns = array('menu_id', 'parent_id', 'human_name', 'weight', 'active_path', 'properties');
    $record = array();

    $record['menu_id'] = $form['menu_id']['#value'];
    $record['parent_id'] = $form_state['values']['settings']['parent_id'];
    $record['human_name'] = $form_state['values']['settings']['human_name'];
    $record['weight'] = $form_state['values']['settings']['weight'];
    $record['active_path'] = $form_state['values']['settings']['active_path'];
    $record['properties'] = $properties;

    if (empty($existing)) {
      drupal_write_record('workbench_menu_items', $record);
      $item_id = $record['id'];
    }
    else {
      $primary_keys = array('id');
      $record['id'] = $item_id;
      drupal_write_record('workbench_menu_items', $record, $primary_keys);
    }

    if (is_null($item_id)) {
      $query = db_select('workbench_menu_items', 'wmi');
      $query->fields('wmi');
      $query->condition('id', $item_id);
      $results = $query->execute()->fetchAll();
    }

    foreach (array('expand', 'item_type') as $name) {
      if (!empty($existing)) {
        $query = db_delete('workbench_menu_' . $name . 's');
        $query->condition('item_id', $item_id);
        $query->execute();
      }

      $options = (array) cf_db_options_get_options('workbench_menu', $name);

      if (!empty($form_state['values']['settings'][$name]) && isset($options[$form_state['values']['settings'][$name]])) {
        $record = array();
        $record['item_id'] = $item_id;
        $record[$name . '_id'] = $options[$form_state['values']['settings'][$name]]->id;

        $query = db_insert('workbench_menu_' . $name . 's');
        $query->fields($record);
        $query->execute();
      }
    }

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Edit an existing menu item from the specified menu.
 *
 * @param string $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param int $access_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param int $item_id
 *   The numerical id for a menu item.
 * @param string $return_destination
 *   A url path string that designates where this form should return to on submit or cancel.
 *
 * @return array
 *   A form array for adding a new menu item.
 */
function workbench_menu_admin_item_settings_delete_form($form, &$form_state, $access_type, $access_id, $item_id, $return_destination) {
  if ($access_type !== 'taxonomy') {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('access_type', "At this time only taxonomy access_type is supported.");
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!is_numeric($item_id)) {
    if (class_exists('cf_error')) {
      cf_error::not_numeric('item_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $term = taxonomy_term_load($access_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id), 'id');

  if (empty($menus)) {
    drupal_not_found();
    drupal_exit();
  }

  $menu = array_pop($menus);

  // do not allow operating on items that do not exist in the selected menu.
  if (!array_key_exists($item_id, $menu->items['flat'])) {
    drupal_not_found();
    drupal_exit();
  }

  $items = (array) workbench_menu_get_load_menu_items(array('id' => $item_id), 'id');

  if (!isset($items['flat'][$item_id])) {
    drupal_not_found();
    drupal_exit();
  }

  $item = &$items['flat'][$item_id]['item'];

  $form['access_id']= array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['item_id'] = array(
    '#type' => 'value',
    '#value' => $item->id,
  );

  $form['menu_id'] = array(
    '#type' => 'value',
    '#value' => $item->menu_id,
  );

  $form['item_human_name'] = array(
    '#type' => 'value',
    '#value' => $item->human_name,
  );

  $form['menu_human_name'] = array(
    '#type' => 'value',
    '#value' => $term->name,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  return confirm_form($form,
    t("Are you sure you want to delete %item_name from %menu Menu?", array('%item_name' => $item->human_name, '%menu' => $term->name)),
    $destination,
    t("This action cannot be undone."),
    t("Delete"),
    t("Cancel")
  );
}

/**
 * Validation for workbench_menu_admin_item_settings_edit_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_delete_form_validate($form, &$form_state) {
  if (workbench_menu_menu_exists($form['menu_id']['#value']) !== TRUE) {
    form_set_error('menu_id', t("An invalid menu id has been specified."));
  }

  if (isset($form['item_id']['#value'])) {
    if (workbench_menu_menu_item_exists($form['item_id']['#value']) !== TRUE) {
      form_set_error('menu_id', t("An invalid item id has been specified."));
    }

    if (isset($form['access_id']['#value'])) {
      if (workbench_menu_menu_exists($form['access_id']['#value'], TRUE) !== TRUE) {
        form_set_error('access_id', t("An invalid access id has been specified."));
      }
    }
    else {
      form_set_error('', t("The specified menu associated with this menu item does not exist."));
    }
  }
}

/**
 * Submit process for workbench_menu_admin_item_settings_delete_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_delete_form_submit($form, &$form_state) {
  $user = cf_current_user();
  $child_items = array();
  $item_id = & $form['item_id']['#value'];

  try {
    $query = db_select('workbench_menu_items', 'wmi');
    $query->fields('wmi', array('parent_id'));
    $query->condition('wmi.id', $item_id);

    $parent_id = (int) $query->execute()->fetchField();

    $query = db_select('workbench_menu_items', 'wmi');
    $query->fields('wmi', array('id', 'human_name'));
    $query->condition('wmi.parent_id', $item_id);

    $menu_item_children = array();
    $results = (array) $query->execute()->fetchAll();
    foreach ($results as &$result) {
      $menu_item_children[$result->id] = $result->human_name;
    }
  }
  catch (Exception $e) {
    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }

    watchdog('Workbench Menu', t("An unexpected error happened while trying to process child menu items with a parent id of %item_id."), array('%item_id' => $item_id), WATCHDOG_ERROR);
  }

  watchdog('Workbench Menu', "The user %user has deleted the item %item (%item_id) from the menu %menu (%menu_id)", array('%user' => $user->name, '%item' => $form_state['values']['item_human_name'], '%item_id' => $item_id, '%menu' => $form_state['values']['menu_human_name'], '%menu_id' => $form['menu_id']['#value']), WATCHDOG_INFO);
  watchdog('Workbench Menu', "Due to deletion of %item (%item_id), the following child items are being re-assigned to the item %parent_item_id: %child_items", array('%item' => $form_state['values']['item_human_name'], '%item_id' => $item_id, '%parent_item_id' => $parent_id, '%child_items' => print_r($menu_item_children, TRUE)), WATCHDOG_INFO);

  $transaction = db_transaction();

  try {
    $query = db_delete('workbench_menu_items');
    $query->condition('id', $item_id);
    $query->execute();

    foreach (array('expand', 'item_type') as $name) {
      $query = db_delete('workbench_menu_' . $name . 's');
      $query->condition('item_id', $item_id);
      $query->execute();
    }

    // re-attach all child menu items to the parent of the deleted menu item to prevent dangling items.
    $query = db_update('workbench_menu_items');
    $query->condition('parent_id', $item_id);
    $query->fields(array('parent_id' => $parent_id));
    $query->execute();

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Submit process for resetting the cache for a given menu.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_reset_cache_form($form, &$form_state) {
  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }

  if ($form['path']['#default_value'] != $form_state['values']['path']) {
    form_set_error('path', t("The menu path has been changed, please save your changes before clearing the menu cache."));
    $form_state['rebuild'] = TRUE;
    return;
  }

  if (!empty($form['path']['#default_value'])) {
    workbench_menu_clear_cache_at_path($form['path']['#default_value']);

    drupal_set_message(t("The menu cache for the path '%path' has been reset.", array('%path' => $form['path']['#default_value'])));
  }
  else {
    drupal_set_message(t("Unable to reset the menu cache because no path was given."), 'warning');
  }
}
