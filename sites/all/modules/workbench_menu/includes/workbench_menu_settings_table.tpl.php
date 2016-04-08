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

  print('<div class="workbench_menu menu_settings-table-wrapper">');
  print(theme('table', $menu_items_table));

  print('<div class="menu_settings-add_item_link">');
  print(l(t("Add new menu item."), $add_path));
  print('</div>');

  if (!empty($disabled_items_table['rows'])) print(theme('table', $disabled_items_table));
  print('</div>');
