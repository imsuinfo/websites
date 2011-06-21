<?php

/**
 * @file
 * Default theme implementation to display a list of validation results of a single display level in a list format.
 * @todo this is a quick and simple implementation so tests can be performed, this will be altered later to have an improved design.
 *
 * Available variables:
 *
 * Validation Results variables:
 * - $severity_id: The error id of the display level whose results this represents.
 * - $severity_machine_name: The error id of the display level whose results this represents.
 * - $severity_human_name: The error id of the display level whose results this represents.
 * - $severity_description: The description of the display level whose results this represents.
 * - $severity_results: An array containing the results for this display level.
 *
 * Styling variables:
 * - $base_class: A generated base css class.
 * - $specific_class: A generated css class that is specific to the severity of the validation results.
 * - $markup_format: The filter format to use for check_markup calls.
 * - $title_block: The type of block in which the title is.
 * - $display_title: Define whether or not to display the title block.
 * - $display_description: Define whether or not to display the title block.
 */
?>
<div class="id-<?php print($specific_class); ?>-wrapper <?php print($base_class); ?>-wrapper">
  <?php if ($display_title){ ?>
    <<?php print($title_block); ?> class="<?php print($base_class); ?>-title"><?php print($severity_human_name); ?></<?php print($title_block); ?>>
  <?php } ?>
  <?php if ($display_description){ ?>
    <div class="<?php print($base_class); ?>-description description">
      <?php print($severity_description); ?>
    </div>
  <?php } ?>
  <?php if ($severity_results['total'] > 0){ ?>
    <div class="<?php print($base_class); ?>-reports">
      <?php foreach ($severity_results as $test_name => $test_results) { ?>
        <?php
          // @todo it may make far more sense to add another theme hook and call that here so that there need not be as much inline php inside the loop
          $test_name_css = drupal_strtolower(check_plain($test_name));
        ?>
        <div class="<?php print($base_class . '-report-' . $test_name_css); ?>-wrapper <?php print($base_class); ?>-report-wrapper">
          <?php if (isset($test_results['body']['title'])){ ?>
            <fieldset id="<?php print($specific_class . '-report-' . $test_name_css); ?>" class="<?php print($base_class); ?>-report collapsible collapsed">
              <legend><span class="fieldset-legend"><?php print(check_plain($test_results['body']['title'])); ?></span></legend>
              <div class="fieldset-wrapper">
                <?php if (isset($test_results['body']['description'])){ ?>
                  <div class="<?php print($base_class . '-report-' . $test_name_css); ?>-description <?php print($base_class); ?>-report-description">
                    <?php print(check_markup($test_results['body']['description'], $markup_format)); ?>
                  </div>
                <?php } ?>
                <?php if (!empty($test_results['problems'])){ ?>
                  <div class="<?php print($base_class . '-report-' . $test_name_css); ?>-problems <?php print($base_class); ?>-report-problems">
                    <?php foreach ($test_results['problems'] as $problem_id => $problem_data){ ?>
                      <?php if (isset($problem_data['line'])){ ?>
                        <div class="<?php print($base_class . '-report-' . $test_name_css); ?>-problem-line-<?php print(check_plain($problem_id)); ?> <?php print($base_class . '-report-' . $test_name_css); ?>-problem-line <?php print($base_class); ?>-report-problem-line">
                          <?php print(t("At line @line:", array('@line' => check_plain($problem_data['line'])))); ?>
                        </div>
                      <?php } ?>
                      <?php if (isset($problem_data['element'])){ ?>
                        <div class="<?php print($base_class . '-report-' . $test_name_css); ?>-problem-markup-<?php print(check_plain($problem_id)); ?> <?php print($base_class . '-report-' . $test_name_css); ?>-problem-markup <?php print($base_class); ?>-report-problem-markup">
                          <?php print(check_markup($problem_data['element'], $markup_format)); ?>
                        </div>
                      <?php } ?>
                    <?php } ?>
                  </div>
                <?php } ?>
              </div>
            </fieldset>
          <?php } ?>
        </div>
      <?php } ?>
    </div>
  <?php } else { ?>
    <div class="<?php print($base_class); ?>-no_reports">
      <?php print(t("There is nothing to report.")); ?>
    </div>
  <?php } ?>
</div>
