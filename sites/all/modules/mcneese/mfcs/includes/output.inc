<?php

/**
 * @file
 * Defines McNeese facilities use output functions.
 */

/**
 * @addtogroup mfcs
 * @{
 */


/**
 * Build a month calendar display.
 *
 * @param array $items
 *   An array of request items to link to.
 * @param int $month_start
 *   Unix timestamp for the start of the month.
 * @param int $month_stop
 *   Unix timestamp for the first day of the next month.
 * @param int $absolute_start
 *   Unix timestamp for the first day of the week in which the first day of
 *   the month may be found.
 * @param int $absolute_stop
 *   Unix timestamp for the last day of the week in which the last day of
 *   the month may be found.
 * @param string $title
 *   An HTML title for describing the calendar.
 * @param string $id
 *   (optional) A unique HTML id to associate with the calendar.
 *
 * @return string|bool
 *   The generate HTML markup or FALSE on error.
 */
function mfcs_build_calendar_month_markup($items, $month_start, $month_stop, $absolute_start, $absolute_stop, $title, $id = NULL) {
  if (!is_array($items)) {
    cf_error::invalid_array('items');
    return FALSE;
  }

  if (!cf_is_integer($month_start)) {
    cf_error::invalid_integer('month_start');
    return FALSE;
  }

  if (!cf_is_integer($month_stop)) {
    cf_error::invalid_integer('month_stop');
    return FALSE;
  }

  if (!cf_is_integer($absolute_start)) {
    cf_error::invalid_integer('absolute_start');
    return FALSE;
  }

  if (!cf_is_integer($absolute_stop)) {
    cf_error::invalid_integer('absolute_stop');
    return FALSE;
  }

  if (!is_string($title)) {
    cf_error::invalid_string('title');
    return FALSE;
  }

  $id_string = "";
  if (!empty($id)) {
    if (!is_string($id)) {
      cf_error::invalid_string('id');
      return FALSE;
    }

    $id_string = ' id="' . check_plain($id) . '"';
  }

  global $base_path;

  $week_start = 0;
  $week_stop = 6;

  // for ISO-8601.
  #$week_start = 1;
  #$week_stop = 7;

  $days_of_week = array(
    0 => 'Sunday',
    1 => 'Monday',
    2 => 'Tuesday',
    3 => 'Wednesday',
    4 => 'Thursday',
    5 => 'Friday',
    6 => 'Saturday',
    7 => 'Sunday', // In the case of ISO-8601, Sunday = 7.
  );


  // begin wrappers
  $markup = '<div' . $id_string . ' class="calendar-month-wrapper"><div class="calendar-month">';


  // title
  $markup .= '<div class="calendar-title">' . $title . '</div>';


  // navigation
  $last_month = strtotime('midnight first day of last month', $month_start);
  $next_month = strtotime('midnight first day of next month', $month_start);

  $url_base_month = $base_path . 'requests/calendar-0/month/';
  $url_base_day = $base_path . 'requests/calendar-0/day/';

  $last_url = $url_base_month . date('Y', $last_month) . '/' . strtolower(date('F', $last_month));
  $next_url = $url_base_month . date('Y', $next_month) . '/' . strtolower(date('F', $next_month));

  $markup .= '<div class="calendar-navigation">';
  $markup .= '<div class="calendar-navigation-previous">';
  $markup .= '<a href="' . $last_url . '">Previous</a>';
  $markup .= '</div>';
  $markup .= '<div class="calendar-navigation-next">';
  $markup .= '<a href="' . $next_url . '">Next</a>';
  $markup .= '</div>';
  $markup .= '</div>';


  // header
  $markup .= '<div class="calendar-header-wrapper"><div class="calendar-header">';
  $markup .= '<div class="calendar-week-wrapper week_number-0"><div class="calendar-week week_number-0">';
  $week_count = $week_start;
  while ($week_count <= $week_stop) {
    $markup .= '<div class="calendar-day-wrapper weekday-' . $week_count . '">';
    $markup .= '<div class="calendar-day weekday-' . $week_count . '">' . $days_of_week[$week_count] . '</div>';
    $markup .= '</div>';
    $week_count++;
  }
  $markup .= '</div></div></div></div>';


  // body
  $markup .= '<div class="calendar-body-wrapper"><div class="calendar-body">';
  $current_date = $absolute_start;
  $week_first = date('W', $current_date);
  $week_count = $week_start;

  $weeks_processed = array();
  while ($current_date < $absolute_stop) {
    $current_year = date('Y', $current_date);
    $current_week = date('W', $current_date);
    $current_month = date('n', $current_date);
    $current_month_name = strtolower(date('F', $current_date));
    $current_day = date('j', $current_date);
    $current_month_day = $current_month . '-' . $current_day;

    if ($week_count == $week_start && !isset($weeks_processed[$week_first])) {
      $markup .= '<div class="calendar-week-wrapper week_number-' . $current_week . '"><div class="calendar-week week_number-' . $current_week . '">';
      $weeks_processed[$week_first] = TRUE;
    }

    $extra_class = ' month-current';
    if ($current_date < $month_start) {
      $extra_class = ' month-previous';
    }
    elseif ($current_date >= $month_stop) {
      $extra_class = ' month-next';
    }

    $markup .= '<div class="calendar-day-wrapper weekday-' . $week_count . ' day-' . $current_day . ' month-' . $current_month . $extra_class . '">';
    $markup .= '<div class="calendar-day weekday-' . $week_count . ' day-' . $current_day . ' month-' . $current_month . $extra_class . '">';
    $markup .= '<div class="calendar-label"><a href="' . $url_base_day . $current_year . '/' . $current_month_name . '/' . $current_day. '">' . $current_day . '</a></div>';
    if (!empty($items[$current_month_day])) {
      foreach ($items[$current_month_day] as $item_id => $item) {
        $markup .= '<div class="calendar-item"><a class="calendar-item-link" href="' . $item['href'] . '" title="' . $item['tooltip'] . '">' . $item['title'] . '</a></div>';
      }
    }
    $markup .= '</div></div>';

    $current_date = strtotime('midnight tomorrow', $current_date);

    $week_count++;
    if ($week_count > $week_stop) {
      $markup .= '</div></div>';

      $week_first = date('W', $current_date);
      $week_count = $week_start;
    }
  }
  $markup .= '</div></div>';


  // end wrappers (calendar-month)
  $markup .= '</div>';


  // iCal links
  $year = date('Y', $month_start);
  $month = date('F', $month_start);
  $markup .= '<div class="calendar-ical-links no-print">' . "\n";
  $markup .= '<h3 class="calendar-ical-title">iCal Download Options</h3>' . "\n";
  $markup .= '  <ul class="calendar-ical-links-list">' . "\n";
  $markup .= '    <li class="calendar-ical-links-list-item calendar-ical-links-list-item-month"><div class="calendar-ical-links-list-item-label">Month:</div> <a href="' . $base_path . 'requests/ical-0/month/' . $year . '/' . $month . '" title="Download iCal for the month of ' . $month . ' ' . $year . '">' . $month . ' ' . $year . '</a></li>' . "\n";
  $markup .= '    <li class="calendar-ical-links-list-item calendar-ical-links-list-item-year"><div class="calendar-ical-links-list-item-label">Year:</div> <a href="' . $base_path . 'requests/ical-0/year/' . $year . '" title="Download iCal for the year ' . $year . '">' . $year . '</a></li>' . "\n";
  $markup .= '  </ul>' . "\n";
  $markup .= '</div>' . "\n";


  // end wrapper (calendar-month-wrapper)
  $markup .= '</div>';

  return $markup;
}

/**
 * Build a day calendar display.
 *
 * @param array $items
 *   An array of request items to link to.
 * @param int $day_start
 *   Unix timestamp for the start of the time interval.
 *   A >= operator is used on this.
 * @param int $day_stop
 *   Unix timestamp for the stop of the time interval.
 *   A < operator is used on this.
 * @param string $title
 *   An HTML title for describing the calendar.
 * @param string $id
 *   (optional) A unique HTML id to associate with the calendar.
 *
 * @return string|bool
 *   The generate HTML markup or FALSE on error.
 */
function mfcs_build_calendar_day_markup($items, $day_start, $day_stop, $title, $id = NULL) {
  if (!is_array($items)) {
    cf_error::invalid_array('items');
    return FALSE;
  }

  if (!cf_is_integer($day_start)) {
    cf_error::invalid_integer('day_start');
    return FALSE;
  }

  if (!cf_is_integer($day_stop)) {
    cf_error::invalid_integer('day_stop');
    return FALSE;
  }

  if (!is_string($title)) {
    cf_error::invalid_string('title');
    return FALSE;
  }

  $id_string = "";
  if (!empty($id)) {
    if (!is_string($id)) {
      cf_error::invalid_string('id');
      return FALSE;
    }

    $id = check_plain($id);
    $id_string = ' id="' . $id . '"';
  }

  global $base_path;

  $module_path = drupal_get_path('module', 'mfcs');

  mfcs_include(MFCS_INCLUDE_LIST_OPTIONS);

  $request_type_options = mfcs_get_request_type_list_options();

  // prepare the inline CSS markup
  $css_screen = '';
  $css_portrait = '@media (orientation:portrait) {';
  $css_landscape = '@media (orientation:landscape) {';

  // begin wrappers (main)
  $markup = '<div' . $id_string . ' class="calendar-day-wrapper">';

  // begin wrappers (All Day)
  $markup .= '<div class="calendar-day calendar-all_day">';


  // title (All Day)
  $markup .= '<div class="calendar-title">' . $title . '</div>';


  // navigation
  $last_day = strtotime('midnight yesterday', $day_start);
  $next_day = strtotime('midnight tomorrow', $day_start);

  $url_base_day = $base_path . 'requests/calendar-0/day/';

  $last_url = $url_base_day . date('Y', $last_day) . '/' . strtolower(date('F', $last_day)) . '/' . date('d', $last_day);
  $next_url = $url_base_day . date('Y', $next_day) . '/' . strtolower(date('F', $next_day)) . '/' . date('d', $next_day);

  $markup .= '<div class="calendar-navigation">';
  $markup .= '<div class="calendar-navigation-previous">';
  $markup .= '<a href="' . $last_url . '">Previous</a>';
  $markup .= '</div>';
  $markup .= '<div class="calendar-navigation-next">';
  $markup .= '<a href="' . $next_url . '">Next</a>';
  $markup .= '</div>';
  $markup .= '</div>';


  // header (All Day)
  $markup .= '<div class="calendar-header-wrapper calendar-all_day"><div class="calendar-header">';
  $markup .= '<div class="calendar-day-wrapper"><div class="calendar-day">';
  $markup .= "All Day";
  $markup .= '</div></div></div></div>';


  // body (All Day)
  $markup .= '<div class="calendar-body-wrapper calendar-all_day"><div class="calendar-body">';
  if (empty($items['all_day'])) {
    $markup .= '<div class="calendar-item-wrapper even row-0"><div class="calendar-item">';
    $markup .= "No requests found.";
    $markup .= '</div></div>';
  }
  else {
    $row_even = TRUE;
    $row_count = 0;
    foreach ($items['all_day'] as $floor_time => $all_day) {
      $row_class = 'row-' . $row_count . ' ';
      if ($row_even) {
        $row_class .= 'even ';
      }
      else {
        $row_class .= 'odd ';
      }

      $markup .= '<div class="calendar-time_slot-wrapper ' . $row_class . '"><div class="calendar-time_slot">';
      $markup .= '<div class="calendar-time_slot-request-wrapper"><div class="calendar-time_slot-request">';

      $item_even = TRUE;
      $item_count = 0;
      $left_offset = ($row_even ? 4 : 16);
      foreach ($all_day as $item_id => $item) {
        $item_class = "";

        $item_class = 'row-' . $item_count . ' ';
        if ($item_even) {
         $item_class .= 'even ';
        }
        else {
          $item_class .= 'odd ';
        }

        $item_class .= 'location-' . $item['location'] . ' ';
        $item_class .= 'building-' . $item['building'] . ' ';
        $item_class .= 'room-' . $item['room'] . ' ';
        $item_class .= 'type-' . $item['type'] . ' ';

        $markup .= '<div class="calendar-item-wrapper ' . $item_class . '"><div class="calendar-item">';
        $markup .= '<div class="calendar-item-link-wrapper"><a class="calendar-item-link" href="' . $item['href'] . '" title="' . $item['tooltip'] . '">' . $item['title'] . '</a></div>';
        $markup .= '<div class="calendar-item-location-wrapper"><div class="calendar-item-location">' . $item['building_name'] . ' ' . $item['room_name'] . '</div></div>';

        if (!empty($request_type_options[$item['type']])) {
          $markup .= '<div class="calendar-item-type-wrapper"><div class="calendar-item-type">' . $request_type_options[$item['type']] . '</div></div>';
        }

        $markup .= '</div></div>';

        $item_count++;
        $item_even = !$item_even;
      }
    }

    $markup .= '</div></div></div></div>';

    $row_count++;
    $row_even = !$row_even;
  }
  $markup .= '</div></div>';

  // end wrappers (All Day)
  $markup .= '</div>';


  // begin wrappers (Range)
  $markup .= '<div class="calendar-day calendar-range">';

  // title (Range)
  $markup .= '<div class="calendar-title element-invisible">' . $title . '</div>';


  // header (Range)
  $markup .= '<div class="calendar-header-wrapper calendar-range"><div class="calendar-header">';
  $markup .= '<div class="calendar-time_slot-wrapper"><div class="calendar-time_slot">';
  $markup .= '<div class="calendar-time_slot-label-wrapper"><div class="calendar-time_slot-label">' . "Time" . '</div></div>';
  $markup .= '<div class="calendar-time_slot-request-wrapper"><div class="calendar-time_slot-request">' . "Requests" . '</div></div>';
  $markup .= '</div></div></div></div>';


  // body (Range)
  $markup .= '<div class="calendar-body-wrapper calendar-range"><div class="calendar-body">';

  $populated = array();
  $row_even = TRUE;
  $row_count = 0;
  foreach ($items['range'] as $floor_time => $range) {
    $row_class = 'row-' . $row_count . ' ';
    if ($row_even) {
      $row_class .= 'even ';
    }
    else {
      $row_class .= 'odd ';
    }

    $time_slot_id = 'time_slot-' . date('h_00a', $floor_time);

    $markup .= '<div id="' . $time_slot_id . '"class="calendar-time_slot-wrapper ' . $row_class . '"><div class="calendar-time_slot">';
    $markup .= '<div class="calendar-time_slot-label-wrapper"><div class="calendar-time_slot-label">' . date("h:i a", $floor_time) . '</div></div>';
    $markup .= '<div class="calendar-time_slot-request-wrapper"><div class="calendar-time_slot-request">';

    if (!array_key_exists($floor_time, $populated)) {
      $populated[$floor_time] = array();
    }

    $item_even = TRUE;
    $item_count = 0;
    $item_total = count($populated[$floor_time]) + count($range);
    $item_width = 178;

    $left_offset = 0;
    $left_offset_min = 30;
    $left_offset_max = 186;

    $left_offset_increment = $left_offset_min;
    $left_offset_increment_portrait = $left_offset_min;
    if ($item_total > 0) {
      $left_offset_increment = floor(790 / $item_total);
      $left_offset_increment_portrait = floor(590 / $item_total);

      if ($left_offset_increment < $left_offset_min) {
        $left_offset_increment = $left_offset_min;
      }
      elseif ($left_offset_increment > $left_offset_max) {
        $left_offset_increment = $left_offset_max;
      }

      if ($left_offset_increment_portrait < $left_offset_min) {
        $left_offset_increment_portrait = $left_offset_min;
      }
      elseif ($left_offset_increment_portrait > $left_offset_max) {
        $left_offset_increment_portrait = $left_offset_max;
      }
    }


    foreach ($range as $item_id => $item) {
      $time = strtotime('+1 hours', $floor_time);
      $stuck_slot = NULL;
      do {
        if (!array_key_exists($time, $populated)) {
          $populated[$time] = array();
        }

        if (is_null($stuck_slot)) {
          $max_position = count($populated[$time]);
          for ($i = 0; $i <= $max_position; $i++) {
            if (!isset($populated[$time][$i])) {
              $populated[$time][$i] = $item_id;
              $stuck_slot = $i;
              break;
            }
          }
          unset($max_position);
          unset($i);
        }
        else {
          $i = 0;
          while ($i < $stuck_slot) {
            if (!array_key_exists($i, $populated[$time])) {
              $populated[$time][$i] = NULL;
            }

            $i++;
          }

          $populated[$time][$stuck_slot] = $item_id;
          unset($i);
        }

        $time = strtotime('+1 hours', $time);
      } while ($item['time_stop'] > $time);

      $left_offset = $left_offset_increment * $stuck_slot;
      $left_offset_portrait = $left_offset_increment_portrait * $stuck_slot;

      $item_class = "";

      $item_class = 'row-' . $item_count . ' ';
      if ($item_even) {
       $item_class .= 'even ';
      }
      else {
        $item_class .= 'odd ';
      }

      $item_class .= 'item-zindex-normal ';
      $item_class .= 'location-' . $item['location'] . ' ';
      $item_class .= 'building-' . $item['building'] . ' ';
      $item_class .= 'room-' . $item['room'] . ' ';
      $item_class .= 'type-' . $item['type'] . ' ';

      // split the time difference into height where each minute is equal to 1 pixel(s).
      $item_minutes = (($item['time_stop'] - $item['time_start']) / 60);
      $item_hours = $item_minutes / 60;

      // each minute should represent (90/60) pixels or 1.5 pixels.
      $item_height = $item_minutes * 1.5;

      // for each hour add the appropriate margin + padding + border
      $item_height += 5 * $item_hours;

      // subtract the margin + padding + border.
      $item_height -= 2;

      if ($item_height < 54) {
        $item_height = 54;
      }

      // when there are more than 30 characters, shrink the box size by the expected line height of 15 pixels.
      if (strlen($item['title']) > 30) {
        $item_height -= 15;
      }

      // designate the top offset based on when the request starts relative to the top of the hour.
      $top_offset = ($item['time_start'] - $floor_time) / 60;
      if ($top_offset < 0) {
        $top_offset = 0;
      }

      if (is_null($id)) {
        $item_id = 'calendar-item-' . $item_id;
        $item_id_parent = '';
      }
      else {
        $item_id = $id . '-calendar-item-' . $item_id;
        $item_id_parent = '#' . $id . ' ';
      }

      $css_screen .= $item_id_parent . '#' . $item_id . ' {' . "\n";
      $css_screen .= ' top: ' . $top_offset . 'px;' . "\n";
      $css_screen .= ' left: ' . $left_offset . 'px;' . "\n";
      $css_screen .= ' min-height: ' . $item_height . 'px;' . "\n";
      $css_screen .= ' width: ' . $item_width . 'px;' . "\n";
      $css_screen .= ' min-width: ' . $item_width . 'px;' . "\n";
      $css_screen .= ' max-width: ' . $item_width . 'px;' . "\n";
      $css_screen .= '}' . "\n";

      $css_portrait .= $item_id_parent . '#' . $item_id . ' {' . "\n";
      $css_portrait .= ' top: ' . $top_offset . 'px;' . "\n";
      $css_portrait .= ' left: ' . $left_offset_portrait . 'px;' . "\n";
      $css_portrait .= ' min-height: ' . $item_height . 'px;' . "\n";
      $css_portrait .= ' width: ' . $item_width . 'px;' . "\n";
      $css_portrait .= ' min-width: ' . $item_width . 'px;' . "\n";
      $css_portrait .= ' max-width: ' . $item_width . 'px;' . "\n";
      $css_portrait .= '}' . "\n";

      $css_landscape .= $item_id_parent . '#' . $item_id . ' {' . "\n";
      $css_landscape .= ' top: ' . $top_offset . 'px;' . "\n";
      $css_landscape .= ' left: ' . $left_offset . 'px;' . "\n";
      $css_landscape .= ' min-height: ' . $item_height . 'px;' . "\n";
      $css_landscape .= ' width: ' . $item_width . 'px;' . "\n";
      $css_landscape .= ' min-width: ' . $item_width . 'px;' . "\n";
      $css_landscape .= ' max-width: ' . $item_width . 'px;' . "\n";
      $css_landscape .= '}' . "\n";

      $markup .= '<div id="' . $item_id . '" class="calendar-item-wrapper ' . $item_class . '"><div class="calendar-item">';
      $markup .= '<div class="calendar-item-link-wrapper"><a class="calendar-item-link" href="' . $item['href'] . '" title="' . $item['tooltip'] . '">' . $item['title'] . '</a></div>';
      $markup .= '<div class="calendar-item-time-wrapper"><div class="calendar-item-time">' . date("h:i a", $item['time_start']) . " to " . date("h:i a", $item['time_stop']) . '</div></div>';
      $markup .= '<div class="calendar-item-location-wrapper"><div class="calendar-item-location">' . $item['building_name'] . ' ' . $item['room_name'] . '</div></div>';

      if (!empty($request_type_options[$item['type']])) {
        $markup .= '<div class="calendar-item-type-wrapper"><div class="calendar-item-type">' . $request_type_options[$item['type']] . '</div></div>';
      }

      $markup .= '</div></div>';

      $item_count++;
      $item_even = !$item_even;
    }

    $markup .= '</div></div></div></div>';

    $row_count++;
    $row_even = !$row_even;
  }

  // end body (Range)
  $markup .= '</div></div>';


  // end wrappers (Range)
  $markup .= '</div>';


  // iCal links
  $year = date('Y', $day_start);
  $month = date('F', $day_start);
  $day = date('d', $day_start);
  $markup .= '<div class="calendar-ical-links no-print">' . "\n";
  $markup .= '<h3 class="calendar-ical-title">iCal Download Options</h3>' . "\n";
  $markup .= '  <ul class="calendar-ical-links-list">' . "\n";
  $markup .= '    <li class="calendar-ical-links-list-item calendar-ical-links-list-item-day"><div class="calendar-ical-links-list-item-label">Day:</div> <a href="' . $base_path . 'requests/ical-0/day/' . $year . '/' . $month . '/' . $day . '" title="Download iCal for ' . $month . ' ' . $day . ', ' . $year . '">' . $month . ' ' . $day . ', ' . $year . '</a></li>' . "\n";
  $markup .= '    <li class="calendar-ical-links-list-item calendar-ical-links-list-item-month"><div class="calendar-ical-links-list-item-label">Month:</div> <a href="' . $base_path . 'requests/ical-0/month/' . $year . '/' . $month . '" title="Download iCal for the month of ' . $month . ' ' . $year . '">' . $month . ' ' . $year . '</a></li>' . "\n";
  $markup .= '    <li class="calendar-ical-links-list-item calendar-ical-links-list-item-year"><div class="calendar-ical-links-list-item-label">Year:</div> <a href="' . $base_path . 'requests/ical-0/year/' . $year . '" title="Download iCal for the year ' . $year . '">' . $year . '</a></li>' . "\n";
  $markup .= '  </ul>' . "\n";
  $markup .= '</div>' . "\n";


  // end wrappers (main)
  $markup .= '</div>';

  // add the inline css
  $css_portrait .= '}';
  $css_landscape .= '}';

  drupal_add_css($css_screen, array('type' => 'inline', 'group' => CSS_DEFAULT, 'media' => 'screen', 'preprocess' => FALSE));
  drupal_add_css($css_portrait, array('type' => 'inline', 'group' => CSS_DEFAULT, 'media' => 'print', 'preprocess' => FALSE));
  drupal_add_css($css_landscape, array('type' => 'inline', 'group' => CSS_DEFAULT, 'media' => 'print', 'preprocess' => FALSE));

  return $markup;
}


/**
 * Renders HTML for a self-contained request view page useful for printing.
 *
 * @param string $html
 *   The HTML markup to convert to a PDF.
 * @param array $parameters
 *   (optional) Change the default behavior on how the page is generated.
 *
 * @return string|bool
 *   Renderred markup on success, FALSE otherwise.
 */
function mfcs_output_print_page_view($html, $parameters = array()) {
  if (!is_string($html)) {
    cf_error::invalid_string('html');
    return FALSE;
  }

  if (!is_array($parameters)) {
    cf_error::invalid_array('parameters');
    return FALSE;
  }

  global $base_path;

  // determine the number of directories in the base path so that they can be stripped out properly.
  $base_path_count = count(explode('/', $base_path));
  $css_path_0 = $base_path_count;
  $css_path_1 = $base_path_count + 1;
  $css_path_2 = $base_path_count + 2;
  $css_path_3 = $base_path_count + 3;

  // Reconstruct the HTML output.
  $cf_dom = new cf_dom(TRUE, TRUE, TRUE, $html);

  $meta_tags = $cf_dom->get_body()->getElementsByTagName('meta');
  $style_tags = $cf_dom->get_body()->getElementsByTagName('style');
  $link_tags = $cf_dom->get_body()->getElementsByTagName('link');
  $title_tags = $cf_dom->get_body()->getElementsByTagName('title');
  $main_tag = $cf_dom->get_dom()->getElementById('mcneese-content-main');

  $head_markup = '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">';
  $body_markup = '';
  $css_markup = '';

  // meta tags
  foreach ($meta_tags as $tag) {
    $head_markup .= $cf_dom->get_dom()->saveHTML($tag) . "\n";
  }

  // style tags
  $head_markup = '';
  foreach ($style_tags as $tag) {
    // drupal generates css styles using @import, remove all of these.
    $matched = array();
    $matches = preg_match_all('/@import url\("([^"]*)/i', $tag->nodeValue, $matched);

    $media = NULL;
    if ($tag->hasAttribute('media')) {
      $media = $tag->getAttribute('media');
    }
    if (empty($media)) {
      $media = 'all';
    }

    if ($matches > 0) {
      if ($media == 'all' || $media == 'print,embossed,tv' || $media == 'print') {
        $match = array_pop($matched);

        foreach ($match as $m) {
          // skip past css uneeded css
          if ($media == 'all') {
            $parts = explode('/', $m);
            if (!isset($parts[$css_path_1])) {
              continue;
            }
            if ($parts[$css_path_0] == 'system') {
            }

            if (isset($parts[$css_path_2])) {
              if ($parts[$css_path_2] == 'mcneese') {
              }
              elseif ($parts[$css_path_2] == 'mcneese_fcs') {
              }
              elseif ($parts[$css_path_2] == 'mfcs') {
              }
              else {
                continue;
              }
            }
          }

          $fixed_path = preg_replace('/\?.*$/i', '', $m);
          $parts = explode('/', $fixed_path);

          $bpc = 2;
          while ($bpc <= $base_path_count) {
            array_shift($parts);
            $bpc++;
          }

          $fixed_path = implode('/', $parts);
          $content = drupal_load_stylesheet(DRUPAL_ROOT . '/' . $fixed_path);

          if (!empty($content)) {
            $css_markup .= $content;
          }

          unset($content);
          unset($parts);
          unset($fixed_path);
        }
      }
    }
    else {
      $head_markup .= $cf_dom->get_dom()->saveHTML($tag) . "\n";
    }
  }

  if (!empty($css_markup)) {
    $head_markup .= '<style>' . $css_markup . '</style>';
  }

  // link tags
  foreach ($link_tags as $tag) {
    $head_markup .= $cf_dom->get_dom()->saveHTML($tag) . "\n";
  }

  // title tags
  if (empty($parameters['page_title'])) {
    $page_title = "Facilities Use Request";
  }
  else {
    $page_title = $parameters['page_title'];
  }

  if (empty($parameters['title'])) {
    if (empty($parameters['page_title'])) {
      $head_markup .= '<title>Request</title>';
    }
    else {
      $head_markup .= '<title>' . $page_title . '</title>';
    }
  }
  else {
    $head_markup .= '<title>' . $parameters['title'] . '</title>';
  }

  $body_markup .= $cf_dom->get_dom()->saveHTML($main_tag) . "\n";

  $body_class = 'mcneese';
  $cf = &drupal_static('cf_theme_get_variables', array());
  if (isset($cf['markup_css']['body']['class'])) {
    $body_class .= $cf['markup_css']['body']['class'];
  }

  if (!empty($parameters['body_class']) && is_array($parameters['body_class'])) {
    $body_class .= ' ' . implode(' ', $parameters['body_class']);
  }

  $html = '<!DOCTYPE html>' . "\n";
  $html .= '<html lang="en" dir="ltr">' . "\n";

  $html .= '<head>' . $head_markup . '</head>';
  $html .= '<body class="' . $body_class . '">';
  $html .= '  <div id="mcneese-page" class="mcneese-page">';
  $html .= '    <div id="mcneese-page-content" class="mcneese-content full" role="main">';
  $html .= '    <header class="page-title html_tag-header">';
  $html .= '      <hgroup class="html_tag-hgroup">';
  $html .= '        <h1 class="page-title html_tag-heading" role="main">' . $page_title . '</h1>';
  $html .= '      </hgroup>';
  $html .= '    </header>';
  $html .= '    ' . $body_markup;
  $html .= '    </div>';
  $html .= '  </div>';
  $html .= '</body>';
  $html .= '</html>' . "\n";

  return $html;
}

/**
 * Renders a PDF of a given request using the wkhtmltopdf library.
 *
 * @param int $request_id
 *   The request id, which is used to generate the filename.
 * @param int $revision_id
 *   The revision id, which is used to generate the filename.
 * @param string $html
 *   The HTML markup to convert to a PDF. Recommend converting the markup
 *   using mfcs_output_print_page_view().
 * @param bool $local
 *   (optional) If TRUE, then the file is saved to the local disk.
 *   If FALSE, then the file is print to stdout.
 * @param int $type
 *   (optional) What the PDF represents, such as a request or an agreement.
 *
 * @return bool
 *   TRUE on success, FALSE otherwise.
 *
 * @see: mfcs_output_print_page_view()
 */
function mfcs_request_pdf_0_page_using_wkhtmltopdf($request_id, $revision_id, $html, $local = FALSE, $type = MFCS_PDF_TYPE_REQUEST) {
  if (!cf_is_integer($request_id)) {
    cf_error::invalid_integer('request_id');
    return FALSE;
  }

  if (!is_null($revision_id) && !cf_is_integer($revision_id)) {
    cf_error::invalid_integer('revision_id');
    return FALSE;
  }

  if (!is_string($html)) {
    cf_error::invalid_string('html');
    return FALSE;
  }

  if (!is_bool($local)) {
    cf_error::invalid_bool('local');
    return FALSE;
  }

  if (!cf_is_integer($type)) {
    cf_error::invalid_integer('type');
    return FALSE;
  }

  $library = libraries_load('phpwkhtmltopdf');
  if ($library === FALSE || empty($library['loaded'])) {
    return FALSE;
  }

  $wkpdf = new WkHtmlToPdf();

  $wk_options = array();

  $wk_options['binPath'] = '/usr/local/bin/wkhtmltopdf';
  $wk_options['binName'] = 'wkhtmltopdf';

  // when not ignored, the PDF does not get generated.
  $wk_options['ignoreWarnings'] = TRUE;

  $wrapper = file_stream_wrapper_get_instance_by_scheme('temporary');
  if (is_object($wrapper)) {
    $wk_options['tmp'] = $wrapper->getDirectoryPath();
  }

  // setup the default options
  $wkpdf->setOptions($wk_options);

  $wkpdf->addPage($html);

  $filename = mfcs_build_filename($request_id, $revision_id, 0, $type);

  if ($local) {
    // save the file, locally.
    $temporary_name = drupal_tempnam('temporary://', 'file');
    $wkpdf->saveAs($temporary_name);

    file_unmanaged_move($temporary_name, 'private://requests/local/' . $filename, FILE_EXISTS_REPLACE);
  }
  else {
    drupal_send_headers();

    // send to the browser
    #$result = $wkpdf->send($filename, TRUE);

    // send as a download
    $result = $wkpdf->send($filename, FALSE);
  }

  if (!$result) {
    watchdog(MFCS_WATCHDOG_ID, "Error occured while generating the PDF of request @id: @error", array('@id' => $request_id, '@error' => $wkpdf->getError()), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Builds the PDF filename string for an request.
 *
 * @param int $request_id
 *   The Request ID.
 * @param bool|int $revision_id
 *   (optional) Set to NULL for latest revision. Otherwise this is the number
 *   that represents the revision of the request to load.
 * @param int $mode
 *   (optional) Determines how the filename is generated:
 *   0 - use the local filename method.
 *   1 - use the BDM filename PDF.
 *   2 - use the BDM filename Meta.
 *   3 - use the Email filename marker.
 * @param int $type
 *   (optional) What the PDF represents, such as a request or an agreement.
 *
 * @return string|false
 *   The built filename string or FALSE on error.
 */
function mfcs_build_filename($request_id, $revision_id = NULL, $mode = MFCS_VIEW_MODE_NORMAL, $type = MFCS_PDF_TYPE_REQUEST) {
  if (!is_null($revision_id) && !cf_is_integer($revision_id)) {
    cf_error::invalid_integer('revision_id');
    return FALSE;
  }

  $request = mfcs_load_request_by_id($request_id, NULL, $revision_id);

  if (empty($request)) {
    return FALSE;
  }

  if (!cf_is_integer($mode) || $mode < MFCS_VIEW_MODE_NORMAL || $mode > MFCS_VIEW_MODE_OPERATIONS) {
    cf_error::invalid_integer('mode');
    return FALSE;
  }

  if (!cf_is_integer($type)) {
    cf_error::invalid_integer('type');
    return FALSE;
  }

  $name = 'unknown';
  if ($type == MFCS_PDF_TYPE_REQUEST) {
    $name = 'request';
  }
  elseif ($type == MFCS_PDF_TYPE_AGREEMENT) {
    $name = 'agreement';
  }

  $fixed_title = preg_replace('/\W/i', '_', $request['information']['title'][0]->value);
  $fixed_title = preg_replace('/_+/i', '_', $fixed_title);

  // remove leading and trailing '_'.
  $fixed_title = preg_replace('/^_+/i', '', $fixed_title);
  $fixed_title = preg_replace('/_+$/i', '', $fixed_title);

  $extra = "";
  if ($mode == MFCS_VIEW_MODE_NORMAL) {
  }
  elseif ($mode == MFCS_VIEW_MODE_DISPLAY) {
    $extra = '-display';
  }
  elseif ($mode == MFCS_VIEW_MODE_LOG) {
    $extra = '-log';
  }
  elseif ($mode == MFCS_VIEW_MODE_OPERATIONS) {
    $extra = '-operations';
  }
  else {
    return FALSE;
  }

  if (!is_null($revision_id)) {
    $extra .= '-revision-' . $revision_id;
  }

  return sprintf("%'010s", $request_id) . '-' . $name . '-' . $fixed_title . $extra . '.pdf';
}

/**
 * @} End of '@addtogroup mfcs'.
 */