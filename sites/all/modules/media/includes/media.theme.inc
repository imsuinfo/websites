<?php

/**
 * @file
 * Media Theming
 *
 * Theming functions for the Media module.
 */

/**
 * Adds a wrapper around a preview of a media file.
 */
function theme_media_thumbnail($variables) {
  $label = '';
  $element = $variables['element'];

 // Wrappers to go around the thumbnail.
  $attributes = array(
    'title' => $element['#name'],
    'class' => 'media-item',
    'data-fid' => $element['#file']->fid,
  );
  $prefix = '<div ' . drupal_attributes($attributes) . '><div class="media-thumbnail">';
  $suffix = '</div></div>';

  // Arguments for the thumbnail link.
  $thumb = $element['#children'];
  if (file_entity_access('update', $element['#file'])) {
    $target = 'file/' . $element['#file']->fid . '/edit';
    $title = t('Click to edit details');
  }
  else {
    $target = 'file/' . $element['#file']->fid;
    $title = t('Click to view details');
  }
  $options = array(
    'query' => drupal_get_destination(),
    'html' => TRUE,
    'attributes' => array('title' => $title),
  );

  // Element should be a field renderable array. This should be improved.
  if (!empty($element['#show_names']) && $element['#name']) {
    $label = '<div class="label-wrapper"><label class="media-filename">' . $element['#name'] . '</label></div>';
  }

  $link_to_file = '';
  $wrapper = file_stream_wrapper_get_instance_by_uri($element['#file']->uri);
  if (is_object($wrapper)) {
    $external_url = $wrapper->getExternalUrl();

    if (!empty($external_url)) {
      $link_to_file = '<a href="' . $external_url . '" class="media-item-url" title="URL: ' . check_plain($external_url) . '" alt="link to file"></a>';
    }
  }

  $output = $prefix;
  if (!empty($element['#add_link'])) {
    $output .= l($thumb, $target, $options);
  }
  else {
    $output .= $thumb;
  }
  $output .= $label . $suffix;
  $output .= $link_to_file;
  return $output;
}

/**
 * Preprocess the media thumbnail.
 */
function template_preprocess_media_thumbnail(&$variables) {
  // Set the name for the thumbnail to be the filename.  This is done here so
  // that other modules can hijack the name displayed if it should not be the
  // filename.
  $variables['element']['#name'] = isset($variables['element']['#file']->filename) ? check_plain($variables['element']['#file']->filename) : NULL;
}

/**
 * Field formatter for displaying a file as a large icon.
 */
function theme_media_formatter_large_icon($variables) {
  $file = $variables['file'];
  $icon_dir = variable_get('media_icon_base_directory', 'public://media-icons') . '/' . variable_get('media_icon_set', 'default');
  $icon = file_icon_path($file, $icon_dir);
  $variables['path'] = $icon;

  // theme_image() requires the 'alt' attribute passed as its own variable.
  // @see http://drupal.org/node/999338
  if (!isset($variables['alt']) && isset($variables['attributes']['alt'])) {
    $variables['alt'] = $variables['attributes']['alt'];
  }

  // Add image height and width for the image theme functions.
  if ($info = image_get_info($icon)) {
    $variables += $info;
  }

  if ($variables['style_name']) {
    $output = theme('image_style', $variables);
  }
  else {
    $output = theme('image', $variables);
  }
  return $output;
}

/**
 * Field formatter for displaying a file link only (similar to file_link).
 * Does not display the file icon, unlike file_link.
 */
function theme_media_formatter_link($variables) {
  $file = $variables['file'];

  $url = rawurldecode(file_create_url($file->uri));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<span class="file file_link file_link-icon_none">' . l($link_text, $url, $options) . '</span>';
}

/**
 * Field formatter for displaying a file link only (similar to file_link).
 * Display icon, on left side.
 */
function theme_media_formatter_link_icon_before($variables) {
  $variables['icon_postfix'] = FALSE;

  if (empty($variables['icon_directory'])) {
    $variables['icon_directory'] = variable_get('file_icon_directory', drupal_get_path('module', 'file') . '/icons');
  }

  return theme_file_link($variables);
}

/**
 * Field formatter for displaying a file link only (similar to file_link).
 * Display icon, on right side.
 */
function theme_media_formatter_link_icon_after($variables) {
  $variables['icon_postfix'] = TRUE;

  if (empty($variables['icon_directory'])) {
    $variables['icon_directory'] = variable_get('file_icon_directory', drupal_get_path('module', 'file') . '/icons');
  }

  return theme_file_link($variables);
}

/**
 * Add messages to the page.
 */
function template_preprocess_media_dialog_page(&$variables) {
  $variables['messages'] = theme('status_messages');
}
