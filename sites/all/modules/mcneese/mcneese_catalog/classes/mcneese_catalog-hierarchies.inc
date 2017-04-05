<?php

/**
 * @file
 * Defines McNeese Catalog hierarchies class.
 */

/**
 * @addtogroup mcneese_catalog
 * @{
 */

class mcneese_catalog_class_hierarchies extends mcneese_catalog_class_base {
  /**
   * Class constuctor.
   */
  public function __construct($catalog_id, $legacy_id) {
    parent::__construct($catalog_id, $legacy_id);

    $this->table          = 'mcneese_catalog_hierarchies';
    $this->path_name      = 'hierarchies';
    $this->cache_name     = 'hierarchies';
    $this->cache_lifetime = '+24 hours';

    $this->select_condition = ' where display and active';

    $this->list_name = 'hierarchy-list';
  }

  /**
   * Process the loaded data.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  protected function process_data() {
    if (empty($this->loaded->{$this->list_name})) {
      $this->loaded = NULL;
      $this->processed = array();
      return TRUE;
    }

    $catalog = NULL;

    foreach ($this->loaded->{$this->list_name} as $item) {
      // require the id.
      if (!property_exists($item, 'id') || !is_numeric($item->id)) {
        continue;
      }

      // require the name string to exist and be non-empty.
      if (empty($item->name) || !is_string($item->name)) {
        continue;
      }

      // ignore archived items.
      if (property_exists($item, 'display') && $item->display) {
        continue;
      }

      // ignore unpublished items.
      if (property_exists($item, 'active') && !$item->active) {
        continue;
      }

      // @todo
    }

    $this->loaded = NULL;
    $this->processed = array($catalog);
    return TRUE;
  }

  /**
   * Renders the markup.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  protected function render_markup() {
    if (is_null($this->processed)) {
      return FALSE;
    }

    $markup = '';

    // @todo

    return TRUE;
  }
}

/**
 * @} End of '@addtogroup mcneese_catalog'.
 */
