<?php global $base_dir; ?><!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">
<html lang="<?php print($language->language); ?>" dir="<?php print $language->dir; ?>" version="HTML+RDFa 1.1" <?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <!--(begin_head)-->
  <?php print($head); ?>
  <title><?php print($head_title); ?></title>
  <?php print($styles); ?>
  <?php print($scripts);?>
  <!--(end_head)-->
</head>
<body id="mcneese_drupal-body" class="<?php print($classes); print($in_overlay_css); ?>" <?php print($attributes);?>>
<?php  if (!isset($in_overlay) || $in_overlay != 'child'){ ?><div id="mcneese_drupal-skip_nav" class="clearfix mcneese_drupal-text_shadow">
    <!--(begin_skipnav)-->
    <div id="mcneese_drupal-skip_nav-list">
      <div id="mcneese_drupal-skip_nav-list-content"><a id="mcneese_drupal-skip_nav-list-content-link" class="mcneese_drupal-skipnav-link element-invisible element-focusable" href="#mcneese_drupal-content"><?php print t("Skip to main content"); ?></a></div>
      <?php if (isset($search)){ ?><div id="mcneese_drupal-skip_nav-list-search"><a id="mcneese_drupal-skip_nav-list-search-link" class="mcneese_drupal-skipnav-link element-invisible element-focusable" href="#mcneese_drupal-search"><?php print t("Skip to search"); ?></a></div><?php } ?>
    </div>
    <!--(end_skipnav)-->
  </div><?php } ?>

  <!--(begin_body)-->
  <div id="mcneese_drupal-page_top" class="mcneese_drupal">
    <!--(begin_page_top)-->
    <?php print($page_top); ?>
    <!--(end_page_top)-->
  </div>

  <div id="mcneese_drupal-page" class="mcneese_drupal">
    <!--(begin_page)-->
    <div id='mcneese_drupal-header_region' class='clearfix page-header mcneese_drupal-text_shadow'>
      <div id='mcneese_drupal-header'>
        <!--(begin_header)-->
        <?php if (isset($logo) && !empty($logo)){ ?>
          <div id='mcneese_drupal-website_logo' class='mcneese_drupal-text_shadow'>
            <!--(begin_website_logo)-->
            <a id="mcneese_drupal-website_logo-link" href="<?php print($base_dir . '/'); ?>" title="Home Page">
              <img id="mcneese_drupal-website_logo-image" src="<?php print($logo); ?>" alt="<?php if (isset($site_name) && !empty($site_name)) print($site_name); ?>">
            </a>
            <!--(end_website_logo)-->
          </div>
        <?php } ?>
        <!--(end_header)-->
      </div>

      <div id="mcneese_drupal-header-horizontal_ruler"></div>
    </div>
    <div id='mcneese_drupal-message_region' class='clearfix mcneese_drupal-text_shadow'>
      <?php if (isset($messages) && is_string($messages)){ ?>
        <!--(begin_messages)-->
        <div id='mcneese_drupal-messages' class='clearfix'>
          <h2 class='element-invisible mcneese_drupal-text_shadow'><?php print t("Messages"); ?></h2>
          <?php print($messages); ?>
        </div>
        <!--(end_messages)-->
      <?php } ?>

      <?php if (!empty($help)){ ?>
        <!--(begin_help)-->
        <div id='mcneese_drupal-help' class='clearfix mcneese_drupal-text_shadow'>
          <h2 class='element-invisible'><?php print t("Help"); ?></h2>
          <?php print($help); ?>
        </div>
        <!--(end_help)-->
      <?php } ?>
    </div>

    <div id='mcneese_drupal-title_region' class='clearfix page-title mcneese_drupal-text_shadow'>
      <div id='mcneese_drupal-title'>
        <!--(begin_title)-->
        <?php if (isset($title) && is_string($title)){ ?>
          <h1 id='mcneese_drupal-page_title' class='mcneese_drupal-text_shadow'><?php print($title); ?></h1>
        <?php } ?>
        <!--(end_title)-->
      </div>

      <?php if (isset($side_links) && is_string($side_links)) { ?>
        <div id='mcneese_drupal-side_links'>
          <!--(begin_side_links)-->
          <h2 class='element-invisible'><?php print t("Side Links"); ?></h2>
          <ul class='links mcneese_drupal-text_shadow'><?php print($side_links); ?></ul>
          <!--(end_side_links)-->
        </div>
      <?php } ?>

      <?php if (is_array($primary_local_tasks)){ ?>
        <div id='mcneese_drupal-primary_tabs'>
          <!--(begin_primary_tabs)-->
          <h2 class='element-invisible'><?php print t("Primary Tabs"); ?></h2>
          <ul class='tabs primary mcneese_drupal-text_shadow'><?php print render($primary_local_tasks); ?></ul>
          <!--(end_primary_tabs)-->
        </div>
      <?php } ?>

      <?php if (!empty($breadcrumb)){ ?>
        <div id="mcneese_drupal-breadcrumb">
          <?php print $breadcrumb; ?>
        </div>
      <?php } ?>
    </div>

    <div id='mcneese_drupal-content_region' class='page-title mcneese_drupal-text_shadow'>
      <?php if ($secondary_local_tasks){ ?>
        <div id='mcneese_drupal-secondary_tabs'>
          <!--(begin_secondary_tabs)-->
          <h2 class='element-invisible'><?php print(t("Secondary Tabs")); ?></h2>
          <ul class='tabs secondary'><?php print(render($secondary_local_tasks)); ?></ul>
          <!--(end_secondary_tabs)-->
        </div>
      <?php } ?>

      <?php if ($action_links){ ?>
        <div id='mcneese_drupal-action_links'>
          <!--(begin_action_links)-->
          <h2 class='element-invisible'><?php print(t("Action Links")); ?></h2>
          <ul class="action-links"><?php print render($action_links); ?></ul>
          <!--(end_action_links)-->
        </div>
      <?php } ?>

      <div id='mcneese_drupal-content' class='clearfix'>
        <!--(begin_content)-->
        <h2 class='element-invisible'><?php print(t("Primary Content")); ?></h2>
        <?php print($content); ?>
        <!--(end_content)-->
      </div>
    </div>

    <div id="mcneese_drupal-content_bottom"></div>

    <div id="mcneese_drupal-undercontent">
      <!--(begin_undercontent)-->
      <div id="mcneese_drupal-MSU" class="mcneese_drupal-outline" title='McNeese State University'>McNeese State University</div>
      <div id="mcneese_drupal-UCS" class="mcneese_drupal-outline" title='University Computing Services'>University Computing Services</div>
      <!--(end_undercontent)-->
    </div>
    <!--(end_page)-->
  </div>

  <div id="mcneese_drupal-page_bottom" class="mcneese_drupal">
    <!--(begin_page_bottom)-->
    <?php print($page_bottom); ?>
    <!--(end_page_bottom)-->
  </div>
  <!--(end_body)-->
</body>
</html>
