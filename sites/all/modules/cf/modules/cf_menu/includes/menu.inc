<?php

/**
 * Displays the content accessibility menu page which presents links to content easement management pages.
 */
function cf_menu_accessibility_page() {
  $blocks = array();

  drupal_set_title(t("Content Accessibility"), PASS_THROUGH);

  $query = db_select('menu_router', 'mr');
  $query->innerjoin('menu_links',  'ml', 'mr.path = ml.router_path');

  $query->fields('mr');
  $query->fields('ml');
  $query->orderBy('mr.weight', 'ASC');
  $query->orderBy('ml.link_title', 'ASC');
  $query->distinct();

  $query->condition('mr.path', 'admin/content/accessibility/%', 'LIKE');
  $query->condition('mr.path', 'admin/content/accessibility/%/%', 'NOT LIKE');
  $query->condition('ml.plid', 0, '<>');

  $executed = $query->execute();

  if ($executed){
    $available_paths = $executed->fetchAll();

    foreach ($available_paths as $key => $value){
      $block = get_object_vars($value);

      _menu_link_translate($block);

      // The link description, either derived from 'description' in hook_menu()
      // or customized via menu module is used as title attribute.
      if (!empty($block['block']['attributes']['title'])) {
        $block['description'] = $value['localized_options']['attributes']['title'];
        unset($block['localized_options']['attributes']['title']);
      }

      $block['title'] = l($block['link_title'], $block['router_path']);
      $block['content'] = '';

      $compact = system_admin_compact_mode();

      if (isset($block['description'])){
        $style='';

        if ($compact) {
          $style = " style='display: none;'";
        }

        $block['content'] .= '<div' . $style . '>' . filter_xss_admin($block['description']) . '</div>';
      }

      if (!empty($block['content'])) {
        $block['show'] = TRUE;
      }

      // Prepare for sorting as in function _menu_tree_check_access().
      // The weight is offset so it is always positive, with a uniform 5-digits.
      $blocks[(50000 + $block['weight']) . ' ' . $block['title'] . ' ' . $block['mlid']] = $block;
    }
  }

  if (empty($blocks)) {
    return t("There are no links available on this page.");
  }
  else {
    ksort($blocks);
    return theme('admin_page', array('blocks' => $blocks));
  }
}
