<?php
// $Id$

/**
 *  @file
 *  Outputs the view.
 *
 */
?>

<div id="msu-slider-<?php print $id; ?>" class="msu-slider">

  <div id="msu-images-<?php print $id; ?>" class="<?php print $classes; ?>">
    <?php foreach ($images as $image): ?>
     <?php print $image ."\n"; ?>
    <?php endforeach; ?>
  </div>

</div>

