<?php

/**
 * @file
 * Defines statistics page functions.
 */

/**
 * @addtogroup mcneese_management
 * @{
 */

/**
 * Provides the group leader settings form page.
 */
function mcneese_management_group_leader_settings_page() {
  $dependency = mcneese_management_check_group_leader_dependencies();
  if (!is_null($dependency)) {
    return $dependency;
  }

  $output = '';

  $fallback_uid = variable_get('mcneese_management_group_leader_unassigned', 1);

  $output .= t("Each group has exactly 1 group leader. These group leaders are referred to as the point of contact for making changes for that group and are considered responsible for that content. These users are expected to be e-mailed directly for issues associated with content within their group.");

  $leaders = variable_get('mcneese_management_group_leaders', array());

  $query = db_select('workbench_access_user', 'wau');
  $query->fields('wau', array('access_id', 'uid'));
  $query->distinct();
  $query->condition('access_scheme', 'taxonomy');
  $query->orderBy('access_id', 'ASC');

  $groups = (array) $query->execute()->fetchAllAssoc('access_id');
  $users = array();
  $users[$fallback_uid] = user_load($fallback_uid);

  $access = workbench_access_get_access_tree();
  $active = workbench_access_get_active_tree();

  foreach ($active['tree'] as $access_id => &$section) {
    if (!cf_is_integer($access_id)) continue;

    $sorted[$access_id] = &$section['name'];
  }

  asort($sorted);

  $menus = (array) workbench_menu_get_menus(array('access_id' => array_keys($sorted), 'fast' => NULL), 'access_id');

  $rows = array();
  foreach ($sorted as $access_id => $section_name) {
    if (!isset($active['active'][$access_id])) {
      continue;
    }

    $section = & $active['tree'][$access_id];

    $row = array();
    $row[] = l($section['name'], 'admin/content/management/group_leader/assign/' . $access_id);

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'path')) {
      $default_path = $menus[$access_id]->path;

      // be user-friendly and show normal spaces instead of %20.
      $row[] = preg_replace('/%20/', ' ', $default_path);
    }
    else {
      $row[] = '';
    }

    if (isset($menus[$access_id]) && property_exists($menus[$access_id], 'id')) {
      $row[]  = intval($menus[$access_id]->id);
    }
    else {
      $row[] = "";
    }

    $row[] = intval($access_id);

    $target_uid = $fallback_uid;
    if (array_key_exists($access_id, $leaders)) {
      $target_uid = $leaders[$access_id];
    }

    if (!array_key_exists($target_uid, $users)) {
      $users[$target_uid] = user_load($target_uid);
    }

    $row[] = l($users[$target_uid]->name, 'user/' . $target_uid);
    $rows[] = $row;
  }

  $header = array(t("Group Name"), t("Group Path"), t("Menu ID"), t("Access ID"), t("Group Leader"));
  $attributes = array();
  $attributes['id'] = 'mcneese_management_group_leaders';
  $attributes['summary'] = t("A listing of menus containing links for making changes to each individual menu.");
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t("Group Leaders"), 'attributes' => $attributes, 'empty' => t("There are no groups available.")));

  return $output;
}

/**
 * Provides the group leader settings form page.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 * @param int $access_id
 *   The taxonomy access id for the group to operate on.
 */
function mcneese_management_group_leader_operation_form($form, &$form_state, $access_id) {
  $dependency = mcneese_management_check_group_leader_dependencies();
  if (!is_null($dependency)) {
    drupal_not_found();
    drupal_exit();
  }

  if (!cf_is_integer($access_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('access_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $term = taxonomy_term_load($access_id);

  if (!is_object($term)) {
    drupal_not_found();
    drupal_exit();
  }

  $group_name = $term->name . ' [tid:' . $term->tid . ']';

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/management/group_leader';
  $fallback_uid = variable_get('mcneese_management_group_leader_unassigned', 1);
  $leaders = variable_get('mcneese_management_group_leaders', array());

  $fallback_user = user_load($fallback_uid);
  $fallback_leader = $fallback_user->name . ' [uid:' . $fallback_uid . ']';

  if (array_key_exists($access_id, $leaders)) {
    $leader_uid = $leaders[$access_id];
  }
  else {
    $leader_uid = $fallback_uid;
  }

  $leader = user_load($leader_uid);
  $default_leader = $leader->name . ' [uid:' . $leader_uid . ']';

  $form = array();

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  drupal_set_title(t("Assign Group Leader for %group", array('%group' => $group_name)), PASS_THROUGH);

  $form['settings']['group_leader'] = array(
    '#type' => 'textfield',
    '#title' => t("Username"),
    '#description' => t("Select a user to assign as the group leader."),
    '#default_value' => $default_leader,
    '#autocomplete_path' => 'admin/content/management/group_leader/ajax/autocomplete/group_leader',
  );

  $form['access_id'] = array(
    '#type' => 'value',
    '#value' => $access_id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Assign User"),
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
 * Validation for mcneese_management_group_leader_operation_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 */
function mcneese_management_group_leader_operation_form_validate($form, &$form_state) {
  if (empty($form_state['values']['settings']['group_leader'])) {
    form_set_error('settings][group_leader', "No valid user has been defined for the group leader.");
    return;
  }

  $user_id = NULL;
  $user = $form_state['values']['settings']['group_leader']; 
  $matches = array();
  $matched = preg_match('/ \[uid:(\d+)]\s*$/i', $user, $matches);

  if ($matched) {
    // check to see if the user exists.
    $query = db_select('users', 'u');
    $query->fields('u');
    $query->condition('u.uid', $matches[1]);
    $query->countQuery();

    $result = $query->execute()->fetchField();

    if ($result !== FALSE) {
      $user_id = $matches[1];
    }
  }
  else {
    // if a username was specified but no id, then try getting the uid.
    $query = db_select('users', 'u');
    $query->fields('u', array('uid'));
    $query->condition('u.name', $user);

    $result = $query->execute()->fetchField();

    if ($result !== FALSE) {
      $user_id = $result;
      $form_state['values']['settings']['group_leader'] .= ' [uid:' . $user_id . ']';
    }
  }

  if (is_null($user_id)) {
    form_set_error('settings][group_leader', t("The user %user does not exist.", array('%user' => $user)));
  }
}

/**
 * Submit process for mcneese_management_group_leader_operation_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 */
function mcneese_management_group_leader_operation_form_submit($form, &$form_state) {
  $user = $form_state['values']['settings']['group_leader']; 
  $user_id = NULL;
  $matches = array();
  $matched = preg_match('/ \[uid:(\d+)]\s*$/i', $user, $matches);

  if ($matched) {
    $user_id = $matches[1];
  }
  else {
    return;
  }


  // save the group leader.
  $leaders = variable_get('mcneese_management_group_leaders', array());
  $leaders[$form['access_id']['#value']] = $user_id;
  variable_set('mcneese_management_group_leaders', $leaders);

  // apply new group leader to all content in that group.
  $changed = mcneese_management_assign_group_leader($form['access_id']['#value'], $user_id);

  if ($changed === FALSE) {
    watchdog('alert', "Failed to assign group leader %user to all associated nodes.", array('%user' => $user));
  }


  // redirect after submitting.
  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/content/management/group_leader';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * @} End of '@addtogroup mcneese_management'.
 */