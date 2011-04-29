<?php
// $Id: template.php,v 1.10 2011/01/14 02:57:57 jmburnz Exp $

/**
 * Preprocess and Process Functions SEE: http://drupal.org/node/254940#variables-processor
 * 1. Rename each function to match your subthemes name,
 *    e.g. if you name your theme "themeName" then the function
 *    name will be "themeName_preprocess_hook". Tip - you can
 *    search/replace on "layout_587".
 * 2. Uncomment the required function to use.
 */

/**
 * Override or insert variables into all templates.
 */
/* -- Delete this line if you want to use these functions
function layout_587_preprocess(&$vars, $hook) {
}
function layout_587_process(&$vars, $hook) {
}
// */

/**
 * Override or insert variables into the html templates.
 */
function layout_587_preprocess_html(&$vars) {
  drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE, 'weight' => 2));
}
/*
function layout_587_process_html(&$vars) {
}
//*/

/**
 * Override or insert variables into the page templates.
 */
function layout_587_preprocess_page(&$vars) {
}
function layout_587_process_page(&$vars) {
  $vars['page']['leaderboard'] = isset($vars['page']['leaderboard']) ? render($vars['page']['leaderboard']) : '';
  $vars['page']['primary_links'] = isset($vars['page']['primary_links']) ? render($vars['page']['primary_links']) : '';
  $vars['page']['header'] = isset($vars['page']['header']) ? render($vars['page']['header']) : '';
  $vars['page']['subboard'] = isset($vars['page']['subboard']) ? render($vars['page']['subboard']) : '';
  $vars['page']['help'] = isset($vars['page']['help']) ? render($vars['page']['help']) : '';
  $vars['page']['secondary_content'] = isset($vars['page']['secondary_content']) ? render($vars['page']['secondary_content']) : '';
  $vars['page']['sidebar_first'] = isset($vars['page']['sidebar_first']) ? render($vars['page']['sidebar_first']) : '';
  $vars['page']['highlighted'] = isset($vars['page']['highlighted']) ? render($vars['page']['highlighted']) : '';
  $vars['page']['content'] = isset($vars['page']['content']) ? render($vars['page']['content']) : '';
  $vars['page']['sidebar_second'] = isset($vars['page']['sidebar_second']) ? render($vars['page']['sidebar_second']) : '';
  $vars['page']['tertiary_content'] = isset($vars['page']['tertiary_content']) ? render($vars['page']['tertiary_content']) : '';
  $vars['page']['footer'] = isset($vars['page']['footer']) ? render($vars['page']['footer']) : '';
  $vars['page']['renderred_tabs'] = isset($vars['tabs']) ? render($vars['tabs']) : '';
  $vars['page']['renderred_action_links'] = isset($vars['action_links']) ? render($vars['action_links']) : '';
  $vars['page']['sidebar_css'] = 'sidebar-none';
  $vars['page']['is_front_css'] = '';
  $vars['page']['subboard_image'] = '';

  if (drupal_is_front_page() === TRUE) {
    $vars['page']['is_front_css'] = 'is_front';
  }
}

/**
 * Override or insert variables into the node templates.
 */
/* -- Delete this line if you want to use these functions
function layout_587_preprocess_node(&$vars) {
}
function layout_587_process_node(&$vars) {
}
// */

/**
 * Override or insert variables into the comment templates.
 */
/* -- Delete this line if you want to use these functions
function layout_587_preprocess_comment(&$vars) {
}
function layout_587_process_comment(&$vars) {
}
// */

/**
 * Override or insert variables into the block templates.
 */
/* -- Delete this line if you want to use these functions
function layout_587_preprocess_block(&$vars) {
}
function layout_587_process_block(&$vars) {
}
// */
