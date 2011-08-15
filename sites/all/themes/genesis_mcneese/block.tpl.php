<?php ?>
<div id="block-<?php print $block->module . '-' . $block->delta; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="block-inner">

    <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h2 class="block-title"<?php print $title_attributes; ?>><?php print $title; ?></h2>
      <?php endif;?>
    <?php print render($title_suffix); ?>

    <div class="content"<?php print $content_attributes; ?>>
      <?php print $content ?>
    </div>

  </div>
</div>
