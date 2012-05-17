<?php

/**
 * @file
 * Provides the derror exception class.
 */

/**
 * @addtogroup cf_error
 * @{
 */

class cf_error {
  const BACKTRACE_MODE_NONE = 1;
  const BACKTRACE_MODE_SHORT = 2;
  const BACKTRACE_MODE_FULL = 3;

  /**
   * Returns an options array list of available backtrace modes.
   *
   * @return array
   *   An options array list of available backtrace modes.
   */
  public static function get_backtrace_options_list() {
    $backtrace_options_list = array();

    $backtrace_options_list[cf_error::BACKTRACE_MODE_NONE] = t("No Backtrace");
    $backtrace_options_list[cf_error::BACKTRACE_MODE_SHORT] = t("Short Backtrace");
    $backtrace_options_list[cf_error::BACKTRACE_MODE_FULL] = t("Full Backtrace");

    return $backtrace_options_list;
  }

  /**
   * Reports variables as invalid to the watchdog system.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param string $why
   *   The specific reason for this watchdog report.
   * @param array $variables
   *   (optional) Locale safe parameter handling for all text found in the 'why'
   *   parameter.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_variable($argument_name, $why, $variables = array(), $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_variable($error, $argument_name, $why, $variables);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be a string but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function not_string($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_not_string($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be non-empty string but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function empty_string($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_empty_string($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be non-empty array but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function empty_array($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_empty_array($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be an array but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_array($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_array($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be an object but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_object($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_object($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given array is missing a specific array key.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param string $key_name
   *   The name of the array key that is missing.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function missing_array_key($argument_name, $key_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    if (empty($key_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'key_name');
      $key_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_missing_array_key($error, $argument_name, $key_name);

    return $error;
  }

  /**
   * Reports that a given object is missing a property.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param string $property_name
   *   The name of the object property that is missing.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function missing_object_property($argument_name, $property_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    if (empty($property_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'property_name');
      $property_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_missing_object_property($error, $argument_name, $property_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be numeric but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function not_numeric($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code();

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_not_numeric($error, $argument_name);

    return $error;
  }

  /**
   * Reports query execution failures.
   *
   * @param object $exception
   *   The query exception object.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function on_query_execution($exception, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (is_object($exception)) {
      $exception_message = $exception->getMessage();
      $query_string = $exception->query_string;
      $query_arguments = print_r($exception->args, TRUE);
    }
    else {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_invalid_object($error2, 'exception');
      $exception_message = "";
      $query_string = "";
      $query_arguments = "";
    }

    $error->set_severity($severity);
    $error->set_type('sql');
    self::p_load_backtrace($error);
    self::p_on_query_execution($error, $exception_message, $query_string, $query_arguments);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be an boolean but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_bool($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_bool($error, $argument_name);

    return $error;
  }

  /**
   * Reports if a variable is not a float, double, or real.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_float($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_float($error, $argument_name);

    return $error;
  }

  /**
   * Reports if an argument is not a callable function.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_callable($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_callable($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is not an integer or a long.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_integer($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_integer($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be a resource but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_resource($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_resource($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be a scalar but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_scalar($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_scalar($error, $argument_name);

    return $error;
  }

  /**
   * Reports that a given argument is supposed to be a null but is not.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param int $severity
   *   (optional) The severity of the message, as per RFC 3164. Possible values
   *   are WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
   *
   * @see: watchdog()
   * @see: watchdog_severity_levels()
   */
  public static function invalid_null($argument_name, $severity = WATCHDOG_ERROR) {
    $error = new cf_error_code;

    if (empty($argument_name)) {
      $error2 = new cf_error_code;
      $error2->set_severity(WATCHDOG_ERROR);
      $error2->set_backtrace(debug_backtrace());
      self::p_empty_string($error2, 'argument_name');
      $argument_name = '(unknown)';
    }

    $error->set_severity($severity);
    self::p_load_backtrace($error);
    self::p_invalid_null($error, $argument_name);

    return $error;
  }

  /**
   * Loads the backtrace log.
   *
   * This removes parts of the backtrace that where errors from this class were
   * called from. Therefore, the backtrace log stops where the error actually
   * happens and not where this function is called.
   */
  private static function p_load_backtrace(cf_error_code &$error) {
    $backtrace = debug_backtrace();

    // Remove the backtrace log for p_load_backtrace().
    array_shift($backtrace);

    // Remove the backtrace log of the error function.
    array_shift($backtrace);

    $error->set_backtrace($backtrace);
  }

  /**
   * Reports variables as invalid to the watchdog system.
   *
   * @param string $argument_names
   *   The variable name of the argument in question.
   * @param string $why
   *   The specific reason for this watchdog report.
   * @param array $variables
   *   Locale safe parameter handling for all text found in the 'why' parameter.
   */
  private static function p_invalid_variable(cf_error_code $error, $argument_name, $why, array $variables) {
    self::p_print_message($error, "The argument '%cf_error-argument_name' is invalid or has a problem, reason: " . $why, array_merge($variables, array('%cf_error-argument_name' =>  $argument_name)));
  }

  /**
   * Reports that a given argument is supposed to be a string but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_not_string(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Must be a string.", array());
  }

  /**
   * Reports that a given argument is supposed to be non-empty string but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_empty_string(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Must not be an empty string.", array());
  }

  /**
   * Reports that a given argument is supposed to be non-empty array but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_empty_array(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Must not be an empty array.", array());
  }

  /**
   * Reports that a given argument is supposed to be an array but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_array(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid array.", array());
  }

  /**
   * Reports that a given argument is supposed to be an object but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_object(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid object.", array());
  }

  /**
   * Reports that a given array is missing a specific array key.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   * @param string $key_name
   *   Name of the array key that is missing.
   */
  private static function p_missing_array_key(cf_error_code $error, $argument_name, $key_name) {
    self::p_invalid_variable($error, $argument_name, "The array key '%cf_error-key_name' is missing.", array('%cf_error-key_name' => $key_name));
  }

  /**
   * Reports that a given object is missing a property.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_missing_object_property(cf_error_code $error, $argument_name, $property_name) {
    self::p_invalid_variable($error, $argument_name, "The object property '%cf_error-property_name' is missing.", array('%cf_error-property_name' => $property_name));
  }

  /**
   * Reports that a given argument is supposed to be numeric but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_not_numeric(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a numeric value.", array());
  }

  /**
   * Reports query execution failures.
   *
   * @param string $exception_message
   *   The message reported by the exception.
   * @param string $query_string
   *   The full sql query used that generated the exception.
   *   This gives more information than the exception message.
   * @param string $query_arguments
   *   The arguments passed to the sql query.
   *   This gives more information than the exception message.
   */
  private static function p_on_query_execution(cf_error_code $error, $exception_message, $query_string, $query_arguments) {
    self::p_print_message($error, "Query Exception: %cf_error-exception_message.", array('%cf_error-exception_message' => $exception_message, '%cf_error-query_string' => $query_string, '%cf_error-query_arguments' => $query_arguments), " SQL Query = %cf_error-query_string. SQL Arguments = %cf_error-query_arguments.");
  }

  /**
   * Reports that a given argument is supposed to be an boolean but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_bool(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid boolean.", array());
  }

  /**
   * Reports if a variable is not a float, double, or real.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_float(cf_error_code $error, $argument_name) {
    p_invalid_variable($error, $argument_name, "Not a valid float, double, or real.", array());
  }

  /**
   * Reports if an argument is not a callable function.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_callable(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Cannot be called as a function.", array());
  }

  /**
   * Reports that a given argument is not an integer or a long.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_integer(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid integer or long.", array());
  }

  /**
   * Reports that a given argument is supposed to be a resource but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_resource(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid resource.", array());
  }

  /**
   * Reports that a given argument is supposed to be a scalar but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_scalar(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not a valid scalar.", array());
  }

  /**
   * Reports that a given argument is supposed to be a null but is not.
   *
   * @param string $argument_name
   *   The variable name of the argument in question.
   */
  private static function p_invalid_null(cf_error_code $error, $argument_name) {
    self::p_invalid_variable($error, $argument_name, "Not NULL.", array());
  }

  /**
   * Prints error messages to the screen.
   *
   * This uses drupal_set_message() and watchdog() to print the messages.
   *
   * Why:
   *   This facilitates printing the error messages without having each and every
   *   usage need to manually do so.
   *
   * @param string $message
   *   A string to display.
   * @param array $variables_array
   *   An array of string substitutions for anything in the $message string.
   * @param string $additional
   *   (optional) Additional information to present only in the watchdog logs.
   *    This does not get displayed via drupal_set_message().
   *
   * @see drupal_set_message()
   * @see watchdog()
   * @see watchdog_severity_levels()
   */
  private static function p_print_message(cf_error_code $error, $message, array $variables_array, $additional = "") {
    switch ($error->get_severity()) {
      case WATCHDOG_EMERGENCY:
        if (user_access('view cf emergency messages')) {
          drupal_set_message(t($message, $variables_array), 'error', FALSE);
        }
        break;

      case WATCHDOG_ALERT:
      case WATCHDOG_CRITICAL:
      case WATCHDOG_ERROR:
        if (user_access('view cf error messages')) {
          drupal_set_message(t($message, $variables_array), 'error', FALSE);
        }
        break;

      case WATCHDOG_WARNING:
        if (user_access('view cf warning messages')) {
          drupal_set_message(t($message, $variables_array), 'warning', FALSE);
        }
        break;

      case WATCHDOG_NOTICE:
      case WATCHDOG_INFO:
        if (user_access('view cf information messages')) {
          drupal_set_message(t($message, $variables_array), 'status', FALSE);
        }
        break;

      case WATCHDOG_DEBUG:
        if (user_access('view cf debug messages')) {
          drupal_set_message(t($message, $variables_array), 'status', FALSE);
        }
        break;
    }

    $message .= $additional;

    static $show_backtrace;

    if (!isset($show_backtrace)) {
      $show_backtrace = & variable_get('cf_error_backtrace_mode', self::BACKTRACE_MODE_SHORT);
    }

    if ($show_backtrace != self::BACKTRACE_MODE_NONE) {
      if ($show_backtrace == self::BACKTRACE_MODE_SHORT) {
        $short_backtrace = array();
        $full_backtrace = $error->get_backtrace();
        $backtrace_size = count($full_backtrace);

        if ($backtrace_size > 0) {
          $short_backtrace[] = reset($full_backtrace);
        }

        if ($backtrace_size > 1) {
          $short_backtrace[] = $full_backtrace[1];
        }

        if ($backtrace_size > 2) {
          $short_backtrace[] = $full_backtrace[2];
        }

        $variables_array['%cf_error-backtrace'] = print_r($short_backtrace, TRUE);
      }
      else {
        $variables_array['%cf_error-backtrace'] = print_r($error->get_backtrace(), TRUE);
      }

      $message .= " \nBacktrace: %cf_error-backtrace";
    }

    watchdog($error->get_type(), $message, $variables_array, $error->get_severity());
  }
}

/**
 * @} End of '@addtogroup cf_error'.
 */
