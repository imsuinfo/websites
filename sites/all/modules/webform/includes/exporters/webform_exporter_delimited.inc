<?php

/**
 * @file
 * Webform exporter for creating CSV/TSV delimited files.
 */
class webform_exporter_delimited extends webform_exporter {
  public $line_ending;
  public $delimiter;
  public $encoding;

  function __construct($options) {
    $this->line_ending = webform_variable_get('webform_csv_line_ending');
    $this->delimiter = isset($options['delimiter']) ? $options['delimiter'] : ',';
    $this->encoding = isset($options['encoding']) ? $options['encoding'] : variable_get('webform_export_encoding', 'UTF-16LE');
    // Convert tabs.
    if ($this->delimiter == '\t') {
      $this->delimiter = "\t";
    }
    $options['delimiter'] = $this->delimiter;
    parent::__construct($options);
  }

  function bof(&$file_handle) {
    $output = '';

    // Include at BOM at the beginning of the file for Little Endian.
    // This makes tab-separated imports work correctly in MS Excel.
    if (function_exists('mb_convert_encoding') && $this->delimiter == "\t") {
      $output = chr(255) . chr(254);
      $encoding_current = mb_detect_encoding($output, array_keys(webform_encoding_list()));

      if ($this->encoding != $encoding_current) {
        $output = mb_convert_encoding($output, $this->encoding, $encoding_current);
      }
    }
    @fwrite($file_handle, $output);
  }

  function add_row(&$file_handle, $data, $row_count) {
    foreach ($data as $key => $value) {
      // Escape inner quotes and wrap all contents in new quotes.
      $data[$key] = '"' . str_replace('"', '""', $data[$key]) . '"';

      // Remove <script> tags, which mysteriously cause Excel not to import.
      $data[$key] = preg_replace('!<(/?script.*?)>!', '[$1]', $data[$key]);
    }
    $row = implode($this->delimiter, $data) . $this->line_ending;

    if (function_exists('mb_convert_encoding')) {
      $encoding_current = mb_detect_encoding($row, array_keys(webform_encoding_list()));
      if ($this->encoding != $encoding_current) {
        $row = mb_convert_encoding($row, $this->encoding, $encoding_current);
      }
    }

    @fwrite($file_handle, $row);
  }

  function set_headers($filename) {
    parent::set_headers($filename);

    // Convert tabs.
    if ($this->delimiter == "\t") {
      $extension = 'tsv';
      $content_type = 'text/tab-separated-values';
    }
    else {
      $extension = 'csv';
      $content_type = 'text/csv';
    }

    $content_type .= '; charset=' . drupal_strtolower($this->encoding);

    drupal_add_http_header('Content-Type', $content_type);
    drupal_add_http_header('Content-Disposition', "attachment; filename=$filename.$extension");
    drupal_send_headers();
  }
}
