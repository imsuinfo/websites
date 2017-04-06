<?php

/**
 * @file
 * Defines McNeese Catalog programs class.
 */

/**
 * @addtogroup mcneese_catalog
 * @{
 */

class mcneese_catalog_class_programs extends mcneese_catalog_class_base {
  /**
   * Class constuctor.
   */
  public function __construct($catalog_id, $legacy_id) {
    parent::__construct($catalog_id, $legacy_id);

    $this->table          = 'mcneese_catalog_programs';
    $this->path_name      = 'programs';
    $this->cache_name     = 'programs';
    $this->cache_lifetime = '+24 hours';

    $this->select_condition = ' where display and active';

    $this->list_name = 'program-list';
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
   * @param int|null $page
   *   (optional) If NULL, then no pager is used.
   *   If an integer, then this represents the particular page in use for generating a pager.
   * @param int|null $per_page
   *   (optional) $page is defined and is an integer, then this represents the number of items per page.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  protected function render_markup($page = NULL, $per_page = NULL) {
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
