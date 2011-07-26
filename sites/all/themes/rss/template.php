<?php
/**
 * Override or insert variables into the html template.
 */
function rss_preprocess_html(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  $current_time = date('r', REQUEST_TIME);
  $website_name = '';

  $vars['rss_title'] = '';
  $vars['rss_description'] = '';
  $vars['rss_timestamp'] = '';
  $vars['rss_link'] = $_SERVER['HTTP_HOST'] . request_uri();

  if (empty($_SERVER['HTTPS'])){
    $vars['rss_link'] = 'http://' . $vars['rss_link'];
  }
  else {
    $vars['rss_link'] = 'https://' . $vars['rss_link'];
  }

  if (!empty($vars['head_title_array']['name'])){
    $website_name = $vars['head_title_array']['name'];
    $vars['rss_description'] = $website_name;
  }

  if (!empty($vars['head_title_array']['title'])){
    $vars['rss_title'] = $vars['head_title_array']['title'];

    if (empty($vars['rss_description'])){
      $vars['rss_description'] = $vars['rss_title'];
    }
    else {
      $vars['rss_description'] .= ' - ' . $vars['rss_title'];
    }
  }
  else if (empty($website_name)){
    $vars['rss_title'] = 'RSS feed';
    $vars['rss_description'] = 'RSS feed';
  }
  else {
    $vars['rss_title'] = $website_name;
  }

  if (!empty($current_time)){
    $vars['rss_timestamp'] = $current_time;
  }
}
