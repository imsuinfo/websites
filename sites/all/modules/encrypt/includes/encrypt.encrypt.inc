<?php

/**
 * @file
 * This file holds the ffunctions necessary to encrypt and decrypt
 *
 * @ingroup encrypt
 */

/**
 * Private Encrypt and Decrypt
 *
 * Private internal function to Encrypt and Decrypt text.
 *
 * @param string $op
 *   Whether to encrypt or decrypt.
 *   - "encrypt": Encrypt text
 *   - "decrypt": Decrypt text
 *
 * @param string $text
 *   Text to encrypt
 *
 * @param array $options
 *   Array of options for encryption
 *
 * @param mixed $method
 *   String name of method to use.  Uses setting
 *   default if NULL
 *
 * @param mixed $key_provider
 *   The key provider used to fetch the encryption key. Uses default if NULL.
 *
 * @return string
 *   Depends on the value of $op
 *   - "encrypt": A serialized array representing the encrypted text, method,
 *     and key provider.
 *   - "decrypt": The decrypted text.
 */
function _encrypt_decrypt($op = 'encrypt', $text = '', $options = array(), $method = NULL, $key_provider = NULL) {
  $encryption_array = array();
  $processed = '';

  // Check text.
  if ($text === '') {
    return $processed;
  }

  // Check op.
  if ($op !== 'encrypt') {
    $op = 'decrypt';
  }

  // If decrypting we need to split the encryption array up.
  // We need to do some ternary stuff in here to account for when the user
  // passes in all paramters to the decrypt() function instead of just a
  // serialized array.
  if ($op == 'decrypt') {
    // The text may not actually be serialized, so suppress the warning.
    $encryption_array = @unserialize($text);
    $method = isset($encryption_array['method']) ? $encryption_array['method'] : $method;
    $key_provider = isset($encryption_array['key_provider']) ? $encryption_array['key_provider'] : $key_provider;
    $text = isset($encryption_array['text']) ? $encryption_array['text'] : $text;
  }

  // If there is no method, assume we will be using the default.
  if (!isset($method) || !$method) {
    $method = variable_get('encrypt_encryption_method', 'default');
  }

  // Same for key provider.
  if (!isset($key_provider) || !$key_provider) {
    $key_provider = variable_get('encrypt_key_provider', 'drupal_private_key');
  }

  // Get the encryption key from the default key provider.
  $key = encrypt_get_key_from_key_provider($key_provider);

  // Call callback function for encryption and decryption.
  $encryption_method = encrypt_get_encryption_method($method);
  $function = ctools_plugin_get_function($encryption_method, 'encrypt callback');
  $processed = call_user_func($function, $op, $text, $key, $options);

  // Check for returned value.
  if (!empty($processed) && $op == 'encrypt') {
    $encryption_array = array(
      'text' => $processed,
      'method' => $method,
      'key_provider' => $key_provider,
    );
    // Serialize array.
    $processed = serialize($encryption_array);
  }

  return $processed;
}
