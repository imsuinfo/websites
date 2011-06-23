<?php

/**
 * Provides the accessibility tab page.
 */
function quail_api_node_accessibility_tab_page($form, &$form_state, $node) {
  $form = array();

  $results = array();
  $display_levels = quail_api_get_display_levels();
  $methods = quail_api_get_validation_methods();
  $markup_format = 'full_html';
  $automatic = TRUE;
  $database = FALSE;
  $perform_validation = FALSE;

  if (isset($form_state['perform_validation'])){
    $perform_validation = TRUE;
  }

  if (!empty($node->accessibility['format'])){
    $markup_format = $node->accessibility['format'];
  }

  if (!empty($node->accessibility['method']) && !empty($methods[$node->accessibility['method']])){
    $automatic = $methods[$node->accessibility['method']]['automatic'];
    $database = $methods[$node->accessibility['method']]['database'];
  }

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

  if ($automatic || $perform_validation){
    $results = quail_api_node_validate(array($node->nid));

    if ($database){
      // @todo: save the database results, this does not need to happen when automatic is disabled because there is no new report data to save.
    }
  }
  else if ($database){
    // @todo: manually load the database settings, this does not need to happen when automatic is enabled because there is already a fresh report available
  }

  if (!empty($results[$node->nid]['report'])){
    foreach($results[$node->nid]['report'] as $severity => $severity_results){
      $form['accessibility_validation_results'][$display_levels[$severity]['machine_name']]['markup'] = array(
        '#type' => 'markup',
        '#markup' => theme('quail_api_results', array('severity_id' => $severity, 'severity_array' => $display_levels[$severity], 'severity_results' => $severity_results, 'markup_format' => $markup_format, 'display_title' => FALSE)),
      );
    }
  }
  else {
    foreach ($display_levels as $severity => $severity_settings){
      $form['accessibility_validation_results'][$severity_settings['machine_name']]['markup'] = array(
        '#type' => 'markup',
        '#markup' => theme('quail_api_results', array('severity_id' => $severity, 'severity_array' => $display_levels[$severity], 'severity_results' => array('total' => 0), 'markup_format' => $markup_format, 'display_title' => FALSE)),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');

  if (!$automatic){
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Validate"),
    );
  }

  return $form;
}

/**
 * Provides the accessibility tab page validation button submit.
 * This is a stub function.
 */
function quail_api_node_accessibility_tab_page_submit($form, &$form_state){
  $form_state['perform_validation'] = TRUE;
  $form_state['rebuild'] = TRUE;
}
