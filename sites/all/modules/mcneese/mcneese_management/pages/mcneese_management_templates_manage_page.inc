<?php

/**
 * @file
 * Defines templates management page functions.
 */

/**
 * @addtogroup mcneese_management
 * @{
 */

/**
 * Provides the templates manage page.
 */
function mcneese_management_templates_manage_page() {
  $output = '';
  $rows = array();

  $template_url = 'admin/content/management/template';

  try {
    $query = db_select('mcneese_management_template', 'mmt');
    $query->fields('mmt');
    $query->orderBy('machine_name', 'ASC');

    $executed = $query->execute();

    foreach ($executed as $result) {
      $row = array();
      $id = check_plain($result->id);
      $row[] = $id;
      $row[] = check_plain($result->machine_name);
      $row[] = check_plain($result->human_name);

      $links = '';
      $links .= '<a href="' . url($template_url . '/manage/user/' . $id) . '">' . t("Users") . '</a> ';
      $links .= '<a href="' . url($template_url . '/manage/edit/' . $id) . '">' . t("Edit") . '</a> ';
      $links .= '<a href="' . url($template_url . '/manage/delete/' . $id) . '">' . t("Delete") . '</a> ';
      $row[] = $links;

      $rows[] = $row;
    }
  }
  catch (Exception $e) {
    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  $header = array(t("ID"), t("Machine Name"), t("Human Name"), t("Operations"));
  $attributes = array();
  $attributes['id'] = 'mcneese_management_template_manage';
  $attributes['summary'] = t("A listing of templates available for management.");
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t("Templates"), 'attributes' => $attributes, 'empty' => t("There are no templates available.")));

  $output .= '<br>';
  $output .= '<a href="' . url($template_url . '/manage/add') . '">Add New Template</a>';

  $output .= '<br><br>';
  $output .= '<a id="edit-cancel" href="' . url($template_url) . '">Cancel</a>';

  return $output;
}

/**
 * Add/Edit form for managing templates.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param int $template_id
 *   (optional) The numeric id of the template to edit.
 */
function mcneese_management_templates_manage_edit_form($form, &$form_state, $template_id = NULL) {
  $form = array();

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/management/template/manage';

  $default_human_name = '';
  $default_machine_name = '';
  $default_data = array('template' => '', 'content_types' => array());
  $default_template = '';
  $default_content_types = '';

  // load existing settings
  if (!empty($template_id)) {
    if (!cf_is_integer($template_id)) {
      if (class_exists('cf_error')) {
        cf_error::invalid_numeric('template_id');
      }

      drupal_not_found();
      drupal_exit();
    }

    $query = db_select('mcneese_management_template', 'mmt');
    $query->fields('mmt');
    $query->condition('id', $template_id);

    $result = $query->execute()->fetchObject();

    if (empty($result)) {
      drupal_not_found();
      drupal_exit();
    }
    else {
      $default_human_name = $result->human_name;
      $default_machine_name = $result->machine_name;
      $default_data = unserialize($result->data);
      $default_template = $default_data['template'];
      $default_content_types = $default_data['content_types'];
    }
  }

  $content_types = array();
  $names = node_type_get_names();
  foreach ($names as $type => $name) {
    $content_types[$type] = $name;
  }

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Human Name"),
    '#size' => 64,
    '#default_value' => $default_human_name,
    '#description' => t("A human-friendly name that identifies this template. Does not have to be unique, but it is highly recommended to be unique."),
    '#required' => TRUE,
  );

  $form['settings']['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Machine Name"),
    '#size' => 64,
    '#maxlength' => 64,
    '#default_value' => $default_machine_name,
    '#description' => t("A machine-friendly name that uniquely identifies this template. (Lower case character and underscores only)"),
    '#required' => TRUE,
  );

  $form['settings']['content_types'] = array(
    '#type' => 'select',
    '#title' => t("Content Types"),
    '#default_value' => $default_content_types,
    '#description' => t("Select the content types that this template will support."),
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#options' => $content_types,
  );

  $form['settings']['template'] = array(
    '#type' => 'textarea',
    '#title' => t("Template Data"),
    '#default_value' => $default_template,
    '#description' => t("A machine-friendly name that uniquely identifies this template. (Lower case character and underscores only)"),
    '#required' => TRUE,
  );

  $help_markup = '<div>';
  $help_markup .= "The template works by using Template Input fields or " . '<strong>' . check_plain('<ti></ti>') . '</strong>.<br>';
  $help_markup .= "For example, a Template Input field called field_title would be: " . '<br>';
  $help_markup .= '<ul>';
  $help_markup .=  '  <li><strong>' . check_plain('<ti_field_title title="Field Name" type="textfield" maxlength="64" required="true">');
  $help_markup .=  "The field is described here with optional text and markup." . check_plain('</ti_field_title>') . '</strong>.</li>';
  $help_markup .= '</ul>';
  $help_markup .= '<br>';
  $help_markup .= "All custom elements must be defined in the reserved element called " . '<strong>' . check_plain("<ti></ti>") . '</strong>, such as:<br>';
  $help_markup .= '<ul><li><strong>';
  $help_markup .= check_plain('<ti>') . '<br>';
  $help_markup .= '&nbsp;&nbsp;' . check_plain('<ti_field_title title="Title" type="textfield"></ti_field_title>') . '<br>';
  $help_markup .= '&nbsp;&nbsp;' . check_plain('<ti_field_body title="Body" type="textarea"></to_field_body>') . '<br>';
  $help_markup .= check_plain('</ti>') . '<br>';
  $help_markup .= '</strong></li></ul>';
  $help_markup .= '<br>';
  $help_markup .= "The following custom attributes are also supported:" . '<br>';
  $help_markup .= '<ul>';
  $help_markup .= '<li><strong>wysiwyg</strong>: When set to "1", provides the user with a WYSIWYG such as ckeditor (may only work with textarea fields).</li>';
  $help_markup .= '<li><strong>markup</strong>: When set to "1", adds the values of this field as HTML markup instead of plain text. This is done automatically when the wysiwyg attribute is set to "1".</li>';
  $help_markup .= '</ul>';
  $help_markup .= '<br>';
  $help_markup .= "The form fields are populated using Template Input Targets,  " . '<strong>' . check_plain("<tit></tit>") . '</strong>, such as:<br>';
  $help_markup .= '<ul>';
  $help_markup .= '<li><strong>';
  $help_markup .= check_plain('<tit field="title">');
  $help_markup .= check_plain('<ti_field_title></ti_field_title>');
  $help_markup .= check_plain('</tit>') . '<br>';
  $help_markup .= '</strong></li>';
  $help_markup .= '<li><strong>';
  $help_markup .= check_plain('<tit field="body][und][0][value">');
  $help_markup .= check_plain('<ti_field_body></ti_field_body>');
  $help_markup .= check_plain('</tit>') . '<br>';
  $help_markup .= '</strong></li>';
  $help_markup .= '<li><strong>';
  $help_markup .= check_plain('<tit field="field_group][und][0][tid">');
  $help_markup .= check_plain('<ti_field_group></ti_field_group>');
  $help_markup .= check_plain('</tit>') . '<br>';
  $help_markup .= '</strong></li>';
  $help_markup .= '</ul>';
  $help_markup .= '<br>';
  $help_markup .= "Template Input Targets also support different types (and each type accepts additional arguments):, such as:" . '<br>';
  $help_markup .= '<ul>';
  $help_markup .= '<li><strong>normal</strong>: This is the default when no type is specified. This applies no special structure to the form values.</li>';
  $help_markup .= '<li><strong>textfield</strong>: This is almost identicaly to "normal" except that the data structure has additional information. Some form fields require this instead "normal". This requires additional attributes: "format".</li>';
  $help_markup .= '<li><strong>textarea</strong>: This is required for textarea form fields to be properly populated. This requires additional attributes: "format".</li>';
  $help_markup .= '</ul>';
  $help_markup .= '<br>';
  $help_markup .= "If conditions are also supported using Template If, " . '<strong>' . check_plain("<tif></tif>") . '</strong>, such as:<br>';
  $help_markup .= '<ul><li><strong>';
  $help_markup .= '&nbsp;&nbsp;' . check_plain('<tif target="ti_machine_name_1" type="not_empty"><ti_machine_name_1 title="Title" type="textfield"></ti_machine_name_1></tif>') . '<br>';
  $help_markup .= '</strong></li></ul>';
  $help_markup .= '<br>';
  $form['settings']['template_help'] = array(
    '#type' => 'markup',
    '#title' => t("Template Help"),
    '#markup' => $help_markup,
  );

  $form['template_id'] = array(
    '#type' => 'value',
    '#value' => $template_id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save Settings"),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for mcneese_management_templates_manage_edit_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_edit_form_validate($form, &$form_state) {
  if (empty($form_state['values']['settings']['human_name'])) {
    form_set_error('settings][human_name', t("An invalid human name has been specified."));
  }

  $machine_name = NULL;
  if (empty($form_state['values']['settings']['machine_name'])) {
    form_set_error('settings][machine_name', t("An invalid machine name has been specified."));
  }
  else {
    $machine_name = strtolower($form_state['values']['settings']['machine_name']);
  }

  if (empty($form_state['values']['settings']['template'])) {
    form_set_error('settings][template', t("An invalid template has been specified."));
  }

  $check_machine_name = FALSE;
  if (empty($form['template_id']['#value'])) {
    if (!empty($machine_name)) {
      $check_machine_name = TRUE;
    }
  }
  else {
    if (cf_is_integer($form['template_id']['#value'])) {
      try {
        $query = db_select('mcneese_management_template', 'mmt');
        $query->fields('mmt', array('machine_name'));
        $query->condition('id', $form['template_id']['#value']);

        $result = $query->execute()->fetchField();

        if (empty($result)) {
          form_set_error('template_id', t("An invalid template id has been specified."));
        }
        elseif ($result != $machine_name) {
          $check_machine_name = TRUE;
        }
      }
      catch (Exception $e) {
        if (class_exists('cf_error')) {
          cf_error::on_query_execution($e);
        }
      }
    }
    else {
      form_set_error('template_id', t("An invalid template id has been specified."));
    }
  }

  if ($check_machine_name) {
    try {
      $query = db_select('mcneese_management_template', 'mmt');
      $query->fields('mmt', array('id'));
      $query->condition('machine_name', $machine_name);
      $query->countQuery();

      $result = $query->execute()->fetchField();

      if ($result > 0) {
        form_set_error('machine_name', t("The specified machine_name '%machine_name' already exists and therefore cannot be used.", array('%machine_name' => $machine_name)));
      }
    }
    catch (Exception $e) {
      if (class_exists('cf_error')) {
        cf_error::on_query_execution($e);
      }
    }
  }
}

/**
 * Submit process for mcneese_management_templates_manage_edit_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_edit_form_submit($form, &$form_state) {
  $template = array();
  $template['human_name'] = $form_state['values']['settings']['human_name'];
  $template['machine_name'] = $form_state['values']['settings']['machine_name'];

  $data = array();
  $data['template'] = $form_state['values']['settings']['template'];
  $data['content_types'] = $form_state['values']['settings']['content_types'];
  $template['data'] = serialize($data);

  $transaction = db_transaction();

  try {
    if (empty($form['template_id']['#value'])) {
      $query = db_insert('mcneese_management_template');
      $query->fields($template);
      $result = $query->execute();
    }
    else {
      $query = db_update('mcneese_management_template');
      $query->condition('id', $form['template_id']['#value']);
      $query->fields($template);
      $result = $query->execute();
    }

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/content/management/template/manage';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Deletes a specified template.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param int $template_id
 *   The numeric id of the template to delete.
 */
function mcneese_management_templates_manage_delete_form($form, &$form_state, $template_id) {
  $form = array();

  if (!cf_is_integer($template_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('template_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/management/template/manage';

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['template_id'] = array(
    '#type' => 'value',
    '#value' => $template_id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  return confirm_form($form,
    t("Are you sure you want to delete this template?", array()),
    $destination,
    t("This action cannot be undone."),
    t("Delete"),
    t("Cancel")
  );
}

/**
 * Validation for mcneese_management_templates_manage_delete_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_delete_form_validate($form, &$form_state) {
  if (!isset($form['template_id']['#value']) || !cf_is_integer($form['template_id']['#value'])) {
    form_set_error('template_id', t("An invalid template id has been specified."));
  }
}

/**
 * Submit process for mcneese_management_templates_manage_delete_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_delete_form_submit($form, &$form_state) {
  $transaction = db_transaction();

  try {
    $query = db_delete('mcneese_management_template');
    $query->condition('id', $form['template_id']['#value']);
    $query->execute();

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/content/management/template/manage';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Provides the templates user manage page.
 *
 * @param int $template_id
 *   The numeric id of the template to display users who have access.
 */
function mcneese_management_templates_manage_user_page($template_id) {
  $output = '';
  $template_url = 'admin/content/management/template';

  if (!cf_is_integer($template_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('template_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $query = db_select('mcneese_management_template_user', 'mmtu');
  $query->fields('mmtu');
  $query->condition('template_id', $template_id);
  $query->orderBy('user_id', 'ASC');

  $executed = $query->execute();

  $rows = array();
  foreach ($executed as $result) {
    $row = array();

    $user_id = check_plain($result->user_id);

    $row[] = $user_id;

    $user = user_load($user_id);

    if (is_object($user)) {
      $row[] = check_plain($user->name);
    }
    else {
      $row[] = t("User Not Found");
    }

    $links = '';
    $links .= '<a href="' . url($template_url . '/manage/user/delete/' . check_plain($template_id) . '/' . $user_id) . '">' . t("Delete") . '</a> ';
    $row[] = $links;

    $rows[] = $row;
  }

  $header = array(t("ID"), t("User Name"), t("Operations"));
  $attributes = array();
  $attributes['id'] = 'mcneese_management_template_manage_user';
  $attributes['summary'] = t("A listing of users who have been granted access to the specified template.");
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t("Users"), 'attributes' => $attributes, 'empty' => t("No users have been granted access to this template.")));

  $output .= '<br>';
  $output .= '<a href="' . url($template_url . '/manage/user/add/' . $template_id) . '">Grant User Access to Template</a>';

  $output .= '<br><br>';
  $output .= '<a id="edit-cancel" href="' . url($template_url . '/manage') . '">Cancel</a>';

  return $output;
}

/**
 * Add form for managing template users.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param string $template_id
 *   The numeric ID of the template to delete.
 */
function mcneese_management_templates_manage_user_add_form($form, &$form_state, $template_id) {
  $form = array();

  if (!cf_is_integer($template_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('template_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/management/template/manage/user/' . $template_id;

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['user'] = array(
    '#type' => 'textfield',
    '#title' => t("User"),
    '#description' => t("Select a user to grant access to use this theme."),
    '#default_value' => '',
    '#autocomplete_path' => 'admin/content/management/template/ajax/autocomplete/user',
    '#required' => TRUE,
  );

  $form['template_id'] = array(
    '#type' => 'value',
    '#value' => $template_id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save Settings"),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t("Cancel"),
    '#href' =>  $destination,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validation for mcneese_management_templates_manage_user_add_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_user_add_form_validate($form, &$form_state) {
  $valid_template = FALSE;

  $user_id = mcneese_management_validate_user($form_state['values']['settings']['user']);
  if ($user_id === FALSE) {
    form_set_error('settings][user', t("The user '%user' does not exist."), array('%user' => $form_state['values']['settings']['user']));
  }

  if (!isset($form['template_id']['#value']) || !cf_is_integer($form['template_id']['#value'])) {
    form_set_error('template_id', t("An invalid template id has been specified."));
  }
  else {
    $valid_template = TRUE;
  }

  if ($user_id !== FALSE && $valid_template) {
    try {
      $query = db_select('mcneese_management_template_user', 'mmtu');
      $query->fields('mmtu', array('user_id'));
      $query->condition('template_id', $form['template_id']['#value']);
      $query->condition('user_id', $user_id);
      $query->countQuery();

      $result = $query->execute()->fetchField();

      if ($result > 0) {
        form_set_error('machine_name', t("The specified user '%user' is already added and therefore cannot be added again.", array('%user' => $form_state['values']['settings']['user'])));
      }
    }
    catch (Exception $e) {
      if (class_exists('cf_error')) {
        cf_error::on_query_execution($e);
      }
    }
  }
}

/**
 * Submit process for mcneese_management_templates_manage_user_add_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_user_add_form_submit($form, &$form_state) {
  $user = $form_state['values']['settings']['user'];
  $user_id = NULL;

  if (cf_is_integer($user)) {
    $user_id = $user;
  }
  else {
    $matches = array();
    $matched = preg_match('/ \[uid:(\d+)]\s*$/i', $user, $matches);

    if ($matched) {
      $user_id = $matches[1];
    }
    else {
      return;
    }
  }

  $user = array();
  $user['template_id'] = $form['template_id']['#value'];
  $user['user_id'] = $user_id;

  $transaction = db_transaction();

  try {
    $query = db_insert('mcneese_management_template_user');
    $query->fields($user);
    $result = $query->execute();

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/content/management/template/manage';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * Deletes access that was granted to a user for a specific template.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 * @param int $template_id
 *   The numeric id of the template to delete.
 * @param int $user_id
 *   The numeric id of the user to remove access from.
 */
function mcneese_management_templates_manage_user_delete_form($form, &$form_state, $template_id, $user_id) {
  $form = array();

  if (!cf_is_integer($template_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('template_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  if (!cf_is_integer($user_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('user_id');
    }

    drupal_not_found();
    drupal_exit();
  }

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/management/template/manage/user/' . $template_id;

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['template_id'] = array(
    '#type' => 'value',
    '#value' => $template_id,
  );

  $form['user_id'] = array(
    '#type' => 'value',
    '#value' => $user_id,
  );

  $form['redirect_to'] = array(
    '#type' => 'value',
    '#value' => $destination,
  );

  return confirm_form($form,
    t("Are you sure you want to delete access for this user for this template?", array()),
    $destination,
    t("This action cannot be undone."),
    t("Delete"),
    t("Cancel")
  );
}

/**
 * Validation for mcneese_management_templates_manage_user_delete_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_user_delete_form_validate($form, &$form_state) {
  if (!isset($form['template_id']['#value']) || !cf_is_integer($form['template_id']['#value'])) {
    form_set_error('template_id', t("An invalid template id has been specified."));
  }

  if (!isset($form['user_id']['#value']) || !cf_is_integer($form['user_id']['#value'])) {
    form_set_error('user_id', t("An invalid user id has been specified."));
  }
}

/**
 * Submit process for mcneese_management_templates_manage_user_delete_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api.
 */
function mcneese_management_templates_manage_user_delete_form_submit($form, &$form_state) {
  $transaction = db_transaction();

  try {
    $query = db_delete('mcneese_management_template_user');
    $query->condition('template_id', $form['template_id']['#value']);
    $query->condition('user_id', $form['user_id']['#value']);
    $query->execute();

    // enforce transaction execution
    unset($transaction);
  }
  catch (Exception $e) {
    $transaction->rollback();

    if (class_exists('cf_error')) {
      cf_error::on_query_execution($e);
    }
  }

  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = 'admin/content/management/template/manage';
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * @} End of '@addtogroup mcneese_management'.
 */
