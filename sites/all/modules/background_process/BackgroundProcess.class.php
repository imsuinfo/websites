<?php
/**
 * @file
 *
 * Class for handling background processes.
 */

/**
 * BackgroundProcess class.
 */
class BackgroundProcess {
  public $handle;
  public $connection;
  public $service_host;
  public $service_group;
  public $uid;

  public static function load($process) {
    $new = new BackgroundProcess($process->handle);
    @$new->service_host = $process->service_host;
    @$new->service_group = $process->service_group;
    @$new->uid = $process->uid;
    return $new;
  }

  /**
   * Constructor.
   *
   * @param type $handle
   *   Handle to use. Optional; leave out for auto-handle.
   */
  public function __construct($handle = NULL) {
    $this->handle = $handle ? $handle : background_process_generate_handle('auto');
    $this->token = background_process_generate_handle('token');
    $this->service_group = variable_get('background_process_default_service_group', 'default');
  }

  public function lock() {
    // Preliminary select to avoid unnecessary write-attempt
    if (background_process_get_process($this->handle)) {
      // watchdog('bg_process', 'Will not attempt to lock handle %handle, already exists', array('%handle' => $this->handle), WATCHDOG_NOTICE);
      return FALSE;
    }

    // "Lock" handle
    if (!background_process_lock_process($this->handle)) {
      // If this happens, we might have a race condition or an md5 clash
      watchdog('bg_process', 'Could not lock handle %handle', array('%handle' => $this->handle), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Start background process
   *
   * Calls the service handler through http passing function arguments as serialized data
   * Be aware that the callback will run in a new request
   *
   * @global string $base_url
   *   Base URL for this Drupal request
   *
   * @param $callback
   *   Function to call.
   * @param $args
   *   Array containg arguments to pass on to the callback.
   * @return mixed
   *   TRUE on success, NULL on failure, FALSE on handle locked.
   */
  public function start($callback, $args = array()) {
    if (!$this->lock()) {
      return FALSE;
    }

    return $this->execute($callback, $args);
  }

  public function queue($callback, $args = array()) {
    if (!$this->lock()) {
      return FALSE;
    }

    if (!background_process_set_process($this->handle, $callback, $this->uid, $args, $this->token)) {
      // Could not update process
      return NULL;
    }

    module_invoke_all('background_process_pre_execute', $this->handle, $callback, $args, $this->token);

    // Initialize progress stats
    progress_remove_progress($this->handle);

    $queues = variable_get('background_process_queues', array());
    $queue_name = isset($queues[$callback]) ? 'bgp:' . $queues[$callback] : 'background_process';
    $queue = DrupalQueue::get($queue_name);
    $queue->createItem(array(rawurlencode($this->handle), rawurlencode($this->token)));
    _background_process_ensure_cleanup($this->handle, TRUE);
  }


  public function determineServiceHost() {
    // Validate explicitly selected service host
    $service_hosts = background_process_get_service_hosts();
    if ($this->service_host && empty($service_hosts[$this->service_host])) {
      $this->service_host = variable_get('background_process_default_service_host', 'default');
      if (empty($service_hosts[$this->service_host])) {
        $this->service_host = NULL;
      }
    }

    // Find service group if a service host is not explicitly specified.
    if (!$this->service_host) {
      if (!$this->service_group) {
        $this->service_group = variable_get('background_process_default_service_group', 'default');
      }
      if ($this->service_group) {
        $service_groups = variable_get('background_process_service_groups', array());
        if (isset($service_groups[$this->service_group])) {
          $service_group = $service_groups[$this->service_group];

          // Default method if none is provided
          $service_group += array(
            'method' => 'background_process_service_group_random'
          );
          if (is_callable($service_group['method'])) {
            $this->service_host = call_user_func($service_group['method'], $service_group);
            // Revalidate service host
            if ($this->service_host && empty($service_hosts[$this->service_host])) {
              $this->service_host = NULL;
            }
          }
        }
      }
    }

    // Fallback service host
    if (!$this->service_host || empty($service_hosts[$this->service_host])) {
      $this->service_host = variable_get('background_process_default_service_host', 'default');
      if (empty($service_hosts[$this->service_host])) {
        $this->service_host = 'default';
      }
    }

    return $this->service_host;
  }

  public function execute($callback, $args = array()) {
    if (!background_process_set_process($this->handle, $callback, $this->uid, $args, $this->token)) {
      // Could not update process
      return NULL;
    }

    module_invoke_all('background_process_pre_execute', $this->handle, $callback, $args, $this->token);

    // Initialize progress stats
    progress_remove_progress($this->handle);

    $this->connection = FALSE;

    $this->determineServiceHost();

    list($url, $headers) = background_process_build_request('bgp:start/' . rawurlencode($this->handle) . '/' . rawurlencode($this->token), $this->service_host);

    db_update('background_process')
      ->fields(array(
        'service_host' => $this->service_host ? $this->service_host : '',
      ))
      ->condition('handle', $this->handle)
      ->execute();

    $options = array('method' => 'POST', 'headers' => $headers);
    $result = background_process_http_request($url, $options);
    if (empty($result->error)) {
      $this->connection = $result->fp;
      _background_process_ensure_cleanup($this->handle, TRUE);
      return TRUE;
    }
    else {
      background_process_remove_process($this->handle);
      watchdog('bg_process', 'Could not call service %handle for callback %callback', array('%handle' => $this->handle, '%callback' => $callback), WATCHDOG_ERROR);
      // Throw exception here instead?
      return NULL;
    }
    return FALSE;
  }
}

