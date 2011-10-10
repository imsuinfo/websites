<?php

/**
 * Field handler to present unsoft delete a node.
 */
class node_accessibility_handler_link_validate_node extends views_handler_field_node_link {
  function access() {
    return user_access('perform node accessibility validation');
  }

  function render_link($data, $values) {
    $function_history = array();
    cf_error_append_history($function_history, __FUNCTION__);

    if (!is_object($values)){
      cf_error_invalid_object($function_history, 'values');
      return;
    }

    $type = '';

    if (property_exists($values, 'type')){
      $type = $values->type;
    }
    else if (property_exists($values, 'nid')){
      $node = node_load($values->nid);
      $type = $node->type;
    }

    if (empty($type) || !node_accessibility_is_enabled($type)){
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'node/' . $values->nid . '/accessibility';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t("validate");

    return $text;
  }
}
