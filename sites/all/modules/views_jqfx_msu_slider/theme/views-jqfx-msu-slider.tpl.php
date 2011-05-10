<?php
// $Id$

/**
 *  @file
 *  Outputs the view.
 *
 */
?>

<div id="views-jqfx-msu-slider-<?php print $id; ?>" class="views-jqfx-msu-slider">

  <div id="views-jqfx-msu-slider-images-<?php print $id; ?>" class="<?php print $classes; ?>">
    <?php foreach ($images as $image): ?>
     <?php print $image ."\n"; ?>
    <?php endforeach; ?>
  </div>

</div>

