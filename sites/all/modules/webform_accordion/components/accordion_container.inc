<?php
/**
 * @file
 * Defines the Accordion Container webform component.
 */


/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_accordion_grp() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'title_display' => 0,
      'collapsible' => 0,
      'collapsed' => 0,
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_accordion_grp($component) {
  $form = array();

  // Empty placeholder so that users don't get a warning about
  // the component not having an edit function defined.
  $form['webform_accordion_grp_placeholder'] = array(
    '#type' => 'markup',
    '#value' => '',
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_accordion_grp($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL,
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attributes' => array('class' => array('webform-component-accordion-grp'), 'id' => 'webform-component-' . $component['form_key']),
    '#pre_render' => array('webform_accordion_grp_prerender', 'webform_element_title_display'),
    '#translatable' => array('title', 'description'),
    '#prefix' => '<div class="accordion-container">',
    '#suffix' => '</div>',
    '#markup' => '',
  );
  return $element;
}

/**
 * Pre-render function to set a fieldset ID.
 */
function webform_accordion_grp_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_accordion_grp($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme_wrappers' => array('webform_element_text'),
      '#translatable' => array('title'),
    );
  }
  else {
    $element = _webform_render_accordion_grp($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
