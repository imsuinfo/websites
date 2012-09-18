<?php

/**
 * @file
 * Defines statistics page functions.
 */

/**
 *
 */
function mcneese_management_emergency_page() {
  return array();

  $markup = '';

  $markup .= "TODO: this will eventually have relevant content here";

  return $markup;
}

/**
 *
 */
function mcneese_management_emergency_settings_page($form, &$form_state) {
  $form = array();

  $form['temp'] = array(
    '#type' => 'markup',
    '#markup' => "TODO: this will eventually have relevant content here",
  );

  return $form;
}
