<?php

/**
 * @file
 *
 * Pages for background batch.
 *
 * @todo Implement proper error page instead of just 404.
 */


/**
 * System settings page.
 */
function background_batch_settings_form() {
  $form = array();
  $form['background_batch_delay'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('background_batch_delay', BACKGROUND_BATCH_DELAY),
    '#title' => 'Delay',
    '#description' => t('Time in microseconds for progress refresh'),
  );
  $form['background_batch_process_lifespan'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('background_batch_process_lifespan', BACKGROUND_BATCH_PROCESS_LIFESPAN),
    '#title' => 'Process lifespan',
    '#description' => t('Time in milliseconds for progress lifespan'),
  );
  $form['background_batch_show_eta'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('background_batch_show_eta', BACKGROUND_BATCH_PROCESS_ETA),
    '#title' => 'Show ETA of batch process',
    '#description' => t('Whether ETA (estimated time of arrival) information should be shown'),
  );
  return system_settings_form($form);
}

/**
 * Overview of current and recent batch jobs.
 */
function background_batch_overview_page() {
  $data = array();
  $sql = "
SELECT b.bid, p.progress, p.message, p.start_stamp, p.end_stamp, p.current_stamp
FROM {batch} b
JOIN {progress} p
ON p.name = CONCAT('_background_batch:', b.bid)
ORDER BY b.bid
";
  $result = db_query($sql);
  while ($row = $result->fetchObject()) {
    $eta = progress_estimate_completion($row);
    $data[] = array(
      $row->end_stamp ? $row->bid : l($row->bid, 'batch', array('query' => array('op' => 'start', 'id' => $row->bid))),
      ($row->progress * 100) . '%',
      $row->message,
      $row->start_stamp ? format_date((int)$row->start_stamp, 'small') : t('N/A'),
      $row->end_stamp ? format_date((int)$row->end_stamp, 'small') : ($eta ? format_date((int)$eta, 'small') : t('N/A')),
    );
  }
  $header = array('Batch ID', 'Progress', 'Message', 'Started', 'Finished/ETA');
  return theme('table', array(
    'header' => $header,
    'rows' => $data
  ));
}

/**
 * State-based dispatcher for the batch processing page.
 */
function background_batch_page() {
  $id = isset($_REQUEST['id']) ? $_REQUEST['id'] : FALSE;
  if (!$id) {
    return drupal_not_found();
  }

  // Retrieve the current state of batch from db.
  $data = db_query("SELECT batch FROM {batch} WHERE bid = :bid", array(':bid' => $id))->fetchColumn();
  if (!$data) {
    return drupal_not_found();
  }

  $batch =& batch_get();
  $batch = unserialize($data);

  // Check if the current user owns (has access to) this batch.
  global $user;
  if ($batch['uid'] != $user->uid) {
    return drupal_access_denied();
  }

  $op = isset($_REQUEST['op']) ? $_REQUEST['op'] : '';
  switch ($op) {
    case 'start':
      return _background_batch_page_start();
    case 'do':
      return _background_batch_page_do_js();
    case 'do_nojs':
      return _background_batch_page_do_nojs();
    case 'finished':
      progress_remove_progress('_background_batch:' . $id);
      return _batch_finished();
    default:
      drupal_goto('admin/config/system/batch/overview');
  }
}

/**
 * Start a batch job in the background
 */
function _background_batch_initiate() {
  require_once('includes/batch.inc');
  $batch =& batch_get();
  $id = $batch['id'];

  $handle = 'background_batch:' . $id;
  // If batch is already in progress, goto to the status page instead of starting it.
  $process = background_process_get_process($handle);
  $progress = progress_get_progress('_' . $handle);
  if (!$process) {
    if (!$progress || !$progress->end_stamp) {
      // Hasn't run yet or has stopped. (re)start batch job.
      $process = new BackgroundProcess($handle);
      $process->service_host = 'background_batch';
      if ($process->lock()) {
        $message = $batch['sets'][0]['init_message'];
        progress_initialize_progress('_' . $handle, $message);
        db_query("UPDATE {progress} SET start_stamp = :start WHERE name = :name", array(':start' => $batch['timestamp'], ':name' => '_' . $handle));
        $result = $process->execute('_background_batch_process', array($id));
      }
    }
    else {
      // Job has already run and finished. Cleanup?
    }
  }
  else {
    // Job running. Goto status page.
  }
}

function _background_batch_page_start() {
  _background_batch_initiate();
  if (isset($_COOKIE['has_js']) && $_COOKIE['has_js']) {
    return _background_batch_page_progress_js();
  }
  else {
    return _background_batch_page_do_nojs();
  }
}

/**
 * Batch processing page with JavaScript support.
 */
function _background_batch_page_progress_js() {
  require_once('includes/batch.inc');

  $batch = batch_get();

  $current_set = _batch_current_set();
  drupal_set_title($current_set['title'], PASS_THROUGH);

  // Merge required query parameters for batch processing into those provided by
  // batch_set() or hook_batch_alter().
  $batch['url_options']['query']['id'] = $batch['id'];

  $js_setting['batch'] = array();
  $js_setting['batch']['errorMessage'] =  $current_set['error_message'] . '<br />' . $batch['error_message'];
  // Check wether ETA information should be shown.
  if (variable_get('background_batch_show_eta', BACKGROUND_BATCH_PROCESS_ETA)) {
    $js_setting['batch']['initMessage'] = 'ETA: ' . t('N/A') . '<br/>' . $current_set['init_message'];
  }
  else {
    $js_setting['batch']['initMessage'] = $current_set['init_message'];
  }
  $js_setting['batch']['uri'] = url($batch['url'], $batch['url_options']);
  $js_setting['batch']['delay'] = variable_get('background_batch_delay', BACKGROUND_BATCH_DELAY);

  drupal_add_js($js_setting, 'setting');
  drupal_add_library('background_batch', 'background-process.batch');

  return '<div id="progress"></div>';
}

/**
 * Do one pass of execution and inform back the browser about progression
 * (used for JavaScript-mode only).
 */
function _background_batch_page_do_js() {
  // HTTP POST required.
  if ($_SERVER['REQUEST_METHOD'] != 'POST') {
    drupal_set_message(t('HTTP POST is required.'), 'error');
    drupal_set_title(t('Error'));
    return '';
  }

  $batch = &batch_get();
  $id = $batch['id'];

  drupal_save_session(FALSE);

  // Get progress
  if ($progress = progress_get_progress('_background_batch:' . $id)) {
    $percentage = $progress->progress * 100;
    $message = $progress->message;
    progress_estimate_completion($progress);
    // Check wether ETA information should be shown.
    if (variable_get('background_batch_show_eta', BACKGROUND_BATCH_PROCESS_ETA)) {
      $message = "ETA: " . ($progress->estimate ? format_date((int)$progress->estimate, 'large') : t('N/A')) . "<br/>$message";
    }
    else {
      $js_setting['batch']['initMessage'] = $message;
    }
  }
  else {
    $percentage = t('N/A');
    $message = '';
  }

  drupal_json_output(array('status' => TRUE, 'percentage' => sprintf("%.02f", $percentage), 'message' => $message));
}

/**
 * Output a batch processing page without JavaScript support.
 *
 * @see _batch_process()
 */
function _background_batch_page_do_nojs() {
  $batch = &batch_get();
  $id = $batch['id'];

  $current_set = _batch_current_set();
  drupal_set_title($current_set['title'], PASS_THROUGH);

  $new_op = 'do_nojs';

  // This is one of the later requests; do some processing first.

  // Error handling: if PHP dies due to a fatal error (e.g. a nonexistent
  // function), it will output whatever is in the output buffer, followed by
  // the error message.
  ob_start();
  $fallback = $current_set['error_message'] . '<br />' . $batch['error_message'];
  $fallback = theme('maintenance_page', array('content' => $fallback, 'show_messages' => FALSE));

  // We strip the end of the page using a marker in the template, so any
  // additional HTML output by PHP shows up inside the page rather than below
  // it. While this causes invalid HTML, the same would be true if we didn't,
  // as content is not allowed to appear after </html> anyway.
  list($fallback) = explode('<!--partial-->', $fallback);
  print $fallback;

  // Perform actual processing.
  // Get progress
  if ($progress = progress_get_progress('_background_batch:' . $id)) {
    $percentage = $progress->progress * 100;
    $message = $progress->message;
    progress_estimate_completion($progress);
    // Check wether ETA information should be shown.
    if (variable_get('background_batch_show_eta', BACKGROUND_BATCH_PROCESS_ETA)) {
      $message = "ETA: " . ($progress->estimate ? format_date((int)$progress->estimate, 'large') : t('N/A')) . "<br/>$message";
    }
    else {
      $js_setting['batch']['initMessage'] = $message;
    }
  }
  else {
    $percentage = t('N/A');
    $message = '';
  }

  if ($percentage == 100) {
    $new_op = 'finished';
  }

  // PHP did not die; remove the fallback output.
  ob_end_clean();

  // Merge required query parameters for batch processing into those provided by
  // batch_set() or hook_batch_alter().
  $batch['url_options']['query']['id'] = $batch['id'];
  $batch['url_options']['query']['op'] = $new_op;

  $url = url($batch['url'], $batch['url_options']);
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'Refresh',
      'content' => '0; URL=' . $url,
    ),
  );
  drupal_add_html_head($element, 'batch_progress_meta_refresh');

  return theme('progress_bar', array('percent' => sprintf("%.02f", $percentage), 'message' => $message));
}

