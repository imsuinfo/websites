<?php

/**
 * Field handler to present unsoft delete a node.
 */
class quail_api_node_handler_link_validate_node extends views_handler_field_node_link {
  function access() {
    return user_access('perform node accessibility validation');
  }

  function render_link($data, $values) {
    if (!quail_api_node_is_enabled($values->type){
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'node/' . $values->nid . '/validate/perform';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t("validate");

    return $text;
  }
}
