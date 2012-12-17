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
      'private' => 1,
    ),
  );
}

function _webform_edit_serial($component) {
  drupal_set_message(t('Do not disable the <em>Private</em> checkbox.'), 'warning');
  return array();
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

      $submission->data[$cid]['value'] = array(0 => ++$current);
    }
  }
}
