<?php

/**
 * Provides the accessibility tab page.
 */
function quail_api_node_accessibility_tab_page($form, &$form_state, $node) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  drupal_set_title(t("Accessibility Information for %title", array('%title' => $node->title)), PASS_THROUGH);

  $form = array();

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

  if (property_exists($node, 'accessibility') && is_array($node->accessibility)){
    if (!empty($node->accessibility['format'])){
      $markup_format = $node->accessibility['format'];
    }

    if (!empty($node->accessibility['method']) && !empty($methods[$node->accessibility['method']])){
      $automatic = $methods[$node->accessibility['method']]['automatic'];
      $database = $methods[$node->accessibility['method']]['database'];
    }
  }

  $form['about_this_node'] = array(
    '#type' => 'item',
    '#title' => t("About This Node"),
  );

  $form['about_this_node']['node_information'] = array(
    '#type' => 'fieldset',
    '#title' =>  t("Node Information"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#description' => theme('quail_api_node_information', array('node' => $node)),
  );

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
    $results = quail_api_node_perform_validation(array($node), array(), NULL, NULL, $function_history);

    if (isset($results[$node->nid][$node->vid]['report'])){
      $results = $results[$node->nid][$node->vid]['report'];
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

  // store the variables in the form such that hook_form_alter() functions don't have to build the data
  $form['variables']['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  $form['variables']['results'] = array(
    '#type' => 'value',
    '#value' => $results,
  );

  $form['variables']['display_levels'] = array(
    '#type' => 'value',
    '#value' => $display_levels,
  );

  $form['variables']['methods'] = array(
    '#type' => 'value',
    '#value' => $methods,
  );

  $form['variables']['markup_format'] = array(
    '#type' => 'value',
    '#value' => $markup_format,
  );

  $form['variables']['automatic'] = array(
    '#type' => 'value',
    '#value' => $automatic,
  );

  $form['variables']['database'] = array(
    '#type' => 'value',
    '#value' => $database,
  );

  $form['variables']['perform_validation'] = array(
    '#type' => 'value',
    '#value' => $perform_validation,
  );

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
