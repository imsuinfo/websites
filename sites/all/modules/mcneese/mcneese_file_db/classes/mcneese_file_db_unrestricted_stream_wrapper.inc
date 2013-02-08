<?php

/**
 * @file
 * Provides the stream wrapper: dbu://
 *
 * This class will handle upload & download of unrestricted files.
 * The following paths are supports:
 * - upload: dbu://u/[random]/[filename]
 * - download: dbu://c/[short_checksum]/[filename (optional)]
 *
 * The external url for these files are:
 * - unique download path: http(s)://[website]/f/c/[short_checksum]/[filename]
 *
 * The sha1 checksums of the files can be downloaded via:
 * - unique download path: http(s)://[website]/f/s/[short_checksum]/[filename]
 */

/**
 * @addtogroup mcneese_file_db
 * @{
 */

class mcneese_file_db_unrestricted_stream_wrapper extends mcneese_file_db_stream_wrapper {
  const SCHEME = 'dbu';
  const CORE = 'public';


  /**
   * Implements rename().
   */
  public function rename($from_uri, $to_uri) {
    return FALSE;
  }

  /**
   * Implements setUri().
   */
  public function setUri($uri) {
    if (empty($uri)) {
      return FALSE;
    }

    $parsed = $this->parse_uri($uri);

    if ($parsed === FALSE) {
      return FALSE;
    }

    $new_uri = $this->build_uri();

    if (empty($new_uri)) {
      $this->uri = $uri;
    }
    else {
      $this->uri = $new_uri;
    }

    return TRUE;
  }

  /**
   * Implements getUri().
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Implements stream_close().
   */
  public function stream_close() {
    if (!is_null($this->stream_fhandle) && $this->stream_fid != 0) {
      $stat = fstat($this->stream_fhandle);
      fclose($this->stream_fhandle);

      $file_meta = $this->load_file_meta_by_fid($this->stream_fid, FALSE);

      // if the checksum does not exist, then the file has not yet been saved.
      if (empty($file_meta->checksum)) {
        $fields = array();
        $fields['size'] = $stat['size'];

        if (!empty($this->tmp_filename)) {
          $filename_parts = explode('.', $this->tmp_filename);

          if (count($filename_parts) > 1) {
            $fields['extension'] = array_pop($filename_parts);
          }
          else {
            $fields['extension'] = '';
          }

          $fields['filename'] = implode('.', $filename_parts);
        }

        include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
        $mapping = file_mimetype_mapping();

        $fields['mimetype'] = file_get_mimetype($this->stream_fname, $mapping);

        if (empty($fields['extension'])) {
          $mime_id = array_search($fields['mimetype'], $mapping['mimetypes']);

          if ($mime_id === FALSE) {
            $fields['extension'] = '';
          }
          else {
            $fields['extension'] = $mapping['extensions'][$mime_id];
          }
        }
        else if ($fields['mimetype'] == 'application/octet-stream') {
          // try to avoid using octet-strema and instead use the extension to derive the mime type.
          if (!empty($mapping['extensions'][$fields['extension']])) {
            $fields['mimetype'] = $mapping['mimetypes'][$mapping['extensions'][$fields['extension']]];
          }
        }

        $fields['checksum'] = sha1_file($this->stream_fname);
        $this->tmp_checksum = $fields['checksum'];

        // if the checksum already exists in the database, then delete the current newly created file.
        if ($this->checksum_exists($this->tmp_checksum)) {
          $transaction = db_transaction();

          try {
            $server_id = mcneese_file_db_get_server_id();
            $file_db = mcneese_file_db_prepare_connection();
            $options = mcneese_file_db_prepare_options();

            $query = $file_db->delete('mcneese_file_db_files', $options);
            $query->condition('id', $this->stream_fid);
            $query->execute();

            // if a unique_id was specified, then save the unique id, mapping to the existing file id.
            if (!empty($this->tmp_unique)) {
              $files = (array) mcneese_file_db_load_available_files(array('checksum' => $this->tmp_checksum));

              if (!empty($files)) {
                $tmp_meta = array_pop($files);
                unset($files);

                $query = $file_db->select('mcneese_file_db_associations', 'mfda', $options);
                $query->fields('mfda', array('id'));
                $query->condition('mfda.file_id', $tmp_meta->id);
                $query->condition('mfda.server_id', $server_id);

                $association_id = $query->execute()->fetchField();

                $query = $file_db->insert('mcneese_file_db_uniques', $options);
                $fields = array('unique_id' => $this->tmp_unique, 'association_id' => $association_id);
                $query->fields($fields);

                $write_result = $query->execute();
              }
            }
          }
          catch (Exception $e) {
            $transaction->rollback();
            cf_error::on_query_execution($e);

            throw $e;
          }

          // flush the transaction
          unset($transaction);
        }
        else {
          $transaction = db_transaction();

          try {
            $server_id = mcneese_file_db_get_server_id();
            $file_db = mcneese_file_db_prepare_connection();
            $options = mcneese_file_db_prepare_options();

            $query = $file_db->update('mcneese_file_db_files', $options);
            $query->fields($fields);
            $query->condition('id', $this->stream_fid);

            $write_result = $query->execute();

            $query = $file_db->insert('mcneese_file_db_associations', $options);
            $fields = array('file_id' => $this->stream_fid, 'server_id' => $server_id);
            $query->fields($fields);

            $write_result = $query->execute();

            $query = $file_db->insert('mcneese_file_db_uniques', $options);
            $fields = array('unique_id' => $this->tmp_unique, 'association_id' => $write_result);
            $query->fields($fields);

            $write_result = $query->execute();

            $this->load_user();

            $query = $file_db->insert('mcneese_file_db_users', $options);
            $fields = array('file_id' => $this->stream_fid, 'user_id' => $this->user, 'server_id' => $server_id);
            $query->fields($fields);

            $write_result = $query->execute();

            // @todo: add group relations
          }
          catch (Exception $e) {
            $transaction->rollback();
            cf_error::on_query_execution($e);

            throw $e;
          }

          // flush the transaction
          unset($transaction);
        }
      }

        // @todo: step 5: the database should perform integrity checks for file data that has been on the system for, say at least 24 hours, then if the file is still NULL for things like checksum, then

      unset($this->stream_fhandle);
      unlink($this->stream_fname);

      $this->stream_fhandle = null;
      $this->stream_fname = null;

      $uri = $this->build_uri();

      if (!empty($uri)) {
        $this->uri = $uri;
      }
    }

    unset($this->stream);

    $this->stream = null;
    $this->stream_blocks = 0;
    $this->stream_position = 0;
    $this->stream_last_read = 0;
    $this->stream_fid = 0;
    $this->user = 0;

    return TRUE;
  }

  /**
   * Implements stream_open().
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    // prevent opening of a new stream until the existing one is closed
    // @todo: alternatively, should I close an open stream instead of returning FALSE?
    if (!empty($this->stream)) {
      return FALSE;
    }

    if (empty($this->uri)) {
      $this->setUri($uri);
    }

    if (empty($this->file_meta)) {
      if (!empty($this->tmp_checksum)) {
        $this->load_file_meta($this->tmp_checksum);
      }
      else if (!empty($this->tmp_unique)) {
        $this->load_file_meta_by_unique($this->tmp_unique);

        // newly upload files do not have to exist and directly opened via stream_open().
        if (empty($this->file_meta)) {
          return TRUE;
        }
      }
    }

    if (empty($this->file_meta)) {
      return TRUE;
    }
    else {
      try {
        $server_id = mcneese_file_db_get_server_id();

        $query = mcneese_file_db_build_data_query($server_id);
        $query->condition('mfda.file_id', $this->file_meta->id);
        $this->stream = $query->execute();

        if ($this->stream === FALSE) {
          $this->stream_close(); // @todo: is this necessary?
          return FALSE;
        }

        $this->stream_fid = $this->file_meta->id;
        $this->stream_position = 0;
        $this->stream_last_read = 0;
        $this->stream_blocks = $this->stream->rowCount();
      }
      catch (Exception $e) {
        cf_error::on_query_execution($e);

        throw $e;
      }
    }

    return TRUE;
  }

  /**
   * Implements stream_read().
   */
  public function stream_read($bytes) {
    if (empty($this->stream) || empty($this->stream_fid)) {
      return FALSE;
    }

    $output = '';

    if ($bytes == 0) {
      return $output;
    }

    // determine the number of blocks to read.
    $offset = $bytes + $this->stream_last_read;
    $blocks = intval($offset / MCNEESE_FILE_DB_BLOCK_SIZE);
    $underflow = $offset - ($blocks * MCNEESE_FILE_DB_BLOCK_SIZE);

    $stream_blocks = $this->stream_position + $blocks;

    if ($underflow > 0) {
      $stream_blocks++;
    }

    if ($stream_blocks > $this->stream_blocks) {
      $stream_blocks = $this->stream_blocks;
      $underflow = 0;
    }

    $server_id = mcneese_file_db_get_server_id();
    $file_db = mcneese_file_db_prepare_connection();
    $options = mcneese_file_db_prepare_options();

    while ($this->stream_position < $stream_blocks) {
      try {
        $query = $file_db->select('mcneese_file_db_file_data', 'mfdfd', $options);
        $query->fields('mfdfd', array('data'));
        $query->condition('mfdfd.file_id', $this->stream_fid);
        $query->condition('mfdfd.block', $this->stream_position);

        $results = $query->execute()->fetchField();
        $raw_bytes = mcneese_file_db_bytea_to_bin($results);

        if ($underflow > 0 && $this->stream_position + 1 == $stream_blocks) {
          $substring = substr($raw_bytes, $this->stream_last_read, $underflow);
          $bytes_read = strlen($substring);
        }
        else if ($this->stream_last_read > 0) {
          // Re-sync reads with the block size to improve performance
          $substring = substr($raw_bytes, $this->stream_last_read, MCNEESE_FILE_DB_BLOCK_SIZE - $this->stream_last_read);
          $bytes_read = strlen($substring);
        }
        else {
          $substring = $raw_bytes;
          $bytes_read = MCNEESE_FILE_DB_BLOCK_SIZE;
        }

        if ($bytes_read + $this->stream_last_read == MCNEESE_FILE_DB_BLOCK_SIZE) {
          $this->stream_last_read = 0;
        }
        else {
          $this->stream_last_read += $bytes_read;
        }

        $output .= $substring;
      }
      catch (Exception $e) {
        cf_error::on_query_execution($e);

        throw $e;
      }

      $this->stream_position++;
    }

    return $output;
  }

  /**
   * Implements stream_stat().
   */
  public function stream_stat() {
    if (empty($this->file_meta)) {
      if (!empty($this->stream_fid)) {
        $this->load_file_meta_by_fid($this->stream_fid);
      }
      else if (!empty($this->tmp_checksum)) {
        $this->load_file_meta($this->tmp_checksum);
      }
      else if (!empty($this->tmp_unique)) {
        $this->load_file_meta_by_unique($this->tmp_unique);
      }

      if (is_null($this->file_meta)) {
        return FALSE;
      }
    }

    $timestamp = strtotime($this->file_meta->timestamp);

    $stat = array();
    $stat['ino'] = 0;
    $stat['ino'] = 0;
    $stat['nlink'] = 1;
    $stat['uid'] = 0; // @todo: should this be made to match the drupal uid?
    $stat['gid'] = 0;
    $stat['rdev'] = 0;
    $stat['size'] = $this->file_meta->size;
    $stat['atime'] = $timestamp;
    $stat['mtime'] = $timestamp;
    $stat['ctime'] = $timestamp;
    $stat['blksize'] = MCNEESE_FILE_DB_BLOCK_SIZE;
    $stat['blocks'] = $this->stream_blocks;

    return $stat;
  }

  /**
   * Implements stream_write().
   */
  public function stream_write($data) {
    // @todo: try to see if the file is already in the database.
    // if file_meta is already populated, then it in fact is in the database.
    if (!empty($this->file_meta)) {
      if (!empty($this->file_meta->checksum)) {
        // @todo: finish this, I should be performing a check to see if the specified checksum actually exists.
        return strlen($data);
      }
    }

    // initialize the raw file so that it can be stat'd on stream_close().
    if (is_null($this->stream_fhandle) || empty($this->stream_fid)) {
      $this->stream_fname = drupal_tempnam('temporary://', MCNEESE_FILE_DB_KEY);
      $this->stream_position = 0;
      $this->stream_fhandle = fopen($this->stream_fname, 'w+b');
    }

    $block_length = fwrite($this->stream_fhandle, $data, MCNEESE_FILE_DB_BLOCK_SIZE);
    $block = substr($data, 0, $block_length);

    try {
      $server_id = mcneese_file_db_get_server_id();
      $file_db = mcneese_file_db_prepare_connection();
      $options = mcneese_file_db_prepare_options();

      if (empty($this->stream_fid)) {
        try {
          $query = $file_db->insert('mcneese_file_db_files', $options);
          $query->useDefaults(array('timestamp')); // @todo: use php timestamp or leave it alone to postgresql?
          $this->stream_fid = $query->execute();
        }
        catch (Exception $e) {
          cf_error::on_query_execution($e);

          throw $e;
        }
      }

      $fields = array('file_id' => $this->stream_fid, 'block' => $this->stream_position, 'data' => $block); // @todo how does one call encode() and respectively decode() with drupal and is this necessary?

      $query = $file_db->insert('mcneese_file_db_file_data', $options);
      $query->fields(array_keys($fields));
      $query->values($fields);
      $write_result = $query->execute();

      $this->stream_position++;

      return $block_length;
    }
    catch (Exception $e) {
      cf_error::on_query_execution($e);

      throw $e;
    }
  }

  /**
   * Implements unlink().
   */
  public function unlink($uri) {
    return FALSE;
  }

  /**
   * Implements url_stat().
   */
  public function url_stat($uri, $flags) {
    if (empty($this->uri)) {
      $this->setUri($uri);
    }

    return $this->stream_stat();
  }

  /**
   * Implements get_mimetype().
   */
  public function get_mimetype() {
    if (empty($this->file_meta)) {
      return 'application/octet-stream';
    }

    if (empty($this->file_meta->mimetype)) {
      return 'application/octet-stream';
    }

    return $this->file_meta->mimetype;
  }

  /**
   * Implements getMimeType().
   */
  public static function getMimeType($uri, $mapping = NULL) {
    // Drupal made getMimeType() static, so getting the mimetype requires creating another object and then calling a custom get_mimetype() function.
    $wrapper = new mcneese_file_db_unrestricted_stream_wrapper();
    $wrapper->setUri($uri);
    return $wrapper->get_mimetype();
  }

  /**
   * Implements get_scheme().
   */
  protected function get_scheme() {
    return mcneese_file_db_unrestricted_stream_wrapper::SCHEME;
  }

  /**
   * Implements get_external().
   */
  protected function get_external() {
    return MCNEESE_FILE_DB_FILE_PATH;
  }

  /**
   * Implements get_upload().
   */
  protected function get_upload() {
    return MCNEESE_FILE_DB_UPLOAD;
  }

  /**
   * Implements get_path_by_hash().
   */
  protected function get_path_by_hash() {
    return MCNEESE_FILE_DB_PATH_BY_HASH;
  }

  /**
   * Implements get_core().
   */
  protected function get_core() {
    return mcneese_file_db_unrestricted_stream_wrapper::CORE;
  }
}

/**
 * @} End of '@addtogroup mcneese_file_db'.
 */
