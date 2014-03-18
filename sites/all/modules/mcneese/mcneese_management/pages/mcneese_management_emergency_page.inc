<?php

/**
 * @file
 * Defines emergency settings page functions.
 */

/**
 * @addtogroup mcneese_management
 * @{
 */

/**
 * Provides the emergency settings form page.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_emergency_settings_page($form, &$form_state) {
  $emergency_mode = mcneese_management_get_emergency_mode();
  $emergency_node = mcneese_management_get_emergency_node();

  $node = NULL;
  if (!is_null($emergency_node)) {
    if ($emergency_node > 0) {
      $node = node_load($emergency_node);
    }
  }

  if (is_object($node)) {
    $value = $node->title . ' [nid: ' . $node->nid . ']';
    $emergency_node_value = check_plain($value);
  }

  $form = array();

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['emergency_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Emergency Mode'),
    '#default_value' => $emergency_mode,
    '#description' => t("Enable/Disable emergency mode."),
  );

  $form['settings']['emergency_node'] = array(
    '#type' => 'textfield',
    '#title' => t("Emergency Node"),
    '#default_value' => $emergency_node_value,
    '#descriptio' =>
      t("Specify the numeric node id of the page that is to be used as the emergency node.") . '<br>' .
      t("You can search for a specific node id by typing in the title in this field."),
    '#size' => 64,
    '#autocomplete_path' => 'admin/content/management/emergency/autocomplete_nid',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save Settings"),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  'admin/content/management',
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for mcneese_management_emergency_settings_page().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_emergency_settings_page_validate($form, &$form_state) {
  $emergency_node_value = &$form_state['values']['settings']['emergency_node'];

  $nid = NULL;
  if (cf_is_integer($emergency_node_value)) {
    if ($nid > 0) {
      $nid = $emergency_node_value;
    }
  }
  else {
    $matches = array();
    $matched = preg_match('/ \[nid:\s*(\d+)]\s*$/i', $emergency_node_value, $matches);

    if ($matched && isset($matches[1]) && cf_is_integer($matches[1]) && $matches[1] > 0) {
      $nid = $matches[1];
      $emergency_node_value = $nid;
    }
  }

  if (is_null($nid)) {
    form_set_error('emergency_node', t("An invalid value has been specified for the emergency node."));
  }
  else {
    $node = node_load($nid);

    if (!is_object($node)) {
      form_set_error('emergency_node', t("Cannot use %nid for the emergency node because no node by that id exists.", array('%nid' => $emergency_nid)));
    }
  }
}

/**
 * Submit process for mcneese_management_emergency_settings_page().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_emergency_settings_page_submit($form, &$form_state) {
  // avoid making changes if possible because some changes requires clearing the entire site cache.
  if ($form['settings']['emergency_node']['#default_value'] != $form_state['values']['settings']['emergency_node']) {
    mcneese_management_set_emergency_node($form_state['values']['settings']['emergency_node']);
  }

  if ($form['settings']['emergency_mode']['#default_value'] != $form_state['values']['settings']['emergency_mode']) {
    mcneese_management_set_emergency_mode($form_state['values']['settings']['emergency_mode']);
  }
}

/**
 * @} End of '@addtogroup mcneese_management'.
 */
