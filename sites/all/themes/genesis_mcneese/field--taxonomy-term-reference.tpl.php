<?php ?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if (!$label_hidden) : ?>
    <h3 class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</h3>
  <?php endif; ?>
  <ul class="field-items"<?php print $content_attributes; ?>>
    <?php foreach ($items as $delta => $item) : ?>
      <li class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>>
        <?php print render($item); ?>
      </li>
    <?php endforeach; ?>
  </ul>
</div>
