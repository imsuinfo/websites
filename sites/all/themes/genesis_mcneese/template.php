<?php

/**
 * Custom initialization function designed to set defaults.
 *
 * This is provided to handle the case where the cf functions are not available, for whatever reason.
 * This provides a manual setup of the default array keys used in the appropriate *.tpl.php theme templates.
 * The initialized variables is the bare minimumi and defaults most conditionals to FALSE.
 */
function genesis_mcneese_initialize_cf_array(&$vars) {
  $vars['cf'] = array();
  $cf = &$vars['cf'];
  $page = &$vars['page'];

  $cf['agent'] = array();
  $cf['agent']['doctype'] = '';

  $cf['at'] = array();
  $cf['is'] = array();
  $cf['show'] = array();
  $cf['show']['page'] = array();

  foreach (array('front', 'path', 'alias') as $item) {
    $cf['at'][$item] = '';
  }

  foreach (array('overlay', 'unsupported', 'emergency') as $item) {
    $cf['is'][$item] = FALSE;
  }

  foreach (array('help', 'subboard', 'sidebar_second', 'tertiary_content') as $item) {
    $cf['show']['page'][$item] = FALSE;
  }

  foreach (array('emergency', 'subboard_image', 'logo', 'title_prefix', 'title_suffix', 'site_name', 'sidenote', 'breadcrumb', 'site_slogan') as $item) {
    $cf['show'][$item] = FALSE;
  }

  foreach (array('header', 'content', 'footer', 'leaderboard', 'sidebar_first', 'highlighted', 'secondary_content') as $item) {
    $cf['data']['page'][$item] = drupal_render($page[$item]);
    $cf['show']['page'][$item] = TRUE;
  }

  foreach (array('primary_local_tasks', 'secondary_local_tasks', 'main_menu_links', 'secondary_menu_links', 'action_links') as $item) {
    $cf['show'][$item] = FALSE;
  }

  foreach (array('messages', 'title') as $item) {
    $cf['show'][$item] = TRUE;
  }

  // some defaults we can guess
  $cf['is']['front'] = drupal_is_front_page();

  $cf['markup_css'] = array();
  $cf['markup_css']['body'] = array();
  $cf['markup_css']['body']['class'] = '';
  $cf['markup_css']['container'] = array();
  $cf['markup_css']['container']['class'] = '';
  $cf['markup_css']['content'] = array();
  $cf['markup_css']['content']['class'] = '';
}

/**
 * Override or insert variables into the maintenance page template.
 */
function genesis_mcneese_preprocess_maintenance_page(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  // convert drupal core theme structure to cf theme structure
  $keys_to_render = array('logo', 'title_prefix', 'title_suffix', 'side_links', 'action_links');
  foreach ($keys_to_render as $key) {
    if (isset($vars[$key])) {
      $cf['data'][$key] = & $vars[$key];
    }
  }

  if (!function_exists('cf_theme_get_variables')){
    genesis_mcneese_initialize_cf_array($vars);
    return;
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  $keys_to_render = array('logo', 'title_prefix', 'title_suffix', 'side_links', 'action_links');
  cf_theme_render_variables($vars, $keys_to_render);

  $keys_to_render = array('leaderboard', 'primary_links', 'header', 'help', 'secondary_content', 'sidebar_first', 'highlighted', 'content', 'sidebar_second', 'tertiary_content', 'footer', 'action_links', 'subboard');
  cf_theme_render_variables($vars, $keys_to_render, 'page');

  genesis_mcneese_process_variables($vars);

  // while is considered not accessible, it should be done on the maintenance page to help ensure accessibility
  // this is because the maintenance page means the site is not available
  // with this enabled on the maintenance page, it should help the user gain access to the website as soon as it is up.
  // TODO: add support for specifying an approximate refresh time when the site is put into maintenance mode.
  // default to a 30-minute page expiration/refresh.
  $vars['cf']['meta']['name']['refresh'] = '1800';

  $date_value = strtotime('+1800 seconds', $vars['cf']['request']);
  $vars['cf']['meta']['name']['expires'] = gmdate('D, d M Y H:i:s T', $date_value);
  $vars['cf']['meta']['http-equiv']['expires'] = gmdate('D, d M Y H:i:s T', $date_value);

  // register that this is a maintenance page
  $vars['cf']['is']['maintenance'] = TRUE;
}

/**
 * Override or insert variables into the html templates.
 */
function genesis_mcneese_preprocess_html(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  if (!function_exists('cf_theme_get_variables')){
    genesis_mcneese_initialize_cf_array($vars);
    return;
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  genesis_mcneese_process_variables($vars);

  if ($vars['cf']['at']['machine_name'] == 'sandbox.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'sandbox') {
    $vars['head_title'] = "Sandbox of (" . $vars['head_title'] . ")";
  } else if ($vars['cf']['at']['machine_name'] == 'wwwdev.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'wwwdev') {
    $vars['head_title'] = $vars['head_title'] . " | Development";
  } else if ($vars['cf']['at']['machine_name'] == 'wwwdev2.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'wwwdev2') {
    $vars['head_title'] = $vars['head_title'] . " | Development";
  } else if ($vars['cf']['at']['machine_name'] == 'wwwdev3.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'wwwdev3') {
    $vars['head_title'] = $vars['head_title'] . " | Development";
  }

  // show google verification on front page and then only on www.mcneese.edu.
  if ($vars['cf']['is']['front']) {
    if ($vars['cf']['at']['machine_name'] == 'www.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'www') {
      $vars['cf']['meta']['name']['google-site-verification'] = 'zvxqEbtWmsaA-WXhhueU_iVFT0I9HJRH-QO0ecOL1XI';
    }
  }
}

/**
 * Override or insert variables into the page templates.
 */
function genesis_mcneese_preprocess_page(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  // convert drupal core theme structure to cf theme structure
  $keys_to_render = array('logo', 'title_prefix', 'title_suffix', 'side_links', 'action_links');
  foreach ($keys_to_render as $key) {
    if (isset($vars[$key])) {
      $cf['data'][$key] = & $vars[$key];
    }
  }

  if (!function_exists('cf_theme_get_variables')){
    genesis_mcneese_initialize_cf_array($vars);
    return;
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  $keys_to_render = array('logo', 'title_prefix', 'title_suffix', 'side_links', 'action_links');
  cf_theme_render_variables($vars, $keys_to_render);

  $keys_to_render = array('leaderboard', 'primary_links', 'header', 'help', 'secondary_content', 'sidebar_first', 'highlighted', 'content', 'sidebar_second', 'tertiary_content', 'footer', 'subboard');
  cf_theme_render_variables($vars, $keys_to_render, 'page');

  genesis_mcneese_process_variables($vars);
}

/**
 * Perform additional customization to the pre-preprocessed variables.
 */
function genesis_mcneese_process_variables(&$vars){
  global $base_path;

  $vars['primary_local_tasks']   = menu_primary_local_tasks();
  $vars['secondary_local_tasks'] = menu_secondary_local_tasks();

  // originally from the genesis theme
  if (!empty($vars['main_menu'])) {
    $vars['main_menu_links'] = theme('links__system_main_menu', array('links' => $vars['main_menu'], 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'clearfix')), 'heading' => t("Main Menu")));
  }

  if (!empty($vars['secondary_menu'])) {
    $vars['secondary_menu_links'] = theme('links__system_secondary_menu', array('links' => $vars['secondary_menu'], 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'clearfix')), 'heading' => t("Secondary Menu")));
  }

  $keys_to_render = array('primary_local_tasks', 'secondary_local_tasks', 'main_menu_links', 'secondary_menu_links');
  cf_theme_render_variables($vars, $keys_to_render);

  // always show the following fields (unless disabled on frontpage)
  $vars['cf']['show']['title'] = TRUE;
  $vars['cf']['show']['breadcrumb'] = TRUE;
  $vars['cf']['show']['messages'] = TRUE;
  $vars['cf']['show']['page']['content'] = TRUE;
  $vars['cf']['show']['page']['footer'] = TRUE;

  if ($vars['cf']['is']['front']){
    // never show these if on the front page
    $vars['cf']['show']['title_prefix'] = FALSE;
    $vars['cf']['show']['title'] = FALSE;
    $vars['cf']['show']['title_suffix'] = FALSE;
    $vars['cf']['show']['breadcrumb'] = FALSE;
  }

  // never show the following on the given special pages (FIXME: this should be moved to cf_www)
  if ($vars['cf']['at']['alias'] == 'preview/front'){
      $vars['cf']['show']['title_prefix'] = FALSE;
      $vars['cf']['show']['title'] = FALSE;
      $vars['cf']['show']['title_suffix'] = FALSE;
      $vars['cf']['show']['breadcrumb'] = FALSE;
  }

  // add container-specific css based on the content css
  $vars['cf']['markup_css']['container'] = array();
  $vars['cf']['markup_css']['container']['class'] = (empty($vars['cf']['markup_css']['content']['class']) ? '' : $vars['cf']['markup_css']['content']['class']);
  $vars['cf']['markup_css']['container']['class'] .= (empty($vars['cf']['markup_css']['page']['class']) ? '' : $vars['cf']['markup_css']['page']['class']);

  if ($vars['cf']['show']['subboard_image']){
    $vars['cf']['markup_css']['container']['class'] .= $vars['cf']['data']['subboard_image']['css'];
  }

  // perform sanitation
  if ($vars['cf']['is']['emergency']){
    $vars['cf']['is_data']['emergency']['notice'] = check_markup($vars['cf']['is_data']['emergency']['notice'], 'full_html');
  }

  // process logo
  $vars['cf']['data']['logo']['title'] = $vars['cf']['at']['human_name'];
  $vars['cf']['data']['logo']['alt'] = $vars['cf']['at']['human_name'];
  $vars['cf']['data']['logo']['src'] = $base_path . path_to_theme() . '/images/web_logo.png';

  if ($vars['cf']['at']['machine_name'] == 'sandbox.mcneese.edu' || $vars['cf']['at']['machine_name'] == 'sandbox') {
    $vars['cf']['data']['logo']['title'] = 'Sandbox of ' . $vars['cf']['at']['human_name'];
    $vars['cf']['data']['logo']['alt'] = 'Sandbox of ' . $vars['cf']['at']['human_name'];
    $vars['cf']['data']['logo']['src'] = $base_path . path_to_theme() . '/images/sandbox.png';
  }
}

/**
 * Implements hook_cf_theme_get_variables_alter().
 */
function genesis_mcneese_cf_theme_get_variables_alter(&$cf, $variables){
  $cf['theme']['path'] = path_to_theme();
  $cf['theme']['machine_name'] = 'genesis_mcneese';
  $cf['theme']['human_name'] = t("Genesis McNeese");

  $cf['meta']['name']['copyright'] = '2011© McNeese State University';
  $cf['meta']['name']['description'] = 'McNeese State University Website';
  $cf['meta']['name']['distribution'] = 'web';

  foreach (array('logo_url', 'sidebar_both', 'sidebar_left', 'sidebar_right', 'sidebar_none', 'in_ie_compatibility_mode') as $key){
    $cf['is'][$key] = FALSE;
    $cf['is_data'][$key] = array();
  }

  foreach (array('title', 'site_logo', 'site_name', 'site_slogan', 'main_menu_links', 'secondary_menu_links') as $key){
    if (empty($variables[$key])){
      $cf['show'][$key] = FALSE;
    }
    else {
      $cf['show'][$key] = TRUE;
    }
  }

  $cf['show']['sidenote'] = FALSE;
  $cf['data']['sidenote'] = array();

  $cf['show']['title'] = TRUE;
  $cf['date']['enabled'] = TRUE;

  if ($cf['is']['front']){
    $cf['show']['breadcrumb'] = FALSE;
  }
  else {
    $cf['show']['breadcrumb'] = TRUE;
  }

  $cf['show']['logo'] = TRUE;
  $cf['data']['logo'] = array();

  $cf['show']['subboard_image'] = FALSE;
  $cf['data']['subboard_image'] = array('content' => '', 'css' => '');

  if (!$cf['is']['logged_in']){
    $date_value = strtotime('+3 hours', $cf['request']);
    $cf['meta']['name']['expires'] = gmdate('D, d M Y H:i:s T', $date_value);
    $cf['meta']['http-equiv']['expires'] = gmdate('D, d M Y H:i:s T', $date_value);
  }
  else {
    $cf['meta']['http-equiv']['cache-control'] = 'no-cache';
  }

  // html 5 doctype
  $cf['agent']['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">';

  switch($cf['agent']['machine_name']){
    case 'firefox':
      break;
    case 'mozilla':
      $matches = array();

      $result = preg_match('/rv:(\d*)\.(\d*)/i', $cf['agent']['raw'], $matches);
      if ($result > 0){
        if (isset($matches[1]) && isset($matches[2])) {
          if ($matches[1] <= 1 && $matches[2] <= 7){
            $custom_css = array();
            $custom_css['data'] = $cf['theme']['path'] . '/css/moz_old.css';
            $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');
            $cf['is']['unsupported'] = TRUE;

            //$cf['css'][] = $custom_css;
            drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));
          }
        }
      }

      break;
    case 'ie':
      // IE ignores non-ancient css unless the following doctype is used
      $cf['agent']['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">';

      // enforce ie8 compatibility mode
      $cf['meta']['http-equiv']['X-UA-Compatible'] = 'IE=Edge; IE=9; IE=8';

      if ($cf['agent']['major_version'] <= 8){
        $custom_css = array();
        $custom_css['data'] = $cf['theme']['path'] . '/css/ie8.css';
        $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');

        //$cf['css'][] = $custom_css;
        drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));
      }

      if ($cf['agent']['major_version'] < 8){
        $custom_css = array();
        $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 3, 'media' => 'all');
        $custom_css['data'] = $cf['theme']['path'] . '/css/ie_old.css';
        $cf['is']['unsupported'] = TRUE;

        //$cf['css'][] = $custom_css;
        drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));

        if ($cf['agent']['major_version'] == 7){
          if (preg_match("@; Trident/@", $cf['agent']['raw']) > 0){
            $cf['is']['in_ie_compatibility_mode'] = TRUE;
          }
        }
      }

      break;
    case 'chrome':
    case 'safari':
    case 'midori':
      $custom_css = array();
      $custom_css['data'] = $cf['theme']['path'] . '/css/webkit.css';
      $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');

      //$cf['css'][] = $custom_css;
      drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));

      break;
    default:
      switch($cf['agent']['engine']){
        case 'webkit':

          $custom_css = array();
          $custom_css['data'] = $cf['theme']['path'] . '/css/webkit.css';
          $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');

          //$cf['css'][] = $custom_css;
          drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));
          break;
        case 'trident':
          $custom_css = array();
          $custom_css['data'] = $cf['theme']['path'] . '/css/ie8.css';
          $custom_css['options'] = array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');

          //$cf['css'][] = $custom_css;
          drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));
          break;
      }

      break;
  }

  // FIXME: this should be moved to cf_www
  if ($cf['is']['unsupported']){
    if ($cf['is']['in_ie_compatibility_mode']){
      $cf['is_data']['unsupported']['message'] = t("You are running Internet Explorer in compatibility mode. To improve your experience using this website, please <a href='@alternate_browser_url'>turn off compatibility mode</a>.", array('@alternate_browser_url' => "http://www.sevenforums.com/tutorials/1196-internet-explorer-compatibility-view-turn-off.html#post_message_10408"));
    }
    else {
      $cf['is_data']['unsupported']['message'] = t("You are using an unsupported version of :name. Please upgrade your webbrowser or <a href='@alternate_browser_url'>download an alternative browser</a>.", array(':name' => $cf['agent']['machine_name'], '@alternate_browser_url' => "/supported_browsers"));
    }
  }

  // FIXME: this should be moved to cf_www
  if (function_exists('node_load') && $cf['is']['maintenance']){
    $loaded_node = node_load(914);
    if (is_object($loaded_node)){
      if (property_exists($loaded_node, 'status') && $loaded_node->status == NODE_PUBLISHED){
        $cf['is']['emergency'] = TRUE;
        $cf['is_data']['emergency'] = array('title' => $loaded_node->title, 'body' => $loaded_node->body['und']['0']);

        $cf['is_data']['emergency']['notice'] = 'This website is operating in <span class="emergency_mode-notice-emergency_mode">Emergency Mode</span>.<br>' . "\n";
        $cf['is_data']['emergency']['notice'] .= 'To exit <span class="emergency_mode-notice-emergency_mode">Emergency Mode</span>, you must <a href="/emergency_page">Unpublish the Emergency Page</a>.' . "\n";

        $cf['meta']['http-equiv']['cache-control'] = 'no-cache';
      }
    }
  }

  $subboard_image_display = '_large';

  if (isset($variables['page'])){
    if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])){
      $cf['is']['sidebar_both'] = TRUE;
      //$subboard_image_display = '_small';
    }
    else if (!empty($variables['page']['sidebar_first'])){
      $cf['is']['sidebar_left'] = TRUE;
      $subboard_image_display = '';
    }
    else if (!empty($variables['page']['sidebar_second'])){
      $cf['is']['sidebar_right'] = TRUE;
      $subboard_image_display = '';
    }
    else {
      $cf['is']['sidebar_none'] = TRUE;
    }
  } else {
    // this is the case for maintenance pages
    if (!empty($variables['sidebar_first']) && !empty($variables['sidebar_second'])){
      $cf['is']['sidebar_both'] = TRUE;
      //$subboard_image_display = '_small';
    }
    else if (!empty($variables['sidebar_first'])){
      $cf['is']['sidebar_left'] = TRUE;
      $subboard_image_display = '';
    }
    else if (!empty($variables['sidebar_second'])){
      $cf['is']['sidebar_right'] = TRUE;
      $subboard_image_display = '';
    }
    else {
      $cf['is']['sidebar_none'] = TRUE;
    }
  }

  // FIXME: this should be moved to cf_www
  // If the page is part of a group content type, then display the group_image view.
  if ($cf['is']['node']) {
    if (is_object($cf['is_data']['node']['object'])){
      $cf['show']['sidenote'] = TRUE;
      $cf['data']['sidenote']['content'] = '';

      if (property_exists($cf['is_data']['node']['object'], 'field_group') && !empty($cf['is_data']['node']['object']->field_group)){
        $cf['data']['sidenote']['content'] = '<div class="sidenote-item_wrapper">' . views_embed_view('subtitle_information', 'default', $variables['node']->nid) . '</div>';
      }

      if ($cf['is']['logged_in']){
        $cf['data']['sidenote']['content'] .= '<div class="sidenote-item_wrapper"><span class="sidenote-node_id-label">' . t("Node") . '</span> <span class="sidenote-node_id-number">' . check_plain($cf['is_data']['node']['object']->nid) . '</span></div>';
      }
    }

    if (property_exists($cf['is_data']['node']['object'], 'field_group_image_show') && !empty($cf['is_data']['node']['object']->field_group_image_show)){
      if (!empty($variables['node']->field_group_image_show['und']['0']['value']) && $variables['node']->field_group_image_show['und']['0']['value'] == 1){
        $view_result = views_embed_view('group_image_page', 'group_image' . $subboard_image_display, $variables['node']->nid);

        if (!empty($view_result) && preg_match('/<img\b/i', $view_result) > 0){
          $cf['show']['subboard_image'] = TRUE;
          $cf['data']['subboard_image']['content'] = $view_result;
        }
      }
    }

    if (property_exists($cf['is_data']['node']['object'], 'field_search_engine_optimization') && !empty($cf['is_data']['node']['object']->field_search_engine_optimization)){
      if (!empty($variables['node']->field_search_engine_optimization['und'])) {
        $first_keyword = TRUE;

        foreach ($variables['node']->field_search_engine_optimization['und'] as $key => &$value) {
          if (!isset($value['tid'])) continue;

          $term = taxonomy_term_load($value['tid']);

          if ($first_keyword) {
            $cf['meta']['name']['keywords'] = $term->name;
            $first_keyword = FALSE;
          }
          else {
            $cf['meta']['name']['keywords'] .= ', ' . $term->name;
          }
        }
      }
    }
  }

  // Additional body classes to help out themers. (originally from genesis theme template file)
  if (!$cf['is']['front']) {
    $path = drupal_get_path_alias($_GET['q']);
    list($section, ) = explode('/', $path, 2);

    if ($cf['is']['node']){
      $section = 'page-node-' . $cf['is_data']['node']['object']->nid;
    }
    else if (arg(0) == 'node' && arg(1) == 'add'){
      $section = 'page-node-add';
    }

    // ?
    $vars['classes_array'][] = drupal_html_class('section-' . $section);
  }
}

/**
 * Override or insert variables in comment templates.
 * This was originally from genesis theme template file.
 */
function genesiss_mcneese_preprocess_comment(&$vars) {
  // Add odd and even classes to comments
  $vars['classes_array'][] = $vars['zebra'];
}

/**
 * Override or insert variables into block templates.
 * This was originally from genesis theme template file.
 */
function genesis_mcneese_preprocess_block(&$vars) {
  $block = $vars['block'];
  $vars['title'] = $block->subject;
  // Special classes for blocks
  $vars['classes_array'][] = 'block-' . $vars['block_zebra'];
  $vars['classes_array'][] = 'block-' . drupal_html_class($block->region);
  $vars['classes_array'][] = 'block-count-' . $vars['id'];
}


/**
 * Implements hook_form_FORM_ID_alter() for search_block_form.
 */
function genesis_mcneese_form_search_block_form_alter(&$form, &$form_state, $form_id){
  // This themes css uses a background image instead of the #value text, therefore the input buttons search text must be removed.
  // If this is not removed then a number of browser, some webkit-based, some mozilla-based will have presentation problems.
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => '', '#attributes' => array('title' => t("Search")));
}

/**
 * Implements hook_breadcrumb().
 */
function genesis_mcneese_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  $output = '<h2 class="element-invisible">' . t("Breadcrumbs") . '</h2>';
  $output .= '<!--REPLACE_CUSTOM_DATA-->';
  $output .= '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';

  return $output;
}
