<?php

/**
 * @file
 * CCK Field for Canadian phone numbers.
 */

function phone_ca_metadata($field = NULL) {
  $min_digits = isset($field['ca_phone_digits_minimum']) ? $field['ca_phone_digits_minimum'] : 10;

  if ($min_digits == 4) {
    return array(
      'error' => '"%value" is not a valid North American phone number<br>North American Phone numbers should only contain numbers and + and - and . and ( and ) and spaces and be like 9999, 999-9999, or 999-999-9999. Please enter a valid four-digit, seven-digit, or ten-digit phone number with optional extension.',
    );
  }
  elseif ($min_digits == 7) {
    return array(
      'error' => '"%value" is not a valid North American phone number<br>North American Phone numbers should only contain numbers and + and - and . and ( and ) and spaces and be like 999-9999 or 999-999-9999. Please enter a valid seven-digit or ten-digit phone number with optional extension.',
    );
  }
  elseif ($min_digits == 10 || $min_digits == 11) {
    return array(
      'error' => '"%value" is not a valid North American phone number<br>North American Phone numbers should only contain numbers and + and - and . and ( and ) and spaces and be like 999-999-9999. Please enter a ten-digit valid phone number with optional extension.',
    );
  }
}

/**
 * Breaks apart the North American Phone Number.
 *
 * $param string $phonenumber
 *   Phone number string to parse.
 * @param int $min_digits
 *   Minimum number of digits to proces, either:
 *   - 4
 *   - 7
 *   - 10
 *   - 11
 *
 * @return array|bool
 *   Results of the preg_match().
 *   FALSE is returned on preg_match() errors.
 */
function build_ca_phone_number_matches($phonenumber, $min_digits = 10) {
  $matches = array();
  $phonenumber = trim($phonenumber);

  if ($min_digits == 4) {
    $area_code = '([2-9][0-8]\d)?';
    $prefix  = '([2-9]\d{2})?';
  }
  elseif ($min_digits == 7) {
    $area_code = '([2-9][0-8]\d)?';
    $prefix  = '([2-9]\d{2})';
  }
  else {
    $area_code = '([2-9][0-8]\d)';
    $prefix  = '([2-9]\d{2})';
  }

  // define regular expression
  $regex = '/^\D*(1)?\D*' . $area_code . '\D*' . $prefix . '\D*(\d{4})\D*(\d*)$/i';

  // return true if valid, false otherwise
  $result = preg_match($regex, $phonenumber, $matches);

  if (!$result) return FALSE;

  return $matches;
}

/**
 * Verifies that $phonenumber is a valid ten-digit North American phone number
 *
 * @param string $phonenumber
 * @param array|null $field
 * @param int $min_digits
 * @return boolean Returns boolean FALSE if the phone number is not valid.
 */

function valid_ca_phone_number($phonenumber, $field = NULL) {
  $min_digits = isset($field['ca_phone_digits_minimum']) ? $field['ca_phone_digits_minimum'] : 10;
  $matches = build_ca_phone_number_matches($phonenumber, $min_digits);

  if ($matches === FALSE) return FALSE;

  $country_code = isset($matches[1]) && $matches[1] == '1';
  $digits = preg_replace('/\D/i', '', $matches[0]);
  $length = strlen($digits);

  if ($length > 10 && $country_code) $length--;

  if ($min_digits == 4) {
    if ($length == 4) {
      return (strlen($matches[4]) == 4);
    }
    elseif ($length == 7) {
      return (strlen($matches[4]) == 4 && (strlen($matches[3]) == 3 || strlen($matches[2]) == 3));
    }
    elseif ($length >= 10) {
      return (strlen($matches[4]) == 4 && strlen($matches[3]) == 3 && strlen($matches[2]) == 3);
    }
  }
  elseif ($min_digits == 7) {
    if ($length == 7) {
      return (strlen($matches[4]) == 4 && (strlen($matches[3]) == 3 || strlen($matches[2]) == 3));
    }
    elseif ($length >= 10) {
      return (strlen($matches[4]) == 4 && strlen($matches[3]) == 3 && strlen($matches[2]) == 3);
    }
  }
  elseif ($min_digits == 10) {
    if ($length >= 10) {
      return (strlen($matches[4]) == 4 && strlen($matches[3]) == 3 && strlen($matches[2]) == 3);
    }
  }
  elseif ($min_digits == 11) {
    if (!$country_code) return FALSE;

    if ($length >= 10) {
      return (strlen($matches[4]) == 4 && strlen($matches[3]) == 3 && strlen($matches[2]) == 3);
    }
  }

  return FALSE;
}

/**
 * Convert a valid North American phone number into standard (444) 867-5309 x1234 format
 *
 * @param string $phonenumber must be a valid ten-digit number (with optional extension)
 * @param array $field
 *
 */
function format_ca_phone_number($phonenumber, $field) {
  $min_digits = isset($field['ca_phone_digits_minimum']) ? $field['ca_phone_digits_minimum'] : 10;
  $matches = build_ca_phone_number_matches($phonenumber, $min_digits);

  if ($matches === FALSE) return FALSE;

  $separator = isset($field['ca_phone_separator']) ? $field['ca_phone_separator'] : '-';
  $parenthesis = isset($field['ca_phone_parentheses']) && $field['ca_phone_parentheses'];
  $country_code = isset($field['phone_country_code']) && $field['phone_country_code'];

  $digits = preg_replace('/\D/i', '', $matches[0]);

  if (strlen($digits) < 10 && $country_code) $country_code = FALSE;

  // construct phone number
  $constructed = '';

  if (empty($matches[2])) {
    if (!empty($matches[3])) {
      $constructed .= $matches[3] . $separator;
    }
  }
  elseif (empty($matches[3])) {
    if (!empty($matches[2])) {
      $constructed .= $matches[2] . $separator;
    }
  }
  else {
    if ($parenthesis) {
      if ($country_code) {
        $constructed = '1 ';
      }

      $constructed .= '(' . $matches[2] . ') ';
    }
    else {
      if ($country_code) {
        $constructed = '1' . $separator;
      }

      $constructed .= $matches[2] . $separator;
    }

    $constructed .= $matches[3] . $separator;
  }

  if (!empty($matches[4])) {
    $constructed .= $matches[4];
  }


  // Optional extension
  if (!empty($matches[5])) {
    $constructed .= ' x' . $matches[5];
  }

  return $constructed;
}
