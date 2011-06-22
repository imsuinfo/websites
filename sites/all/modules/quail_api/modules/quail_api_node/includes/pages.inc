<?php

/**
 * Provides the accessibility tab page.
 */
function quail_api_node_accessibility_tab_page($form, $form_state, $node) {
  $form = array();

  // @todo this is a very sloppy implementation made just to show results for quick tests cases, completely redo this to be usable and presentable

  $results = quail_api_node_validate(array($node->nid));
  $display_levels = quail_api_get_display_levels();

  $form['accessibility_validation_results'] = array(
    '#type' => 'item',
    '#title' => t("Accessibility Validation Results"),
  );

  foreach ($display_levels as $severity => $severity_settings){
    $form['accessibility_validation_results'][$severity_settings['machine_name']] = array(
      '#type' => 'fieldset',
      '#title' =>  $severity_settings['human_name'],
      '#collapsible' => TRUE,
      '#collapsed' => ($severity == 1 ? FALSE : TRUE),
      '#tree' => TRUE,
    );
  }

  // @todo this is by all means not the proper place to put these, be sure to create an appropriate *_theme() function for this tab page containg this information
  if (!empty($results[$node->nid]['report'])){
    foreach($results[$node->nid]['report'] as $severity => $severity_results){
      $form['accessibility_validation_results'][$display_levels[$severity]['machine_name']]['markup'] = array(
        '#type' => 'markup',
        // @todo Provide runtime configuration options that allows the user to select which markup_format to use
        '#markup' => theme('quail_api_results', array('severity_id' => $severity, 'severity_array' => $display_levels[$severity], 'severity_results' => $severity_results, 'markup_format' => 'full_html', 'display_title' => FALSE)),
      );
    }
  }

  return $form;
}
