<?php

/**
 * @file
 * Defines McNeese Catalog catalogs class.
 */

/**
 * @addtogroup mcneese_catalog
 * @{
 */

class mcneese_catalog_class_catalogs extends mcneese_catalog_class_base {
  /**
   * Class constuctor.
   */
  public function __construct($catalog_id = FALSE, $legacy_id = FALSE) {
    parent::__construct(FALSE, FALSE);

    $this->table          = 'mcneese_catalog_catalogs';
    $this->path_name      = 'catalogs';
    $this->cache_name     = 'catalogs';
    $this->cache_lifetime = '+24 hours';

    $this->select_condition = ' where not archived and published';
    $this->on_conflict_fields = array('id');

    $this->list_name = 'catalog-list';

    $this->as_link = self::AS_DIVIDER;
  }

  /**
   * Returns the name field from processed data, if any exists.
   *
   * @return array|null
   *   An array of processed data.
   *   NULL if not defined.
   */
  public function get_name() {
    if (is_null($this->processed)) {
      return FALSE;
    }

    // only a single catalog is used.
    if (!isset($this->processed[0]->name) || !is_string($this->processed[0]->name)) {
      return FALSE;
    }

    return $this->processed[0]->name;
  }

  /**
   * Returns the description field from processed data, if any exists.
   *
   * @return array|null
   *   An array of processed data.
   *   NULL if not defined.
   */
  public function get_description() {
    if (is_null($this->processed)) {
      return FALSE;
    }

    // only a single catalog is used.
    if (!isset($this->processed[0]->description) || !is_string($this->processed[0]->description)) {
      return FALSE;
    }

    return $this->processed[0]->description;
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
      // require the legacy id.
      if (!property_exists($item, 'legacy-id') || !is_numeric($item->{'legacy-id'})) {
        continue;
      }

      // require the name string to exist and be non-empty.
      if (empty($item->name) || !is_string($item->name)) {
        continue;
      }

      // ignore archived items.
      if (property_exists($item, 'archived') && $item->archived) {
        continue;
      }

      // ignore unpublished items.
      if (property_exists($item, 'published') && !$item->published) {
        continue;
      }

      // The current should be the only entry without archived set, but just in case loop until done and use last entry found.
      $catalog = new stdClass();
      $catalog->id = (int) $item->id;
      $catalog->legacy_id = (int) $item->{'legacy-id'};
      $this->catalog_id = $catalog->id;
      $this->legacy_id = $catalog->legacy_id;

      $catalog->name = $item->name;

      $catalog->description = '';
      if (property_exists($item, 'description') && is_string($item->description)) {
        $catalog->description =  $this->sanitize_markup($item->description);

        // remove all line endings.
        $catalog->description = preg_replace("/\r/i", '', $catalog->description);
        $catalog->description = preg_replace("/\n/i", '', $catalog->description);
      }

      if (!empty($item->created) && is_string($item->created)) {
        $sanitized_date = strtotime($item->created);
        $catalog->created = date('r', $sanitized_date);
        unset($sanitized_date);
      }

      if (!empty($item->modified) && is_string($item->modified)) {
        $sanitized_date = strtotime($item->modified);
        $catalog->modified = date('r', $sanitized_date);
        unset($sanitized_date);
      }

      $catalog->archived = FALSE;

      $catalog->locked = TRUE;
      if (property_exists($item, 'locked') && !$item->locked) {
        $catalog->locked = FALSE;
      }

      $catalog->copying = FALSE;
      if (property_exists($item, 'copying') && $item->copying) {
        $catalog->copying = TRUE;
      }

      $catalog->published = TRUE;

      $catalog->import_locked = FALSE;
      if (property_exists($item, 'import_locked') && $item->import_locked) {
        $catalog->import_locked = TRUE;
      }

      if (property_exists($item->{'catalog-type'}, 'id') && is_numeric($item->{'catalog-type'}->id) && !empty($item->{'catalog-type'}->name) && is_string($item->{'catalog-type'}->name)) {
        $catalog->catalog_type_id = (int) $item->{'catalog-type'}->id;
        $catalog->catalog_type_name = $item->{'catalog-type'}->name;

        if (property_exists($item->{'catalog-type'}, 'organization-id') && is_numeric($item->{'catalog-type'}->{'organization-id'})) {
          $catalog->catalog_type_organization_id = (int) $item->{'catalog-type'}->{'organization-id'};
        }

        if (!empty($item->{'catalog-type'}->created) && is_string($item->{'catalog-type'}->created)) {
          $sanitized_date = strtotime($item->{'catalog-type'}->created);
          $catalog->catalog_type_created = date('r', $sanitized_date);
          unset($sanitized_date);
        }

        if (!empty($item->{'catalog-type'}->modified) && is_string($item->{'catalog-type'}->modified)) {
          $sanitized_date = strtotime($item->{'catalog-type'}->modified);
          $catalog->catalog_type_modified = date('r', $sanitized_date);
          unset($sanitized_date);
        }

        if (property_exists($item->{'catalog-type'}, 'legacy-id') && is_numeric($item->{'catalog-type'}->{'legacy-id'})) {
          $catalog->catalog_type_legacy_id = (int) $item->{'catalog-type'}->{'legacy-id'};
        }

        if (property_exists($item, 'legacy-id') && is_numeric($item->{'legacy-id'}))  {
          $catalog->legacy_id = (int) $item->{'legacy-id'};
        }
      }
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

    // only a single catalog is used.
    if (!isset($this->processed[0]) || !is_object($this->processed[0])) {
      return FALSE;
    }

    if ($this->list_as == self::AS_LINK) {
      $this->markup = '<a href="" class="' . $this->get_class_base_name() . '-title list-as_link">' . check_plain($this->processed[0]->name);

      if ($this->use_description) {
        $this->markup .= '<div class="' . $this->get_class_base_name() . '-description">' . check_markup($item->processed[0]->description, 'full_html') . '</div>';
        $this->markup .= '</div>';
      }

      $this->markup .= '</a>';
    }
    elseif ($this->list_as == self::AS_HEADER) {
      $this->markup = '<h3 class="' . $this->get_class_base_name() . '-title list-as_header">' . check_plain($this->processed[0]->name);

      if ($this->use_description) {
        $this->markup .= '<div class="' . $this->get_class_base_name() . '-description">' . check_markup($item->processed[0]->description, 'full_html') . '</div>';
        $this->markup .= '</div>';
      }

      $this->markup .= '</h3>';
    }
    elseif ($this->list_as == self::AS_DIVIDER) {
      $this->markup = '<div class="' . $this->get_class_base_name() . '-title list-as_divider"' . check_plain($this->processed[0]->name);

      if ($this->use_description) {
        $this->markup .= '<div class="' . $this->get_class_base_name() . '-description">' . check_markup($item->processed[0]->description, 'full_html') . '</div>';
        $this->markup .= '</div>';
      }

      $this->markup .= '</div>';
    }
    elseif ($this->list_as == self::AS_SPAN) {
      $this->markup = '<span class="' . $this->get_class_base_name() . '-title list-as_span">' . check_plain($this->processed[0]->name);

      if ($this->use_description) {
        $this->markup .= '<div class="' . $this->get_class_base_name() . '-description">' . check_markup($item->processed[0]->description, 'full_html') . '</div>';
        $this->markup .= '</div>';
      }

      $this->markup .= '</div>';
    }
    elseif ($this->list_as == self::AS_LIST) {
      $this->markup = '<li class="' . $this->get_class_base_name() . '-title list-as_list">' . check_plain($this->processed[0]->name);

      if ($this->use_description) {
        $this->markup .= '<div class="' . $this->get_class_base_name() . '-description">' . check_markup($item->processed[0]->description, 'full_html') . '</div>';
      }

      $this->markup .= '</li>';
    }

    // only shows a single entry, so a pager is not needed here.
    #$this->markup .= $this->build_pager($page, $per_page);

    return TRUE;
  }
}

/**
 * @} End of '@addtogroup mcneese_catalog'.
 */