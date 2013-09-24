<?php
/**
 * @file
 * Template for rendering an individual component's analysis data.
 *
 * Available variables:
 * - $component: The component whose analysis is being rendered.
 * - $component_analysis: A renderable containing this components analysis.
 * - $data: An array of array containing the analysis data. Contains the keys:
 *   - table_header: If this table has more than a single column, an array
 *     of header labels.
 *   - table_rows: If this component has a table that should be rendered, an
 *     array of values
 *   - table_caption: A caption or heading displayed on the table.
 *   - table_summary: A summary describing the table.
 */
?>
<div class="<?php print $classes; ?>">
  <div class="webform-analysis-component-inner">
    <?php print drupal_render_children($component_analysis); ?>
  </div>
</div>
