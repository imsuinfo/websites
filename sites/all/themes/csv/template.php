<?php
/**
 * Override or insert variables into the html template.
 */
function csv_preprocess_html(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  $current_time = date('Y_m_d-H_i_s', REQUEST_TIME);
  $csv_front = '';

  if (!empty($vars['head_title_array']['name'])){
    $csv_front = $vars['head_title_array']['name'];
    $csv_front = preg_replace('/\s/i', '_', $csv_front);
    $csv_front = preg_replace('/\W/i', '', $csv_front);
  }

  if (!empty($vars['head_title_array']['title'])){
    if (!empty($csv_front)){
      $csv_front .= '-';
    }

    $csv_front_part = $vars['head_title_array']['title'];
    $csv_front_part = preg_replace('/\s/i', '_', $csv_front_part);
    $csv_front .= preg_replace('/\W/i', '', $csv_front_part);
  }

  if (empty($csv_front)){
    $csv_front = 'download';
  }

  if (!empty($current_time)) {
    $vars['csv_filename'] = $csv_front . '-' . check_plain($current_time) . '.csv';
  }

  if (empty($vars['csv_filename'])) $vars['csv_filename'] = 'download.csv';
}
