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
<body id="maintenance_mode-body" class="<?php print($classes); ?>" <?php print($attributes);?>>
  <!--(begin_body)-->
  <div id="maintenance_mode-page_top" class="maintenance_mode">
    <!--(begin_page_top)-->
    <?php print($page_top); ?>
    <!--(end_page_top)-->
  </div>

  <div id="maintenance_mode-page" class="maintenance_mode">
    <!--(begin_page)-->
    <div id='maintenance_mode-title_region' class='clearfix page-title' style='<?php if (isset($title_logo_style) && is_string($title_logo_style)) print($title_logo_style);?>'>
      <div id='maintenance_mode-title'>
        <!--(begin_title)-->
        <?php print(render($title_prefix)); ?>
        <?php if (isset($site_name) && is_string($site_name)){ ?>
          <div id='maintenance_mode-website_title'>
            <?php print($site_name); ?>
          </div>
        <?php } ?>
        <?php if (isset($title) && is_string($title)){ ?>
          <h1 id='maintenance_mode-page_title'><?php print($title); ?></h1>
        <?php } ?>
        <?php print(render($title_suffix)); ?>
        <!--(end_title)-->
      </div>

      <?php if (isset($side_links) && is_string($side_links)) { ?>
        <div id='maintenance_mode-side_links'>
          <!--(begin_side_links)-->
          <h2 class='element-invisible'><?php print t("Side Links"); ?></h2>
          <ul class='links'><?php print($side_links); ?></ul>
          <!--(end_side_links)-->
        </div>
      <?php } ?>
    </div>

    <div id='maintenance_mode-message_region' class='clearfix'>
      <?php if (isset($messages) && is_string($messages)){ ?>
        <!--(begin_messages)-->
        <div id='maintenance_mode-messages' class='clearfix'>
          <h2 class='element-invisible'><?php print t('Messages'); ?></h2>
          <?php print($messages); ?>
        </div>
        <!--(end_messages)-->
      <?php } ?>

      <?php if ($help){ ?>
        <!--(begin_help)-->
        <div id='maintenance_mode-help' class='clearfix'>
          <h2 class='element-invisible'><?php print t("Help"); ?></h2>
          <?php print($help); ?>
        </div>
        <!--(end_help)-->
      <?php } ?>
    </div>

    <div id='maintenance_mode-content_region' class='page-title'>
      <?php if ($content){ ?>
        <div id='maintenance_mode-content' class='clearfix'>
          <!--(begin_content)-->
          <h2 class='element-invisible'><?php print(t("Primary Content")); ?></h2>
          <?php print($content); ?>
          <!--(end_content)-->
        </div>
      <?php } ?>
    </div>

    <div id="maintenance_mode-undercontent">
      <!--(begin_undercontent)-->
      <div id="maintenance_mode-MSU" class="maintenance_mode-outline" title='McNeese State University'>McNeese State University</div>
      <div id="maintenance_mode-UCS" class="maintenance_mode-outline" title='University Computing Services'>University Computing Services</div>
      <!--(end_undercontent)-->
    </div>
    <!--(end_page)-->
  </div>

  <div id="maintenance_mode-page_bottom" class="maintenance_mode">
    <!--(begin_page_bottom)-->
    <?php print($page_bottom); ?>
    <!--(end_page_bottom)-->
  </div>
  <!--(end_body)-->
</body>
</html>
