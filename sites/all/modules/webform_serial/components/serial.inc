<?php
/**
 * @file
 * Webform module serial component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_serial() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'initial' => '',
      'increment' => 1,
      'private' => TRUE,
    ),
  );
}

function _webform_edit_serial($component) {
  drupal_set_message(t('Please do not disable the <em>Private</em> checkbox.'), 'warning');

  $form = array();
  $form['extra']['initial'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial value'),
    '#default_value' => $component['extra']['initial'],
    '#description' => t('The initial value of the field. Leave blank to start counting at 1.'),
    '#size' => 8,
    '#maxlength' => 1024,
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_serial_validate'),
  );
  $form['extra']['increment'] = array(
    '#type' => 'textfield',
    '#title' => t('Increment value'),
    '#default_value' => empty($component['extra']['increment']) ? 1 : $component['extra']['increment'],
    '#description' => t('The increment value of the field. Each time the form is submitted, add this many to the serial field.'),
    '#size' => 8,
    '#maxlength' => 1024,
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_serial_validate'),
  );
  return $form;
}

/**
 * Validation of serial edit form items.
 */
function _webform_edit_serial_validate($element, &$form_state) {
  $values = $form_state['values']['extra'];
  switch ($element['#name']) {
    case 'extra[initial]':
      if (!empty($values['initial']) && !is_numeric($values['initial'])) {
        form_error($element, t('Initial value needs to be empty or a number.'));
      }
      break;

    case 'extra[increment]':
      if (empty($values['increment']) || !is_numeric($values['increment'])) {
        form_error($element, t('Increment value needs to be a number and may not be zero.'));
      }
      break;
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_serial($component, $value, $format = 'text') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => 'text',
    '#markup' => $value[0],
    '#webform_component' => $component,
    '#field_suffix' => NULL,
    '#field_prefix' => NULL,
  );
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_serial($component, $value, $format = 'text') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title'),
    '#field_suffix' => NULL,
    '#field_prefix' => NULL,
  );
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_serial($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC));
  $query->addExpression('MIN(data)', 'minimum');
  $query->condition('nid', $component['nid']);
  $query->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }
  $minimum = $query->execute()->fetchField();

  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC));
  $query->addExpression('MAX(data)', 'maximum');
  $query->condition('nid', $component['nid']);
  $query->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }
  $maximum = $query->execute()->fetchField();

  $rows[0] = array(t('Lowest serial'), $minimum);
  $rows[1] = array(t('Highest serial'), $maximum);
  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_serial($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_serial($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_serial($component, $export_options, $value) {
  return $value;
}

/**
 * Overridable theme function.
 */
function theme_webform_display_serial($variables) {
  $element = $variables['element'];
  return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
 * Implements hook_webform_submission_presave()
 */
function webform_serial_webform_submission_presave($node, &$submission) {
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['type'] == 'serial' && empty($submission->data[$cid])) {
      // If no value, generate one.
      $query = db_select('webform_submitted_data', 'd');
      $query->addExpression('MAX(data)', 'current');
      $query->condition('nid', $node->nid, '=');
      $query->condition('cid', $cid, '=');

      // Cast this to int, because the first query will return NULL.
      $current = (int)$query->execute()->fetchField();

      // Calculate the increment (default to 1).
      $increment = empty($component['extra']['increment']) ? 1 : (int)$component['extra']['increment'];

      // If there is no current (first entry) then check if we have an
      // initial value we should be using.
      if (empty($current)) {
        // Subtract $increment because we're incrementing again below.
        $current = (int)$component['extra']['initial'] - $increment;
      }

      $submission->data[$cid]['value'] = array(0 => $current + $increment);
    }
  }
}
