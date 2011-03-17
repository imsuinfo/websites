<?php?><!DOCTYPE html>
<html lang="<?php print($language->language); ?>" dir="<?php print $language->dir; ?>">

<head>
  <!--(begin_head)-->
  <?php print($head); ?>
  <title><?php print($head_title); ?></title>
  <?php print($styles); ?>
  <?php print($scripts);?>
  <!--(end_head)-->
</head>
<body id="mcneese_drupal-body" class="<?php print($classes); ?>" <?php print($attributes);?>>
  <!--(begin_body)-->
  <div id="mcneese_drupal-page_top" class="mcneese_drupal">
    <!--(begin_page_top)-->
    <?php print($page_top); ?>
    <!--(end_page_top)-->
  </div>

  <div id="mcneese_drupal-page" class="mcneese_drupal">
    <!--(begin_page)-->
    <div id='mcneese_drupal-title_region' class='clearfix page-title mcneese_drupal-text_shadow' style='<?php if (isset($title_logo_style) && is_string($title_logo_style)) print($title_logo_style);?>'>
      <div id='mcneese_drupal-title'>
        <!--(begin_title)-->
        <?php print(render($title_prefix)); ?>
        <?php if (isset($site_name) && is_string($site_name)){ ?>
          <div id='mcneese_drupal-website_title' class='mcneese_drupal-text_shadow'>
            <?php print($site_name); ?>
          </div>
        <?php } ?>
        <?php if (isset($title) && is_string($title)){ ?>
          <h1 id='mcneese_drupal-page_title' class='mcneese_drupal-text_shadow'><?php print($title); ?></h1>
        <?php } ?>
        <?php print(render($title_suffix)); ?>
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
    </div>

    <div id='mcneese_drupal-message_region' class='clearfix mcneese_drupal-text_shadow'>
      <?php if (isset($messages) && is_string($messages)){ ?>
        <!--(begin_messages)-->
        <div id='mcneese_drupal-messages' class='clearfix'>
          <h2 class='element-invisible mcneese_drupal-text_shadow'><?php print t('Messages'); ?></h2>
          <?php print($messages); ?>
        </div>
        <!--(end_messages)-->
      <?php } ?>

      <?php if ($help){ ?>
        <!--(begin_help)-->
        <div id='mcneese_drupal-help' class='clearfix mcneese_drupal-text_shadow'>
          <h2 class='element-invisible'><?php print t("Help"); ?></h2>
          <?php print($help); ?>
        </div>
        <!--(end_help)-->
      <?php } ?>
    </div>

    <div id='mcneese_drupal-content_region' class='page-title mcneese_drupal-text_shadow'>
      <?php if ($content){ ?>
        <div id='mcneese_drupal-content' class='clearfix'>
          <!--(begin_content)-->
          <h2 class='element-invisible'><?php print(t("Primary Content")); ?></h2>
          <?php print($content); ?>
          <!--(end_content)-->
        </div>
      <?php } ?>
    </div>

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
