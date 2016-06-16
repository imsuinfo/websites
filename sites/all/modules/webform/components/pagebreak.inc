<?php

/**
 * @file
 * Webform module page break component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_pagebreak() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'private' => FALSE,
      'next_page_label' => '',
      'prev_page_label' => '',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_pagebreak() {
  return array(
    'webform_display_pagebreak' => array(
      'render element' => 'element',
      'file' => 'components/pagebreak.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_pagebreak($component) {
  $form = array();
  // Force the parent to always be root.
  $form['position']['pid'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );

  $form['extra']['next_page_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Next page button label'),
    '#description' => t('This is used for the <em>Next Page</em> button on the page before this page break. Default: <em>Next Page &gt;</em>'),
    '#default_value' => $component['extra']['next_page_label'],
    '#size' => 30,
  );
  $form['extra']['prev_page_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous page button label'),
    '#description' => t('This is used for the <em>Previous Page</em> button on the page after this page break. Default: <em>&lt; Prev Page</em>'),
    '#default_value' => $component['extra']['prev_page_label'],
    '#size' => 30,
  );

  $form['extra']['pb_title_display'] = array(
    '#type' => 'select',
    '#title' => t('Title Display'),
    '#description' => t('This is used to display the pagebreak field name as a title between each page break entry.'),
    '#default_value' => isset($component['extra']['pb_title_display']) ? $component['extra']['pb_title_display'] : NULL,
    '#multiple' => FALSE,
    '#options' => array(
      NULL => "Do not display",
      2 => "Heading 2",
      3 => "Heading 3",
      4 => "Heading 4",
      5 => "Heading 5",
      6 => "Heading 6",
      7 => "Divider",
      8 => "Span",
    ),
  );

  $form['extra']['pb_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Title Classes'),
    '#default_value' => isset($component['extra']['pb_classes']) ? $component['extra']['pb_classes'] : '',
    '#description' => t('Apply a class to the title field. Separate multiple classes by spaces.'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_pagebreak($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $element = array(
    '#type' => 'hidden',
    '#value' => $component['name'],
    '#weight' => $component['weight'],
  );
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_pagebreak($component, $value = NULL, $format = 'html', $submission = array()) {
  $element = array(
    '#theme' => 'webform_display_pagebreak',
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#translatable' => array('title'),
    '#page_break_settings' => array(
      'title_display' => isset($component['extra']['pb_title_display']) ? (int) $component['extra']['pb_title_display'] : NULL,
      'title_classes' => isset($component['extra']['pb_classes']) ? $component['extra']['pb_classes'] : '',
    ),
  );


  return $element;
}

/**
 * Format the text output data for this component.
 */
function theme_webform_display_pagebreak($variables) {
  $element = $variables['element'];

  // when title display is NULL (or the settings are not defined) then do not display the page break.
  if (!isset($element['#page_break_settings']['title_display'])) {
    return '';
  }

  $attributes = array(
    'classes' => array('webform-page'),
  );

  $classes = explode(' ', $element['#page_break_settings']['title_classes']);
  $attributes['classes'] = array_merge($attributes['classes'], $classes); 
  unset($classes);

  $tag = 'h2';
  if (isset($element['#page_break_settings']['title_display'])) {
    switch ($element['#page_break_settings']['title_display']) {
      case 2:
        $tag = 'h2';
        break;
      case 3:
        $tag = 'h3';
        break;
      case 4:
        $tag = 'h4';
        break;
      case 5:
        $tag = 'h5';
        break;
      case 6:
        $tag = 'h6';
        break;
      case 7:
        $tag = 'div';
        break;
      case 8:
        $tag = 'span';
        break;
    }
  }

  return $element['#format'] == 'html' ? '<' . $tag . drupal_attributes($attributes) . '>' . check_plain($element['#title']) . '</' . $tag . '>' : '==' . $element['#title'] . "==\n";
}
