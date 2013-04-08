<?php

/**
 * @file
 * Provides base stream wrapper class for mcneese_file_db.
 *
 * Requires file_db_local_name variable to be defined in settings.php.
 * Requires file_db to be defined in $databases array of settings.php.
 */

/**
 * @addtogroup mcneese_file_db
 * @{
 */

abstract class mcneese_file_db_stream_wrapper implements DrupalStreamWrapperInterface {
  protected $file_meta = null;
  protected $user = 0;
  protected $uri = null;

  protected $tmp_filename = null;
  protected $tmp_checksum = null;

  protected $stream = null;
  protected $stream_blocks = 0;
  protected $stream_position = 0;
  protected $stream_last_read = 0;
  protected $stream_fhandle = null;
  protected $stream_fname = null;
  protected $stream_fid = 0;

  protected $image_style = null;
  protected $image_style_uri = null;


  /**
   * Implements getExternalUrl().
   *
   * Return the HTML URI of a dynamic file.
   */
  public function getExternalUrl() {
    $built_uri = $this->build_uri();

    if ($built_uri === FALSE) {
      return FALSE;
    }

    $parts = explode('://', $built_uri, 2);

    if (empty($parts[1])) {
      return FALSE;
    }

    global $base_path;

    return $base_path . $this->get_external() . '/' . $parts[1];
  }

  /**
   * Implements getLocalPath().
   *
   * Return the HTML URI of a dynamic file.
   */
  public function getLocalPath($uri = NULL) {
    if (!is_null($uri)) {
      $parsed = $this->parse_uri($uri);

      if ($parsed === FALSE) {
        return FALSE;
      }
    }

    $built_uri = $this->build_uri();

    if ($built_uri === FALSE) {
      return FALSE;
    }

    $parts = explode('://', $built_uri, 2);

    if (empty($parts[1])) {
      return FALSE;
    }

    return $this->get_external() . '/' . $parts[1];
  }

  /**
   * Implements chmod().
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Implements getTarget().
   */
  public function getTarget($uri = NULL) {
    if (!is_null($uri)) {
      $parsed = $this->parse_uri($uri);

      if ($parsed === FALSE) return FALSE;
    }

    $built_uri = $this->build_uri();

    if (empty($this->image_style) || empty($this->image_style_uri)) {
      if ($built_uri === FALSE) {
        return FALSE;
      }

      $parts = explode('://', $built_uri, 2);

      if (empty($parts[1])) {
        return FALSE;
      }

      return $this->get_external() . '/' . $parts[1];
    }
    else {
      return $this->image_style_uri;
    }

    return FALSE;
  }

  /**
   * Implements realpath().
   */
  public function realpath() {
    $built_uri = $this->build_uri();

    if ($built_uri === FALSE) {
      return FALSE;
    }

    $parts = explode('://', $built_uri, 2);

    if (empty($parts[1])) {
      return FALSE;
    }

    return url($this->get_external() . '/' . $parts[1], array('absolute' => TRUE));
  }

  /**
   * Implements stream_lock().
   */
  public function stream_lock($operation) {
    return FALSE;
  }

  /**
   * Implements stream_eof().
   */
  public function stream_eof() {
    if (empty($this->stream) || empty($this->stream_fid)) {
      return FALSE;
    }

    if ($this->stream_position < $this->stream_blocks) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Implements stream_seek().
   */
  public function stream_seek($offset, $whence) {
    return FALSE;
  }

  /**
   * Implements stream_flush().
   */
  public function stream_flush() {
    return FALSE;
  }

  /**
   * Implements stream_tell().
   */
  public function stream_tell() {
    return FALSE;
  }

  /**
   * Implements mkdir().
   */
  public function mkdir($uri, $mode, $options) {
    return TRUE;
  }

  /**
   * Implements rmdir().
   */
  public function rmdir($uri, $options) {
    return TRUE;
  }

  /**
   * Implements dirname().
   */
  public function dirname($uri = NULL) {
    if (!is_null($uri)) {
      $parsed = $this->parse_uri($uri);

      if ($parsed === FALSE) return '';
    }

    $directory = $this->build_uri(NULL, FALSE);

    return $directory;
  }

  /**
   * Implements dir_opendir().
   */
  public function dir_opendir($uri, $options) {
    return FALSE;
  }

  /**
   * Implements dir_readdir().
   */
  public function dir_readdir() {
    return FALSE;
  }

  /**
   * Implements dir_rewinddir().
   */
  public function dir_rewinddir() {
    return FALSE;
  }

  /**
   * Implements dir_closedir().
   */
  public function dir_closedir() {
    return FALSE;
  }

  /**
   * Implements getDirectoryPath().
   */
  public function getDirectoryPath() {
    return '';
  }

  /**
   * Wrapper function to load the current scheme string.
   *
   * @return string
   *   The scheme string.
   */
  abstract protected function get_scheme();

  /**
   * Wrapper function to load the current external string.
   *
   * @return string
   *   The external string.
   */
  abstract protected function get_external();

  /**
   * Wrapper function to load the current path by hash string.
   *
   * @return string
   *   The path by hash string.
   */
  abstract protected function get_path_by_hash();

  /**
   * Wrapper function to load the associated drupal core scheme.
   *
   * @return string
   *   The drupal core scheme, such as 'public' or 'private'.
   */
  abstract protected function get_core();

  /**
   * Loads the file meta if a valid checksum is supplied.
   *
   * @param string $checksum
   *   use the specified checksum to load the file meta.
   * @param bool $assign
   *   (optional) When TRUE, save the file meta object in $this->file_meta.
   *
   * @return object|FALSE
   *   A file_meta object as returned by the database.
   */
  protected function load_file_meta($checksum, $assign = TRUE) {
    $file_meta = FALSE;

    if (empty($checksum)) {
      return $file_meta;
    }

    // use existing data if possible
    if (!empty($this->file_meta) && $this->file_meta->checksum == $checksum) {
      return $this->file_meta;
    }

    $files = (array) mcneese_file_db_load_available_files(array('checksum' => $checksum));

    if (!empty($files)) {
      $file_meta = array_pop($files);
    }

    if ($assign) {
      unset($this->file_meta);
      $this->file_meta = null;

      if ($file_meta !== FALSE) {
        $this->file_meta = $file_meta;
      }
    }

    return $file_meta;
  }

  /**
   * Loads the file meta if a valid checksum is supplied.
   *
   * @param string $fid
   *   use the specified file id to load the file meta.
   * @param bool $assign
   *   (optional) When TRUE, save the file meta object in $this->file_meta.
   *
   * @return object|FALSE
   *   A file_meta object as returned by the database.
   */
  protected function load_file_meta_by_fid($fid, $assign = TRUE) {
    $file_meta = FALSE;

    if (empty($fid)) {
      return $file_meta;
    }

    // use existing data if possible
    if (!empty($this->file_meta) && $this->file_meta->id == $fid) {
      return $this->file_meta;
    }

    $files = (array) mcneese_file_db_load_available_files(array('id' => $fid));

    if (!empty($files)) {
      $file_meta = array_pop($files);
    }

    if ($assign) {
      unset($this->file_meta);
      $this->file_meta = null;

      if ($file_meta !== FALSE) {
        $this->file_meta = $file_meta;
      }
    }

    return $file_meta;
  }

  /**
   * Checks to see if a file by the given checksum exists.
   *
   * @param string $checksum
   *   Checksum to validate against
   *
   * @param bool $associated
   *   (optional) If TRUE, existance check is against associated files only.
   *   If FALSE, existence check is against all files in the database.
   *
   * @return bool
   *   TRUE if file exists, FALSE otherwise.
   */
  protected function checksum_exists($checksum, $associated = TRUE) {
    try {
      $file_db = mcneese_file_db_prepare_connection();
      $options = mcneese_file_db_prepare_options();

      $built = mcneese_file_db_build_query_by_checksum($checksum, $associated);
      $results = $file_db->queryRange($built['query'], 0, 1, $built['arguments'], $options);

      return $results->rowCount() != 0;
    }
    catch (Exception $e) {
      cf_error::on_query_execution($e);

      throw $e;
    }

    return FALSE;
  }

  /**
   * Returns the file id for a given checksum.
   *
   * @param string $checksum
   *   Checksum to validate against
   *
   * @param bool $associated
   *   (optional) If TRUE, existance check is against associated files only.
   *   If FALSE, existence check is against all files in the database.
   *
   * @return int|bool
   *   File ID if it exists, FALSE otherwise.
   */
  protected function get_checksum_file_id($checksum, $associated = TRUE) {
    try {
      $file_db = mcneese_file_db_prepare_connection();
      $options = mcneese_file_db_prepare_options();

      $built = mcneese_file_db_build_query_by_checksum($checksum, $associated);
      $results = $file_db->queryRange($built['query'], 0, 1, $built['arguments'], $options);

      $row = (array) $results->fetchAll();

      if (empty($row)) {
        return FALSE;
      }

      return $row[0]->id;
    }
    catch (Exception $e) {
      cf_error::on_query_execution($e);

      throw $e;
    }

    return FALSE;
  }

  /**
   * Loads the current user id if one is not defined.
   */
  protected function load_user() {
    if ($this->user > 0) {
      return;
    }

    $user = cf_current_user();
    $this->user = $user->uid;
  }

  /***
   * Process a given uri, loading & storing its information.
   *
   * @param string $uri
   *   The uri string to process.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  protected function parse_uri($uri) {
    $this->file_meta = null;
    $this->tmp_checksum = null;
    $this->tmp_filename = null;
    $this->image_style = null;
    $this->image_style_uri = null;

    $outer_parts = explode('://', $uri, 2);

    if ($outer_parts[0] != $this->get_scheme()) {
      return FALSE;
    }

    if (count($outer_parts) > 1) {
      // first eliminate all repeated '/', such as 'dbu://a/b/////c/d' which should be read as 'dbu://a/b/c/d'.
      $fixed_uri = preg_replace('@/+@i', '/', $outer_parts[1]);

      $parts = explode('/', $fixed_uri);

      if (count($parts) < 2) {
        return FALSE;
      }

      // A uri may also contain a link to a style path, such as dbu://styles/square_thumbnail/dbu/f/c/01234567/gksu.png .
      if ($parts[0] == MCNEESE_FILE_DB_STYLES) {
        $this->image_style = $parts[1];
        $this->image_style_uri = null;

        if ($parts[2] != $this->get_scheme()) {
          return FALSE;
        }

        if ($parts[3] == $this->get_external()) {
          $parts = array_slice($parts, 4);
        }
        else if ($parts[3] == $this->get_path_by_hash()) {
          $parts = array_slice($parts, 3);
        }
        else {
          return FALSE;
        }
      }

      // allow being passed a path beginning with external path such as: /MCNEESE_FILE_DB_FILE_PATH .
      // doing this allows for modules to find the scheme using only the final url.
      if (empty($parts[0]) && $parts[1] == $this->get_external()) {
        array_shift($parts);
        array_shift($parts);

        if (count($parts) < 2) {
          return FALSE;
        }
      }

      if ($parts[0] == $this->get_path_by_hash()) {
        $this->file_meta = $this->load_file_meta($parts[1]);

        if ($this->file_meta === FALSE) {
          $this->file_meta = null;
          $thos->tmp_checksum = $parts[1];

          if (!empty($parts[2])) {
            $this->tmp_filename = $parts[2];
          }

          return TRUE;
        }

        $this->tmp_checksum = $this->file_meta->checksum;

        // look for the custom filename that overrides the one saved in the database if provided.
        if (empty($parts[2])) {
          $this->tmp_filename = $this->file_meta->filename;

          if (!empty($this->file_meta->extension)) {
            $this->tmp_filename .= '.' . $this->file_meta->extension;
          }
        }
        else {
          $this->tmp_filename = $parts[2];
        }

        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Uses the file data to build the appropriate uri.
   *
   * @param string $method
   *   Designates the method followed when building the uri.
   * @param bool $include_filename
   *   Designates to include the filename string as part of the path.
   *   This string is not necessary to return the file, but it is 'pretty'.
   *
   * @return string
   *   The built uri.
   */
  protected function build_uri($method = NULL, $include_filename = TRUE) {

    // Give $method a default argument that is determed by a function call.
    if (is_null($method)) {
      $method = $this->get_path_by_hash();
    }

    if (empty($this->file_meta)) {
      if (!empty($this->stream_fid)) {
        $this->load_file_meta_by_fid($this->stream_fid);
      }
      else if (!empty($this->tmp_checksum)) {
        $this->load_file_meta($this->tmp_checksum);
      }
    }

    if (!empty($this->file_meta)) {
      if ($method == $this->get_path_by_hash()) {
        $checksum = substr($this->file_meta->checksum, 0, MCNEESE_FILE_DB_SHORT_SUM_SIZE);

        $new_uri = $this->get_scheme() . '://';
        $new_uri .= $this->get_path_by_hash() . '/';
        $new_uri .= $checksum;
        $filename = '';

        if (!empty($this->image_style)) {
          $image_source_uri = $this->get_scheme() . '://';
          $image_source_uri .= $this->get_path_by_hash() . '/';
          $image_source_uri .= $checksum;

          $this->image_style_uri = $this->get_core() . '://styles/';
          $this->image_style_uri .= $this->image_style . '/';
          $this->image_style_uri .= $this->get_scheme() . '/';
          $this->image_style_uri .= $this->get_external() . '/';
          $this->image_style_uri .= $this->get_path_by_hash() . '/';
          $this->image_style_uri .= $checksum;
        }

        if (empty($this->tmp_filename)) {
          if (!empty($this->file_meta->filename)) {
            $filename .= '/' . $this->file_meta->filename;

            if (!empty($this->file_meta->extension)) {
              $filename .= '.' . $this->file_meta->extension;
            }
          }
        }
        else {
          $filename .= '/' . $this->tmp_filename;
        }

        if ($include_filename) {
          $new_uri .= $filename;
        }

        if (!empty($this->image_style_uri)) {
          $this->image_style_uri .= $filename;

          if (!file_exists($this->image_style_uri)) {
            $this->create_image_style($image_source_uri);
          }
        }

        return $new_uri;
      }
    }

    return '';
  }

  /***
   * Creates the image style for images.
   *
   * @param string $image_source_uri
   *   Database file path to image file.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  protected function create_image_style($image_source_uri) {
    if (empty($this->image_style) || empty($this->image_style_uri) || empty($this->file_meta) || empty($image_source_uri)) {
      return FALSE;
    }

    // custom variant of image_style_create_derivative() to properly create the image style.
    $dirname = file_stream_wrapper_get_instance_by_scheme($this->get_core())->dirname($this->image_style_uri);

    if (!file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      watchdog('image', "Failed to create style directory: %directory", array('%directory' => $dirname), WATCHDOG_ERROR);
      return FALSE;
    }

    if (!$image = image_load($image_source_uri)) {
      return FALSE;
    }

    $style = image_style_load($this->image_style);

    foreach ($style['effects'] as $effect) {
      image_effect_apply($image, $effect);
    }

    if (!image_save($image, $this->image_style_uri)) {
      if (file_exists($this->image_style_uri)) {
        watchdog('image', "Cached image file %destination already exists. There may be an issue with your rewrite configuration.", array('%destination' => $this->image_style_uri), WATCHDOG_ERROR);
      }

      return FALSE;
    }

    return TRUE;
  }
}

/**
 * @} End of '@addtogroup mcneese_file_db'.
 */
