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
  if (isset($form_element['#value'])) {
    // Multiple values come in as array.
    if (is_array($form_element['#value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#value']);
    }
    return $form_element['#value'] === $value;
  }
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
 * Returns TRUE if both arrays contain the same values, regardless of their keys
 * and value ordering.
 */
function rules_forms_equal_array_values($array1, $array2) {
  $diff1 = array_diff($array1, $array2);
  $diff2 = array_diff($array2, $array1);
  return empty($diff1) && empty($diff2);
}

/**
 * Action: Load a form element.
 */
function rules_forms_action_load_element($form, $element) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (!empty($form_element)) {
    return array('element_fetched' => $element);
  }
}

/**
 * Validation callback for create element action.
 */
function rules_forms_action_create_element_validate($element) {
  if (!isset($element->settings['type'])) {
    throw new RulesException('Invalid element type specified.', array(), array($element, 'parameter', 'type'));
  }
}

/**
 * Action: Create a new form element.
 *
 * @param $form
 *   The form on which the element will be placed. This argument is passed
 *   as an ArrayObject, so we reference it.
 * @param $type
 *   The type of form element being created.
 * @param $parent
 *   A string identifying the parent element of the form element bring created.
 * @param $name
 *   The machine-readable name of a form element.
 * @param $title
 *   The '#title' attribute of the created form element.
 */
function rules_forms_action_create_element($form, $type, $parent, $name, $title) {
  // Create the element by reference using the parent.
  if (!empty($parent)) {
    $element_id = $parent .'['. $name .']';
    $parent_element = &_rules_forms_get_element($form, $parent);
    $parent_element[$name] = array('#type' => $type, '#title' => $title);
  }
  // Create the element directly on the form.
  else {
    $element_id = $name;
    $form[$name] = array('#type' => $type, '#title' => $title);
  }

  // Retrieve the element from the form.
  $form_element = &_rules_forms_get_element($form, $element_id);
  return array('element_created' => $element_id);
}

/**
 * Defines attributes importance for form elements.
 */
function _rules_forms_element_attributes() {
  return array(
    'type' => RULES_FORMS_REQUIRED,
    'title' => RULES_FORMS_REQUIRED,
    'default_value' => RULES_FORMS_OPTIONAL,
    'description' => RULES_FORMS_OPTIONAL,
    'disabled' => RULES_FORMS_OPTIONAL,
    'required' => RULES_FORMS_OPTIONAL,
    'access' => RULES_FORMS_OPTIONAL,
  );
}

/**
 * Action: Set the redirect target.
 */
function rules_forms_action_redirect($form_state, $path, $query, $fragment) {
  $form_state['redirect'] = array($path, array('query' => array($query), 'fragment' => $fragment));
}

/**
 * Action: Adjust weight of a form element.
 */
function rules_forms_action_adjust_weight($form, $element, $weight) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#weight'] = $weight;
}

/**
 * Action: Insert prefix/suffix HTML code.
 */
function rules_forms_action_prefix_suffix_html($form, $element, $prefix, $suffix) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (!empty($prefix)) {
    $form_element['#prefix'] = $prefix;
  }
  if (!empty($suffix)) {
    $form_element['#suffix'] = $suffix;
  }
}

/**
 * Action: Set form error.
 */
function rules_forms_action_set_error($form, $element, $message) {
  if (substr($element, strlen($element)-1) === ']') {
    $form_element = &_rules_forms_get_element($form, $element);
    if (isset($form_element['#parents'])) {
      $element = implode('][', $form_element['#parents']);
    }
    else {
      // Remove trailing ']'.
      $element = substr($element, 0, strlen($element)-1);
      // Insert ']' before the first '['.
      $first_part = substr($element, 0, strpos($element, '['));
      $second_part = substr($element, strpos($element, '['));
      $element = $first_part .']'. $second_part;
    }
  }
  form_set_error($element, $message);
}

/**
 * Action: Set the default value.
 */
function rules_forms_action_set_element($form, $element, $value) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (empty($form_element)) {
    return;
  }
  $lines = explode("\r\n", $value);
  if (count($lines) == 1) {
    $form_element['#default_value'] = $value;
  }
  else {
    $form_element['#default_value'] = $lines;
  }
}

/**
 * Action: Set text attributes of a form element.
 */
function rules_forms_action_set_text($form, $element, $title, $description) {
  $form_element = &_rules_forms_get_element($form, $element);
  $attributes = _rules_forms_element_attributes();
  foreach ($attributes as $key => $value) {
    // Empty attribute strings will be ignored.
    if (!empty(${$key})) {
      $attribute = '#'. $key;
      $form_element[$attribute] = ${$key};
    }
  }
}

/**
 * Action: Set form attributes.
 */
function rules_forms_action_set_access($form, $element, $access, $disabled, $required) {
  // Access is selected by the user in the opposite fashion of how it actually works.
  // Set the '#access' attribute based on whether to *hide* the form element or not.
  $access = $access == 0;
  $form_element = &_rules_forms_get_element($form, $element);
  $attributes = _rules_forms_element_attributes();
  foreach ($attributes as $key => $value) {
    $attribute = '#'. $key;
    // Since we're dealing with boolean values, we just check if the setting changed.
    if (isset(${$key})) {
      $form_element[$attribute] = ${$key};
    }
  }
}

/**
 * Action: Set multiple value element options.
 */
function rules_forms_action_set_options($form, $element, $options) {
  $form_element = &_rules_forms_get_element($form, $element);
  $form_element['#options'] = explode("\r\n", $options);
}

/**
 * Helper function to extract a reference to a form element by a given name.
 */
function &_rules_forms_get_element(&$form, $name) {
  $names = explode('[', $name);
  $element = &$form;
  foreach ($names as $name) {
    if (substr($name, strlen($name)-1) === ']') {
      // Remove trailing ']'.
      $name = substr($name, 0, strlen($name)-1);
    }
    $element =& $element[$name];
  }
  return $element;
}
