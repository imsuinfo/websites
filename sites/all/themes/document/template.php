<?php


/**
 * Implements hook_theme().
 */
function document_theme($existing, $type, $theme, $path) {
  $themes = array();

  $themes['document_node_information'] = array(
    'template' => 'document_node_information',
    'variables' => array(
      'node' => NULL,
    ),
    'path' => drupal_get_path('theme', 'document') . '/templates',
  );

  return $themes;
}

/**
 * Template preprocess function for document_node_information.tpl.php
 */
function document_preprocess_document_node_information(&$variables) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  $root_class_name = 'document-node_information';

  $variables['title'] = '';
  $variables['nid'] = '';
  $variables['vid'] = '';
  $variables['uid'] = '';
  $variables['username'] = '';
  $variables['revision_uid'] = '';
  $variables['revision_username'] = '';
  $variables['created'] = '';
  $variables['changed'] = '';

  $variables['base_class'] = $root_class_name;

  if (!is_object($variables['node'])){
    cf_error_invalid_object($function_exists, 'node');
    return;
  }

  $node_user = user_load($variables['node']->uid);
  $node_revision_user = user_load($variables['node']->revision_uid);

  $variables['title'] = $variables['node']->title;
  $variables['nid'] = $variables['node']->nid;
  $variables['vid'] = $variables['node']->vid;
  $variables['uid'] = $variables['node']->uid;
  $variables['username'] = $node_user->name;
  $variables['revision_uid'] = $variables['node']->revision_uid;
  $variables['revision_username'] = $node_revision_user->name;
  $variables['created'] = date('Y/m/d h:ia', $variables['node']->created);
  $variables['changed'] = date('Y/m/d h:ia', $variables['node']->changed);

  unset($variables['node']);
}

/**
 * Override or insert variables into the maintenance page template.
 */
function document_preprocess_maintenance_page(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  if (!function_exists('cf_theme_get_variables')){
    return;
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  $keys_to_render = array('messages', 'primary_local_tasks', 'secondary_local_tasks', 'action_links');
  cf_theme_render_variables($vars, $keys_to_render);

  $keys_to_render = array('messages', 'website_menu', 'help', 'node_information', 'editing', 'content');
  cf_theme_render_variables($vars, $keys_to_render);

  // always show the following fields
  $vars['cf']['show']['breadcrumb'] = TRUE;
  $vars['cf']['show']['content'] = TRUE;

  // while is considered not accessible, it should be done on the maintainance page to help ensure accessibility
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

  // load all messages so that they can be stored in the 'messages' region.
  $messages = theme('status_messages', array('display' => NULL));

  if (!empty($messages)){
    if (!isset($vars['messages'])){
      $vars['messages'] = '';
    }

    $vars['cf']['show']['messages'] = TRUE;
    $vars['messages'] .= $messages;
  }
}

/**
 * Override or insert variables into the html template.
 */
function document_preprocess_html(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  // refresh is considered not accessible
  $vars['cf']['meta']['name']['refresh'] = '';
}

/**
 * Override or insert variables into the page template.
 */
function document_preprocess_page(&$vars) {
  if (!is_array($vars)){
    $vars = array();
  }

  if (empty($vars['cf'])){
    $vars['cf'] = cf_theme_get_variables($vars);
  }

  $vars['primary_local_tasks']   = menu_primary_local_tasks();
  $vars['secondary_local_tasks'] = menu_secondary_local_tasks();

  $keys_to_render = array('messages', 'primary_local_tasks', 'secondary_local_tasks', 'action_links');
  cf_theme_render_variables($vars, $keys_to_render);

  $keys_to_render = array('messages', 'website_menu', 'help', 'node_information', 'editing', 'content');
  cf_theme_render_variables($vars, $keys_to_render, 'page');

  // always show the following fields
  $vars['cf']['show']['breadcrumb'] = TRUE;
  $vars['cf']['show']['page']['content'] = TRUE;

  // load all messages so that they can be stored in the 'messages' region.
  $messages = theme('status_messages', array('display' => NULL));

  if (!empty($messages)){
    if (!isset($vars['messages'])){
      $vars['messages'] = '';
    }

    $vars['cf']['show']['messages'] = TRUE;
    $vars['messages'] .= $messages;
  }

  if ($vars['cf']['show']['node_information']){
    $vars['node_information'] = $vars['cf']['data']['node_information'];
  }
}

/**
 * Implements hook_cf_theme_get_variables_alter().
 */
function document_cf_theme_get_variables_alter(&$cf, $variables){
  $cf['theme']['path'] = path_to_theme();
  $cf['theme']['machine_name'] = 'document';
  $cf['theme']['human_name'] = t("Document");

  $cf['date']['enabled'] = TRUE;

  foreach (array('in_ie_compatibility_mode') as $key){
    $cf['is'][$key] = FALSE;
    $cf['is_data'][$key] = array();
  }

  $cf['show']['sidenote'] = FALSE;
  $cf['data']['sidenote'] = array();

  $cf['show']['node_information'] = FALSE;
  $cf['data']['node_information'] = array();
  $cf['show']['editing'] = FALSE;
  $cf['data']['editing'] = array();

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
    case 'ie':
      $cf['meta']['http-equiv']['X-UA-Compatible'] = 'IE=Edge; IE=9';

      if ($cf['agent']['major_version'] <= 8){
        $cf['is']['unsupported'] = TRUE;
        $cf['meta']['http-equiv']['X-UA-Compatible'] = 'IE=Edge; IE=9 IE=8';

        drupal_add_js(drupal_get_path('theme', 'document') . '/js/ie_html5.js', array('group' => JS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'weight' => 10, 'preprocess' => TRUE));

        if ($cf['agent']['major_version'] == 7){
          if (preg_match("@; Trident/@", $cf['agent']['raw']) > 0){
            $cf['is']['in_ie_compatibility_mode'] = TRUE;
          }
        }

        $custom_css = array();
        $custom_css['data'] = $cf['theme']['path'] . '/css/ie8/ie8-all.css';
        $custom_css['options'] = array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'media' => 'all');

        drupal_add_css($custom_css['data'], (!empty($custom_css['options']) ? $custom_css['options'] : NULL));
      }

      break;
  }

  if ($cf['is']['logged_in']){
    $cf['show']['editing'] = TRUE;

    if ($cf['is']['node'] && is_object($cf['is_data']['node']['object'])) {
      $cf['show']['node_information'] = TRUE;
      $cf['data']['node_information'] = theme('document_node_information', array('node' =>$cf['is_data']['node']['object']));
    }
  }
}

/**
 * Implimenets hook_workbench_menu_render_menu_item_alter().
 **/
function document_workbench_menu_render_menu_item_alter($arguments){
  $variables = &$arguments['variables'];

  $variables['settings'][$variables['id']]['attributes']['tabindex'] = array('3');
}
