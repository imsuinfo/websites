<?php

/**
 * @file
 * Base classes for tests for the API module.
 */

/**
 * Provides a base class for testing the API module.
 */
class ApiTestCase extends DrupalWebTestCase {
  /**
   * Default branch object.
   */
  protected $default_branch;

  /**
   * User with permission to administer and see everything.
   */
  protected $super_user;

  /**
   * Default set up: Sets up branch using API calls, removes PHP branch, parses.
   */
  function setUp() {
    $this->baseSetUp();
    $this->setUpBranchAPICall();
    $this->removePHPBranch();

    $this->resetBranchesAndCache();
    api_update_all_branches();
    $count = $this->processApiParseQueue();
    $this->assertEqual($count, 11, "11 files were parsed ($count)");
  }

  /**
   * Sets up modules for API tests, and a super-user.
   */
  function baseSetUp() {
    DrupalWebTestCase::setUp('api', 'ctools', 'gplib', 'node', 'comment');

    module_load_include('inc', 'api', 'api.admin');
    module_load_include('inc', 'api', 'parser');

    // Set up a super-user.
    $this->super_user = $this->drupalCreateUser(array(
      'access API reference',
      'administer API reference',
      'access content',
      'access administration pages',
      'administer blocks',
      'administer nodes',
    ));
    $this->drupalLogin($this->super_user);
  }

  /**
   * Sets up a files branch using API function calls.
   *
   * @param $prefix
   *   Directory prefix to prepend on the data directories.
   * @param $default
   *   TRUE to set this as the default branch; FALSE to not set it as default.
   * @param $info
   *   Array of branch information to override the defaults (see function
   *   code to see what they are). Note that $prefix is applied after this
   *   information is read, and that only one directory and one excluded are
   *   supported in this function.
   *
   * @return
   *   Array of information (defaults with overrides) used to create the
   *   branch.
   */
  function setUpBranchAPICall($prefix = '', $default = TRUE, $info = array()) {
    // Set up defaults.
    $info += array(
      'project' => 'test',
      'project_title' => 'Project 6',
      'branch_name' => '6',
      'title' => 'Testing 6',
      'directory' => drupal_get_path('module', 'api') . '/tests/sample',
      'excluded' => drupal_get_path('module', 'api') . '/tests/sample/to_exclude',
    );

    // Save this information as a branch.
    $branch = new stdClass();
    $branch->type = 'files';
    $branch->status = 1;
    $branch->project = $info['project'];
    $branch->project_title = $info['project_title'];
    $branch->branch_name = $info['branch_name'];
    $branch->title = $info['title'];
    $branch->data = array(
      'directories' => $prefix . $info['directory'],
      'excluded_directories' => $prefix . $info['excluded'],
    );

    api_save_branch($branch);
    if ($default) {
      variable_set('api_default_branch', $branch->branch_id);
    }

    $this->assertEqual(variable_get('api_default_branch', 99), $branch->branch_id, 'Variable for default branch is set correctly');

    return $info;
  }

  /**
   * Removes the PHP branch, which most tests do not need.
   */
  function removePHPBranch() {
    db_delete('api_branch')
      ->condition('type', 'php')
      ->execute();
  }

  /**
   * Returns the first branch in the branches list.
   */
  function getBranch() {
    $branches = api_get_branches();
    return reset($branches);
  }

  /**
   * Asserts the right number of documentation objects are in the given branch.
   *
   * @param $branch
   *   Branch object to look in. Omit to use the default branch.
   * @param $num
   *   Number of objects to assert. Omit to use the current number that should
   *   be present for the default branch.
   */
  function assertObjectCount($branch = NULL, $num = 67) {
    if (is_null($branch)) {
      $branch = $this->getBranch();
    }

    $count = db_query("SELECT count(*) FROM {api_documentation} WHERE branch_id = :branch_id", array(':branch_id' => $branch->branch_id))->fetchField();

    $this->assertEqual($count, $num, 'Found ' . $count . ' documentation objects (should be ' . $num . ')');
  }

  /**
   * Makes sure all variables and branches have been reset.
   */
  function resetBranchesAndCache() {
    cache_clear_all('variables', 'cache_bootstrap', 'cache');
    variable_initialize();
    api_get_branches(TRUE);
    menu_rebuild();
  }

  /**
   * Processes the API parse queue.
   *
   * @param $verbose
   *   TRUE to print verbose output; FALSE (default) to omit.
   *
   * @return
   *   Number of files parsed.
   */
  function processApiParseQueue($verbose = FALSE) {
    $queues = api_cron_queue_info();
    drupal_alter('cron_queue_info', $queues);

    $queue_name = 'api_parse';
    $info = $queues[$queue_name];
    $function = $info['worker callback'];
    $queue = DrupalQueue::get($queue_name);

    $count = 0;
    while ($item = $queue->claimItem()) {
      if ($verbose) {
        $this->verbose('Processing queue ' . $queue_name . ' - file ' . $item->data['path']);
      }
      $function($item->data);
      $queue->deleteItem($item);
      $count++;
    }

    api_shutdown();
    $this->resetBranchesAndCache();

    return $count;
  }

  /**
   * Returns the approximate number of items in the API parse queue.
   */
  function countParseQueue() {
    $queue = DrupalQueue::get('api_parse');
    return $queue->numberOfItems();
  }

  /**
   * Returns the number of files that have been marked as needing to be parsed.
   */
  function howManyToParse() {
    return db_query('SELECT COUNT(*) from {api_file} WHERE modified < :modified', array(':modified' => 100))->fetchField();
  }
}

/**
 * Provides a base class for testing web pages (user/admin) for the API module.
 */
class ApiWebPagesBaseTest extends ApiTestCase {

  /**
   * Array of branch information for the sample functions branch.
   */
  protected $branch_info;

  /**
   * Array of branch information for the sample PHP functions branch.
   */
  protected $php_branch_info;

  /**
   * Overrides ApiTestCase::setUp().
   *
   * Sets up the sample branch, using the administrative interface, removes the
   * default PHP branch, adds our fake PHP branch, and updates everything.
   */
  public function setUp() {
    $this->baseSetUp();

    // Create a "file" branch with the sample code, from the admin interface.
    $this->branch_info = $this->setUpBranchUI();

    // Remove the default PHP branch, which most tests do not need.
    $this->removePHPBranch();

    // Create a "php" branch with the sample PHP function list, from the admin
    // interface.
    $this->php_branch_info = $this->createPHPBranchUI();

    // Parse the code.
    $this->resetBranchesAndCache();
    api_update_all_branches();
    $this->processApiParseQueue();
  }

  /**
   * Sets up a files branch using the user interface.
   *
   * @param $prefix
   *   Directory prefix to prepend on the data directories.
   * @param $default
   *   TRUE to set this as the default branch; FALSE to not set it as default.
   * @param $info
   *   Array of branch information to override the defaults (see function
   *   code to see what they are). Note that $prefix is applied after this
   *   information is read, and that only one directory and one excluded are
   *   supported in this function. You can set $info['excluded'] to 'none' to
   *   completely omit the excluded directories setting.
   *
   * @return
   *   Array of information (defaults with overrides) used to create the
   *   branch.
   */
  function setUpBranchUI($prefix = '', $default = TRUE, $info = array()) {
    // Set up defaults.

    $info += array(
      'project' => 'test',
      'project_title' => 'Project 6',
      'branch_name' => '6',
      'title' => 'Testing 6',
      'directory' => drupal_get_path('module', 'api') . '/tests/sample',
      'excluded' => drupal_get_path('module', 'api') . '/tests/sample/to_exclude',
    );

    $info['data[directories]'] = $prefix . $info['directory'];
    if ($info['excluded'] != 'none') {
      $info['data[excluded_directories]'] = $prefix . $info['excluded'];
    }
    unset($info['directory']);
    unset($info['excluded']);

    $this->drupalPost('admin/config/development/api/branches/new/files',
      $info,
      t('Save branch')
    );

    if ($default) {
      // Make this the default branch.
      $branches = api_get_branches(TRUE);
      $this_id = 0;
      foreach ($branches as $branch) {
        if ($branch->title == $info['title']) {
          $this_id = $branch->branch_id;
          break;
        }
      }

      $this->drupalPost('admin/config/development/api/branches/list',
        array(
          'default_branch' => $this_id,
        ),
        t('Save changes')
      );
    }

    return $info;
  }

  /**
   * Sets up a PHP functions branch using the sample code, in the admin UI.
   *
   * @return
   *   Information array used to create the branch.
   */
  function createPHPBranchUI() {
    $info = array(
      'branch_name' => 'php2',
      'data[summary]' => url('<front>', array('absolute' => TRUE )) . '/' . drupal_get_path('module', 'api') . '/tests/php_sample/funcsummary.txt',
      'data[path]' => 'http://example.com/function/!function',
    );

    $this->drupalPost('admin/config/development/api/branches/new/php',
      $info,
      t('Save branch')
    );

    return $info;
  }

  /**
   * Asserts that a link exists, with the given URL.
   *
   * @param $label
   *   Label of the link to find.
   * @param $url
   *   URL to match.
   * @param $message_link
   *   Message to use in link exist assertion.
   * @param $message_url
   *   Message to use for URL matching assertion.
   */
  protected function assertLinkURL($label, $url, $message_link, $message_url) {
    // Code follows DrupalWebTestCase::clickLink() and assertLink().
    $links = $this->xpath('//a[text()="' . $label . '"]');
    $this->assert(isset($links[0]), $message_link);
    if (isset($links[0])) {
      $url_target = $this->getAbsoluteUrl($links[0]['href']);
      $this->assertEqual($url_target, $url, $message_url);
    }
  }

  /**
   * Asserts that a link exists, with substring matching on the URL.
   *
   * @param $label
   *   Label of the link to find.
   * @param $url
   *   URL to match. The test passes if $url is a substring of the link's URL.
   * @param $message_link
   *   Message to use in link exist assertion.
   * @param $message_url
   *   Message to use for URL matching assertion.
   * @param $index
   *   Index of the link on the page, like assertLink().
   */
  protected function assertLinkURLSubstring($label, $url, $message_link, $message_url, $index = 0) {
    // Code follows DrupalWebTestCase::clickLink() and assertLink().
    $links = $this->xpath('//a[text()="' . $label . '"]');
    $this->assert(isset($links[$index]), $message_link);
    if (isset($links[$index])) {
      $url_target = $this->getAbsoluteUrl($links[$index]['href']);
      $this->assertTrue(strpos($url_target, $url) !== FALSE, $message_url);
    }
  }

  /**
   * Asserts that the current page's title contains a string.
   *
   * @param $string
   *   String to match in the title.
   * @param $message
   *   Message to print.
   */
  protected function assertTitleContains($string, $message) {
    $title = current($this->xpath('//title'));
    $this->assertTrue(strpos($title, $string) !== FALSE, $message);
  }

  /**
   * Asserts that the current page's URL contains a string.
   *
   * @param $string
   *   String to match in the URL.
   * @param $message
   *   Message to print.
   */
  protected function assertURLContains($string, $message) {
    $this->assertTrue(strpos($this->url, $string) !== FALSE, $message);
  }
}
