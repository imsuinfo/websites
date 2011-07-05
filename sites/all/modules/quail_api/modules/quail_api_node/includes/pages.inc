<?php

/**
 * Provides the accessibility tab page.
 */
function quail_api_node_accessibility_tab_page($form, &$form_state, $node) {
  $form = array();
  $function_history = array(__FUNCTION__);

  $results = array();
  $display_levels = quail_api_get_display_levels(NULL, $function_history);
  $methods = quail_api_get_validation_methods(NULL, $function_history);
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
    $results = quail_api_node_validate(array($node), NULL, NULL, array(__FUNCTION__));

    if (isset($results[$node->nid]['report'])){
      $results = $results[$node->nid]['report'];
    }
    else {
      $results = array();
    }

    if ($database && !empty($results)){
      $no_failures = TRUE;

      foreach($results as $severity => $severity_results){
        if (isset($severity_results['total']) && $severity_results['total'] > 0){
          $no_failures = FALSE;
          break;
        }
      }

      if ($no_failures){
        quail_api_node_delete_node_tests($node->nid, $node->vid);
      }
      else {
        quail_api_node_save_node_tests($node->nid, $node->vid, $results);
      }
    }
  }
  else if ($database){
    $results = quail_api_node_restructure_results($node->nid, $node->vid, $display_levels);
  }

  if (!empty($results)){
    foreach($results as $severity => $severity_results){
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
