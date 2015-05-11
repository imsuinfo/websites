<?php

/**
 * @file
 * Defines McNeese facilities use request problems page functions.
 */

/**
 * @addtogroup mfcs
 * @{
 */

/**
 * Provides the facilities use request manage problems page.
 *
 * @return string
 *   The HTML output representing the page.
 */
function mfcs_management_problems_page() {
  global $base_path;
  global $mfcs_determined;

  $url_arguments = '';
  if (!empty($mfcs_determined['complete'])) {
    $url_arguments .= '?' . $mfcs_determined['complete'];
  }

  $user = cf_current_user();

  mfcs_add_canonical_headers('requests/problems-0');

  $manager = user_access('mfcs manage', $user);
  $administer = user_access('mfcs administer', $user);

  $items_1 = array();
  $items_2 = array();

  $items_array = &$items_1;
  $items_array_current = 0;

  if ($manager) {
    $link = '<h4 class="item-link-wrapper inline-block"><a href="' . $base_path . 'requests/problems-0/users' . $url_arguments . '" class="item-link">User Problems</a></h4>';
    $help = '<div class="item-description inline-block">Manage what users are allowed to approve/deny requests by request type.</div>';
    $items_array[] = array(
      'data' => $link . '<br>' . $help,
      'class' => array(
      ),
    );
  }

  $markup = '<div id="mfcs-management_page-choices" class="mfcs-management_page-choices">';

  if (!empty($items_1)) {
    $item_list_1 = theme('item_list', array('items' => $items_1, 'type' => 'ul'));
    $markup .= '<div class="mfcs-management_page-choices-list_1">' . $item_list_1 . '</div>';
  }

  if (!empty($items_2)) {
    $item_list_2 = theme('item_list', array('items' => $items_2, 'type' => 'ul'));
    $markup .= '<div class="mfcs-management_page-choices-list_2">' . $item_list_2 . '</div>';
  }

  $markup .= '</div>';

  return $markup;
}

/**
 * View and manage problems associated with a specific user.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 */
function mfcs_request_problems_0_users_form($form, $form_state, $page_number = 0) {
  global $base_path;
  global $mfcs_determined;

  $url_arguments = '';
  if (!empty($mfcs_determined['complete'])) {
    $url_arguments .= '?' . $mfcs_determined['complete'];
  }

  if (!cf_is_integer($page_number) || $page_number < 0) {
    $page_number = 0;
  }

  mfcs_add_canonical_headers('requests/problems-0/users/' . $page_number);

  $user = cf_current_user();

  mfcs_include(MFCS_INCLUDE_LIST_OPTIONS);
  mfcs_include(MFCS_INCLUDE_MANAGEMENT);

  // load sorting information
  $order = NULL;
  if (isset($_GET['order'])) {
    $order = strtolower($_GET['order']);
  }

  $sort = NULL;
  if (isset($_GET['sort'])) {
    $sort = strtoupper($_GET['sort']);

    if ($sort != 'ASC' && $sort != 'DESC') {
      $sort = NULL;
    }
  }

  $class_user_id = 'no-script-hidden script-hidden ';
  $class_name = '';
  $class_first_name = '';
  $class_last_name = '';
  $class_problems_account = '';
  $class_problems_forms = '';
  $class_problems_other = '';
  $class_date = '';
  $class_operations = 'no-print ';

  $sorting = array();
  if (is_null($order)) {
    $sorting['user_id'] = 'ASC';
    $order_column = array(0 => strtolower($sorting['user_id']));
    $class_user_id .= 'sorting_1 ';
  }
  else {
    if ($order == 'account id') {
      $sorting['user_id'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(0 => strtolower($sorting[$order]));
      $class_user_id .= 'sorting_1 ';
    }
    elseif ($order == 'account') {
      $sorting['name'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(1 => strtolower($sorting[$order]));
      $class_name .= 'sorting_1 ';
    }
    elseif ($order == 'first name') {
      $sorting['first_name'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(2 => strtolower($sorting[$order]));
      $class_first_name .= 'sorting_1 ';
    }
    elseif ($order == 'last name') {
      $sorting['last_name'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(3 => strtolower($sorting[$order]));
      $class_last_name .= 'sorting_1 ';
    }
    elseif ($order == 'problems account') {
      $sorting['problems_account'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(4 => strtolower($sorting[$order]));
      $class_problems_account .= 'sorting_1 ';
    }
    elseif ($order == 'problems forms') {
      $sorting['problems_forms'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(5 => strtolower($sorting[$order]));
      $class_problems_forms .= 'sorting_1 ';
    }
    elseif ($order == 'problems other') {
      $sorting['problems_other'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(6 => strtolower($sorting[$order]));
      $class_problems_other .= 'sorting_1 ';
    }
    elseif ($order == 'date') {
      $sorting['date'] = is_null($sort) ? 'DESC' : $sort;
      $order_column = array(6 => strtolower($sorting[$order]));
      $class_date .= 'sorting_1 ';
    }
    else {
      $sorting['user_id'] = 'ASC';
      $order_column = array(0 => strtolower($sorting['user_id']));
      $class_user_id .= 'sorting_1 ';
    }
  }

  $rows_per_page = 20;
  $options = array(
    'sort' => $sorting,
    'join' => TRUE,
    'start' => $page_number * $rows_per_page,
    'limit' => $rows_per_page,
    'count' => TRUE,
  );

  $conditions = array();
  $conditions['and_0']['value_only_problems'] = array(
    'field' => 'mu.problems',
    'operator' => MFCS_OPERATOR_NOT_EQUAL,
    'search' => MFCS_USER_PROBLEM_NONE,
  );

  $problems_returned = mfcs_management_search_users($conditions, $options);

  $problems_total = 0;
  if (array_key_exists('count', $problems_returned)) {
    $problems_total = $problems_returned['count'];
  }

  $problems = array();
  if (isset($problems_returned['results']) && is_array($problems_returned['results'])) {
    $problems = $problems_returned['results'];
  }

  // do not provide pages when the page number would bring the user beyond the available items.
  if ($problems_total > 0 && $options['start'] >= $problems_total) {
    drupal_not_found();
    drupal_exit();
    return array();
  }

  $taxonomy_problems = mfcs_management_get_problems_taxonomy();
  $problems_mask = 0;

  $rows = array();
  foreach ($problems as $user_id => $problem) {
    $row = array();

    $problem_account = NULL;
    foreach (array(MFCS_USER_PROBLEM_ACCOUNT_CREATE_BANNER_UNAVAILABLE, MFCS_USER_PROBLEM_ACCOUNT_SYNC_BANNER_UNAVAILABLE, MFCS_USER_PROBLEM_ACCOUNT_LOGIN_PROBLEMS_MULTIPLE) as $taxonomy_id) {
      if (($problem->problems & $taxonomy_id) > 0) {
        if (is_null($problem_account)) {
          $problem_account = '';
        }
        else {
          $problem_account .= ', ';
        }

        $problem_account .= $taxonomy_problems[$taxonomy_id];
      }
    }

    $problem_forms = NULL;
    foreach (array(MFCS_USER_PROBLEM_FORM_ERROR_USER, MFCS_USER_PROBLEM_FORM_ERROR_USER) as $taxonomy_id) {
      if (($problem->problems & $taxonomy_id) > 0) {
        if (is_null($problem_forms)) {
          $problem_forms = '';
        }
        else {
          $problem_forms .= ', ';
        }

        $problem_forms .= $taxonomy_problems[$taxonomy_id];
      }
    }

    $problem_other = NULL;
    foreach (array(MFCS_USER_PROBLEM_SYSTEM, MFCS_USER_PROBLEM_MFCS) as $taxonomy_id) {
      if (($problem->problems & $taxonomy_id) > 0) {
        if (is_null($problem_other)) {
          $problem_other = '';
        }
        else {
          $problem_other .= ', ';
        }

        $problem_other .= $taxonomy_problems[$taxonomy_id];
      }
    }

    // @todo: add date search

    $row['account_id'] = array(
      'data' => $problem->user_id,
      'class' => $class_user_id,
    );

    $row['account'] = array(
      'data' => '<a href="' . $base_path . 'user/' . $problem->user_id . $url_arguments . '" class="user-link">' . check_plain($problem->name) . '</a>' . ' <span class="user-id script-hidden">(id: ' .  $problem->user_id . ')</span>',
      'class' => $class_name,
    );

    $row['first_name'] = array(
      'data' => check_plain($problem->first_name),
      'class' => $class_first_name,
    );

    $row['last_name'] = array(
      'data' => check_plain($problem->last_name),
      'class' => $class_last_name,
    );

    $row['problems_account'] = array(
      'data' => $problem_account,
      'class' => $class_problems_account,
    );

    $row['problems_forms'] = array(
      'data' => $problem_forms,
      'class' => $class_problems_forms,
    );

    $row['problems_other'] = array(
      'data' => $problem_other,
      'class' => $class_problems_other,
    );

    $row['date'] = array(
      'data' => check_plain(date("Y/m/d h:i:s a", $problem->date)),
      'class' => $class_date,
    );

    $row['operations'] = array(
      'data' => '<a href="' . $base_path . 'requests/problems-0/users/user/' . $problem->user_id . $url_arguments . '" class="operation-link operation-link-view">View</a>',
      'class' => $class_operations,
    );

    $rows[] = $row;
  }

  $problem_list = array();
  $problem_list['caption'] = t("User Problems");
  $problem_list['header'] = array(
    'account_id' => array('field' => 'user_id', 'data' => "Account ID", 'align' => 'center', 'class' => array('no-script-hidden', 'script-hidden')),
    'account' => array('field' => 'name', 'data' => "Account", 'align' => 'center'),
    'first_name' => array('field' => 'first_name', 'data' => "First Name", 'align' => 'center'),
    'last_name' => array('field' => 'last_name', 'data' => "Last Name", 'align' => 'center'),
    'problems_account' => array('field' => 'problems', 'data' => "Problems Account", 'align' => 'center'),
    'problems_form' => array('field' => 'problems', 'data' => "Problems Forms", 'align' => 'center'),
    'problems_other' => array('field' => 'problems', 'data' => "Problems Other", 'align' => 'center'),
    'date' => array('field' => 'date', 'data' => "Date", 'align' => 'center'),
    'operations' => array('data' => "Operations", 'align' => 'center', 'class' => array('no-print')),
  );

  $problem_list['attributes'] = array();
  $problem_list['attributes']['id'] = 'mfcs-problem_form';
  $problem_list['attributes']['class'] = array(
    'no_default',
    'mfcs-problem_form',
    'display',
    'hover',
    'order-column',
    'stripe',
    'cell-border',
  );
  $problem_list['attributes']['summary'] = 'Collection of users in which the system has detected problems associated with.';

  $problem_list['rows'] = $rows;
  $problem_list['empty'] = t("There are no users with system detected problems.");

  $datatable_settings = array(
    'sort' => strtolower($sort),
    'order' => $order_column,
    #'stateSave' => TRUE,
    'lengthMenu' => array(
       5 => '5',
      10 => '10',
      20 => '20',
      40 => '40',
      80 => '80',
      -1 => '"All"',
    ),
    'processing' => TRUE,
    'serverSide' => TRUE,
    'ajax' => "'" . $base_path . 'requests/problems-0/users/ajax' . "'",
    'deferRender' => TRUE,
    'columns' => array(
      array('id' => 0, 'column' => 'account_id', 'name' => "Account ID", 'className' => 'column-account_id', 'sanitize' => FALSE, 'visible' => FALSE),
      array('id' => 1, 'column' => 'account', 'name' => "Account", 'className' => 'column-account'),
      array('id' => 2, 'column' => 'first_name', 'name' => "First Name", 'className' => 'column-first_name'),
      array('id' => 3, 'column' => 'last_name', 'name' => "Last Name", 'className' => 'column-last_name'),
      array('id' => 4, 'column' => 'problem_account', 'name' => "Problems Account", 'className' => 'column-problem_account'),
      array('id' => 5, 'column' => 'problem_forms', 'name' => "Problems Forms", 'className' => 'column-problem_forms'),
      array('id' => 6, 'column' => 'problem_other', 'name' => "Problems Other", 'className' => 'column-problem_other'),
      array('id' => 7, 'column' => 'date', 'name' => "Date", 'className' => 'column-date'),
      array('id' => 8, 'column' => NULL, 'name' => "Operations", 'className' => 'column-operations no-print', 'searchable' => FALSE, 'orderable' => FALSE),
    ),
    'displayStart' => $options['start'],
    'pageLength' => $rows_per_page,
    'dom' => 'lfCrtip',
    'KeyTable' => FALSE,
    'oColVis' => array(
      'buttonText' => '"Show / Hide"',
    ),
    'oLanguage' => array(
      'sSearch' => '"Filter: "',
    ),
    'searchDelay' => 1200,
  );

  $navigation_top = '';
  $navigation_bottom = '';
  if ($page_number > 0) {
    if ($page_number == 1) {
      $navigation_top .= '<a href="' . $base_path . 'requests/problems-0/users' . $url_arguments . '" class="table_listing-choice table_listing-choice-previous no-print" title="Previous Page">Prev</a>';
      $navigation_bottom .= '<a href="' . $base_path . 'requests/problems-0/users' . $url_arguments . '" class="table_listing-choice table_listing-choice-previous no-print" title="Previous Page">Prev</a>';
    }
    else {
      $previous_page = $page_number - 1;
      $navigation_top .= '<a href="' . $base_path . 'requests/problems-0/users/' . $previous_page . $url_arguments . '" class="table_listing-choice table_listing-choice-previous no-print" title="Previous Page">Prev</a>';
      $navigation_bottom .= '<a href="' . $base_path . 'requests/problems-0/users/' . $previous_page . $url_arguments . '" class="table_listing-choice table_listing-choice-previous no-print" title="Previous Page">Prev</a>';
    }
  }

  $navigation_start = $options['start'] + 1;
  $navigation_total = $options['start'] + $rows_per_page;

  if ($navigation_total > $problems_total) {
    $navigation_total = $problems_total;
  }

  $navigation_bottom .= '<div class="table_listing-comment">' . "\n";
  $navigation_bottom .= "Showing " . $navigation_start . " to " . $navigation_total . " of " . $problems_total . " entries.\n";
  $navigation_bottom .= '</div>' . "\n";

  if ($problems_total > ($options['start'] + $rows_per_page)) {
    $next_page = $page_number + 1;
    $navigation_top .= '<a href="' . $base_path . 'requests/problems-0/users/' . $next_page . $url_arguments . '" class="table_listing-choice table_listing-choice-next no-print" title="Next Page">Next</a>';
    $navigation_bottom .= '<a href="' . $base_path . 'requests/problems-0/users/' . $next_page . $url_arguments . '" class="table_listing-choice table_listing-choice-next no-print" title="Next Page">Next</a>';
  }

  $form = array();

  $form['problem_list']['#markup'] = '<div class="table_listing-navigator table_listing-navigator-top no-print">' . $navigation_top . '</div>';
  $form['problem_list']['#markup'] .= mcneese_libraries_theme_table($problem_list, $datatable_settings);
  $form['problem_list']['#markup'] .= '<div class="table_listing-navigator table_listing-navigator-bottom">' . $navigation_bottom . '</div>';

  return $form;
}

/**
 * Validation for mfcs_request_problems_0_users_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 *
 * @see: mfcs_request_problems_0_users_form()
 */
function mfcs_request_problems_0_users_form_validate($form, &$form_state) {
  form_set_error('', "Currently this form has nothing to submit.");
}

/**
 * Submit for mfcs_request_problems_0_users_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 *
 * @see: mfcs_request_problems_0_users_form()
 */
function mfcs_request_problems_0_users_form_submit($form, &$form_state) {
  global $mfcs_determined;
  $user = cf_current_user();

  $url_arguments = '';
  if (!empty($mfcs_determined['complete'])) {
    $url_arguments .= '?' . $mfcs_determined['complete'];
  }

  $clicked_id = '';
  if (isset($form_state['triggering_element']['#id'])) {
    $clicked_id = $form_state['triggering_element']['#id'];
  }

  // redirect after submitting.
  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = mfcs_build_redirect_array('requests/problems-0/users');
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * View and manage problems associated with a specific user.
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 */
function mfcs_request_problems_0_user_form($form, $form_state, $problem_user_id = NULL) {
  global $base_path;
  global $mfcs_determined;

  $url_arguments = '';
  if (!empty($mfcs_determined['complete'])) {
    $url_arguments .= '?' . $mfcs_determined['complete'];
  }

  if (!cf_is_integer($problem_user_id) || $problem_user_id < 0) {
    drupal_not_found();
    drupal_exit();
    return array();
  }

  $mfcs_users = mfcs_load_users($problem_user_id);
  if (!is_array($mfcs_users) || !array_key_exists($problem_user_id, $mfcs_users)) {
    drupal_not_found();
    drupal_exit();
    return array();
  }

  $mfcs_user = $mfcs_users[$problem_user_id];
  unset($mfcs_users);
  $problem_user = user_load($problem_user_id);

  mfcs_add_canonical_headers('requests/problems-0/users/user/' . $problem_user_id);

  $user_name = $problem_user->name;
  if (!empty($problem_user->field_user_first_name['und'][0]['safe_value']) && !empty($problem_user->field_user_last_name['und'][0]['safe_value'])) {
    $user_name = $problem_user->field_user_first_name['und'][0]['safe_value'] . ' ' . $problem_user->field_user_last_name['und'][0]['safe_value'];
  }

  drupal_set_title("Problems for " . $user_name . ' (id: ' . $problem_user_id . ')');

  $form = array();

  $form['values'] = array(
    '#tree' => TRUE,
  );

  $form['values']['mfcs_user'] = array(
    '#type' => 'value',
    '#value' => $mfcs_user,
  );

  $form['values']['problem_user'] = array(
    '#type' => 'value',
    '#value' => $problem_user,
  );

  $form['values']['problem_list'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );

  $message = "The system has detected problems for <strong>" . $user_name . "</strong>.<br>\n";
  $message .= "This page provides details of the problems and the ability to reset the problem flags for each of these problems.<br>\n";
  $message .= "Some of these problems can be reset, others cannot.<br>\n";
  $message .= "<br>\n";
  $message .= "The user's information and problem flags were last updated on: <strong>" . date("Y/m/d h:i:s a", $mfcs_user->date) . "</strong>.<br>\n";
  $message .= "<br>\n";

  $form['message'] = array(
    '#markup' => '<div class="problem-message">' . $message . '</div>' . "\n",
  );

  $form['problems'] = array(
    '#tree' => TRUE,
  );

  $resettable_problems = FALSE;


  $form['values']['problem_list']['#value'] = array(
    MFCS_USER_PROBLEM_SYSTEM => array(
      'label' => "Generic System Related Problems",
      'explanation' =>
        "This flag gets set when a generic system error is detected while the specified user was accessing this system.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => TRUE,
    ),
    MFCS_USER_PROBLEM_MFCS => array(
      'label' => "Generic Facilities Use Problems",
      'explanation' =>
        "This flag gets set when an error is detected for the facilities use specific functionality while the specified user was accessing this system.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => TRUE,
    ),
    MFCS_USER_PROBLEM_ACCOUNT_CREATE_BANNER_UNAVAILABLE => array(
      'label' => "Banner Unavailable During Account Creation",
      'explanation' =>
        "The banner system was unavailable during account creation (first login) and the appropriate access controls could not be auto-assigned.<br>\n" .
        "The next time the user logs in this flag will be cleared and the account will load the appropriate banner roles.<br>\n" .
        "The user must log out before the banner information can be updated.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => FALSE,
    ),
    MFCS_USER_PROBLEM_ACCOUNT_SYNC_BANNER_UNAVAILABLE => array(
      'label' => "Banner Unavailable During Login Synchronizeation",
      'explanation' =>
        "The banner system was unavailable during the login process and the account could not be updated if necessary.<br>\n" .
        "This flag will be reset the next time the user logs in and banner is available.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => FALSE,
    ),

    MFCS_USER_PROBLEM_ACCOUNT_LOGIN_PROBLEMS_MULTIPLE => array(
      'label' => "Multiple Login Problems",
      'explanation' =>
        "This flag gets set when a user has multiple problems logging in or multiple failed attempts logging in.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => TRUE,
    ),
    MFCS_USER_PROBLEM_FORM_ERROR_USER => array(
      'label' => "Facilities Use Form Problems",
      'explanation' =>
        "This flag gets set when an error is detected while the users is processing a form.<br>\n" .
        "This could be something as simple as not adding in the date fields or something technical such as a PHP error.<br>\n" .
        "This flag refers to facilities use specific functionality.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => TRUE,
    ),
    MFCS_USER_PROBLEM_FORM_ERROR_SYSTEM => array(
      'label' => "System Form Problems",
      'explanation' =>
        "This flag gets set when an error is detected while the users is processing a form.<br>\n" .
        "This flag refers to the drupal core form functionality and not the facilities use specific functionality.<br>\n" .
        "<br>\n" .
        "This system currently does not provide the details of these problems, but in the future it may.<br>\n",
      'resettable' => TRUE,
    ),
  );

  foreach ($form['values']['problem_list']['#value'] as $problem_id => $problem_settings) {
    if (($mfcs_user->problems & $problem_id) > 0) {
      $form['problems'][$problem_id] = array(
        '#id' => 'problem-fieldset-' . $problem_id,
        '#type' => 'fieldset',
        '#title' => $problem_settings['label'] . " Detected",
        '#tree' => TRUE,
        '#attributes' => array(
          'class' => array(
            'problem-fieldset',
            'problem-fieldset-' . $problem_id,
          ),
        ),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      if ($problem_settings['resettable']) {
        $resettable_problems = TRUE;

        $form['problems'][$problem_id]['reset'] = array(
          '#id' => 'problem-field-reset-' . $problem_id,
          '#type' => 'checkbox',
          '#title' => "Reset " . $problem_settings['label'] . " Flag",
          '#default_value' => FALSE,
          '#required' => FALSE,
          '#attributes' => array(
            'class' => array(
              'problem-field-reset',
              'problem-field-reset-' . $problem_id,
            ),
          ),
        );
      }

      $form['problems'][$problem_id]['explanation'] = array(
        '#markup' => '<div class="problem-explanation problem-explanation-' . $problem_id . '">' . $problem_settings['explanation'] . '</div>' . "\n",
      );
    }
  }

  if ($resettable_problems) {
    $form['problems']['reset'] = array(
      '#id' => 'submit-problems-reset',
      '#type' => 'submit',
      '#default_value' => t("Reset Problems"),
      '#attributes' => array(
        'class' => array(
          'problem-submit-reset',
        ),
      ),
    );
  }

  return $form;
}

/**
 * Validation for mfcs_request_problems_0_user_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 *
 * @see: mfcs_request_problems_0_user_form()
 */
function mfcs_request_problems_0_user_form_validate($form, &$form_state) {
  $clicked_id = '';
  if (isset($form_state['triggering_element']['#id'])) {
    $clicked_id = $form_state['triggering_element']['#id'];
  }

  $reset_problems = array();
  if (isset($form_state['values']['problems'])) {
    $reset_problems = &$form_state['values']['problems'];
  }

  $reset_something = FALSE;
  foreach ($form['values']['problem_list']['#value'] as $problem_id => $problem_settings) {
    if (!$problem_settings['resettable'] || !array_key_exists($problem_id, $reset_problems) || !array_key_exists('reset', $reset_problems[$problem_id])) {
      continue;
    }

    if ($reset_problems[$problem_id]['reset'] == 1) {
      $reset_something = TRUE;
    }
  }

  if (!$reset_something) {
    form_set_error('', "You have not specified anything to reset.");
    return;
  }
}

/**
 * Submit for mfcs_request_problems_0_user_form().
 *
 * @param array $form
 *   The form array as defined by drupals form api.
 * @param array $form_state
 *   The form state array as defined by drupals form api
 *
 * @see: mfcs_request_problems_0_user_form()
 */
function mfcs_request_problems_0_user_form_submit($form, &$form_state) {
  global $mfcs_determined;
  $user = cf_current_user();

  $url_arguments = '';
  if (!empty($mfcs_determined['complete'])) {
    $url_arguments .= '?' . $mfcs_determined['complete'];
  }

  $clicked_id = '';
  if (isset($form_state['triggering_element']['#id'])) {
    $clicked_id = $form_state['triggering_element']['#id'];
  }

  $reset_problems = array();
  if (isset($form_state['values']['problems'])) {
    $reset_problems = &$form_state['values']['problems'];
  }

  $problems = $form['values']['mfcs_user']['#value']->problems;
  foreach ($form['values']['problem_list']['#value'] as $problem_id => $problem_settings) {
    if (!$problem_settings['resettable'] || !array_key_exists($problem_id, $reset_problems) || !array_key_exists('reset', $reset_problems[$problem_id])) {
      continue;
    }

    if ($reset_problems[$problem_id]['reset'] == 1) {
      $problems -= $problem_id;
    }
  }

  $results = mfcs_save_user($form['values']['mfcs_user']['#value']->user_id, array('problems' => $problems));

  if ($results === FALSE) {
    form_set_error('form', "An error occurred while trying to reset the user problem flags. Please contact the support staff.");
    watchdog(MFCS_WATCHDOG_ID, "An error occured while trying to reset the user problem flags, user: @user_id, problems: @problem_code.", array('@user_id' => $form['values']['mfcs_user']['#value']->user_id, $problems), WATCHDOG_ERROR);

    $form_state['rebuild'] = TRUE;
    $form_state['redirect'] = FALSE;
    $form_state['submitted'] = FALSE;

    return;
  }

  // redirect after submitting.
  if (empty($form_state['values']['redirect_to'])) {
    $form_state['redirect'] = mfcs_build_redirect_array('requests/problems-0/users');
  }
  else {
    $form_state['redirect'] = $form_state['values']['redirect_to'];
  }
}

/**
 * @} End of '@addtogroup mfcs'.
 */