<?php
/**
 * @file
 * Default theme implementation to display workbench menu settings table.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $table: An array containing the menu items table variables.
 * - $add_path: A url path for performing the item add operation.
 */
?>
<div class="workbench_menu menu_settings-table-wrapper">
  <h3 class="menu_settings-table-header"><?php print(t("Menu Items")); ?></h3>

  <?php print(theme('table', $table)); ?>

  <div class="menu_settings-add_item_link">
    <?php print(l(t("Add new menu item."), $add_path)); ?>
  </div>
</div>
