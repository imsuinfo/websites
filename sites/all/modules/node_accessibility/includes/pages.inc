<?php

/**
 * @file
 * Defines the accessibilty page functions.
 */

/**
 * Provides the accessibility tab page.
 */
function node_accessibility_accessibility_tab_page($form, &$form_state, $node, $vid = NULL) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  drupal_set_title(t("Accessibility Information for %title", array('%title' => $node->title)), PASS_THROUGH);

  $form = array();
  $reports = array();
  $display_levels = quail_api_get_display_levels(NULL, $function_history);
  $methods = quail_api_get_validation_methods(NULL, $function_history);
  $markup_format = 'full_html';
  $automatic = TRUE;
  $database = FALSE;
  $perform_validation = FALSE;

  $vids = array();


  if ($vid == NULL) {
    if (module_exists('workbench_moderation') && _workbench_moderation_access_current_draft($node)) {
      if (property_exists($node, 'workbench_moderation') && isset($node->workbench_moderation['current'])) {
        $node = node_load($node->workbench_moderation['current']->nid, $node->workbench_moderation['current']->vid);
        $vids[$node->nid] = array($node->vid);
      }
    }
  }
  else if (is_numeric($vid)) {
    $vids[$node->nid] = array($vid);
  }

  if (isset($form_state['perform_validation'])) {
    $perform_validation = TRUE;
  }

  if (property_exists($node, 'accessibility') && is_array($node->accessibility)) {
    if (!empty($node->accessibility['format'])) {
      $markup_format = $node->accessibility['format'];
    }

    if (!empty($node->accessibility['method']) && !empty($methods[$node->accessibility['method']])) {
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
    '#description' => theme('node_accessibility_information', array('node' => $node)),
  );

  $form['accessibility_validation_results'] = array(
    '#type' => 'item',
    '#title' => t("Accessibility Validation Results"),
  );

  foreach ($display_levels as $severity => $severity_settings) {
    $form['accessibility_validation_results'][$severity_settings['machine_name']] = array(
      '#type' => 'fieldset',
      '#title' =>  $severity_settings['human_name'],
      '#collapsible' => TRUE,
      '#collapsed' => ($severity == 1 ? FALSE : TRUE),
      '#tree' => TRUE,
    );
  }

  if ($automatic || $perform_validation) {
    $results = node_accessibility_perform_validation(array($node), $vids, NULL, NULL, $function_history);

    if (isset($results[$node->nid][$node->vid]['report'])) {
      $reports = $results[$node->nid][$node->vid]['report'];
    }

    unset($results);

    if ($database && !empty($reports)) {
      $no_failures = TRUE;

      foreach ($reports as $severity => $severity_results) {
        if (isset($severity_results['total']) && $severity_results['total'] > 0) {
          $no_failures = FALSE;
          break;
        }
      }

      if ($no_failures) {
        node_accessibility_delete_node_problems($node->nid, $node->vid, $function_history);
      }
      else {
        node_accessibility_save_node_problems($node->nid, $node->vid, $reports, $function_history);
      }
    }
  }
  else if ($database) {
    $reports = node_accessibility_restructure_results($node->nid, $node->vid, $display_levels, $function_history);
  }

  if (!empty($reports)) {
    foreach ($reports as $severity => $severity_results) {
      $form['accessibility_validation_results'][$display_levels[$severity]['machine_name']]['markup'] = array(
        '#type' => 'markup',
        '#markup' => theme('quail_api_results', array('severity_id' => $severity, 'severity_array' => $display_levels[$severity], 'severity_results' => $severity_results, 'markup_format' => $markup_format, 'display_title' => FALSE)),
      );
    }
  }
  else {
    foreach ($display_levels as $severity => $severity_settings) {
      $form['accessibility_validation_results'][$severity_settings['machine_name']]['markup'] = array(
        '#type' => 'markup',
        '#markup' => theme('quail_api_results', array('severity_id' => $severity, 'severity_array' => $display_levels[$severity], 'severity_results' => array('total' => 0), 'markup_format' => $markup_format, 'display_title' => FALSE)),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');

  if (!$automatic) {
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
    '#value' => $reports,
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
 *
 * This is a stub function.
 */
function node_accessibility_accessibility_tab_page_submit($form, &$form_state) {
  $form_state['perform_validation'] = TRUE;
  $form_state['rebuild'] = TRUE;
}
