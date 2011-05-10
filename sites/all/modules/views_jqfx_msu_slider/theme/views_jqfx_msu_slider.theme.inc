<?php
// $Id$

/**
 * @file
 * Theme & preprocess functions for the Views jQFX: Msu Slider module.
 */

function template_preprocess_views_jqfx_msu_slider(&$vars) {
  // Initialize our $images array.
  $vars['images'] = array();

  // Strip all images from the $rows created by the original view query.
  foreach ($vars['rows'] as $item) {
    if (preg_match('@(<\s*img\s+[^>]*>)@i', $item, $matches)) {
      $image = $matches[1];
      // We need to add a URL to 'longdesc', as required by the plugin.
      // If our image is in an anchor tag, use its URL.
      if (preg_match('@<\s*a\s+href\s*=\s*"\s*([^"]+)\s*"[^>]*>[^<]*' . preg_quote($image) . '[^<]*<\s*/a\s*>@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Or look to see if we have any other links in our views items.
      elseif (preg_match('@href\s*=\s*"([^"]+)"@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Otherwise we have no $url.
      else {$url = NULL;}

      // Our image gets wrapped with the $url anchor if it is present.
      if ($url) {
        $image = '<a href="'. $url .'">'. $image .'</a>';
      }

      // Add the image to our image array to display.
      $vars['images'][] = $image;
    }
  }

  $options = $vars['options']['views_jqfx_msu_slider'];

  _views_jqfx_msu_slider_add_js($options, 'views-jqfx-msu-slider-images-' . $vars['id']);

}

function _views_jqfx_msu_slider_add_js($options, $id) {
  $libraries_path = 'sites/all/libraries';

  // Add the plugin JS and CSS.
  drupal_add_js($libraries_path . '/' . $options['nivo_library'] . '/jquery.msu.slider.js');
  drupal_add_css($libraries_path . '/' . $options['nivo_library'] . '/msu-slider.css');

  //Add the module JS
  $drupal_path = drupal_get_path('module', 'views_jqfx_msu_slider');
  drupal_add_js($drupal_path . '/js/views_jqfx_msu_slider.js');

  // Load the theme CSS.
  if ($options['custom_style'] && !$options['style']) {
    $theme_url = check_plain($options['custom_style']);
    drupal_add_css($theme_url);
  }
  elseif ($options['style']) {
    drupal_add_css(drupal_get_path('module', 'views_jqfx_msu_slider') . '/theme/styles/' . $options['style'] . '/' . $options['style'] . '-msu-slider.css');
  }
  else {
    $options['style'] = NULL;
  }

  // Get the javascript settings.
  $settings = array(
    'effect'              => check_plain($options['effect']),
    'slices'              => (integer)$options['slices'],
    'animSpeed'           => (integer)$options['anim_speed'],
    'pauseTime'           => (integer)$options['pause_time'],
    'startSlide'          => (integer)$options['start_slide'],
    'directionNav'        => $options['direction_nav'] ? TRUE : FALSE,
    'directionNavHide'    => $options['direction_nav_hide'] ? TRUE : FALSE,
    'controlNav'          => $options['control_nav'] ? TRUE : FALSE,
    'keyboardNav'         => $options['keyboard_nav'] ? TRUE : FALSE,
    'pauseOnHover'        => $options['pause_on_hover'] ? TRUE : FALSE,
    'manualAdvance'       => $options['manual_advance'] ? TRUE : FALSE,
    'captionOpacity'      => (float)$options['caption_opacity'],
  );

  // Since controlNav is boolean we need to tell controlNavThumbs about thumbnails.
  if ($options['control_nav'] == 'thumbnails') {
    $settings['controlNavThumbs'] = TRUE;
    $settings['controlNavThumbsSearch'] = $options['control_nav_thumbs_search'];
    $settings['controlNavThumbsReplace'] = $options['control_nav_thumbs_replace'];
  }

  // These will break the Msu Slider if they are passed empty. Make sure they are set first.
  if ($options['before_change']) {
    $settings['beforeChange'] = $options['before_change'];
  }
  if ($options['after_change']) {
    $settings['afterChange'] = $options['after_change'];
  }
  if ($options['slideshow_end']) {
    $settings['slideshowEnd'] = $options['slideshow_end'];
  }
  if ($options['last_slide']) {
    $settings['lastSlide'] = $options['last_slide'];
  }
  if ($options['after_load']) {
    $settings['afterLoad'] = $options['after_load'];
  }

  // Add the settings array.
  drupal_add_js(array('viewsJqfxMsuSlider' => array($id => $settings)), 'setting');
}
