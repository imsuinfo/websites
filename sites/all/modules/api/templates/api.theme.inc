<?php

/**
 * @file
 * Theme-related functions for the API module.
 */

/**
 * Preprocesses theme('api_defined').
 *
 * Sets up variables for the file link and summary line.
 */
function api_preprocess_api_defined(&$variables) {
  $object = $variables['object'];
  $branch = $variables['branch'];
  $variables['file_link'] = api_file_link($object);
  $file_info = api_filename_load($object->file_name, $branch->project, $branch->branch_name);
  if (isset($file_info)) {
    $variables['file_summary'] = $file_info->summary;
  }
}

/**
 * Preprocesses a documentation item page.
 */
function api_preprocess_api_object_page(&$variables) {
  // Note: branch is not defined for File pages, so don't try to create the
  // 'defined' section of the page.
  if (isset($variables['branch'])) {
    $variables['defined'] = theme('api_defined', array('branch' => $variables['branch'], 'object' => $variables['object']));
  }

  // Find alternative versions in different files and branches.
  $object = $variables['object'];
  $branches = api_get_branches();
  $alternatives = array(
    '#prefix' => '<ol id="api-alternatives">',
    '#suffix' => '</ol>',
  );
  // Try to find objects of the same type with the same object name. But for
  // files, the object name includes the path, so the matching needs to go on
  // the title instead of the object_name.
  if ($object->object_type == 'file') {
    $result = db_select('api_documentation', 'ad')
      ->fields('ad', array('branch_id', 'file_name', 'object_type', 'object_name'))
      ->condition('ad.object_type', $variables['object']->object_type)
      ->condition('ad.title', $variables['object']->title)
      ->execute();
  }
  else {
    $result = db_select('api_documentation', 'ad')
      ->fields('ad', array('branch_id', 'file_name', 'object_type', 'object_name'))
      ->condition('ad.object_type', $variables['object']->object_type)
      ->condition('ad.object_name', $variables['object']->object_name)
      ->execute();
  }

  foreach ($result as $alternative) {
    // Group by project.
    if (!isset($alternatives[$branches[$alternative->branch_id]->project])) {
      $alternatives[$branches[$alternative->branch_id]->project] = array(
        '#prefix' => '<li><strong>' . $branches[$alternative->branch_id]->project_title . '</strong><ol>',
        '#suffix' => '</ol></li>',
      );
    }

    // Construct link label.
    $label = $branches[$alternative->branch_id]->branch_name . ' ' . $alternative->file_name;

    $alternatives[$branches[$alternative->branch_id]->project][] = array(
      '#prefix' => '<li>',
      '#markup' => l($label, api_url($alternative)),
      '#weight' => $branches[$alternative->branch_id]->weight,
      '#suffix' => '</li>',
    );
  }
  $variables['alternatives'] = drupal_render($alternatives);

  // See if this is an override of another method/etc.
  $variables['override'] = '';
  if (!empty($object->overrides_did)) {
    $overrides = api_object_load((int) $object->overrides_did, $branches[$object->branch_id], array('function', 'property', 'constant'));
    if (isset($overrides)) {
      $variables['override'] = '<p class="api-override">' . t('Overrides !link', array('!link' => l($overrides->title, api_url($overrides)))) . '</p>';
    }
  }
}

/**
 * Returns HTML for a reference link on a function or file page.
 *
 * This theme hook is used for the "N functions call function()" and "N
 * functions implement hook()" links on function pages.
 *
 * @param $type
 *   The type of reference link, one of:
 *   - 'calls': Function calls.
 *   - 'implementations': Hook implementations.
 *   - 'theme_invokes': Theme calls to a function or file.
 *   - 'invokes': Calls to module_invokes and similar functions.
 *   - 'references': String references to a function name.
 * @param $count
 *   The number of referenced items.
 * @param $function
 *   The function or file object being referenced.
 *
 * @ingroup themeable
 */
function theme_api_function_reference_link($variables) {
  module_load_include('inc', 'api', 'api.pages');
  $type = $variables['type'];
  $count = $variables['count'];
  $function = $variables['function'];
  if (!isset($function->branch_id)) {
    return '';
  }

  $link_title = api_reference_text($type, $count, $function);

  // Create the link path.
  $processed_file_name = str_replace(API_FILEPATH_SEPARATOR, API_FILEPATH_SEPARATOR_REPLACEMENT, $function->file_name);
  $branch = api_get_branch_by_id($function->branch_id);

  if ($function->object_type == 'file') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/' . $type . '/' . $branch->branch_name;
  }
  else {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/function/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }

  return '<h3>' . l($link_title, $link_path) . '</h3>';
}
