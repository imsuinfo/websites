<?php

/**
 * Displays the content type admin overview page.
 */
function content_easement_overview_types() {
  // TODO: where types and names are pulled in, replace this with a content easement specific type pull
  // TODO: where field_ui is pulled in, make sure to replace all field ui links that point to 'admin/structure/types' into 'admin/structure/easement/types'

  $easement_types_object = content_easement_get_types();
  $easement_types_array  = array();

  if (is_object($easement_types_object)){
    $easement_types_array = $easement_types_object->execute()->fetchAll();
  }

  $field_ui = module_exists('field_ui');
  $header = array(t("Name"), array('data' => t("Operations"), 'colspan' => $field_ui ? '4' : '2'));
  $rows = array();

  foreach ($easement_types_array as $key => $value) {
    if (node_hook($value->type, 'form')) {
      $type_url_str = str_replace('_', '-', $value->type);
      $type_name = check_plain($value->name);
      $type_name .= " <small> (Machine name: " . check_plain($value->type) . ")</small>";
      $type_name .= "<div class='description'>" . filter_xss_admin($value->description) . "</div>";

      $row = array($type_name);

      // Set the edit column.
      $row[] = array('data' => l(t("edit"), 'admin/structure/easement/types/manage/' . $type_url_str));

      if ($field_ui) {
        // Manage fields.
        $row[] = array('data' => l(t("manage fields"), 'admin/structure/easement/types/manage/' . $type_url_str . '/fields'));

        // Display fields.
        $row[] = array('data' => l(t("manage display"), 'admin/structure/easement/types/manage/' . $type_url_str . '/display'));
      }

      // Set the delete column.
      $row[] = array('data' => l(t("delete"), 'admin/structure/easement/types/manage/' . $type_url_str . '/delete'));

      $rows[] = $row;
    }
  }

  $form['node_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t("No node types available. <a href='@link'>Add node type</a>.", array('@link' => url('admin/structure/easement/types/add'))),
  );

  return $form;
}
