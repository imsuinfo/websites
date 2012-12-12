<?php

/**
 * @file
 * Plugin definition for the mcrypt extension and AES-256 encryption method.
 */

$plugin = array(
  'title' => t('Mcrypt AES 256'),
  'description' => t('This uses PHPs mcrypt extension and <a href="http://en.wikipedia.org/wiki/Advanced_Encryption_Standard">AES-256</a>.'),
  'encrypt callback' => '_encrypt_encryption_methods_mcrypt_rij_256',
  'dependency callback' => '_mcrypt_extension_is_present',
);

/**
 * Callback for Encrypt implementation: Mcrypt
 *
 * This method uses a PHPs mcrypt extension and AES-256
 */
function _encrypt_encryption_methods_mcrypt_rij_256($op = 'encrypt', $text = '', $key, $options = array()) {
  $processed_text = '';

  // Key cannot be too long for this encryption
  $key = drupal_substr($key, 0, 32);

  // Define iv cipher
  $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB);
  $iv = mcrypt_create_iv($iv_size, MCRYPT_RAND);
  $use_base64 = array_key_exists('base64', $options) && $options['base64'] !== FALSE;

  // Check op
  if ($op == 'decrypt') {
    // Decrypt
    // Check if we are using base64 encoding
    if ($use_base64) {
      $text = base64_decode($text);
    }

    // Decrypt text
    $processed_text = mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $key, $text, MCRYPT_MODE_ECB, $iv);
  }
  else {
    // Encrypt
    $processed_text = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, $key, $text, MCRYPT_MODE_ECB, $iv);

    // Check if we are using base64 encoding
    if ($use_base64) {
      $processed_text = base64_encode($processed_text);
    }
  }

  return trim($processed_text);
}

/**
 * Callback to see if the MCrypt library is present.
 */
function _mcrypt_extension_is_present() {
  $errors = array();

  if (!function_exists('mcrypt_encrypt')) {
    $errors[] = t('MCrypt library not installed.');
  }

  return $errors;
}
