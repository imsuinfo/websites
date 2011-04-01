<?php

/**
 * Implements hook_rules_file_info().
 */
function killfile_rules_file_info() {
  $items = array();
  $items[] = 'killfile.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function killfile_rules_condition_info() {
   $items = array(
    'node_is_deleted' => array(
      'group' => t("Node"),
      'label' => t("Content is Killfiled"),
      'base' => 'killfile_rules_condition_node_is_deleted',
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t("Content")),
      ),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_event_info() on behalf of the node module.
 */
function killfile_rules_event_info() {
  $items = array(
    'killfile_after_deleting' => array(
      'label' => t("After Killfiling Content"),
      'group' => t("Node"),
      'variables' => rules_events_node_variables(t("Killfiled content"), TRUE),
      'access callback' => 'rules_node_integration_access',
    ),
    'killfile_after_undeleting' => array(
      'label' => t("After Un-killfiling Content"),
      'group' => t("Node"),
      'variables' => rules_events_node_variables(t("Un-killfiled content"), TRUE),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function killfile_rules_action_info() {
  $items = array(
    'killfile_delete' => array(
      'label' => t("Killfile Content"),
      'group' => t("Node"),
      'base' => 'killfile_delete_action',
      'parameter' => killfile_rules_actions_parameters(t("Content to killfile")),
      'access callback' => 'rules_node_integration_access',
    ),
    'killfile_undelete' => array(
      'label' => t("Un-killfile Content"),
      'group' => t("Node"),
      'base' => 'killfile_undelete_action',
      'parameter' => killfile_rules_actions_parameters(t("Content to un-killfile")),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  return $items;
}

/**
 * Returns the standard arguments for rule events.
 *
 * @param $node_label
 *   A string representing the node label
 * @param $update
 *   (optional) a boolean representing whether or not this is an update
 *
 * @return
 *   The standard arguments for rule events.
 */
function killfile_rules_events_parameters($node_label, $update = FALSE) {
  $arguments = array(
    'node' => array('type' => 'node', 'label' => $node_label),
    'author' => array('type' => 'user', 'label' => t("User who triggered this event")),
  );

  if ($update) {
    $arguments += array(
      'node_unchanged' => array(
        'type' => 'node',
        'label' => t("Unchanged Content"),
        'handler' => 'rules_events_entity_unchanged',
      ),
      'author_unchanged' => array('type' => 'user', 'label' => t("Owner of the unchanged content")),
    );
  }

  return $arguments;
}

/**
 * Returns the standard arguments for rule actions.
 *
 * @param $node_label
 *   A string representing the node label
 *
 * @return
 *   The standard arguments for rule actions.
 */
function killfile_rules_actions_parameters($node_label) {
  return array(
    'node' => array(
      'type' => 'node',
      'label' => $node_label,
    ),
    'author' => array(
      'type' => 'user',
      'label' => t("User triggering this event"),
      'optional' => TRUE,
      'default value' => NULL,
    ),
    'bypass' => array(
      'type' => 'boolean',
      'label' => t("Bypass user permissions"),
      'description' => t("Perform this operation even if the user does not have permission to do so."),
      'optional' => TRUE,
      'default value' => FALSE,
    ),
  );
}

/**
 * Condition: Check if the node is killfiled
 *
 * @param $node
 *   A node object
 *
 * @return
 *   TRUE/FALSE depending on if the node is deleted.
 */
function killfile_rules_condition_node_is_deleted($node) {
  if (!is_object($node)){
    cf_error_invalid_object(__FUNCTION__, 'node', WATCHDOG_WARNING);
    return FALSE;
  }

  if (property_exists($node, 'deleted') && $node->deleted !== FALSE){
    return TRUE;
  }

  return FALSE;
}

/**
 * Action: killfile a node
 *
 * $param $node
 *   A node object
 * @param $author
 *   (optional) A user object representing the user who is performing the delete operation
 * @param $bypass
 *   (optional) A boolean reperesenting whether or not to bypass whether or not the user has permissions to delete a given node
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function killfile_delete_action($node, $author = NULL, $bypass = FALSE) {
  if (!is_object($node)){
    cf_error_invalid_object(__FUNCTION__, 'node', WATCHDOG_ERROR);
    return array('node' => $node);
  }

  $node_type_settings_object = killfile_get_node_type_settings($node->type, array(__FUNCTION__));

  if (!is_object($node_type_settings_object)){
    // TODO: present some sort of watchdog message about not being able to killfile or unkillfile a node because the content type does not have killfile enabled
    return array('node' => $node);
  }

  if (is_object($author)){
    $target_user = $author;
  }
  else {
    $target_user = cf_get_user(FALSE, array(__FUNCTION__));
  }

  if ($bypass || node_access('delete', $node, $target_user)) {
    killfile_fake_delete_nodes(array($node), $target_user, array(__FUNCTION__));
  }

  return array('node' => $node);
}

/**
 * Action: Un-killfile a node
 *
 * $param $node
 *   A node object
 * @param $author
 *   (optional) A user object representing the user who is performing the delete operation
 * @param $bypass
 *   (optional) A boolean reperesenting whether or not to bypass whether or not the user has permissions to delete a given node
 *
 * @return
 *   An array containing the node object stored in the key called 'node'.
 */
function killfile_undelete_action($node, $author = NULL, $bypass = FALSE) {
  if (!is_object($node)){
    cf_error_invalid_object(__FUNCTION__, 'node', WATCHDOG_ERROR);
    return array('node' => $node);
  }

  $node_type_settings_object = killfile_get_node_type_settings($node->type, array(__FUNCTION__));

  if (!is_object($node_type_settings_object)){
    // TODO: present some sort of watchdog message about not being able to killfile or unkillfile a node because the content type does not have killfile enabled
    return array('node' => $node);
  }

  if (is_object($author)){
    $target_user = $author;
  }
  else {
    $target_user = cf_get_user(FALSE, array(__FUNCTION__));
  }

  if ($bypass || killfile_can_undelete($node, $target_user, array(__FUNCTION__))) {
    killfile_undelete_nodes(array($node), $target_user, array(__FUNCTION__));
  }

  return array('node' => $node);
}
