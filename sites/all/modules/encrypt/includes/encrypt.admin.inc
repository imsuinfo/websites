<?php

/**
 * @file
 * This file holds the functions for the encrypt Admin settings.
 *
 * @ingroup encrypt
 */

/**
 * Menu callback; displays the encrypt module settings page.
 *
 * @see system_settings_form()
 */
function encrypt_admin_settings($form, &$form_state) {
  // Clear the plugin cache on the first page load (but not on AJAX refreshes).
  if (!isset($form_state['values'])) {
    _encrypt_clear_plugin_cache();
  }

  // Retrieve all methods and providers.
  $methods = encrypt_get_encryption_methods();
  $providers = encrypt_get_key_providers();

  // Make sure all plugin files are included (for form validation, etc.)
  // @todo: There has to be a better/faster/cleaner way to do this.
  foreach ($providers as $provider) {
    $form_state['build_info']['files']["encrypt:plugins:{$provider['name']}"] = $provider['path'] . '/' . $provider['file'];
  }

  $form = array();
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'encrypt') . '/encrypt.css',
    ),
  );

  // Get method plugins.
  $method_options = array();
  foreach ($methods as $id => $method) {
    $method_options[$id] = $method['title'];
  }

  // Define form.
  $form['encrypt_encryption_method'] = array(
    '#type' => 'radios',
    '#title' => t('Default Encrypt Method'),
    '#description' => t('Define the default encryption method for the site. Since encryption methods are stored with the data, this can be changed even after you have stored encrypted data.'),
    '#options' => $method_options,
    '#default_value' => variable_get('encrypt_encryption_method', 'default'),
  );
  // Disable any method with dependency errors.
  _encrypt_admin_form_add_options($methods, $form['encrypt_encryption_method']);

  // Get all key providers and format them for a a form.
  $provider_options = array();
  foreach ($providers as $id => $provider) {
    $provider_options[$id] = $provider['title'];
  }
  $provider = variable_get('encrypt_key_provider', 'drupal_private_key');

  $form['encrypt_key_provider'] = array(
    '#type' => 'radios',
    '#title' => t('Key Provider'),
    '#description' => t('Select the method by which encrypt will retrieve an encryption key. NOTE: Once this is set, it is not a good idea to change it. All of your encrypted data may be lost if the encryption key changes.'),
    '#options' => $provider_options,
    '#default_value' => $provider,
    '#ajax' => array(
      'method' => 'replace',
      'callback' => 'encrypt_key_providers_additional_settings_ajax',
      'wrapper' => 'encrypt-key-providers-additional-settings',
    ),
  );
  // Disable any method with dependency errors.
  _encrypt_admin_form_add_options($providers, $form['encrypt_key_provider']);

  $form['key_settings_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="encrypt-key-providers-additional-settings">',
    '#suffix' => '</div>',
  );

  // This is sort of tricky.
  // If the user has changed the key provider in the form, render the
  // additional settings for that provider. Otherwise, render for the current
  // provider.
  if (isset($form_state['values']['encrypt_key_provider']) && $form_state['values']['encrypt_key_provider'] !== $provider) {
    $provider = $form_state['values']['encrypt_key_provider'];
  }
  if ($settings_form_method = ctools_plugin_get_function($providers[$provider], 'settings form')) {
    $form['key_settings_wrapper']['key_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional Key Provider Settings'),
    );
    $settings_variable = 'encrypt_key_providers_' . $provider . '_settings';
    $defaults = variable_get($settings_variable, array());
    $form['key_settings_wrapper']['key_settings'][$settings_variable] = array('#tree' => TRUE) + call_user_func($settings_form_method, $defaults);
  }

  // Make a system setting form and return.
  $form['#submit'][] = 'encrypt_admin_settings_submit';
  return system_settings_form($form);
}

/**
 * Submit callback for admin settings form.
 *
 * Process submit callbacks for each of the selected encrypt plugins.
 */
function encrypt_admin_settings_submit($form, &$form_state) {
  $method = encrypt_get_encryption_method($form_state['values']['encrypt_encryption_method']);
  $key_provider = encrypt_get_key_provider($form_state['values']['encrypt_key_provider']);

  foreach (array($method, $key_provider) as $plugin) {
    if ($submit_callback = ctools_plugin_get_function($plugin, 'submit callback')) {
      // Create a copy so that the plugin callback cannot change the
      // form state.
      $form_state_copy = $form_state;
      call_user_func($submit_callback, $form, $form_state_copy);
    }
  }
}

/**
 * Callback for AJAX form re-rendering.
 */
function encrypt_key_providers_additional_settings_ajax($form, $form_state) {
  return $form['key_settings_wrapper'];
}

/**
 * Add other elements to forms.
 */
function _encrypt_admin_form_add_options($items, &$element) {
  foreach ($items as $id => $item) {
    $element[$id] = array(
      '#description' => $item['description'],
      '#disabled' => !empty($item['dependency errors']),
    );

    // Add a list of dependency errors (if there are any).
    if (!empty($item['dependency errors'])) {
      $element[$id]['#description'] .= theme('item_list', array(
        'items' => $item['dependency errors'],
        'attributes' => array('class' => 'encrypt-dependency-errors')
      ));
    }
  }
}
