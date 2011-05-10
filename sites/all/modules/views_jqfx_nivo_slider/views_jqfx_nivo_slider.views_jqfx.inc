<?php
// $Id$

/**
 * @file
 * The UI and menu includes.
 *
 * Provides the default options array and generates and
 * validates the settings form.
 */

/**
 * Implements hook_views_jqfx_jqfx_types().
 */
function views_jqfx_nivo_slider_views_jqfx_jqfx_types() {
  $options = array(
    'views_jqfx_nivo_slider' => t('Nivo Slider'),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_option_definition().
 */
function views_jqfx_nivo_slider_views_jqfx_option_definition() {
  // Set our default options.
  $options['views_jqfx_nivo_slider'] = array(
    'contains' => array(
      'style'                       => array('default' => 'basic'),
      'custom_style'                => array('default' => NULL),
      'effect'                      => array('default' => 'random'),
      'slices'                      => array('default' => '15'),
      'anim_speed'                  => array('default' => '500'),
      'pause_time'                  => array('default' => '3000'),
      'start_slide'                 => array('default' => '0'),
      'direction_nav'               => array('default' => TRUE),
      'direction_nav_hide'          => array('default' => TRUE),
      'control_nav'                 => array('default' => TRUE),
      'control_nav_thumbs'          => array('default' => TRUE),
      'control_nav_thumbs_search'   => array('default' => 'large'),
      'control_nav_thumbs_replace'  => array('default' => 'thumbnail'),
      'keyboard_nav'                => array('default' => TRUE),
      'pause_on_hover'              => array('default' => TRUE),
      'manual_advance'              => array('default' => FALSE),
      'caption_opacity'             => array('default' => '0.8'),
      'advanced_functions'          => array('default' => FALSE),
      'before_change'               => array('default' => NULL),
      'after_change'                => array('default' => NULL),
      'slideshow_end'               => array('default' => NULL),
      'last_slide'                  => array('default' => NULL),
      'after_load'                  => array('default' => NULL),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_views_jqfx_type_form().
 */
function views_jqfx_nivo_slider_views_jqfx_jqfx_type_form(&$form, &$form_state, &$view) {
  ctools_include('dependent');
  // Alerts the user if the library is not installed
  if (!file_exists('sites/all/libraries/nivo-slider/jquery.nivo.slider.pack.js')) {
    $form['views_jqfx_nivo_slider']['no_nivo_slider_js'] = array(
      '#markup' => '<div style="color: red">' . t('You need to download the Nivo Slider plugin and copy it to sites/all/libraries/nivo-slider. You can find the plugin at !url.', array('!url' => l(t('http://nivo.dev7studios.com/', array(), array('langcode' => 'en')), 'http://nivo.dev7studios.com/', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')) . '</div>',
    );
  }
  $style = array(
    FALSE => t('Custom (Enter the path below)'),
    'basic1' => t('Basic1'),
    'basic2' => t('Basic2'),
    'basic3' => t('Basic3'),
  );
  $form['views_jqfx_nivo_slider']['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('Basic 1 is the original version. Basic2 and Basic3 are based on the most recent Nivo Slider version. Basic2 is syled for use with button navigation. Basic3 is styled for use with 50x50 thumbnail navigation.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['style'],
    '#options' => $style,
  );
  $form['views_jqfx_nivo_slider']['custom_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom theme path'),
    '#description' => t('Enter the path to your custom css file. If left empty no theme will be loaded'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['custom_style'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-style' => array(FALSE)),
  );
  $effect = array(
    'random' => t('Random'),
    'sliceDown' => t('Slice down'),
    'sliceDownLeft' => t('Slice down left'),
    'sliceUp' => t('Slice up'),
    'sliceUpLeft' => t('Slice up left'),
    'sliceUpDown' => t('Slice up down'),
    'sliceUpDownLeft' => t('Slice up down left'),
    'fold' => t('Fold'),
    'fade' => t('Fade'),
    'slideInLeft' => t('Slide in left'),
    'slideInRight' => t('Slide in right'),
  );
  $form['views_jqfx_nivo_slider']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#description' => t('The transition effect between images'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['effect'],
    '#options' => $effect,
  );
  $form['views_jqfx_nivo_slider']['slices'] = array(
    '#type' => 'textfield',
    '#title' => t('Slices'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['slices'],
    '#description' => t('The number of slices used in the transitions. Must be an integer.'),
  );
  $form['views_jqfx_nivo_slider']['anim_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation Speed'),
    '#description' => t('Animation speed in milliseconds'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['anim_speed'],
  );
  $form['views_jqfx_nivo_slider']['pause_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause Time'),
    '#description' => t('Pause time in milliseconds.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['pause_time'],
  );
  $form['views_jqfx_nivo_slider']['start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Start slide'),
    '#description' => t('Set starting Slide (0 index).'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['start_slide'],
  );
  $form['views_jqfx_nivo_slider']['direction_nav'] = array(
    '#type' => 'select',
    '#title' => t('Direction Navigator'),
    '#description' => t('Previous and next navigation.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['direction_nav'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_nivo_slider']['direction_nav_hide'] = array(
    '#type' => 'select',
    '#title' => t('Direction Navigator Hide'),
    '#description' => t('Only show direction navigator on mouse over.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['direction_nav_hide'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-direction-nav' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_nivo_slider']['control_nav'] = array(
    '#type' => 'select',
    '#title' => t('Control Navigator'),
    '#description' => t('Chose navigation buttons/thumbs.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['control_nav'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Buttons'),
      'thumbnails' => t('Thumbnails'),
    ),
  );
  $presets = array();
  foreach (image_styles() as $p) {
    $presets[$p['name']] = $p['name'];
  }
  $form['views_jqfx_nivo_slider']['control_nav_thumbs_search'] = array(
    '#type' => 'select',
    '#title' => t('Slide image style preset'),
    '#description' => t('Select the image style preset that was chosen for the slide images in your view setting.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['control_nav_thumbs_search'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-control-nav' => array('thumbnails')),
    '#options' => $presets,
  );
  $form['views_jqfx_nivo_slider']['control_nav_thumbs_replace'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail image style preset'),
    '#options' => $presets,
    '#description' => t('The image style preset to be used for the thumbnail images.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['control_nav_thumbs_replace'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-control-nav' => array('thumbnails')),
  );
  $form['views_jqfx_nivo_slider']['keyboard_nav'] = array(
    '#type' => 'select',
    '#title' => t('Keyboard Navigator'),
    '#description' => t('Use left and right arrows.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['keyboard_nav'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_nivo_slider']['pause_on_hover'] = array(
    '#type' => 'select',
    '#title' => t('Pause on Hover'),
    '#description' => t('Stop animation while hovering.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['pause_on_hover'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_nivo_slider']['manual_advance'] = array(
    '#type' => 'select',
    '#title' => t('Manual Advance'),
    '#description' => t('Force manual transitions.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['manual_advance'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_nivo_slider']['caption_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption Opacity'),
    '#description' => t('Universal caption opacity.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['caption_opacity'],
  );
  $form['views_jqfx_nivo_slider']['advanced_functions'] = array(
    '#type' => 'select',
    '#title' => t('Advanced options'),
    '#description' => t('Check to display the advanced options. These must be entered in the form of a %function. (Caution: experts only!).', array('%function' => 'function(){}')),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['advanced_functions'],
    '#options' => array(
      FALSE => t('Hide'),
      TRUE => t('Show'),
    ),
  );
  $form['views_jqfx_nivo_slider']['before_change'] = array(
    '#type' => 'textarea',
    '#title' => t('Before change'),
    '#description' => t('A function that triggers before a slide change.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['before_change'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_nivo_slider']['after_change'] = array(
    '#type' => 'textarea',
    '#title' => t('After change'),
    '#description' => t('A function that triggers after a slide change.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['after_change'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_nivo_slider']['slideshow_end'] = array(
    '#type' => 'textarea',
    '#title' => t('Slideshow end'),
    '#description' => t('A function that triggers after all slides have been shown.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['slideshow_end'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_nivo_slider']['last_slide'] = array(
    '#type' => 'textarea',
    '#title' => t('Last slide'),
    '#description' => t('A function that triggers when last slide is shown.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['last_slide'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_nivo_slider']['after_load'] = array(
    '#type' => 'textarea',
    '#title' => t('After load'),
    '#description' => t('A function that triggers when slider has loaded.'),
    '#default_value' => $view->options['views_jqfx_nivo_slider']['after_load'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-nivo-slider-advanced-functions' => array(TRUE)),
  );
}

// Validate the form options.
function views_jqfx_nivo_slider_views_jqfx_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_nivo_slider']['slices'])) {
    form_error($form['views_jqfx_nivo_slider']['slices'], t('!setting must be numeric!', array('Slices')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_nivo_slider']['anim_speed'])) {
    form_error($form['views_jqfx_nivo_slider']['anim_speed'], t('!setting must be numeric!', array('Animation speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_nivo_slider']['pause_time'])) {
    form_error($form['views_jqfx_nivo_slider']['pause_time'], t('!setting must be numeric!', array('Pause time')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_nivo_slider']['start_slide'])) {
    form_error($form['views_jqfx_nivo_slider']['start_slide'], t('!setting must be numeric!', array('Start slide')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_nivo_slider']['caption_opacity'])) {
    form_error($form['views_jqfx_nivo_slider']['caption_opacity'], t('!setting must be numeric!', array('Universal caption opacity')));
  }
}