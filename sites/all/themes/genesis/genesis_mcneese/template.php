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
 * Override or insert variables into the maintenance page template.
 */
function genesis_mcneese_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // mcneese_drupal_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  genesis_mcneese_preprocess_html($vars);
  genesis_mcneese_preprocess_page($vars);

  if (isset($vars['emergency']['css'])){
    $vars['emergency']['css'] = ' emergency_mode-maintenance_page';
  }
}

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
function genesis_mcneese_preprocess_html(&$vars) {
  $vars['emergency'] = genesis_mcneese_generate_emergency_array();
  $vars['unsupported'] = '';
  $vars['meta_page_expires'] = '';
  $agent_settings = isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '';
  $request_time = isset($_SERVER['REQUEST_TIME']) ? $_SERVER['REQUEST_TIME'] : '';

  if (!empty($request_time)){
    $date_value = strtotime('+1 hours', $request_time);
    $vars['meta_page_expires'] = gmdate('D, d M Y H:i:s T', $date_value);
  }

  if (function_exists('get_browser') && isset($_SERVER['HTTP_USER_AGENT'])){
    $browser_details = get_browser(null, true);

    if (!empty($browser_details['browser'])){
      $browser = strtolower($browser_details['browser']);
    }

    if (empty($browser_details['majorver'])){
      // do not process if no version number can be found
      $browser = '';
    } else {
      $majorver = $browser_details['majorver'];
    }

    switch ($browser){
      case 'firefox':
        if ($majorver < 3){
          $vars['unsupported'] = t("You are using an unsupported version of Mozilla Firefox. To properly view this website, please upgrade your webbrowser or <a href='@alternate_browser_url'>download an alternative browser</a>.", array('@alternate_browser_url' => "/supported_browsers"));
        }
        break;
      case 'mozilla':
        // get the gecko api version and report unsupported for old mozilla apis
        if (!empty($agent_settings)){
          $matches = array();
          $result = preg_match('/rv:(\d*)\.(\d*)/i', $agent_settings, $matches);
          if ($result > 0){
            if (isset($matches[1]) && isset($matches[2])) {
              if ($matches[1] <= 1 && $matches[2] <= 7){
                drupal_add_css(path_to_theme() . '/css/moz_old.css', array('group' => CSS_THEME, 'preprocess' => FALSE, 'weight' => 3));
                $vars['unsupported'] = t("You are using an unsupported version of Mozilla. To properly view this website, please upgrade your webbrowser or <a href='@alternate_browser_url'>download an alternative browser</a>.", array('@alternate_browser_url' => "/supported_browsers"));
              }
            }
          }
        }
        break;
      case 'ie':
        drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE, 'weight' => 2));

        if ($majorver < 8){
          drupal_add_css(path_to_theme() . '/css/ie_old.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE, 'weight' => 3));

          // check for gecko in case some firefox browsers are set to report as ie6 or ie7
          if (!empty($agent_settings) && preg_match('/ Gecko/i', $agent_settings) == 0){
            $vars['unsupported'] = t("You are using an unsupported version of Internet Explorer. To properly view this website, please upgrade your webbrowser or <a href='@alternate_browser_url'>download an alternative browser</a>.", array('@alternate_browser_url' => "/supported_browsers"));
          }
        }
        break;
      case 'chrome':
        // FIXME: what should the min really be?
        //if ($majorver < 11){
        //}
        break;
      case 'opera':
        // FIXME: what should the min really be?
        //if ($majorver < 10){
        //}
        break;
    }
  }
}
/*
function genesis_mcneese_process_html(&$vars) {
}
//*/

/**
 * Override or insert variables into the page templates.
 */
function genesis_mcneese_preprocess_page(&$vars) {
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
  $vars['page']['sidebar_css'] = ' sidebar-none';
  $vars['page']['is_front_css'] = '';
  $vars['page']['subboard_image'] = '';
  $vars['page']['subboard_image_css'] = '';
  $vars['page']['subtitle'] = '';
  $vars['emergency'] = genesis_mcneese_generate_emergency_array();
  $subboard_image_display = '_large';

  if (!empty($vars['page']['sidebar_first']) && !empty($vars['page']['sidebar_second'])){
    $vars['page']['sidebar_css'] = ' sidebar-both';
    //$subboard_image_display = '_small';
  }
  else if (!empty($vars['page']['sidebar_first'])){
    $vars['page']['sidebar_css'] = ' sidebar-left';
    $subboard_image_display = '';
  }
  else if (!empty($vars['page']['sidebar_second'])){
    $vars['page']['sidebar_css'] = ' sidebar-right';
    $subboard_image_display = '';
  }

  if (drupal_is_front_page() === TRUE) {
    $vars['page']['is_front_css'] = ' is_front';
  }

  // If the page is part of a group content type, then display the group_image view.
  if (isset($vars['node']) && is_object($vars['node']) && isset($vars['node']->type)) {
    if (isset($vars['node']->nid) && !empty($vars['node']->nid)){
      if (isset($vars['node']->field_group)){
        if (!empty($vars['node']->type) && $vars['node']->type != 'system_page'){
          $vars['page']['subtitle'] = views_embed_view('subtitle_information', 'default', $vars['node']->nid);
        }
      }

      if (isset($vars['node']->field_group_image_show) && is_array($vars['node']->field_group_image_show)) {
        if (isset($vars['node']->field_group_image_show['und']['0']['value']) && $vars['node']->field_group_image_show['und']['0']['value'] == 1){
          $vars['page']['subboard_image'] = views_embed_view('group_image_page', 'group_image' . $subboard_image_display, $vars['node']->nid);

          if (empty($vars['page']['subboard_image']) || preg_match('/<img\b/i', $vars['page']['subboard_image']) == 0){
            if (isset($vars['node']->field_group)){
              foreach ($vars['node']->field_group as $language_key => $outer_value){
                if (is_object($outer_value) || is_array($outer_value)){
                  foreach ($outer_value as $key => $value){
                    if (isset($value['tid']) && !empty($value['tid']) && is_numeric($value['tid'])){
                      $vars['page']['subboard_image'] = views_embed_view('group_image', 'group_image' . $subboard_image_display, $value['tid']);

                      if (!empty($vars['page']['subboard_image']) && preg_match('/<img\b/i', $vars['page']['subboard_image']) > 0){
                        $vars['page']['subboard_image_css'] = ' subboard-image';
                      }
                      else {
                        $vars['page']['subboard_image'] = '';
                      }
                    }
                  }
                }
              }
            }
          }
          else {
            $vars['page']['subboard_image_css'] = ' subboard-image';
          }
        }
      }
    }
  }
}
/*
function genesis_mcneese_process_page(&$vars) {
}
*/

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


/**
 * Generates an array containing emergency information
 *
 * FIXME: this is currently hard-coded, but should be properly implement later on in the future.
 */
function genesis_mcneese_generate_emergency_array($additional_css = '') {
  $emergency = array();
  $emergency['content'] = '';
  $emergency['css'] = '';
  $loaded_node = '';

  if (function_exists('node_load')){
    $loaded_node = node_load(914);
  }

  if (is_object($loaded_node)){
    if (isset($loaded_node->status) && $loaded_node->status == NODE_PUBLISHED){
      $emergency['content'] = array('title' => $loaded_node->title, 'body' => $loaded_node->body['und']['0']);
      $emergency['css'] = ' emergency_mode';
    }
  }

  return $emergency;
}


/**
 * Implements hook_form_FORM_ID_alter() for search_block_form.
 */
function genesis_mcneese_form_search_block_form_alter(&$form, &$form_state, $form_id){
  // This themes css uses a background image instead of the #value text, therefore the input buttons search text must be removed.
  // If this is not removed then a number of browser, some webkit-based, some mozilla-based will have presentation problems.
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => '', '#attributes' => array('title' => t('Search')));
}
