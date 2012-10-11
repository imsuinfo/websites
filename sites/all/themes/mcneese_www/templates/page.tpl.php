<?php
/**
 * @file
 * Page theme implementation.
 */
  $cf = & drupal_static('cf_theme_get_variables', array());

  mcneese_render_page();
  mcneese_www_render_page();

  $float_tabs = in_array('fixed', $cf['page']['tags']['mcneese_page_tabs_open']['attributes']['class']);
  $float_action_links = in_array('fixed', $cf['page']['tags']['mcneese_page_action_links_open']['attributes']['class']);
  $float_side = in_array('fixed', $cf['page']['tags']['mcneese_page_side_open']['attributes']['class']);
  $float_breadcrumb = in_array('fixed', $cf['page']['tags']['mcneese_page_breadcrumb_open']['attributes']['class']);
  $split_page = !$float_side && ($cf['show']['page']['menus'] || $cf['show']['page']['asides']);
?>
<?php if ($cf['is']['unsupported']) { ?>
  <div id="mcneese-unsupported-message">
    <?php print($cf['is_data']['unsupported']['message']); ?>
  </div>
<?php } ?>

<?php if ($cf['show']['page']['header']) { ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_header_open']) . "\n"); ?>
    <!--(begin-page-header)-->
    <div class="header-section header-top">
      <?php print($cf['data']['page']['header'] . "\n"); ?>

      <?php if ($cf['show']['page']['logo']) { ?>
        <a href="<?php print($cf['data']['page']['logo']['href']); ?>" class="site-logo" title="<?php print($cf['data']['page']['logo']['alt']); ?>" role="image"><?php print($cf['data']['page']['logo']['alt']); ?></a>
      <?php } ?>

      <?php if ($cf['show']['page']['header_menu_1']) { ?>
        <div class="header-menu header-menu-1" role="navigation">
          <?php print($cf['data']['page']['header_menu_1'] . "\n"); ?>
        </div>
      <?php } ?>
    </div>

    <div class="header-separator"></div>

    <div class="header-section header-bottom">
      <?php if ($cf['show']['page']['header_menu_2']) { ?>
        <div class="header-menu header-menu-2" role="navigation">
          <?php print($cf['data']['page']['header_menu_2'] . "\n"); ?>
        </div>
      <?php } ?>
    </div>
    <!--(end-page-header)-->
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_header_close']) . "\n"); ?>
<?php } ?>

<?php if ($cf['show']['page']['messages']) { ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_messages_open']) . "\n"); ?>
    <!--(begin-page-messages)-->
    <?php print($cf['data']['page']['messages']['renderred'] . "\n"); ?>
    <!--(end-page-messages)-->
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_messages_close']) . "\n"); ?>
<?php } ?>

<?php if ($cf['show']['page']['help']) { ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_open']) . "\n"); ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_header_open']) . "\n"); ?>
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
        <h2 class="html_tag-heading">Help</h2>
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_header_close']) . "\n"); ?>

    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_wrapper_open']) . "\n"); ?>
      <!--(begin-page-help)-->
      <?php print($cf['data']['page']['help'] . "\n"); ?>
      <!--(end-page-help)-->
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_wrapper_close']) . "\n"); ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_help_close']) . "\n"); ?>
<?php } ?>

<?php if ($cf['show']['page']['information']) { ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_open']) . "\n"); ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_header_open']) . "\n"); ?>
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
        <h2 class="html_tag-heading">Information</h2>
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_header_close']) . "\n"); ?>

    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_wrapper_open']) . "\n"); ?>
      <!--(begin-page_information)-->
      <?php print($cf['data']['page']['information'] . "\n"); ?>
      <!--(end-page_information)-->
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_wrapper_close']) . "\n"); ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_information_close']) . "\n"); ?>
<?php } ?>

<?php if ($cf['show']['page']['editing']) { ?>
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_editing_open']) . "\n"); ?>
    <!--(begin-page-editing)-->
    <?php print($cf['data']['page']['editing'] . "\n"); ?>
    <!--(end-page-editing)-->
  <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_editing_close']) . "\n"); ?>
<?php } ?>

<?php if ($split_page) { ?>
  <div id="mcneese-page-content" class="split" role="main">
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_side_open']) . "\n"); ?>
      <!--(begin-page_side)-->
      <?php if ($cf['show']['page']['menus']) { ?>
        <?php print($cf['data']['page']['menus']); ?>
      <?php } ?>

      <?php if ($cf['show']['page']['asides']) { ?>
        <?php print($cf['data']['page']['asides']); ?>
      <?php } ?>
      <!--(end-page_side)-->
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_side_close']) . "\n"); ?>

    <div class="column-2">
<?php } else { ?>
  <div id="mcneese-page-content" class="full" role="main">
<?php } ?>

  <?php if ($cf['show']['page']['group_image']) { ?>
    <!--(begin-page-group_image)-->
    <img usemap="#group_image_map" class="<?php print($cf['data']['page']['group_image']['class']); ?>" title="<?php print($cf['data']['page']['group_image']['title']); ?>" alt="<?php print($cf['data']['page']['group_image']['alt']); ?>" src="<?php print($cf['data']['page']['group_image']['src']); ?>" other="<?php print($cf['data']['page']['group_image']['other']); ?>">
    <!--(end-page-group_image)-->
  <?php } ?>

  <?php if ($cf['show']['page']['title']) { ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_title_open']) . "\n"); ?>
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
        <!--(begin-page-title)-->
        <?php if ($cf['show']['page']['title_prefix']) print($cf['data']['page']['title_prefix'] . "\n"); ?>
          <h1 class="page-title html_tag-heading"><?php print($cf['data']['page']['title']); ?></h1>
        <?php if ($cf['show']['page']['title_suffix']) print($cf['data']['page']['title_suffix'] . "\n"); ?>
        <!--(end-page-title)-->
      <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_title_close']) . "\n"); ?>
  <?php } ?>

  <div id="mcneese-float-left" role="navigation">
    <?php if ($cf['show']['page']['tabs'] && $float_tabs) { ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_tabs_open']) . "\n"); ?>
      <!--(begin-page-tabs)-->
      <ul class="navigation_list">
        <?php print($cf['data']['page']['tabs'] . "\n"); ?>
      </ul>
      <!--(end-page-tabs)-->
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_tabs_close']) . "\n"); ?>
    <?php } ?>

    <?php if ($cf['show']['page']['action_links'] && $float_action_links) { ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_action_links_open']) . "\n"); ?>
        <!--(begin-page-action_links)-->
        <ul class="navigation_list">
          <?php print($cf['data']['page']['action_links'] . "\n"); ?>
        </ul>
        <!--(end-page-action_links)-->
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_action_links_close']) . "\n"); ?>
    <?php } ?>

    <?php if ($cf['show']['page']['breadcrumb'] && $float_breadcrumb) { ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_open']) . "\n"); ?>
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_header_open']) . "\n"); ?>
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
            <h2 class="html_tag-heading">Breadcrumb</h2>
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_header_close']) . "\n"); ?>

        <ul class="navigation_list">
          <?php if ($cf['show']['page']['precrumb']) { ?>
            <!--(begin-page-precrumb)-->
            <?php print($cf['data']['page']['precrumb'] . "\n"); ?>
            <!--(end-page-precrumb)-->
          <?php } ?>

          <?php if ($cf['show']['page']['breadcrumb']) { ?>
            <!--(begin-page-breadcrumb)-->
            <?php print($cf['data']['page']['breadcrumb'] . "\n"); ?>
            <!--(end-page-breadcrumb)-->
          <?php } ?>

          <?php if ($cf['show']['page']['postcrumb']) { ?>
            <!--(begin-page-postcrumb)-->
            <?php print($cf['data']['page']['postcrumb'] . "\n"); ?>
            <!--(end-page-postcrumb)-->
          <?php } ?>
        </ul>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_close']) . "\n"); ?>
    <?php } ?>

    <?php if ($float_side && ($cf['show']['page']['menus'] || $cf['show']['page']['asides'])) { ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_side_open']) . "\n"); ?>
      <!--(begin-page-side)-->
      <?php if ($cf['show']['page']['menus']) { ?>
        <?php print($cf['data']['page']['menus']); ?>
      <?php } ?>

      <?php if ($cf['show']['page']['asides']) { ?>
        <?php print($cf['data']['page']['asides']); ?>
      <?php } ?>
      <!--(end-page-side)-->
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_side_close']) . "\n"); ?>
    <?php } ?>

    <?php if ($cf['show']['page']['document_outline']) { ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_outline_open']) . "\n"); ?>
        <!--(begin-page-document-outline)-->
        <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_header_open']) . "\n"); ?>
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
            <h2 class="html_tag-heading">Outline</h2>
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
        <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_header_close']) . "\n"); ?>

        <ul class="navigation_list">
          <?php print($cf['data']['page']['document_outline']['markup']); ?>
        </ul>
        <!--(end-page-document-outline)-->
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_outline_close']) . "\n"); ?>
    <?php } ?>
  </div>

  <?php if ($cf['show']['page']['tabs'] && !$float_tabs) { ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_tabs_open']) . "\n"); ?>
    <!--(begin-page-tabs)-->
    <ul class="navigation_list">
      <?php print($cf['data']['page']['tabs'] . "\n"); ?>
    </ul>
    <!--(end-page-tabs)-->
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_tabs_close']) . "\n"); ?>
  <?php } ?>

  <?php if (!$cf['is']['front'] && ($cf['show']['page']['breadcrumb'] || $cf['show']['page']['precrumb'] || $cf['show']['page']['postcrumb']) && !$float_breadcrumb) { ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_open']) . "\n"); ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_header_open']) . "\n"); ?>
        <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
          <h2 class="html_tag-heading">Breadcrumb</h2>
        <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
      <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_header_close']) . "\n"); ?>

      <ul class="navigation_list">
        <?php if ($cf['show']['page']['precrumb']) { ?>
          <!--(begin-page-precrumb)-->
          <?php print($cf['data']['page']['precrumb'] . "\n"); ?>
          <!--(end-page-precrumb)-->
        <?php } ?>

        <?php if ($cf['show']['page']['breadcrumb']) { ?>
          <!--(begin-page-breadcrumb)-->
          <?php print($cf['data']['page']['breadcrumb'] . "\n"); ?>
          <!--(end-page-breadcrumb)-->
        <?php } ?>

        <?php if ($cf['show']['page']['postcrumb']) { ?>
          <!--(begin-page-postcrumb)-->
          <?php print($cf['data']['page']['postcrumb'] . "\n"); ?>
          <!--(end-page-postcrumb)-->
        <?php } ?>
      </ul>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_breadcrumb_close']) . "\n"); ?>
  <?php } ?>

  <?php if ($cf['show']['page']['action_links'] && !$float_action_links) { ?>
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_action_links_open']) . "\n"); ?>
      <!--(begin-page-action_links)-->
      <?php print($cf['data']['page']['action_links'] . "\n"); ?>
      <!--(end-page-action_links)-->
    <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_action_links_close']) . "\n"); ?>
  <?php } ?>

  <?php if ($cf['show']['page']['watermarks-pre']) { ?>
    <div id="mcneese-page-watermarks-pre">
      <?php print($cf['data']['page']['watermarks-pre']); ?>
    </div>
  <?php } ?>

  <div id="mcneese-page-main" role="main">
    <!--(begin-page-main)-->
    <?php if ($cf['show']['page']['content']) { ?>
      <?php if ($cf['show']['page']['document_header']) { ?>
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_header_open']) . "\n"); ?>
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_open']) . "\n"); ?>
            <!--(begin-page-document-header)-->
              <?php print($cf['data']['page']['document_header']['markup']); ?>
            <!--(end-page-document-header)-->
          <?php print(theme('mcneese_tag', $cf['generic']['tags']['mcneese_hgroup_close']) . "\n"); ?>
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_header_close']) . "\n"); ?>
      <?php } ?>

      <?php print($cf['data']['page']['content']); ?>

      <?php if ($cf['show']['page']['document_footer']) { ?>
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_footer_open']) . "\n"); ?>
          <!--(begin-page-document-footer)-->
            <?php print($cf['data']['page']['document_footer']['markup']); ?>
          <!--(end-page-document-footer)-->
        <?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_www_document_footer_close']) . "\n"); ?>
      <?php } ?>
    <?php } ?>
    <!--(end-page-main)-->
  </div>

  <?php if ($cf['show']['page']['watermarks-post']) { ?>
    <div id="mcneese-page-watermarks-post">
      <?php print($cf['data']['page']['watermarks-post']); ?>
    </div>
  <?php } ?>

<?php if ($split_page) { ?>
    </div>
  </div>
<?php } else { ?>
  </div>
<?php } ?>

<?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_footer_open']) . "\n"); ?>
  <!--(begin-page-footer)-->
  <?php if ($cf['show']['page']['footer']) { ?>
    <?php print($cf['data']['page']['footer']); ?>
  <?php } ?>
  <!--(end-page-footer)-->
<?php print(theme('mcneese_tag', $cf['page']['tags']['mcneese_page_footer_close']) . "\n"); ?>
