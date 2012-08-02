<?php

/**
 * @file
 * McNeese - Base Theme.
 */

/**
 * @defgroup mcneese McNeese - Base Theme
 * @ingroup mcneese
 * @{
 * Provides the base mcneese theme.
 */

/**
 * Implements hook_preprocess().
 */
function mcneese_preprocess(&$vars, $hook) {
  if (!is_array($vars)){
    $vars = array();
  }

  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }
}

/**
 * Implements hook_preprocess_toolbar().
 */
function mcneese_preprocess_toolbar(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }


  // replace the original toolbar elements with our own custom generated ones.
  $vars['mcneese_toolbar'] = array();
  $vars['mcneese_toolbar']['menu'] = array('items' => array());
  $vars['mcneese_toolbar']['shortcuts'] = array('items' => array());

  $tab_index = 1;

  // identify the collapse/expand state of the shortcuts bar.
  if (_toolbar_is_collapsed()) {
    $toggle_state = 'collapsed';
    $cf['is']['toolbar-shortcuts-expanded'] = FALSE;
    $cf['is']['toolbar-shortcuts-collapsed'] = TRUE;
  }
  else {
    $toggle_state = 'expanded';
    $cf['is']['toolbar-shortcuts-expanded'] = TRUE;
    $cf['is']['toolbar-shortcuts-collapsed'] = FALSE;
  }

  $toolbar_autohide = '';
  $toolbar_expanded = 'expanded';
  $toolbar_sticky = 'fixed';
  $cf['is']['toolbar-expanded'] = TRUE;
  $cf['is']['toolbar-collapsed'] = FALSE;

  if (isset($cf['user']['object']->data['mcneese_settings']['toolbar']['autohide'])) {
    if ($cf['user']['object']->data['mcneese_settings']['toolbar']['autohide']) {
      $toolbar_autohide = 'autohide';
      $toolbar_expanded = 'collapsed';
      $cf['is']['toolbar-expanded'] = FALSE;
      $cf['is']['toolbar-collapsed'] = TRUE;
    }
    else {
      $toolbar_expanded = 'expanded';
      $cf['is']['toolbar-expanded'] = TRUE;
      $cf['is']['toolbar-collapsed'] = FALSE;
    }
  }

  if (isset($cf['user']['object']->data['mcneese_settings']['toolbar']['sticky'])) {
    if ($cf['user']['object']->data['mcneese_settings']['toolbar']['sticky']) {
      $toolbar_sticky = 'relative';
    }
    else {
      $toolbar_sticky = 'fixed';
    }
  }


  // add the custom home button
  $item = array();
  $item['attributes'] = array();
  $item['attributes']['class'] = array();
  $item['attributes']['class'][] = 'mcneese-toolbar-home';
  $item['attributes']['class'][] = 'item';
  $item['attributes']['title'] = t("Home");
  $item['markup'] = '<a href="' . base_path() . '" class="link noscript" tabindex="' . $tab_index . '"></a>';
  $vars['mcneese_toolbar']['menu']['items'][] = $item;


  // populate the toolbar menu
  foreach ($vars['toolbar']['toolbar_menu']['#links'] as $link) {
    $item = array();
    $item['attributes'] = array();
    $item['attributes']['class'] = array();
    $item['attributes']['class'][] = 'item';
    $item['attributes']['title'] = '';

    if (!empty($link['attributes']['title'])) {
      $item['attributes']['title'] = $link['attributes']['title'];
    }

    if (!empty($link['attributes']['id'])) {
      $item['attributes']['id'] = 'mcneese-' . $link['attributes']['id'];
    }

    if (isset($link['attributes']['class']) && is_array($link['attributes']['class'])) {
      $item['attributes']['class'] = array_merge($item['attributes']['class'], $link['attributes']['class']);
    }

    $item['markup'] = '<a href="' . base_path() . $link['href'] . '" class="link" tabindex="' . $tab_index . '">' . $link['title'] . '</a>';
    $vars['mcneese_toolbar']['menu']['items'][] = $item;
  }


  // add the custom toggle button
  $destination = drupal_get_destination();

  if (!empty($destination['destination'])) {
    $destination = '?destination=' . $destination['destination'];
  }
  else {
    $destination = '';
  }

  $item = array();
  $item['attributes'] = array();
  $item['attributes']['class'] = array();
  $item['attributes']['class'][] = 'mcneese-toolbar-toggle';
  $item['attributes']['class'][] = 'item';
  $item['attributes']['title'] = t("Toggle Shortcut Bar");
  $item['markup'] = '<a href="' . base_path() . 'toolbar/toggle' . $destination . '" class="link noscript" tabindex="' . $tab_index . '"></a>';

  $item['attributes']['class'][] = $toggle_state;
  $vars['mcneese_toolbar']['menu']['items'][] = $item;

  unset($destination);


  // populate the toolbar menu (with user links)
  // reverse the array so that when the user settings are floated, they are in the correct order.
  $toolbar_user = array_reverse($vars['toolbar']['toolbar_user']['#links']);

  foreach ($toolbar_user as $link) {
    $item = array();
    $item['attributes'] = array();
    $item['attributes']['class'] = array();
    $item['attributes']['class'][] = 'mcneese-toolbar-user';
    $item['attributes']['class'][] = 'item';
    $item['attributes']['title'] = '';

    if (!empty($link['attributes']['title'])) {
      $item['attributes']['title'] = $link['attributes']['title'];
    }

    if (!empty($link['attributes']['id'])) {
      $item['attributes']['id'] = 'mcneese-' . $link['attributes']['id'];
    }

    if (isset($link['attributes']['class']) && is_array($link['attributes']['class'])) {
      $item['attributes']['class'] = array_merge($item['attributes']['class'], $link['attributes']['class']);
    }

    if (isset($link['attributes']['class']) && is_array($link['attributes']['class'])) {
      $item['attributes']['class'] = array_merge($item['attributes']['class'], $link['attributes']['class']);
    }

    $item['markup'] = '<a href="' . base_path() . $link['href'] . '" class="link noscript" tabindex="' . $tab_index . '">' . $link['title'] . '</a>';
    $vars['mcneese_toolbar']['menu']['items'][] = $item;
  }

  // populate the shortcut menu
  if (!empty($vars['toolbar']['toolbar_drawer'][0]['shortcuts'])) {
    foreach ($vars['toolbar']['toolbar_drawer'][0]['shortcuts'] as $link_id => &$link) {
      if (!is_numeric($link_id)) continue;

      $item = array();
      $item['attributes'] = array();
      $item['attributes']['class'] = array();
      $item['attributes']['class'][] = 'mcneese-toolbar-shortcuts';
      $item['attributes']['class'][] = 'item';
      $item['attributes']['title'] = '';

      if (!empty($link['#attributes']['title'])) {
        $item['attributes']['title'] = $link['#attributes']['title'];
      }

      if (!empty($link['#attributes']['id'])) {
        $item['attributes']['id'] = 'mcneese-' . $link['#attributes']['id'];
      }

      if (isset($link['#attributes']['class']) && is_array($link['#attributes']['class'])) {
        $item['attributes']['class'] = array_merge($item['attributes']['class'], $link['#attributes']['class']);
      }

      $item['markup'] = '<a href="' . base_path() . $link['#href'] . '" class="link noscript" tabindex="' . $tab_index . '">' . $link['#title'] . '</a>';
      $vars['mcneese_toolbar']['shortcuts']['items'][] = $item;
    }
  }

  // add custom sticky button
  $item = array();
  $item['attributes'] = array();
  $item['attributes']['class'] = array();
  $item['attributes']['class'][] = 'mcneese-toolbar-sticky';
  $item['attributes']['class'][] = 'item';
  $item['attributes']['title'] = t("Toggle Menu Stickiness");
  $item['markup'] = '<div class="link noscript" tabindex="' . $tab_index . '"></div>';

  $vars['mcneese_toolbar']['shortcuts']['items'][] = $item;


  unset($vars['toolbar']);


  // build the html tags
  $cf['toolbar']['tags'] = array();

  $attributes = array();
  $attributes['id'] = 'mcneese-toolbar';
  $attributes['title'] = t("Toolbar");
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['class'][] = $toolbar_sticky;
  $attributes['class'][] = $toolbar_expanded;
  $attributes['role'] = 'navigation';

  if (!empty($toolbar_autohide)) {
    $attributes['class'][] = $toolbar_autohide;
  }

  $cf['toolbar']['tags']['mcneese_toolbar_open'] = array('name' => 'aside', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['toolbar']['tags']['mcneese_toolbar_close'] = array('name' => 'aside', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);

  $cf['toolbar']['tags']['mcneese_toolbar_nav_close'] = array('name' => 'nav', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);

  $attributes = array();
  $attributes['class'] = array();
  $attributes['class'][] = 'mcneese-toolbar-menu';
  $attributes['class'][] = 'noscript';
  $attributes['class'][] = 'shortcuts-' . $toggle_state;

  $cf['toolbar']['tags']['mcneese_toolbar_menu'] = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);

  $attributes = array();
  $attributes['class'] = array();
  $attributes['class'][] = 'mcneese-toolbar-shortcuts';
  $attributes['class'][] = 'noscript';
  $attributes['class'][] = $toggle_state;

  $cf['toolbar']['tags']['mcneese_toolbar_shortcuts'] = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);

  if ($cf['is']['html5']) {
    $vars['header'] = '<header class="element-invisible">';
    $vars['header'] .= '<h2>';
    $vars['header'] .= t("Toolbar");
    $vars['header'] .= '</h2>';
    $vars['header'] .= '</header>';
  }
  else {
    $vars['header'] = '<h2 class="element-invisible">';
    $vars['header'] .= t("Toolbar");
    $vars['header'] .= '</h2>';
  }
}

/**
 * Implements hook_preprocess_page().
 */
function mcneese_preprocess_page(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['page']['tags'] = array();

  if (!isset($cf['data']['page'])) {
    $cf['data']['page'] = array();
  }


  // setup page logo
  $cf['data']['page']['logo']['title'] = $cf['at']['human_name'];
  $cf['data']['page']['logo']['alt'] = $cf['at']['human_name'];
  $cf['data']['page']['logo']['href'] = base_path();
  $cf['show']['page']['logo'] = TRUE;


  // load page header
  if (empty($vars['page']['header'])) {
    $cf['page']['header'] = array();
  }
  else {
    $cf['page']['header'] = $vars['page']['header'];
    unset($vars['page']['header']);
  }

  if (empty($vars['page']['header_menu_1'])) {
    $cf['page']['header_menu_1'] = array();
  }
  else {
    $cf['page']['header_menu_1'] = $vars['page']['header_menu_1'];
    unset($vars['page']['header_menu_1']);
  }

  if (empty($vars['page']['header_menu_2'])) {
    $cf['page']['header_menu_2'] = array();
  }
  else {
    $cf['page']['header_menu_2'] = $vars['page']['header_menu_2'];
    unset($vars['page']['header_menu_2']);
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-header';
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['class'][] = 'relative';
  $attributes['class'][] = 'expanded';
  $attributes['role'] = 'banner';

  $cf['page']['tags']['mcneese_page_header_open'] = array('name' => 'aside', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_header_close'] = array('name' => 'aside', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load all messages so that they can be stored in the 'messages' region.
  $messages_sticky = 'relative';

  if (isset($cf['user']['object']->data['mcneese_settings']['messages']['sticky'])) {
    if ($cf['user']['object']->data['mcneese_settings']['messages']['sticky']) {
      $messages_sticky = 'relative';
    }
    else {
      $messages_sticky = 'fixed';
    }
  }

  if (!empty($vars['messages'])) {
    if (!empty($vars['cf']['page']['data']['messages']['raw'])) {
      $cf['data']['page']['messages']['raw'] = array_merge($vars['cf']['page']['data']['messages']['raw'], $vars['messages']);
    }
    else {
      $cf['data']['page']['messages']['raw'] = $vars['messages'];
    }

    $cf['show']['page']['messages'] = TRUE;
    unset($vars['messages']);
  }

  if (empty($vars['page']['messages'])) {
    $cf['data']['page']['messages'] = array();
  }
  else {
    $cf['data']['page']['messages']['blocks'] = $vars['page']['messages'];
    $cf['show']['page']['messages'] = TRUE;
    unset($vars['page']['messages']);
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-messages';
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['title'] = t("Messages");

  if ($cf['is']['html5']) {
    $attributes['class'][] = $messages_sticky;

    if ($messages_sticky == 'fixed') {
      $attributes['class'][] = 'collapsed';
    }
    else {
      $attributes['class'][] = 'expanded';
    }

    $attributes['tabindex'] = '2';
  }
  else {
    $attributes['class'][] = 'relative';
    $attributes['class'][] = 'expanded';
  }

  $cf['page']['tags']['mcneese_page_messages_open'] = array('name' => 'aside', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_messages_close'] = array('name' => 'aside', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load all help so that they can be stored in the 'help' region.
  $help_sticky = 'relative';

  if (isset($cf['user']['object']->data['mcneese_settings']['help']['sticky'])) {
    if ($cf['user']['object']->data['mcneese_settings']['help']['sticky']) {
      $help_sticky = 'relative';
    }
    else {
      $help_sticky = 'fixed';
    }
  }

  if (!empty($vars['help'])) {
    $cf['page']['help'] = array_merge($vars['cf']['page']['help'], $vars['help']);
    unset($vars['help']);
  }

  if (empty($vars['page']['help'])) {
    $cf['page']['help'] = array();
  }
  else {
    $cf['page']['help'] = $vars['page']['help'];
    unset($vars['page']['help']);
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-help';
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['title'] = t("Help");

  if ($cf['is']['html5']) {
    $attributes['class'][] = $help_sticky;

    if ($help_sticky == 'fixed') {
      $attributes['class'][] = 'collapsed';
    }
    else {
      $attributes['class'][] = 'expanded';
    }

    $attributes['tabindex'] = '2';
  }
  else {
    $attributes['class'][] = 'relative';
    $attributes['class'][] = 'expanded';
  }

  $cf['page']['tags']['mcneese_page_help_open'] = array('name' => 'aside', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_help_close'] = array('name' => 'aside', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);

  $attributes = array();
  $attributes['class'] = array();

  $cf['page']['tags']['mcneese_page_help_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_help_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load all tasks
  $cf['page']['tabs'] = menu_local_tasks(0);
  $cf['page']['sub_tabs'] = menu_local_tasks(1);

  $tabs_sticky = 'relative';

  if (isset($cf['user']['object']->data['mcneese_settings']['menu_tabs']['sticky'])) {
    if ($cf['user']['object']->data['mcneese_settings']['menu_tabs']['sticky']) {
      $tabs_sticky = 'relative';
    }
    else {
      $tabs_sticky = 'fixed';
    }
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-tabs';
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['tabindex'] = '2';
  $attributes['title'] = t("Menu Tabs");

  if ($cf['is']['html5']) {
    $attributes['class'][] = $tabs_sticky;

    if ($tabs_sticky == 'fixed') {
      $attributes['class'][] = 'collapsed';
    }
    else {
      $attributes['class'][] = 'expanded';
    }
  }
  else {
    $attributes['class'][] = 'relative';
    $attributes['class'][] = 'expanded';
  }

  $cf['page']['tags']['mcneese_page_tabs_open'] = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_tabs_close'] = array('name' => 'nav', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load all action_links
  $action_links_sticky = 'relative';

  if (isset($cf['user']['object']->data['mcneese_settings']['action_links']['sticky'])) {
    if ($cf['user']['object']->data['mcneese_settings']['action_links']['sticky']) {
      $action_links_sticky = 'relative';
    }
    else {
      $action_links_sticky = 'fixed';
    }
  }

  if (empty($vars['action_links'])) {
    $cf['page']['action_links'] = array();
  }
  else {
    $cf['page']['action_links'] = $vars['action_links'];
    unset($vars['action_links']);
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-action_links';
  $attributes['class'] = array();
  $attributes['class'][] = 'noscript';
  $attributes['tabindex'] = '2';
  $attributes['title'] = t("Action Links");

  if ($cf['is']['html5']) {
    $attributes['class'][] = $action_links_sticky;

    if ($action_links_sticky == 'fixed') {
      $attributes['class'][] = 'collapsed';
    }
    else {
      $attributes['class'][] = 'expanded';
    }
  }
  else {
    $attributes['class'][] = 'relative';
    $attributes['class'][] = 'expanded';
  }

  $cf['page']['tags']['mcneese_page_action_links_open'] = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_action_links_close'] = array('name' => 'nav', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load header
  $cf['page']['title'] = drupal_get_title();

  $attributes = array();
  $attributes['class'] = array();
  $attributes['class'][] = 'page-header';

  $cf['page']['tags']['mcneese_page_title_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_title_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);

  // load all breadcrumb and sidecrumb data
  if (empty($vars['breadcrumb'])) {
    $cf['page']['breadcrumb'] = drupal_get_breadcrumb();
  }
  else {
    $cf['page']['breadcrumb'] = $vars['breadcrumb'];
    unset($vars['breadcrumb']);
  }

  $cf['page']['precrumb'] = '';

  if ($cf['user']['object']->uid > 0) {
    if ($cf['is']['node']) {
      $cf['page']['precrumb'] .= '<div class="crumb-node_id">' . t("Node " . $cf['is_data']['node']['object']->nid) . '</div>';
    }

    if ($cf['is']['profile']) {
      $cf['page']['precrumb'] .= '<div class="crumb-user_id">' . t("User " . $cf['is_data']['profile']['object']->uid) . '</div>';
    }
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-breadcrumb';
  $attributes['class'] = array();
  $attributes['role'] = 'navigation';

  $cf['page']['tags']['mcneese_page_breadcrumb_open'] = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_breadcrumb_close'] = array('name' => 'nav', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // Load side content
  if (empty($vars['page']['menus'])) {
    $cf['page']['menus'] = array();
  }
  else {
    $cf['page']['menus'] = $vars['page']['menus'];
    unset($vars['page']['menus']);
  }

  if (empty($vars['page']['asides'])) {
    $cf['page']['asides'] = array();
  }
  else {
    $cf['page']['asides'] = $vars['page']['asides'];
    unset($vars['page']['asides']);
  }


  // load all content
  if (empty($vars['page']['content'])) {
    $cf['page']['content'] = array();
  }
  else {
    $cf['page']['content'] = $vars['page']['content'];
  }


  // load footer so that they can be stored in the 'footer' region.
  if (!empty($vars['footer'])) {
    $cf['page']['footer'] = $vars['footer'];
    unset($vars['footer']);
  }

  $attributes = array();
  $attributes['id'] = 'mcneese-footer';
  $attributes['class'] = array();
  $attributes['class'][] = 'expanded';
  $attributes['class'][] = 'noscript';

  $cf['page']['tags']['mcneese_page_footer_open'] = array('name' => 'aside', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['page']['tags']['mcneese_page_footer_close'] = array('name' => 'aside', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load any messages that might have appeared during the template preprocess operation
  $messages = drupal_get_messages();

  if (!empty($messages)) {
    if (isset($cf['data']['page']['messages']['raw'])) {
      $cf['data']['page']['messages']['raw'] = array_merge($cf['data']['page']['messages']['raw'], $messages);
    }
    else {
      $cf['data']['page']['messages']['raw'] = $messages;
    }

    $cf['show']['page']['messages'] = TRUE;
  }
}

/**
 * Implements hook_preprocess_node().
 */
function mcneese_preprocess_node(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['node'] = array();
  $cf['node']['tags'] = array();


  // load header
  $cf['node']['header'] = &$vars['title'];
  $cf['node']['title_prefix'] = &$vars['title_prefix'];
  $cf['node']['title_suffix'] = &$vars['title_suffix'];
  unset($vars['title']);
  unset($vars['title_prefix']);
  unset($vars['title_suffix']);

  $attributes = array();
  $attributes['class'] = array('node-header', 'element-invisible');

  $cf['node']['tags']['mcneese_node_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['node']['tags']['mcneese_node_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load content
  $cf['node']['content'] = &$vars['content'];
  unset($vars['content']);

  $attributes = array();
  $attributes['class'] = array();
  $attributes['class'][] = 'node';
  $attributes['class'][] = 'node-id-' . check_plain($vars['node']->nid);
  $attributes['class'][] = 'node-revision-' . check_plain($vars['node']->vid);
  $attributes['class'][] = 'node-type-' . check_plain($vars['node']->type);

  if ($vars['node']->status) {
    $attributes['class'][] = 'node-published';
  }
  else {
    $attributes['class'][] = 'node-unpublished';
  }

  // only display uid if the user is logged in
  if ($cf['user']['object']->uid > 0) {
    $attributes['class'][] = 'node-owner_id-' . check_plain($vars['node']->uid);
  }

  $cf['node']['tags']['mcneese_node_open'] = array('name' => 'section', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['node']['tags']['mcneese_node_close'] = array('name' => 'section', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
}

/**
 * Implements hook_preprocess_block().
 */
function mcneese_preprocess_block(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['block'] = array();
  $cf['block']['tags'] = array();


  // load semantic
  if (!property_exists($vars['block'], 'semantic') || empty($vars['block']->semantic)) {
    $cf['block']['semantic'] = 'ignore';
  }
  else {
    $cf['block']['semantic'] = $vars['block']->semantic;
  }


  // load heading
  $cf['block']['header'] = '';
  $cf['block']['heading'] = '';

  if (!empty($vars['block']->subject)) {
    if (!property_exists($vars['block'], 'heading') || empty($vars['block']->heading)) {
      if ($vars['block']->subject != '<none>') {
        $cf['block']['header'] = $vars['block']->subject;
        $cf['block']['heading'] = 2;
      }
    }
    else if (cf_is_integer($vars['block']->heading) && $vars['block']->heading > 0) {
      $cf['block']['header'] = $vars['block']->subject;
      $cf['block']['heading'] = $vars['block']->heading;
    }
    else {
      $cf['block']['header'] = '';
    }
  }

  $attributes = array();
  $attributes['class'] = array('block-header');

  $cf['block']['tags']['mcneese_block_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['block']['tags']['mcneese_block_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // load content
  $cf['block']['content'] = &$vars['content'];
  unset($vars['content']);

  if ($cf['block']['semantic'] != 'none') {
    $attributes = array();
    $attributes['class'] = array();
    $attributes['class'][] = 'block';
    $attributes['class'][] = 'block-id-' . check_plain($vars['block_id']);
    $attributes['class'][] = 'block-name-' . check_plain($vars['block_html_id']);
    $attributes['class'][] = check_plain($vars['block_zebra']);

    if ($cf['block']['semantic'] == 'ignore') {
      $cf['block']['tags']['mcneese_block_open'] = array('name' => 'div', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
      $cf['block']['tags']['mcneese_block_close'] = array('name' => 'div', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
    }
    else {
      $cf['block']['tags']['mcneese_block_open'] = array('name' => $cf['block']['semantic'], 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
      $cf['block']['tags']['mcneese_block_close'] = array('name' => $cf['block']['semantic'], 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
    }
  }
}

/**
 * Implements hook_preprocess_field().
 */
function mcneese_preprocess_field(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['field'] = array();
  $cf['field']['tags'] = array();


  // process specific fields: body
  if ($vars['element']['#field_name'] == 'body') {
    $attributes = isset($vars['item_attributes_array']) ? $vars['item_attributes_array'] : array();
    $attributes['class'] = isset($vars['classes_array']) ? $vars['classes_array'] : array();
    $attributes['class'][] = 'field-item';

    $cf['field']['tags']['mcneese_field__body_open'] = array('name' => 'section', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
    $cf['field']['tags']['mcneese_field__body_close'] = array('name' => 'section', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
  }
}

/**
 * Implements hook_preprocess_user_profile().
 */
function mcneese_preprocess_user_profile(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['user_profile'] = array();
  $cf['user_profile']['tags'] = array();


  // profile header
  $attributes = array();
  $attributes['class'] = array('user_profile-header');

  $cf['user_profile']['tags']['mcneese_user_profile_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['user_profile']['tags']['mcneese_user_profile_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // profile content
  $attributes = array();

  $cf['user_profile']['tags']['mcneese_user_profile_open'] = array('name' => 'section', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['user_profile']['tags']['mcneese_user_profile_close'] = array('name' => 'section', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
}

/**
 * Implements hook_preprocess_user_profile_category().
 */
function mcneese_preprocess_user_profile_category(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['user_profile_category'] = array();
  $cf['user_profile_category']['tags'] = array();


  // profile header
  $attributes = array();
  $attributes['class'] = array('user_profile_category-header');

  $cf['user_profile_category']['tags']['mcneese_user_profile_category_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['user_profile_category']['tags']['mcneese_user_profile_category_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);


  // profile content
  $attributes = array();

  $cf['user_profile_category']['tags']['mcneese_user_profile_category_open'] = array('name' => 'section', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['user_profile_category']['tags']['mcneese_user_profile_category_close'] = array('name' => 'section', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
}

/**
 * Implements hook_preprocess_user_picture().
 */
function mcneese_preprocess_user_picture(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['user_picture']['tags'] = array();

  $variables['user_picture'] = '';

  if (variable_get('user_pictures', 0)) {
    if (!empty($variables['account']->picture)) {
      if (is_numeric($variables['account']->picture)) {
        $variables['account']->picture = file_load($variables['account']->picture);
      }
      if (!empty($variables['account']->picture->uri)) {
        $filepath = $variables['account']->picture->uri;
      }
    }
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }

    if (isset($filepath)) {
      if ($cf['is']['html5']) {
        $alt = "";
        $title = t("@user's Picture", array('@user' => format_username($variables['account'])));
      }
      else {
        $alt = t("@user's Picture", array('@user' => format_username($variables['account'])));
        $title = $alt;
      }

      // If the image does not have a valid Drupal scheme (for eg. HTTP), don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $title));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $title));
      }

      if (!empty($variables['account']->uid) && user_access('access user profiles')) {
        $attributes = array(
          'attributes' => array('title' => t("View user profile.")),
          'html' => TRUE,
        );

        $variables['user_picture'] = l($variables['user_picture'], 'user/' . $variables['account']->uid, $attributes);
      }
    }
  }


  $attributes = array();
  $attributes['class'] = array('file', 'image', 'user-picture');

  $cf['user_picture']['tags']['mcneese_user_picture_open'] = array('name' => 'figure', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['user_picture']['tags']['mcneese_user_picture_close'] = array('name' => 'figure', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
}

/**
 * Implements hook_preprocess_search_block_form().
 */
function mcneese_preprocess_search_block_form(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (empty($cf)) {
    mcneese_initialize_variables($vars);
  }

  $cf['search_block_form'] = array();
  $cf['search_block_form']['tags'] = array();

  $attributes = array();
  $attributes['class'] = array('search_block_form-header');
  $attributes['role'] = 'search';

  $cf['search_block_form']['tags']['mcneese_search_block_form_header_open'] = array('name' => 'header', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $cf['search_block_form']['tags']['mcneese_search_block_form_header_close'] = array('name' => 'header', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);
}

/**
 * Implements hook_cf_theme_get_variables_alter().
 */
function mcneese_cf_theme_get_variables_alter(&$cf, $variables){
  $cf['headers'] = cf_theme_generate_headers($cf);
  $cf['tags'] = array();
  $cf['is']['html5'] = TRUE;
  $cf['is']['legacy'] = FALSE;
  $cf['is_data']['html5'] = array();
  $cf['is_data']['legacy'] = array();
  $cf['agent']['doctype'] = '<!DOCTYPE html>';
  $cf['date']['enabled'] = TRUE;

  if (!isset($cf['show']['messages'])) {
    $cf['data']['page']['messages'] = array('raw' => array(), 'renderable' => array(), 'blocks' => array());
    $cf['show']['page']['messages'] = FALSE;
  }

  if (!isset($cf['show']['skipnav'])) {
    $cf['data']['skipnav'] = array();
    $cf['show']['skipnav'] = TRUE;
  }


  // tweak features based on user-agent
  switch($cf['agent']['machine_name']){
    case 'ie':
      $cf['meta']['http-equiv']['X-UA-Compatible'] = 'IE=Edge; IE=10; IE=9';

      if ($cf['agent']['major_version'] <= 8){
        $cf['is']['unsupported'] = TRUE;
        $cf['meta']['http-equiv']['X-UA-Compatible'] = 'IE=Edge; IE=10; IE=9; IE=8';

        drupal_add_js(drupal_get_path('theme', 'mcneese') . '/js/ie_html5.js', array('group' => JS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'weight' => 10, 'preprocess' => TRUE));

        if ($cf['agent']['major_version'] == 7){
          if (preg_match('@; Trident/@', $cf['agent']['raw']) > 0){
            $cf['is']['in_ie_compatibility_mode'] = TRUE;
          }
        }

        if ($cf['agent']['major_version'] <= 7){
          $cf['is']['html5'] = FALSE;
          $cf['is']['legacy'] = TRUE;
        }
      }

      break;
  }

  if (!$cf['is']['html5']) {
    $cf['show']['skipnav'] = TRUE;
  }


  // initialize toolbar settings
  foreach (array('toolbar', 'toolbar-autoshow', 'toolbar-autohide', 'toolbar-fixed', 'toolbar-relative', 'toolbar-expanded', 'toolbar-collapsed', 'toolbar-shortcuts-expanded', 'toolbar-shortcuts-collapsed') as $key) {
    $cf['is'][$key] = FALSE;
    $cf['is_data'][$key] = array();
  }

  $cf['is']['toolbar'] = user_access('access toolbar');

  if ($cf['is']['toolbar']) {
    $cf['is']['toolbar-expanded'] = TRUE;
    $cf['is']['toolbar-collapsed'] = FALSE;
    $cf['is']['toolbar-sticky'] = FALSE;
    $cf['is']['toolbar-fixed'] = TRUE;
    $cf['is']['toolbar-relative'] = FALSE;
    $cf['is']['toolbar-autoshow'] = FALSE;
    $cf['is']['toolbar-autohide'] = FALSE;

    if (isset($cf['user']['object']->data['mcneese_settings']['toolbar'])) {
      $toolbar_settings = $cf['user']['object']->data['mcneese_settings']['toolbar'];

      if (isset($toolbar_settings['autohide']) && $toolbar_settings['autohide']) {
        $cf['is']['toolbar-expanded'] = FALSE;
        $cf['is']['toolbar-collapsed'] = TRUE;
        $cf['is']['toolbar-autoshow'] = FALSE;
        $cf['is']['toolbar-autohide'] = TRUE;
      }

      if (isset($toolbar_settings['sticky']) && $toolbar_settings['sticky']) {
        $cf['is']['toolbar-fixed'] = FALSE;
        $cf['is']['toolbar-relative'] = TRUE;
      }
    }

    if (_toolbar_is_collapsed()) {
      $cf['is']['toolbar-shortcuts-expanded'] = TRUE;
      $cf['is']['toolbar-shortcuts-collapsed'] = FALSE;
    }
    else {
      $cf['is']['toolbar-shortcuts-expanded'] = TRUE;
      $cf['is']['toolbar-shortcuts-collapsed'] = FALSE;
    }
  }
}

/**
 * Perform initialization of variables array.
 *
 * @param array $vars
 *   The variables array.
 */
function mcneese_initialize_variables(&$vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (!empty($cf)) return;

  if (function_exists('cf_theme_get_variables')) {
    $cf = cf_theme_get_variables($vars);
  }
  else {
    $cf['agent'] = array();

    $cf['at'] = array();
    $cf['is'] = array();
    $cf['show'] = array();
    $cf['show']['page'] = array();
    $cf['show']['html'] = array();
    $cf['data'] = array();
    $cf['data']['page'] = array();
    $cf['data']['html'] = array();

    $cf['show']['page']['breadcrumb'] = FALSE;
    $cf['data']['page']['breadcrumb'] = array();
    $cf['show']['page']['content'] = FALSE;
    $cf['data']['page']['content'] = array();

    $cf['markup_css'] = array();
    $cf['markup_css']['body'] = array();
    $cf['markup_css']['body']['class'] = '';
    $cf['markup_css']['container'] = array();
    $cf['markup_css']['container']['class'] = '';
    $cf['markup_css']['content'] = array();
    $cf['markup_css']['content']['class'] = '';


    // some defaults we can guess
    $cf['is']['front'] = drupal_is_front_page();
    $cf['is']['html5'] = TRUE;

    $cf['agent']['doctype'] = '<!DOCTYPE html>';
  }


  // refresh is considered not accessible
  $cf['meta'] = array();
  $cf['meta']['name'] = array();
  $cf['meta']['name']['refresh'] = '';


  // initialize rdf namespaces
  $cf['show']['html']['rdf_namespaces'] = FALSE;
  $cf['data']['html']['rdf_namespaces'] = drupal_get_rdf_namespaces();
}

/**
 * Render all data for: page.
 */
function mcneese_render_page() {
  $cf = & drupal_static('cf_theme_get_variables', array());


  // standard render
  if (function_exists('cf_theme_render_cf')) {
    $keys = array('header', 'header_menu_1', 'header_menu_2', 'action_links', 'title', 'help', 'menus', 'asides', 'precrumb', 'postcrumb', 'help', 'footer', 'hidden');
    cf_theme_render_cf($cf, $keys, 'page');
  }


  // render breadcrumb
  if (!empty($cf['page']['breadcrumb'])) {
    $cf['data']['page']['breadcrumb'] = theme('breadcrumb', array('breadcrumb' => $cf['page']['breadcrumb']));
    $cf['show']['page']['breadcrumb'] = TRUE;
  }


  // render content
  $cf['data']['page']['content'] = drupal_render_children($cf['page']['content']);

  if (empty($cf['data']['page']['content'])) {
    $cf['show']['page']['content'] = FALSE;
  }
  else {
    $cf['show']['page']['content'] = TRUE;
  }


  // determine positions of all floating menus
  $float_info_keys = array('messages', 'help');
  $float_info_position = 0;

  foreach ($float_info_keys as $key) {
    if ($cf['show']['page'][$key] && in_array('fixed', $cf['page']['tags']['mcneese_page_' . $key . '_open']['attributes']['class'])) {
      $cf['page']['tags']['mcneese_page_' . $key . '_open']['attributes']['class'][] = 'float_info';
      $cf['page']['tags']['mcneese_page_' . $key . '_open']['attributes']['class'][] = 'float_info-' . $float_info_position;

      $float_info_position++;
    }
  }


  // render the message array pieces
  if ($cf['show']['page']['messages']) {
    $cf['data']['page']['messages']['renderred'] = theme('status_messages', array('messages' => $cf['data']['page']['messages']['raw'], 'other' => render($cf['data']['page']['messages']['blocks'])));
  }


  // build the primary and secondary tabs
  if ($cf['is']['logged_in']) {
    mcneese_render_page_tabs();
  }
  else {
    $cf['show']['page']['tabs'] = FALSE;
  }
}

/**
 * Render all data for: node.
 */
function mcneese_render_node() {
  $cf = & drupal_static('cf_theme_get_variables', array());


  if (function_exists('cf_theme_render_cf')) {
    $keys = array('title_prefix', 'title_suffix', 'header', 'content');
    cf_theme_render_cf($cf, $keys, 'node');

    if (empty($cf['data']['node']['content'])) {
      $cf['show']['node']['content'] = FALSE;
    }
  }
}


/**
 * Render all data for: block.
 */
function mcneese_render_block() {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (function_exists('cf_theme_render_cf')) {
    $keys = array('title_prefix', 'title_suffix', 'header', 'content');
    cf_theme_render_cf($cf, $keys, 'block');

    if ($cf['block']['heading'] == 0) {
      $cf['show']['block']['header'] = FALSE;
    }
    else {
      $cf['show']['block']['header'] = TRUE;
    }
  }
}

/**
 * Custom implementation of theme_status_messages().
 *
 * This overrides the core theme.
 * This adds an additional arguments:
 *   messages (optional):
 *   - a pre-generated message array instead of having this theme function
 *     directly call drupal_get_messages().
 *   other (optional):
 *   - additional markup to append after the standard message.
 *   - this is usually a custom block.
 *   - is assumed to be already renderred markup.
 *
 * @see theme_status_messages()
 * @see mcneese_themes_theme_registry_alter()
 */
function mcneese_status_messages($vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  if (!empty($vars['messages']) && is_array($vars['messages'])) {
    $all_messages = $vars['messages'];
  }
  else {
    $display = $vars['display'];
    $all_messages = drupal_get_messages($display);
  }

  $output = '';

  $status_heading = array(
    'status' => t("Status message"),
    'error' => t("Error message"),
    'warning' => t("Warning message"),
  );

  if (isset($cf['is']['html5'])) {
    if ($cf['is']['html5']) {
      $output .= '<header>' . '<h2>' . t("Messages") . '</h2>' . '</header>';
    }
    else {
      $output .= '<h2>' . t("Messages") . '</h2>';
    }
  }
  else {
   $output .= '<h2 class="element-invisible">' . t("Messages") . '</h2>';
  }

  $output .= '<div class="messages-wrapper">';

  foreach ($all_messages as $type => $messages) {
    $output .= '<div class="messages ' . $type . '" role="alert">';

    if (!empty($status_heading[$type])) {
      $output .= '<h3 class="element-invisible">' . $status_heading[$type] . '</h3>';
    }

    if (count($messages) > 1) {
      $output .= ' <ul>';

      foreach ($messages as $message) {
        $output .= '  <li>' . $message . '</li>';
      }

      $output .= ' </ul>';
    }
    else {
      $output .= $messages[0];
    }

    $output .= '</div>';
  }

  if (!empty($vars['other']) && is_string($vars['other'])) {
    $output .= '<div class="messages-other">';
    $output .= $vars['other'];
    $output .= '</div>';
  }

  $output .= '</div>';

  return $output;
}

/**
 * Custom implementation of theme_breadcrumb().
 *
 * This removes the breadcrumb wrapper, requiring the theme to specify the
 * wrapper tag.
 *
 * @see theme_breadcrumb()
 */
function mcneese_breadcrumb($vars) {
  $breadcrumb = (array) $vars['breadcrumb'];

  $breadcrumb[0] = '<a href="' . base_path() . '" class="link-home" title="Home">' . t("Home") . '</a>';

  return implode(' <div class="crumb-trail">»</div> ', $breadcrumb);
}

/**
 * Custom implementation of hook_menu_tree().
 *
 * @see theme_menu_tree()
 */
function mcneese_menu_tree($vars) {
  $cf = & drupal_static('cf_theme_get_variables', array());

  $attributes = array();
  $attributes['class'] = array('menu');

  $menu_open = array('name' => 'nav', 'type' => 'semantic', 'attributes' => $attributes, 'html5' => $cf['is']['html5']);
  $menu_close = array('name' => 'nav', 'type' => 'semantic', 'open' => FALSE, 'html5' => $cf['is']['html5']);

  return theme('mcneese_tag', $menu_open) . $vars['tree'] . theme('mcneese_tag', $menu_close);
}


/**
 * An internal function for rendering primary & secondary tabs.
 */
function mcneese_render_page_tabs() {
  $cf = & drupal_static('cf_theme_get_variables', array());
  $cf['show']['page']['tabs'] = FALSE;

  if (!empty($cf['page']['tabs']['tabs']['output'])) {
    $tabs = & $cf['page']['tabs']['tabs'];

    if (isset($tabs['count']) && $tabs['count'] > 0) {
      $cf['show']['page']['tabs'] = TRUE;
      $cf['data']['page']['tabs'] = '';
      $count = 0;
      $even_odd = 'even';

      // prepend the menu_tabs-command-1 link
      $menu_tabs_text = (in_array('fixed', $cf['page']['tags']['mcneese_page_tabs_open']['attributes']['class']) ? t("Menu Tabs") : "");
      $attributes = array();
      $attributes['class'] = array('tab', 'tab-command', 'tab-command-1');
      $cf['data']['page']['tabs'] .= theme('list_item', array('markup' => '<a title="Collapse Menu Tabs" class="">' . $menu_tabs_text . '</a>', 'attributes' => $attributes));

      while ($count < $tabs['count']) {
        if (empty($tabs['output'][$count]['#link'])) {
          $count++;
          continue;
        }

        $link = & $tabs['output'][$count]['#link'];
        $attributes = array();
        $attributes['title'] = $link['title'];
        $attributes['class'] = array('tab', 'tab-' . $count, $even_odd);
        $markup = $link['title'];
        $even_odd = ($even_odd == 'even' ? 'odd' : 'even');
        $active = FALSE;

        if ($count == 0) {
          $attributes['class'][] = 'tab-first';
        }
        elseif ($count + 1 == $tabs['count']) {
          $attributes['class'][] = 'tab-last';
        }

        if (!empty($tabs['output'][$count]['#active'])) {
          // Add text to indicate active tab for non-visual users.
          $active = ' <span class="element-invisible">' . t("(active tab)") . '</span>';
          $attributes['class'][] = 'active';

          // If the link does not contain HTML already, check_plain() it now.
          // After we set 'html'=TRUE the link will not be sanitized by l().
          if (empty($link['localized_options']['html'])) {
            $link['title'] = check_plain($link['title']);
          }

          $link['localized_options']['html'] = TRUE;
          $markup = t("!local-tab-title!active", array('!local-tab-title' => $link['title'], '!active' => $active));
          $active = TRUE;
        }

        // add all secondary tabs immediately next to the primary tab
        if ($active) {
          $sub_tabs_markup = '';

          if (empty($cf['page']['sub_tabs']['tabs']['output'])) {
            $attributes['class'][] = 'leaf';
          }
          else {
            $sub_tabs = & $cf['page']['sub_tabs']['tabs'];

            $attributes['class'][] = 'expanded';

            $sub_tabs_attributes = array('class' => array());
            $sub_tabs_attributes['class'][] = 'sub_tabs';
            $sub_tabs_markup = theme('mcneese_tag', array('name' => 'nav', 'type' => 'semantic', 'attributes' => $sub_tabs_attributes, 'html5' => $cf['is']['html5']));

            if (isset($sub_tabs['count']) && $sub_tabs['count'] > 0) {
              $sub_count = 0;
              $sub_even_odd = 'even';

              while ($sub_count < $sub_tabs['count']) {
                if (empty($sub_tabs['output'][$sub_count]['#link'])) {
                  $sub_count++;
                  continue;
                }

                $sub_link = & $sub_tabs['output'][$sub_count]['#link'];
                $sub_attributes = array();
                $sub_attributes['title'] = $sub_link['title'];
                $sub_attributes['class'] = array('tab', 'tab-' . $sub_count, $sub_even_odd);
                $sub_markup = $sub_link['title'];
                $sub_even_odd = ($sub_even_odd == 'even' ? 'odd' : 'even');

                if ($sub_count == 0) {
                  $sub_attributes['class'][] = 'tab-first';
                }
                elseif ($sub_count + 1 == $sub_tabs['count']) {
                  $sub_attributes['class'][] = 'tab-last';
                }

                if (!empty($sub_tabs['output'][$sub_count]['#active'])) {
                  // Add text to indicate active tab for non-visual users.
                  $active = ' <span class="element-invisible">' . t("(active tab)") . '</span>';
                  $sub_attributes['class'][] = 'active';

                  // If the link does not contain HTML already, check_plain() it now.
                  // After we set 'html'=TRUE the link will not be sanitized by l().
                  if (empty($sub_link['localized_options']['html'])) {
                    $sub_link['title'] = check_plain($sub_link['title']);
                  }

                  $sub_link['localized_options']['html'] = TRUE;
                  $sub_markup = t("!local-tab-title!active", array('!local-tab-title' => $sub_link['title'], '!active' => $active));
                }

                $sub_tabs_markup .= theme('list_item', array('markup' => l($sub_markup, $sub_link['href'], $sub_link['localized_options']), 'attributes' => $sub_attributes));

                $sub_count++;
              }
            }

            $sub_tabs_markup .= theme('mcneese_tag', array('name' => 'nav', 'type' => 'semantic' , 'open' => FALSE, 'html5' => $cf['is']['html5']));
          }

          $cf['data']['page']['tabs'] .= theme('list_item', array('markup' => l($markup, $link['href'], $link['localized_options']) . $sub_tabs_markup, 'attributes' => $attributes));
        }
        else {
          $cf['data']['page']['tabs'] .= theme('list_item', array('markup' => l($markup, $link['href'], $link['localized_options']), 'attributes' => $attributes));
        }

        $count++;
      }
    }
  }
}

/**
 * @} End of '@defgroup mcneese McNeese - Base Theme'.
 */
