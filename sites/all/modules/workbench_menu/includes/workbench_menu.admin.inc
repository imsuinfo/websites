<?php

/**
 * @file
 * Workbench Menu admin file.
 */

/**
 * Display the workbench menu edit page for menus.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return
 *   An editing form or a list of section menus.
 *
 * @see workbench_menu_edit_settings_form()
 */
function workbench_menu_edit_settings($access_type = NULL, $access_type_id = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if (!(is_null($access_type) && is_null($access_type_id))){
    return drupal_get_form('workbench_menu_edit_settings_form', $access_type, $access_type_id);
  }

  cf_error_append_history($function_history, __FUNCTION__);

  $access = workbench_access_get_access_tree();
  $active = workbench_access_get_active_tree();

  $output = '<h2>' . t("Menus") . '</h2>';
  $output .= '<p>' . t("The following menus are currently available for editing.");

  $rows = array();
  foreach ($active['tree'] as $access_id => $section) {
    if (!is_numeric($access_id)) continue;

    if (!array_key_exists($access_id, $access)){
      continue;
    }

    if (!isset($active['active'][$access_id])) {
      continue;
    }

    $rows[] = array(l($section['name'], 'admin/workbench/menus/' . $active['access_scheme']['access_type'] . '/' . $access_id));
  }

  $header = array(t("Menu Name"));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("There are no menus available for you to edit.")));
  $build['content']['#markup'] = $output;
  return $build;
}

/**
 * Generate a user overview form for a section.
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return
 *   The built form array.
 */
function workbench_menu_edit_settings_form($form, &$form_state, $access_type, $access_type_id) {
  cf_error_append_history($function_history, __FUNCTION__);

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  $access = workbench_access_get_access_tree();

  $form = array();
  $active = workbench_access_get_active_tree();
  $active['access_scheme']['access_id'] = $access_type_id;
  if ($active['access_scheme']['access_type'] != $access_type || !isset($active['active'][$access_type_id])) {
    drupal_access_denied();
    drupal_exit();
  }

  $default_path = '';
  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/workbench/menus';
  $items = array();

  // the menu edit form should always pull the latest data from the database
  $menus = (array) workbench_menu_get_load_menus(array('access_id' => $access_type_id, 'reset' => TRUE), NULL, $function_history);

  if (!empty($menus)){
    $menu = array_pop($menus);

    if (property_exists($menu, 'path')){
      $default_path = &$menu->path;
    }

    if (property_exists($menu, 'items')){
      $items = &$menu->items;
    }
  }
  else {
    drupal_access_denied();
    drupal_exit();
  }

  $form['settings_label']= array(
    '#type' => 'markup',
    '#markup' => '<h2 id="workbench_menu-menus">' . t("Menu Settings for %menu Menu", array('%menu' => $menu->taxonomy->name)) . '</h2>',
    '#weight' => -100,
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Path"),
    '#default_value' => $default_path,
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t("Specify a relative url path in which this menu will be displayed on. Must not begin with a /."),
  );

  if (!user_access('change workbench menu path')){
    $form['path']['#disabled'] = TRUE;
  }


  workbench_menu_build_item_editing_form($form, 'admin/workbench/menus/taxonomy/' . $access_type_id, $items, $function_history);

  $form['workbench_access'] = array(
    '#type' => 'value',
    '#value' => $active['access_scheme'],
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination . '/taxonomy/' . $access_type_id,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Update Menu"),
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
 * Validation for workbench_menu_edit_settings_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
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
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_settings_form_submit()
 */
function workbench_menu_edit_settings_form_submit($form, &$form_state) {
  workbench_menu_admin_settings_form_submit($form, $form_state);
}

/**
 * Theme function for presenting a drag and drop menu administration interface.
 *
 * @param $variables
 *   An array containing the form array as returned by workbench_menu_build_item_editing_form().
 *
 * @see theme_workbench_menu_admin_settings_form()
 */
function theme_workbench_menu_edit_settings_form($variables){
  return theme_workbench_menu_admin_settings_form($variables);
}



/**
 * Display the workbench menu administration page for menu items.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $operation (optional)
 *   A string representing the operation to perform, which can be:
 *   - add: Add a new item.
 *   - edit: Edit an existing item, this requires the $item parameter to be defined.
 *   - delete: Delete an existing item, this requires the $item parameter to be defined.
 * @param $item (optional)
 *   A conditionally optional parameter that is the item id for a given item.
 *
 * @return
 *   An edit existing or add new form for section menus.
 *
 * @see workbench_menu_admin_item_settings_add_form()
 * @see workbench_menu_admin_item_settings_edit_form()
 * @see workbench_menu_admin_item_settings_delete_form()
 */
function workbench_menu_edit_item_settings($access_type, $access_type_id, $operation = NULL, $item = NULL) {
  cf_error_append_history($function_history, __FUNCTION__);

  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  $return_destination = 'admin/workbench/menus/' . $access_type . '/' . $access_type_id;

  if ($operation === 'add'){
    return drupal_get_form('workbench_menu_admin_item_settings_add_form', $access_type, $access_type_id, $return_destination);
  }
  else if ($operation === 'edit' && !is_null($item)){
    return drupal_get_form('workbench_menu_admin_item_settings_edit_form', $access_type, $access_type_id, $item, $return_destination);
  }
  else if ($operation === 'delete' && !is_null($item)){
    return drupal_get_form('workbench_menu_admin_item_settings_delete_form', $access_type, $access_type_id, $item, $return_destination);
  }

  drupal_access_denied();
  drupal_exit();
}

/**
 * Display the workbench menu administration page for menus.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return
 *   An editing form or a list of section menus.
 *
 * @see workbench_menu_admin_settings_form()
 */
function workbench_menu_admin_settings($access_type = NULL, $access_type_id = NULL) {
  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if (!(is_null($access_type) && is_null($access_type_id))){
    return drupal_get_form('workbench_menu_admin_settings_form', $access_type, $access_type_id);
  }

  cf_error_append_history($function_history, __FUNCTION__);

  $active = workbench_access_get_active_tree();

  $output = '<h2>' . t("Menus") . '</h2>';
  $output .= '<p>' . t("The following menus are currently available for editing.");

  if (user_access('administer workbench access')) {
    $output .= ' ' . t("You may <a href=\"!url\">enable or disable sections</a>.", array('!url' => url('admin/config/workbench/access/sections'))) . '</p>';
  }

  $rows = array();
  foreach ($active['tree'] as $access_id => &$section) {
    if (!isset($active['active'][$access_id])) {
      continue;
    }

    if (is_numeric($access_id)){
      $row = array(
        str_repeat('- ', $section['depth'] ) . ' ' . l($section['name'], 'admin/config/workbench/access/menus/' . $active['access_scheme']['access_type'] . '/' . $access_id),
      );
    }
    else {
      $row = array(
        str_repeat('- ', $section['depth'] ) . ' ' . $section['name'],
      );
    }

    $rows[] = $row;
  }

  $header = array(t("Menu Name"));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("There are no menus available for you to edit.")));
  $build['content']['#markup'] = $output;
  return $build;
}

/**
 * Generate a user overview form for a section.
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 *
 * @return
 *   The built form array.
 */
function workbench_menu_admin_settings_form($form, &$form_state, $access_type, $access_type_id) {
  cf_error_append_history($function_history, __FUNCTION__);

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  $form = array();
  $active = workbench_access_get_active_tree();
  $active['access_scheme']['access_id'] = $access_type_id;
  if ($active['access_scheme']['access_type'] != $access_type || !isset($active['active'][$access_type_id])) {
    drupal_access_denied();
    drupal_exit();
  }


  $default_path = '';
  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/workbench/access/menus';
  $items = array();

  // the menu edit form should always pull the latest data from the database
  $menus = (array) workbench_menu_get_load_menus(array('access_id' => $access_type_id, 'reset' => TRUE), NULL, $function_history);

  if (!empty($menus)){
    $menu = array_pop($menus);

    if (property_exists($menu, 'path')){
      $default_path = &$menu->path;
    }

    if (property_exists($menu, 'items')){
      $items = &$menu->items;
    }
  }
  else {
    drupal_access_denied();
    drupal_exit();
  }

  $form['settings_label']= array(
    '#type' => 'markup',
    '#markup' => '<h2 id="workbench_menu-menus">' . t("Menu Settings for %menu Menu", array('%menu' => $menu->taxonomy->name)) . '</h2>',
    '#weight' => -100,
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t("Menu Path"),
    '#default_value' => $default_path,
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t("Specify a relative url path in which this menu will be displayed on. Must not begin with a /."),
  );

  workbench_menu_build_item_editing_form($form, 'admin/config/workbench/access/menus/taxonomy/' . $access_type_id, $items, $function_history);

  $form['workbench_access'] = array(
    '#type' => 'value',
    '#value' => $active['access_scheme'],
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination . '/taxonomy/' . $access_type_id,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Update Menu"),
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
 * Validation for workbench_menu_admin_settings_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_settings_form_validate($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  if (!empty($form_state['values']['path'])) {
    if (!valid_url($form_state['values']['path'], FALSE)){
      form_set_error('path', t("The specified path is not a valid url path."));
    }

    if (preg_match('@^/@i', $form_state['values']['path']) !== 0){
      form_set_error('path', t("The path must not begin with a /."));
    }
  }
}

/**
 * Submit process for workbench_menu_admin_settings_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_settings_form_submit($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  $menu = array();
  $menu['access_id'] = &$form_state['values']['workbench_access']['access_id'];
  $menu['path'] = &$form_state['values']['path'];
  $menu['items'] = array();

  $results = workbench_menu_save_menus(array($menu), $function_history);

  if (!empty($form_state['values']['items'])){
    foreach ($form_state['values']['items'] as $id => &$item){
      if (!is_numeric($id)) continue;

      $primary_keys = array('id');

      $record = array();
      $record['id'] = $id;
      $record['parent_id'] = $item['parent'];
      $record['weight'] = $item['weight'];

      drupal_write_record('workbench_menu_items', $record, $primary_keys);
    }
  }

  if (empty($form_state['values']['redirect_to'])){
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Builds the editing form array that users and administrators use to sort, add, and remove menu items.
 * This function will not load the items, use the items parameter to pass the list of items to show.
 *
 * @param $form
 *   The form array to append the item editing array to.
 * @param $path_prefix
 *   A string that defines where the editing form links will begin.
 *   Such as: admin/config/workbench/access/menus/taxonomy/182.
 *   Do not include the leading /.
 *   Do not include the trailing /.
 * @param $items (optional)
 *   An array containing any existing items to populate the array with.
 * @param $function_history (optional)
 *   An array of function names, ie: array('0' => 'my_function_name').
 */
function workbench_menu_build_item_editing_form(&$form, $path_prefix, $items = array(), array $function_history = array()){
  cf_error_append_history($function_history, __FUNCTION__);

  $form['path_prefix'] = array(
    '#type' => 'value',
    '#value' => $path_prefix,
  );

  $form['items'] = array(
    '#tree' => TRUE,
  );

  $data['fields'] = &$form['items'];

  foreach ($items['nested'] as &$nested_item){
    workbench_menu_build_item_editing_form_recursive_nest($nested_item, $data, $function_history);
  }
}


/**
 * Recursively walk through each menu item until there are no longer any chilgren for each menu item.
 *
 * @param $item
 *   The menu item object.
 * @param $data
 *   An array of data globally shared between the different recursive calls.
 * @param $function_history (optional)
 *   An array of function names, ie: array('0' => 'my_function_name').
 */
function workbench_menu_build_item_editing_form_recursive_nest(&$item, &$data, array $function_history = array()){
  cf_error_append_history($function_history, __FUNCTION__);

  $id = $item['item']->id;

  if (!isset($data['depth'])){
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

  if (!empty($item['children'])){
    $data['depth']++;

    foreach ($item['children'] as &$child){
      workbench_menu_build_item_editing_form_recursive_nest($child, $data, $function_history);
    }

    $data['depth']--;
  }
}

/**
 * Theme function for presenting a drag and drop menu administration interface.
 *
 * @param $variables
 *   An array containing the form array as returned by workbench_menu_build_item_editing_form().
 *
 * @see workbench_menu_build_item_editing_form()
 */
function theme_workbench_menu_admin_settings_form($variables){
  cf_error_append_history($function_history, __FUNCTION__);

  $form = &$variables['form'];

  $table_id = 'workbench_menu-menu_items_table';
  $header = array(t("Item Name"), t("Order"), t("Item Type"), t("Operations"), t("ID"));

  drupal_add_tabledrag($table_id, 'match', 'parent', 'workbench_menu-menu_items_table-parent', 'workbench_menu-menu_items_table-parent', 'workbench_menu-menu_items_table-item');
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'workbench_menu-menu_items_table-weight');

  $rows = array();
  $path_prefix = &$form['path_prefix']['#value'];

  foreach ($form['items'] as $id => &$nested_item){
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

    if (property_exists($item['item'], 'human_name')){
      $name = theme('indentation', array('size' => $depth)) . $item['item']->human_name;
    }

    if (!empty($item['item']->item_type)){
      $value = array_pop($item['item']->item_type);
      $type = $value->human_name;
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

  $output = '';
  $output .= drupal_render_children($form);
  $output .= '<h3 id="workbench_menu-menus-manage_menu">' . t("Menu Items") . '</h3>';
  $output .= '<div class="workbench_menu-manage_menu-menu_items-table">' . theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("This menu has no items."), 'attributes' => array('id' => $table_id))) . '</div>';
  $output .= '<div class="workbench_menu-manage_menu-menu_items-add_item_link">' . l(t("Add new menu item."), $path_prefix . '/items/add') . '</div>';
  $output .= drupal_render_children($actions);

  return $output;
}

/**
 * Display the workbench menu administration page for menu items.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $operation (optional)
 *   A string representing the operation to perform, which can be:
 *   - add: Add a new item.
 *   - edit: Edit an existing item, this requires the $item parameter to be defined.
 *   - delete: Delete an existing item, this requires the $item parameter to be defined.
 * @param $item (optional)
 *   A conditionally optional parameter that is the item id for a given item.
 *
 * @return
 *   An edit existing or add new form for section menus.
 *
 * @see workbench_menu_admin_item_settings_add_form()
 * @see workbench_menu_admin_item_settings_edit_form()
 * @see workbench_menu_admin_item_settings_delete_form()
 */
function workbench_menu_admin_item_settings($access_type, $access_type_id, $operation = NULL, $item = NULL) {
  cf_error_append_history($function_history, __FUNCTION__);

  drupal_add_css(drupal_get_path('module', 'workbench_menu') . '/includes/workbench_menu_admin.css');

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  $return_destination = 'admin/config/workbench/access/menus/' . $access_type . '/' . $access_type_id;

  if ($operation === 'add'){
    return drupal_get_form('workbench_menu_admin_item_settings_add_form', $access_type, $access_type_id, $return_destination);
  }
  else if ($operation === 'edit' && !is_null($item)){
    return drupal_get_form('workbench_menu_admin_item_settings_edit_form', $access_type, $access_type_id, $item, $return_destination);
  }
  else if ($operation === 'delete' && !is_null($item)){
    return drupal_get_form('workbench_menu_admin_item_settings_delete_form', $access_type, $access_type_id, $item, $return_destination);
  }

  drupal_access_denied();
  drupal_exit();
}

/**
 * Creates the form settings array for an individual menu item.
 *
 * @param $item
 *   A menu item object.
 *   If there is no existing item, then NULL may be passed instead.
 * @param $menu
 *   A menu object.
 * @param $function_history (optional)
 *   An array of function names, ie: array('0' => 'my_function_name').
 *
 * @return
 *   A form array, or FALSE on error.
 */
function workbench_menu_admin_item_settings_form_settings($item, $menu, array $function_history = array()){
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_null($item) && !is_object($item)){
    cf_error_invalid_variable($function_history, 'item', "Must be either a valid menu item object or NULL.");
    return FALSE;
  }

  if (!is_object($menu)){
    cf_error_invalid_object($function_history, 'item');
    return FALSE;
  }

  $item_id = NULL;

  $defaults = array();
  $defaults['human_name'] = '';
  $defaults['parent_id'] = '0';
  $defaults['weight'] = '0';
  $defaults['active_path'] = '';
  $defaults['item_type'] = 'item_link';
  $defaults['expand'] = 'expand_auto';
  $defaults['properties'] = array();
  $defaults['properties']['url'] = '';
  $defaults['properties']['disabled'] = FALSE;
  $defaults['properties']['file'] = '';
  $defaults['properties']['file_type'] = 'file_type_file';

  if (is_object($item)){
    $item_id = &$item->id;

    if (property_exists($item, 'human_name') && is_string($item->human_name)){
      $defaults['human_name'] = $item->human_name;
    }

    if (property_exists($item, 'parent_id') && is_numeric($item->parent_id)){
      $defaults['parent_id'] = $item->parent_id;
    }

    if (property_exists($item, 'weight') && is_numeric($item->weight)){
      $defaults['weight'] = $item->weight;
    }

    if (property_exists($item, 'active_path') && is_string($item->active_path)){
      $defaults['active_path'] = $item->active_path;
    }

    if (property_exists($item, 'item_type') && is_array($item->item_type)){
      foreach ($item->item_type as &$type){
        $defaults['item_type'] = $type->machine_name;
      }
    }

    if (property_exists($item, 'expand') && is_array($item->expand)){
      foreach ($item->expand as &$expand){
        $defaults['expand'] = $expand->machine_name;
      }
    }

    if (property_exists($item, 'properties') && is_array($item->properties)){
      foreach ($item->properties as $key => &$value){
        $defaults['properties'][$key] = $value;
      }
    }
  }

  $available = array();
  $available['parents'] = workbench_menu_get_load_menu_items_select_list_options(array('menu_id' => $menu->id, 'reset' => TRUE), 'id', $function_history);
  $available['item_types'] = workbench_menu_get_options_list('item_type', NULL, $function_history);
  $available['expand'] = workbench_menu_get_options_list('expand', NULL, $function_history);

  $available['file_types'] = array(
    'base' => t("Base"),
    'excel' => t("Excel"),
    'external' => t("External File"),
    'file' => t("Generic File"),
    'flash' => t("Flash"),
    'image' => t("Image"),
    'impress' => t("Impress"),
    'math' => t("Math"),
    'pdf' => t("PDF"),
    'powerpoint' => t("Powerpoint"),
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

  $form['settings']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Disabled"),
    '#default_value' => $defaults['properties']['disabled'],
    '#description' => t("Specify that this label represents a disabled or otherwise not available link."),
    '#dependency' => array('edit-settings-item-type' => array('item_label')),
  );

  $imce_javascript = 'window.open(\'/imce?app=workbench_menu_item_file|url@edit-settings-file\', \'\', \'width=760,height=560,resizable=1\');';
  $form['settings']['file'] = array(
    '#type' => 'textfield',
    '#title' => t("File"),
    '#default_value' => $defaults['properties']['file'],
    '#description' => t(
      "!select a file to use as the menu item.",
      array(
        '!select' => '<a class="filefield-sources-imce-browse" href="#" onclick="' . $imce_javascript . '" title="' . t("Select a file using the IMCE wysiwyg"). '">' . t("Select") . '</a>',
      )
    ),
    '#dependency' => array('edit-settings-item-type' => array('item_file')),
    '#size' => 64,
    '#maxlength' => 1024,
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

  return $form;
}

/**
 * Add a new menu item to the specified menu.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $return_destination
 *   A url path string that designates where this form should return to on submit or cancel.
 *
 * @return
 *   A form array for adding a new menu item.
 */
function workbench_menu_admin_item_settings_add_form($form, &$form_state, $access_type, $access_type_id, $return_destination){
  cf_error_append_history($function_history, __FUNCTION__);

  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $menus = (array) workbench_menu_get_load_menus(array('reset' => TRUE, 'access_id' => $access_type_id), 'id', $function_history);

  if (empty($menus)){
    drupal_access_denied();
    drupal_exit();
  }

  foreach ($menus as &$m){
    $menu = $m;
  }

  $form['settings_label']= array(
    '#type' => 'markup',
    '#markup' => '<h2 id="workbench_menu-menu_item_settings">' . t("Add New Item to %menu Menu", array('%menu' => $menu->taxonomy->name)) . '</h2>',
    '#weight' => -100,
  );

  $form['settings'] = (array) workbench_menu_admin_item_settings_form_settings(NULL, $menu, $function_history);

  $form['menu_id'] = array(
    '#type' => 'value',
    '#value' => $menu->id,
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
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_item_settings_edit_form_validate()
 */
function workbench_menu_admin_item_settings_add_form_validate($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  return workbench_menu_admin_item_settings_edit_form_validate($form, $form_state);
}

/**
 * Submit process for workbench_menu_admin_item_settings_add_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 *
 * @see workbench_menu_admin_item_settings_edit_form_submit()
 */
function workbench_menu_admin_item_settings_add_form_submit($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  return workbench_menu_admin_item_settings_edit_form_submit($form, $form_state);
}

/**
 * Edit an existing menu item from the specified menu.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $item_id
 *   The numerical id for a menu item.
 * @param $return_destination
 *   A url path string that designates where this form should return to on submit or cancel.
 *
 * @return
 *   A form array for adding a new menu item, or FALSE on error.
 */
function workbench_menu_admin_item_settings_edit_form($form, &$form_state, $access_type, $access_type_id, $item_id, $return_destination){
  cf_error_append_history($function_history, __FUNCTION__);

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($item_id)){
    cf_error_not_numeric($function_history, 'item_id');
    drupal_access_denied();
    drupal_exit();
  }


  $term = taxonomy_term_load($access_type_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $menus = (array) workbench_menu_get_load_menus(array('reset' => TRUE, 'access_id' => $access_type_id), 'id', $function_history);

  if (empty($menus)){
    drupal_access_denied();
    drupal_exit();
  }

  $items = (array) workbench_menu_get_load_menu_items(array('id' => $item_id), 'id', $function_history);

  if (!isset($items['flat'][$item_id])){
    drupal_access_denied();
    drupal_exit();
  }

  $item = &$items['flat'][$item_id]['item'];

  foreach ($menus as &$m){
    $menu = $m;
  }

  $form['settings_label']= array(
    '#type' => 'markup',
    '#markup' => '<h2 id="workbench_menu-menu_item_settings">' . t("Menu Item %item_name from %menu Menu", array('%item_name' => $item->human_name, '%menu' => $term->name)) . '</h2>',
    '#weight' => -100,
  );

  $form['settings'] = (array) workbench_menu_admin_item_settings_form_settings($item, $menu, $function_history);

  $form['item_id'] = array(
    '#type' => 'value',
    '#value' => $item->id,
  );

  $form['menu_id'] = array(
    '#type' => 'value',
    '#value' => $item->menu_id,
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
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_edit_form_validate($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  $item_type = &$form_state['values']['settings']['item_type'];

  if (isset($form_state['values']['item_id'])){
    if ($form_state['values']['item_id'] === $form_state['values']['settings']['parent_id']) {
      form_set_error('settings][parent_id', t("The menu item cannot be its own parent."));
    }
  }

  if (!empty($form_state['values']['settings']['active_path'])){
    if ($form_state['values']['settings']['active_path'] !== '<top>'){
      if (valid_url($form_state['values']['settings']['url'], TRUE)){
        form_set_error('settings][active_path', t("The active path cannot be a full URL."));
      }
      else if (!valid_url($form_state['values']['settings']['active_path'])){
        form_set_error('settings][active_path', t("'%path' is not a valid path.", array('%path' => $form_state['values']['settings']['active_path'])));
      }
    }
  }

  if (!is_numeric($form_state['values']['settings']['weight'])) {
    form_set_error('settings][weight', t("The items weight can only be a number."));
  }

  switch ($item_type){
    case 'item_link':
      if (empty($form_state['values']['settings']['url'])){
        form_set_error('settings][url', t("The URL is required for an item type of %item.", array('%item' => $form['settings']['settings']['item_type']['#options'][$item_type])));
      }
      else if ($form_state['values']['settings']['url'] !== '<top>'){
        if (!valid_url($form_state['values']['settings']['url'], TRUE)){
          if (!valid_url($form_state['values']['settings']['url'])){
            form_set_error('settings][url', t("'%url' is not a valid URL.", array('%url' => $form_state['values']['settings']['url'])));
          }
        }
      }
      break;

    case 'item_file':
      if (empty($form_state['values']['settings']['file'])){
        form_set_error('settings][file', t("The file is required for an item type of %item.", array('%item' => $form['settings']['settings']['item_type']['#options'][$item_type])));
      }

      if (!valid_url($form_state['values']['settings']['file'], TRUE)){
        if (!valid_url($form_state['values']['settings']['file'])){
          form_set_error('settings][file', t("'%file' is not a valid URL.", array('%file' => $form_state['values']['settings']['file'])));
        }
      }

      break;

    case 'item_menu':
    case 'item_menu_item':
      form_set_error('settings][url', t("The item type '%type' is not yet supported.", array('%type' => $form['settings']['settings']['item_type']['#options'][$item_type])));
      break;
  }
}

/**
 * Submit process for workbench_menu_admin_item_settings_edit_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_edit_form_submit($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  $transaction = db_transaction();

  if (isset($form_state['values']['item_id'])){
    $item_id = $form_state['values']['item_id'];
  }
  else {
    $item_id = NULL;
  }

  $properties = array();

  foreach (array('url', 'disabled', 'file', 'file_type') as $property){
    if (isset($form_state['values']['settings'][$property])){
      $properties[$property] = &$form_state['values']['settings'][$property];
    }
  }

  try {
    if (is_null($item_id)){
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

    $record['menu_id'] = $form_state['values']['menu_id'];
    $record['parent_id'] = $form_state['values']['settings']['parent_id'];
    $record['human_name'] = $form_state['values']['settings']['human_name'];
    $record['weight'] = $form_state['values']['settings']['weight'];
    $record['active_path'] = $form_state['values']['settings']['active_path'];
    $record['properties'] = $properties;

    if (empty($existing)){
      drupal_write_record('workbench_menu_items', $record);
      $item_id = $record['id'];
    }
    else {
      $primary_keys = array('id');
      $record['id'] = $item_id;
      drupal_write_record('workbench_menu_items', $record, $primary_keys);
    }

    if (is_null($item_id)){
      $query = db_select('workbench_menu_items', 'wmi');
      $query->fields('wmi');
      $query->condition('id', $item_id);
      $results = $query->execute()->fetchAll();
    }

    foreach (array('breadcrumb', 'expand', 'item_type') as $name){
      if (!empty($existing)){
        $query = db_delete('workbench_menu_' . $name . 's');
        $query->condition('item_id', $item_id);
        $query->execute();
      }

      $options = (array) workbench_menu_get_options($name, NULL, $function_history);

      if (!empty($form_state['values']['settings'][$name]) && isset($options[$form_state['values']['settings'][$name]])){
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
    cf_error_on_query_execution($function_history, $e);
  }

  if (empty($form_state['values']['redirect_to'])){
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Edit an existing menu item from the specified menu.
 *
 * @param $access_type
 *   The type of access requested (e.g.g taxonomy).
 * @param $access_type_id
 *   The id for this specific access (here, a taxnomy term tid).
 * @param $item_id
 *   The numerical id for a menu item.
 * @param $return_destination
 *   A url path string that designates where this form should return to on submit or cancel.
 *
 * @return
 *   A form array for adding a new menu item.
 */
function workbench_menu_admin_item_settings_delete_form($form, &$form_state, $access_type, $access_type_id, $item_id, $return_destination){
  cf_error_append_history($function_history, __FUNCTION__);

  if ($access_type !== 'taxonomy'){
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($access_type_id)){
    cf_error_not_numeric($function_history, 'access_type_id');
    drupal_access_denied();
    drupal_exit();
  }

  if (!is_numeric($item_id)){
    cf_error_not_numeric($function_history, 'item_id');
    drupal_access_denied();
    drupal_exit();
  }

  $term = taxonomy_term_load($access_type_id);
  $destination = isset($_GET['destination']) ? $_GET['destination'] : $return_destination;
  $items = (array) workbench_menu_get_load_menu_items(array('id' => $item_id), 'id', $function_history);

  if (!isset($items['flat'][$item_id])){
    drupal_access_denied();
    drupal_exit();
  }

  $item = &$items['flat'][$item_id]['item'];

  $form['item_id'] = array(
    '#type' => 'value',
    '#value' => $item->id,
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
 * Submit process for workbench_menu_admin_item_settings_delete_form().
 *
 * @param $form
 *   The form array as defined by drupals form api.
 * @param $form_state
 *   The form state array as defined by drupals form api.
 */
function workbench_menu_admin_item_settings_delete_form_submit($form, &$form_state) {
  cf_error_append_history($function_history, __FUNCTION__);

  $transaction = db_transaction();

  try {
    $query = db_delete('workbench_menu_items');
    $query->condition('id', $form_state['values']['item_id']);
    $query->execute();

    foreach (array('breadcrumb', 'expand', 'item_type') as $name){
      $query = db_delete('workbench_menu_' . $name . 's');
      $query->condition('item_id', $form_state['values']['item_id']);
      $query->execute();
    }

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();
    cf_error_on_query_execution($function_history, $e);
  }

  if (empty($form_state['values']['redirect_to'])){
    $form_state['redirect'] = 'admin/config/workbench/access/menus';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}
