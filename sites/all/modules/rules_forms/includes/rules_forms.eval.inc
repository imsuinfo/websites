<?php

/**
 * @file
 * Evaluation functions for Rules Forms module.
 */

/**
 * Condition: Check a form element value.
 */
function rules_forms_condition_element_value($form, $form_state, $element, $value) {
  $form_element = &_rules_forms_get_element($form, $element);

  // First check if a value exists.
  if (isset($form_element['#value'])) {
    // Multiple values come in as array.
    if (is_array($form_element['#value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#value']);
    }
    return $form_element['#value'] === $value;
  }

  // If no #value existed, check against the default value.
  if (isset($form_element['#default_value'])) {
    if (is_array($form_element['#default_value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#default_value']);
    }
    return $form_element['#default_value'] === $value;
  }
  return FALSE;
}

/**
 * Condition: Form element value has changed.
 */
function rules_forms_condition_element_changed($form, $form_state, $element) {
  $form_id = isset($form['#id']) ? str_replace('-', '_', $form['#id']) : '';

  if (isset($_SESSION['rules_forms_form_'. $form_id .'_values'])) {
    // Get the build form values and the element name being validated.
    $element_name = substr($element, strpos($element, ':') + 1);
    $build_values = $_SESSION['rules_forms_form_'. $form_id .'_values'];

    // Ensure that form values are set. This will prevent the condition from being evaluated during build.
    if (isset($build_values[$element]) && isset($form_state['values']) && isset($form_state['values'][$element_name])) {
      return $build_values[$element] !== $form_state['values'][$element_name];
    }
  }
  return FALSE;
}

/**
 * Returns TRUE if both arrays contain the same values, regardless of their keys
 * and value ordering.
 */
function rules_forms_equal_array_values($array1, $array2) {
  $diff1 = array_diff($array1, $array2);
  $diff2 = array_diff($array2, $array1);
  return empty($diff1) && empty($diff2);
}

/**
 * Action: Set the redirect target.
 */
function rules_forms_action_redirect($form_state, $path, $query, $fragment) {
  // We manually check the form to see if redirect is okay.
  // Setting $form_state['redirect'] has proven to be unreliable.
  if (!empty($form_state['programmed']) || !empty($form_state['rebuild']) || !empty($form_state['no_redirect'])) {
    return;
  }

  // Check if a query was defined and build the options array.
  $options = $query != '' ? array('query' => array($query)) : array();
  $options['fragment'] = $fragment;

  // Redirect the user.
  drupal_goto($path, $options);
}

/**
 * Action: Set the title of a form element.
 */
function rules_forms_action_set_title($form, $element, $title) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#title'] = $title;
}

/**
 * Action: Set the description of a form element.
 */
function rules_forms_action_set_description($form, $element, $description) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#description'] = $description;
}

/**
 * Action: Hide a form element.
 */
function rules_forms_action_set_access($form, $element, $access) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#access'] = $access == 0;
}

/**
 * Action: Disable a form element.
 */
function rules_forms_action_set_disabled($form, $element, $disabled) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#disabled'] = $disabled == 1;
}

/**
 * Action: Require a form element.
 */
function rules_forms_action_set_required($form, $element, $required) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#required'] = $required == 1;
}

/**
 * Action: Set the default value of a form element.
 */
function rules_forms_action_set_default($form, $element, $value) {
  $form_element = &_rules_forms_get_element($form, $element);

  // Try to determine if this is a multiple option element.
  if (isset($form_element['#options']) && is_array($form_element['#options'])) {
    $form_element['#default_value'] = explode("\r\n", $value);
  }
  else {
    $form_element['#default_value'] = $value;
  }
}

/**
 * Action: Set the prefix of a form element.
 */
function rules_forms_action_set_prefix_suffix_html($form, $element, $prefix, $suffix) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#prefix'] = $prefix;
  $form_element['#suffix'] = $suffix;
}

/**
 * Action: Adjust weight of a form element.
 */
function rules_forms_action_set_weight($form, $element, $weight) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#weight'] = (integer) $weight;
}

/**
 * Action: Set form error.
 */
function rules_forms_action_set_error($form, $element, $message) {
  if (strpos($element, ':') !== FALSE) {
    $form_element = &_rules_forms_get_element($form, $element);

    // Remove the element type information.
    $element = substr($element, strpos($element, ':') + 1);

    if (isset($form_element['#parents'])) {
      $element = implode('][', $form_element['#parents']);
    }
    else {
      $element = str_replace(':', '][', $element);
    }
  }
  form_set_error($element, $message);
}

/**
 * Validation callback for set options action.
 */
function rules_forms_action_set_options_validate($element) {
  // Check for duplicate key values to prevent unexpected data loss. Require
  // all options to include a safe_key.
  $lines = explode("\n", trim($element->settings['options']));
  $existing_keys = array();
  $duplicate_keys = array();
  $missing_keys = array();
  $long_keys = array();
  $group = '';

  // Check each option for validity - length, existence, and duplication.
  foreach ($lines as $line) {
    $matches = array();
    $line = trim($line);
    if (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
      $key = $matches[1];

      // Validate the length of the key.
      if (strlen($key) > 128) {
        $long_keys[] = $key;
      }
    }
    // The key was not found.
    else {
      $missing_keys[] = $line;
    }

    // Ensure this is not a duplicate key.
    if (isset($key)) {
      if (isset($existing_keys[$group][$key])) {
        $duplicate_keys[$key] = $key;
      }
      else {
        $existing_keys[$group][$key] = $key;
      }
    }
  }

  // Throw exceptions for validation errors.
  if (!empty($missing_keys)) {
    throw new RulesIntegrityException(t('Every option must have a key specified. Specify each option as "safe_key|Some readable option'), $element);
  }

  if (!empty($long_keys)) {
    throw new RulesIntegrityException(t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys), $element);
  }

  if (!empty($duplicate_keys)) {
    throw new RulesIntegrityException(t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)), $element);
  }
}

/**
 * Action: Set multiple value options of a form element.
 * Note: For multiple option values each key value pair is on its own line
 * and formatted key|value.
 */
function rules_forms_action_set_options($form, $element_id, $value) {
  $form_element = &_rules_forms_get_element($form, $element_id);
  $lines = explode("\r\n", trim($value));

  $processed_options = array();
  foreach ($lines as $line) {
    $line = trim($line);
    if (preg_match('/^([^|]+)\|(.*)$/', $line, $matches)) {
      $processed_options[$matches[1]] = $matches[2];
    }
  }
  $form_element['#options'] = $processed_options;
}

