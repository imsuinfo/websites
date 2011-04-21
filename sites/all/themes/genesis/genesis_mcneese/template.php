<?php
// $Id: template.php,v 1.10 2011/01/14 02:57:57 jmburnz Exp $

/**
 * Preprocess and Process Functions SEE: http://drupal.org/node/254940#variables-processor
 * 1. Rename each function to match your subthemes name,
 *    e.g. if you name your theme "themeName" then the function
 *    name will be "themeName_preprocess_hook". Tip - you can
 *    search/replace on "genesis_mcneese".
 * 2. Uncomment the required function to use.
 */

/**
 * Override or insert variables into all templates.
 */
/* -- Delete this line if you want to use these functions
function genesis_mcneese_preprocess(&$vars, $hook) {
}
function genesis_mcneese_process(&$vars, $hook) {
}
// */

/**
 * Override or insert variables into the html templates.
 */
/* -- Delete this line if you want to use these functions
function genesis_mcneese_preprocess_html(&$vars) {
  // Uncomment the folowing line to add a conditional stylesheet for IE 7 or less.
  // drupal_add_css(path_to_theme() . '/css/ie/ie-lte-7.css', array('weight' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}
function genesis_mcneese_process_html(&$vars) {
}
// */

/**
 * Override or insert variables into the page templates.
 */
function genesis_mcneese_preprocess_page(&$vars) {
}
function genesis_mcneese_process_page(&$vars) {
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

  if (!empty($vars['page']['sidebar_first']) && !empty($vars['page']['sidebar_second'])){
    $vars['page']['sidebar_css'] = 'sidebar-both';
  }
  else if (!empty($vars['page']['sidebar_first'])){
    $vars['page']['sidebar_css'] = 'sidebar-left';
  }
  else if (!empty($vars['page']['sidebar_second'])){
    $vars['page']['sidebar_css'] = 'sidebar-right';
  }

  if (drupal_is_front_page() === TRUE) {
    $vars['page']['is_front_css'] = 'is_front';
  }
}

/**
 * Override or insert variables into the node templates.
 */
/* -- Delete this line if you want to use these functions
function genesis_mcneese_preprocess_node(&$vars) {
}
function genesis_mcneese_process_node(&$vars) {
}
// */

/**
 * Override or insert variables into the comment templates.
 */
/* -- Delete this line if you want to use these functions
function genesis_mcneese_preprocess_comment(&$vars) {
}
function genesis_mcneese_process_comment(&$vars) {
}
// */

/**
 * Override or insert variables into the block templates.
 */
/* -- Delete this line if you want to use these functions
function genesis_mcneese_preprocess_block(&$vars) {
}
function genesis_mcneese_process_block(&$vars) {
}
// */
