<?php?>    <div id='mcneese_drupal-title_region' class='clearfix page-title mcneese_drupal-text_shadow' style='<?php if (isset($title_logo_style) && is_string($title_logo_style)) print($title_logo_style);?>'>
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

        <?php if (isset($search)) { ?>
        <div id='mcneese_drupal-search' class='clearfix mcneese_drupal-text_shadow'>
            <!--(begin_search)-->
            <?php print(render($search)); ?>
            <!--(end_search)-->
          </div>
        <?php } ?>
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

      <?php if (is_array($page) && array_key_exists('help', $page)){ ?>
        <!--(begin_help)-->
        <div id='mcneese_drupal-help' class='clearfix mcneese_drupal-text_shadow'>
          <h2 class='element-invisible'><?php print t("Help"); ?></h2>
          <?php print(render($page['help'])); ?>
        </div>
        <!--(end_help)-->
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

      <?php if (is_array($page) && array_key_exists('content', $page)){ ?>
        <div id='mcneese_drupal-content' class='clearfix'>
          <!--(begin_content)-->
          <h2 class='element-invisible'><?php print(t("Primary Content")); ?></h2>
          <?php print(render($page['content'])); ?>
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
