<?php

/**
 * @file
 * Defines McNeese content page functions.
 */

/**
 * @addtogroup mcneese_content
 * @{
s*/

/**
 * Provides the XML sitemap for robots to process.
 *
 * This function calls drupal_exit() when it is done, unless there is an
 * error. This prevents the drupal theming engine from functioning here,
 * allowing for a true XML sitemap page.
 */
function mcneese_content_sitemap_page() {
  global $base_url, $base_path;
  $rows = '';

  $url = $base_url . $base_path . 'node/';

  // first load all nodes and handle errors before output is printed.
  try {
    $query_1 = db_select('node', 'n');
    $query_1->fields('n', array('nid', 'changed'));

    if (function_exists('killfile_menu')) {
      $query_1->leftJoin('killfile_nodes', 'kn', 'n.nid = kn.nid');
      $query_1->isNull('kn.nid');
    }

    $query_1->condition('n.status', 1);
    $query_1->orderBy('n.nid');
    $executed_1 = $query_1->execute();

    foreach ($executed_1 as $row) {
      $rows .= '  <url>' . "\n";
      $rows .= '    <loc>' . $url . $row->nid . '</loc>' . "\n";
      $rows .= '    <lastmod>' . date('Y-m-d', $row->changed) . '</lastmod>' . "\n";
      $rows .= '  </url>' . "\n";
    }

    if (function_exists('workbench_moderation_menu')) {
      $query_2 = db_select('node', 'n');
      $query_2->fields('n', array('nid', 'changed'));
      $query_2->innerJoin('workbench_moderation_node_history', 'wmnh', 'n.nid = wmnh.nid AND n.vid = wmnh.vid AND wmnh.published = 1');

      if (function_exists('killfile_menu')) {
        $query_2->leftJoin('killfile_nodes', 'kn', 'n.nid = kn.nid');
        $query_2->isNull('kn.nid');
      }

      $query_2->orderBy('n.nid');
      $query_2->condition('n.status', 1);
      $executed_2 = $query_2->execute();

      foreach ($executed_2 as $row) {
        $rows .= '  <url>' . "\n";
        $rows .= '    <loc>' . $url . $row->nid . '</loc>' . "\n";
        $rows .= '    <lastmod>' . date('Y-m-d', $row->changed) . '</lastmod>' . "\n";
        $rows .= '  </url>' . "\n";
      }
    }
  }
  catch (Exception $e) {
    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }

    throw $e;

    // if it can get here, then make sure it returns instead of calling drupal_exit() below.
    return FALSE;
  }

  // make sure the browser realizes that this is not a web page and is instead an xml page.
  drupal_add_http_header('content-type', 'application/xml');
  drupal_send_headers();

  print('<?xml version="1.0" encoding="UTF-8"?>' . "\n");
  print('<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">' . "\n");
  print($rows);
  print('</urlset>');
  drupal_exit();
}
