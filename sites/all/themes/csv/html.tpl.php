<?php
  header('Content-type: text/csv');
  header('Content-Disposition: attachment; filename="' . (empty($csv_filename) ? 'download.csv' : $csv_filename) . '"');

  print($page);
?>
