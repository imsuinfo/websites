<?php
// $Id$

/**
 *  @file
 *  Outputs the view.
 *
 */
?>

<div id="views-jqfx-nivo-slider-<?php print $id; ?>" class="views-jqfx-nivo-slider">

  <div id="views-jqfx-nivo-slider-images-<?php print $id; ?>" class=<?php print $classes; ?>>
    <?php foreach ($images as $image): ?>
     <?php print $image ."\n"; ?>
    <?php endforeach; ?>
  </div>

</div>

