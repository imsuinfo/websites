<?php

/**
 * Plot class object for creating phplots.
 *
 * @todo review how function_history is handled here.
 */
class phplot_api_plot {
  protected $type = NULL;
  protected $data = array();
  protected $settings = array();
  protected $caller_history = array();
  protected $image = NULL;
  protected $file = NULL;


  /**
   * Class Initailizer.
   *
   * @param type
   *   A machine name string of the plot type.
   *
   * @param function_history (optional)
   *   An array of function names, ie: array('0' => 'my_function_name').
   */
  public function phplot_api_plot($type, array $function_history = array()){
    $this->caller_history = $function_history;

    $function_history = $this->history(__FUNCTION__);

    $this->set_type($type);

    foreach ($this->settings['defaults'] as $feature => $value){
      $plot_data[$feature] = $value;
    }
  }

  /**
   * Returns the type of this plot.
   *
   * @return
   *   A machine name string of the plot type.
   */
  public function get_type(){
    return $this->type;
  }

  /**
   * Returns the data assigned to this plot.
   *
   * @param feature (optional)
   *   A machine name string of a specific setting to get.
   *   If NULL, then the entire plot data array is returned.
   *
   * @return
   *   Either the entire plot data array or a specific value for a specified feature.
   */
  public function get_data(string $feature = NULL){
    if (is_null($feature)){
      return $this->data;
    }

    // @todo throw an error if 'feature' does not exist in the array.
    return $this->data[$feature];
  }

  /**
   * Returns the image of this plot.
   *
   * @return
   *   A binary image blob.
   */
  public function get_image(){
    return $this->image;
  }

  /**
   * Returns the image file of this plot.
   *
   * @return
   *   A file object that represents the plot image.
   */
  public function get_file(){
    return $this->file;
  }

  /**
   * Sets the type of this plot.
   *
   * @param type
   *   A machine name string of the plot type.
   */
  public function set_type($type){
    $function_history = $this->history(__FUNCTION__);

    $this->settings = phplot_api_get_plots($type, $function_history);
    $this->type = $type;
  }

  /**
   * Assign some value to a given feature.
   *
   * @param feature
   *   The machine name of the feature.
   * @param value
   *   Data to assign to the specified feature.
   * @param reset (optional)
   *   Whether or not the data should be cleared before assigning a value.
   *   This only does something if the feature supports PHPLOT_API_FEATURE_MULTIPLE.
   *
   * @return
   *   TRUE is returned on success and FALSE on any failure.
   */
  public function set_data($feature, $value, $reset = FALSE){
    $function_history = $this->history(__FUNCTION__);

    if (!cf_has_array_key($feature, $this->settings['features'], $function_history)){
      // @todo watchdog some error here?
      return FALSE;
    }

    // @todo perform data integrity checks here before assigning the value
    if ($this->settings['features'][$feature] == PHPLOT_API_FEATURE_NONE || $this->settings['features'][$feature] == PHPLOT_API_FEATURE_SINGLE){
      $this->data[$feature] = $value;
    }
    else if ($this->settings['features'][$feature] == PHPLOT_API_FEATURE_MULTIPLE){
      if ($reset || !isset($this->data[$feature])){
        $this->data[$feature] = array();
      }

      array_push($this->data[$feature], $value);
    }

    return TRUE;
  }

  /**
   * Assigns the object callers history.
   *
   * @param function_history
   *   An array of function names, ie: array('0' => 'my_function_name').
   */
  public function set_history(array $function_history){
    $caller_history = $function_history;
  }

  /**
   * Builds the plot image file.
   */
  public function build(){
    $function_history = $this->history(__FUNCTION__);

    phplot_api_include_library($function_history);

    if (isset($this->data['width'])){
      $width = $this->data['width'];

      if (!is_numeric($width) || $width <= 0){
        $width = NULL;
      }
    }
    else {
      $width = NULL;
    }

    if (isset($this->data['height'])){
      $height = $this->data['height'];

      if (!is_numeric($height) || $height <= 0){
        $height = NULL;
      }
    }
    else {
      $height = NULL;
    }

    // Create a PHPlot object with the given dimensions
    if (is_null($width) || is_null($height)){
      $plot = new PHPlot();
    }
    else {
      $plot = new PHPlot($width, $height);
    }

    if (isset($this->data['data_type'])){
      $plot->SetDataType($this->data['data_type']);
    }

    if (isset($this->data['data_colors'])){
      $item = $this->data['data_colors'];

      switch(count($item)){
        case 3:
          $plot->SetDataColors($item[0], $item[1], $item[2]);
          break;
        case 2:
          $plot->SetDataColors($item[0], $item[1]);
          break;
        case 1:
          $plot->SetDataColors($item[0]);
          break;
      }
    }

    if (isset($this->data['data_border_colors'])){
      $plot->SetDataBorderColors($this->data['data_border_colors']);
    }

    if (isset($this->data['data_values'])){
      $plot->SetDataValues($this->data['data_values']);
    }

    if (isset($this->data['data_label_angle_x'])){
      $plot->SetXDataLabelAngle($this->data['data_label_angle_x']);
    }

    if (isset($this->data['data_label_position_x'])){
      $plot->SetXDataLabelPos($this->data['data_label_position_x']);
    }

    if (isset($this->data['data_label_type_x'])){
      $plot->SetXDataLabelType($this->data['data_label_type_x']);
    }

    if (isset($this->data['data_label_angle_y'])){
      $plot->SetXDataLabelAngle($this->data['data_label_angle_y']);
    }

    if (isset($this->data['data_label_position_y'])){
      $plot->SetXDataLabelPos($this->data['data_label_position_y']);
    }

    if (isset($this->data['data_label_type_y'])){
      $plot->SetXDataLabelType($this->data['data_label_type_y']);
    }

    if (isset($this->data['title'])){
      $plot->SetTitle($this->data['title']);
    }

    if (isset($this->data['title_color'])){
      $plot->SetTitleColor($this->data['title_color']);
    }

    if (isset($this->data['title_font_size'])){
      $plot->SetTitleFontSize($this->data['title_font_size']);
    }

    if (isset($this->data['ttf_path'])){
      $plot->SetTTFPath($this->data['ttf_path']);
    }

    if (isset($this->data['ttf_default'])){
      $plot->SetDefaultTTFont($this->data['ttf_default']);
    }

    if (isset($this->data['ttf_font'])){
      foreach ($this->data['ttf_font'] as $item){
        switch(count($item)){
          case 4:
            $plot->SetFontTTF($item[0], $item[1], $item[2], $item[3]);
            break;
          case 3:
            $plot->SetFontTTF($item[0], $item[1], $item[2]);
            break;
          case 2:
            $plot->SetFontTTF($item[0], $item[1]);
            break;
        }
      }
    }

    if (isset($this->data['background_color'])){
      $plot->SetBackgroundColor($this->data['background_color']);
    }

    if (isset($this->data['plot_area_background_draw'])){
      $plot->SetDrawPlotAreaBackground($this->data['plot_area_background_draw']);
    }

    if (isset($this->data['plot_background_color'])){
      $plot->SetPlotBgColor($this->data['plot_background_color']);
    }

    if (isset($this->data['precision_x'])){
      $plot->SetPrecisionX($this->data['precision_x']);
    }

    if (isset($this->data['precision_y'])){
      $plot->SetPrecisionY($this->data['precision_y']);
    }

    if (isset($this->data['shading'])){
      $plot->SetShading($this->data['shading']);
    }

    if (isset($this->data['label_color'])){
      $plot->SetLabelColor($this->data['label_color']);
    }

    if (isset($this->data['label_type'])){
      foreach ($this->data['label_type'] as $item){
        if (count($item) == 2){
          $plot->SetLabelType($item[0], $item[1]);
        }
      }
    }

    if (isset($this->data['label_scale_position'])){
      $plot->SetLabelScalePosition($this->data['label_scale_position']);
    }

    // @todo is this a special case where if plot_area_world is an empty array it should call: $plot->SetPlotAreaWorld(); ?
    if (isset($this->data['plot_area_world'])){
      $item = $this->data['plot_area_world'];

      switch(count($item)){
        case 4:
          $plot->SetPlotAreaWorld($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $plot->SetPlotAreaWorld($item[0], $item[1], $item[2]);
          break;
        case 2:
          $plot->SetPlotAreaWorld($item[0], $item[1]);
          break;
        case 1:
          $plot->SetPlotAreaWorld($item[0]);
          break;
      }
    }

    if (isset($this->data['plot_area_background_image'])){
      $plot_area_background_image = $this->data['plot_area_background_image'];

      if (count($plot_area_background_image) == 2){
        $plot->SetLabelType($plot_area_background_image[0], $plot_area_background_image[1]);
      }
    }

    // @todo is this a special case where if plot_area_pixels is an empty array it should call: $plot->SetPlotAreaPixels(); ?
    if (isset($this->data['plot_area_pixels'])){
      $item = $this->data['plot_area_pixels'];

      switch(count($item)){
        case 4:
          $plot->SetPlotAreaPixels($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $plot->SetPlotAreaPixels($item[0], $item[1], $item[2]);
          break;
        case 2:
          $plot->SetPlotAreaPixels($item[0], $item[1]);
          break;
        case 1:
          $plot->SetPlotAreaPixels($item[0]);
          break;
      }
    }

    if (isset($this->data['image_border_color'])){
      $plot->SetImageBorderColor($this->data['image_border_color']);
    }

    if (isset($this->data['image_border_type'])){
      $plot->SetImageBorderType($this->data['image_border_type']);
    }

    if (isset($this->data['image_border_width'])){
      $plot->SetImageBorderWidth($this->data['image_border_width']);
    }

    if (isset($this->data['tick_color'])){
      $plot->SetTickColor($this->data['tick_color']);
    }

    if (isset($this->data['tick_length'])){
      $plot->SetTickLength($this->data['tick_length']);
    }

    if (isset($this->data['legend'])){
      foreach ($this->data['legend'] as $item){
        $plot->SetLegend($item);
      }
    }

    if (isset($this->data['legend_pixels'])){
      $item = $this->data['legend_pixels'];

      switch(count($item)){
        case 2:
          $plot->SetDataColors($item[0], $item[1]);
          break;
        case 1:
          $plot->SetDataColors($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_position'])){
      $item = $this->data['legend_position'];

      switch(count($item)){
        case 7:
          $plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4], $item[5], $item[6]);
          break;
        case 6:
          $plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4], $item[5]);
          break;
        case 5:
          $plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4]);
          break;
        case 4:
          $plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $plot->SetLegendPosition($item[0], $item[1], $item[2]);
          break;
        case 2:
          $plot->SetLegendPosition($item[0], $item[1]);
          break;
        case 1:
          $plot->SetLegendPosition($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_reverse'])){
      $plot->SetLegendReverse($this->data['legend_reverse']);
    }

    if (isset($this->data['legend_style'])){
      $item = $this->data['legend_style'];

      switch(count($item)){
        case 2:
          $plot->SetLegendStyle($item[0], $item[1]);
          break;
        case 1:
          $plot->SetLegendStyle($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_use_shapes'])){
      $plot->SetLegendUseShapes($this->data['legend_use_shapes']);
    }

    if (isset($this->data['legend_world'])){
      $item = $this->data['legend_world'];

      switch(count($item)){
        case 2:
          $plot->SetLegendWorld($item[0], $item[1]);
          break;
        case 1:
          $plot->SetLegendWorld($item[0]);
          break;
      }
    }

    if (isset($this->data['tick_anchor_x'])){
      $plot->SetXTickAnchor($this->data['tick_anchor_x']);
    }

    if (isset($this->data['tick_crossing_x'])){
      $plot->SetXTickCrossing($this->data['tick_crossing_x']);
    }

    if (isset($this->data['tick_increment_x'])){
      $plot->SetXTickIncrement($this->data['tick_increment_x']);
    }

    if (isset($this->data['tick_label_position_x'])){
      $plot->SetXTickLabelPos($this->data['tick_label_position_x']);
    }

    if (isset($this->data['tick_length_x'])){
      $plot->SetXTickLength($this->data['tick_length_x']);
    }

    if (isset($this->data['tick_position_x'])){
      $plot->SetXTickPos($this->data['tick_position_x']);
    }

    if (isset($this->data['tick_anchor_y'])){
      $plot->SetXTickAnchor($this->data['tick_anchor_y']);
    }

    if (isset($this->data['tick_crossing_y'])){
      $plot->SetXTickCrossing($this->data['tick_crossing_y']);
    }

    if (isset($this->data['tick_increment_y'])){
      $plot->SetXTickIncrement($this->data['tick_increment_y']);
    }

    if (isset($this->data['tick_label_position_y'])){
      $plot->SetXTickLabelPos($this->data['tick_label_position_y']);
    }

    if (isset($this->data['tick_length_y'])){
      $plot->SetXTickLength($this->data['tick_length_y']);
    }

    if (isset($this->data['tick_position_y'])){
      $plot->SetXTickPos($this->data['tick_position_y']);
    }

    // @todo provide a modules_invoke call so that modules that provide their own tweaks can alter the plot object.

    // Enforce the defined type here to prevent extending modules from altering this.
    $plot->SetPlotType($this->type);

    // The image must be inline for it to be loaded and returned in this way.
    $plot->SetIsInline(TRUE);

    // Draw the graph into a buffer $image
    ob_start();
    $plot->DrawGraph();
    $this->image = ob_get_contents();
    ob_end_clean();
  }

  /**
   * Saves the phplot image block to the filesystem and the drupal database.
   * The plot file is saved as a temporary file.
   *
   * @param directory (optional)
   *   A string defining the directory structure where the file will be saved.
   *   This should not begin with a '/'.
   * @param file_schema (optional)
   *   The file schema to use.
   *   - if NULL, then file_default_scheme() is used.
   * @param $function_history (optional)
   *   An array of function names, ie: array('0' => 'my_function_name').
   *
   * @return
   *   The file object that was saved to the database or FALSE on error.
   *
   * @see phplot_api_build_plot()
   * @see file_default_scheme()
   */
  function create_file($directory = 'statistics/plots', $file_schema = NULL){
    $function_history = $this->history(__FUNCTION__);

    // @todo how does one validate the image blob?
    if (empty($this->image)){
      return FALSE;
    }

    if (is_null($file_schema)){
      $file_schema = file_default_scheme();
    }

    global $user;
    $filename = 'plot-' . sha1($this->image) . '.png';
    $directory =  $file_schema . '://' . $directory;
    $destination = $directory . '/' . $filename;
    $timestamp = empty($_SERVER['REQUEST_TIME']) ? time() : REQUEST_TIME;

    file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
    $filepath = file_unmanaged_save_data($this->image, $destination, FILE_EXISTS_REPLACE);

    if (!file_exists($filepath)){
      // create a drupal file object so that the file can be properly saved to the drupal database as a temporary file.
      $this->file = new stdClass();
      $this->file->uri         = $filepath;
      $this->file->filename    = basename($filename);
      $this->file->filemime    = 'image/png';
      $this->file->filesize    = filesize($filepath);
      $this->file->uid         = $user->uid;
      $this->file->status      = 0;
      $this->file->timestamp   = $timestamp;

      file_save($this->file);
    }
    else {
      // @todo what should be done here is to attempt to load the file object from the database and return that file object
    }

    return $filepath;
  }

  /**
   * Initializes and assigns function history.
   */
  private function history($function_name){
    if (empty($caller_history)){
      $function_history = array();
    }
    else {
      $function_history = $caller_history;
    }

    cf_error_append_history($function_history, __CLASS__);
    cf_error_append_history($function_history, $function_name);

    return $function_history;
  }
}
