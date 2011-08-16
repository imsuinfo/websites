<?php ?>
  <div id="container" class="<?php print($cf['markup_css']['container']['class']); ?>">
    <?php if (!$cf['is']['overlay']){ ?>
      <?php if ($cf['show']['page']['leaderboard']){ ?>
        <div id="leaderboard" class="clearfix">
          <!--(begin_leaderboard)-->
          <?php print($page['leaderboard']); ?>
          <!--(end_leaderboard)-->
        </div>
      <?php } ?>

      <?php if ($cf['show']['secondary_menu_links']){ ?>
        <div id="secondary-menu-wrapper" class="clearfix">
          <!--(begin_secondary_menu_links)-->
          <div class="secondary-menu-inner"><?php print($secondary_menu_links); ?></div>
          <!--(end_secondary_menu_links)-->
        </div>
      <?php } ?>

      <div id="header" class="clearfix">
        <!--(begin_header)-->
        <?php if ($cf['show']['logo'] || $cf['show']['site_name'] || $cf['show']['site_slogan']){ ?>
          <div id="branding">
            <!--(begin_branding)-->
            <?php if ($cf['show']['logo'] || $cf['show']['site_name']){ ?>
              <?php if ($cf['show']['title']){ ?><div class="logo-site-name" style="font-weight: bold;"><?php } else { ?><h1 class="logo-site-name"><?php } ?>
                <!--(begin_site_name)-->
                <?php if ($cf['show']['logo']){ ?>
                  <span id="logo">
                    <!--(begin_website_logo)-->
                    <a id="website_logo-link" href="/" title="<?php print(t("Home Page")); ?>">
                      <img id="website_logo-image" src="<?php print($logo); ?>" alt="<?php print($cf['at']['human_name']); ?>">
                    </a>
                    <!--(end_website_logo)-->
                  </span>
                <?php } ?>
                <?php if ($cf['show']['site_name']){ ?><span id="site-name"><?php print($site_name); ?></span><?php } ?>
                <!--(end_site_name)-->
              <?php if ($cf['show']['title']){ ?></div><?php } else { ?></h1><?php } ?>
            <?php } ?>

            <?php if ($cf['show']['site_slogan']){ ?>
              <div id="site-slogan">
                <!--(end_site_slogan)-->
                <?php print($site_slogan); ?>
                <!--(end_site_slogan)-->
              </div>
            <?php }  ?>
            <!--(end_branding)-->
          </div>
        <?php } ?>

        <?php if ($cf['show']['page']['header']){ ?>
          <!--(begin_header_blocks)-->
          <div id="header-blocks"><?php print($page['header']); ?></div>
          <!--(end_header_blocks)-->
        <?php } ?>

        <div id="header-horizontal_ruler"></div>
        <!--(end_header)-->
      </div>

      <?php if ($cf['show']['main_menu_links']){ ?>
        <div id="main-menu-wrapper" class="clearfix">
          <!--(begin_main_menu_links)-->
          <div class="main-menu-inner"><?php print($main_menu_links); ?></div>
          <!--(end_main_menu_links)-->
        </div>
      <?php } ?>
    <?php } ?>

    <?php if ($cf['show']['page']['secondary_content'] && !$cf['is']['overlay']){ ?>
      <div id="secondary-content">
        <!--(begin_secondary_content)-->
        <?php print($page['secondary_content']); ?>
        <!--(end_secondary_content)-->
      </div>
    <?php } ?>

    <div id="columns" class="clear clearfix <?php print($cf['markup_css']['container']['class']); ?>">
      <?php if ($cf['show']['page']['sidebar_first']){ ?>
        <div id="sidebar-first" class="sidebar">
          <!--(begin_sidebar_first)-->
          <?php print($page['sidebar_first']); ?>
          <!--(end_sidebar_first)-->
        </div>
      <?php } ?>

      <div id="content-column">
        <!--(begin_content_column)-->
        <?php if ($cf['is']['emergency']){ ?>
          <div class="emergency_mode-notice">
            <!--(begin_emergency_mode_notice)-->
            <?php print($cf['is_data']['emergency']['notice']); ?>
            <!--(end_emergency_mode_notice)-->
          </div>
        <?php } ?>

        <?php if ($cf['show']['messages']){ ?>
          <!--(begin_messages)-->
          <?php print($messages); ?>
          <!--(end_messages)-->
        <?php } ?>

        <?php if ($cf['show']['page']['help']){ ?>
          <!--(begin_help)-->
          <?php print($page['help']); ?>
          <!--(end_help)-->
        <?php } ?>

        <div class="content-inner">
          <?php if ($cf['show']['page']['highlighted']){ ?>
            <!--(begin_highlighted)-->
            <div id="highlighted"><?php print($page['highlighted']); ?></div>
            <!--(end_highlighted)-->
          <?php } ?>

          <div id="main-content">
            <!--(begin_main_content)-->
            <?php if ($cf['show']['page']['subboard'] || $cf['show']['subboard_image']){ ?>
              <div id="subboard">
                <?php if ($cf['show']['subboard_image']){ ?>
                  <div id="subboard-image">
                    <!--(begin_subboard_image)-->
                    <?php print($cf['data']['subboard_image']['content']);?>
                    <!--(end_subboard_image)-->
                  </div>
                <?php } ?>

                <?php if ($cf['show']['page']['subboard']){ ?>
                  <!--(begin_subboard)-->
                  <?php print($page['subboard']); ?>
                  <!--(end_subboard)-->
                <?php } ?>
              </div>
            <?php } ?>

            <?php if ($cf['show']['title']){ ?>
              <div id="page-title-area">
                <!--(begin_title)-->
                <?php if ($cf['show']['title_prefix']) print($title_prefix); ?>
                <?php if ($cf['show']['title']){ ?>
                  <h1 id='page-title' class="drupal_page_title"><?php print($title); ?></h1>
                <?php } ?>
                <?php if ($cf['show']['title_suffix']) print($title_suffix); ?>
                <!--(end_title)-->
              </div>
            <?php } ?>

            <?php if ($cf['show']['primary_local_tasks']){ ?>
              <div id='local-tasks'>
                <!--(begin_primary_tabs)-->
                <ul class='tabs primary'><?php print($primary_local_tasks); ?></ul>
                <!--(end_primary_tabs)-->
              </div>
            <?php } ?>

            <?php if ($cf['show']['breadcrumb'] || $cf['show']['subtitle']){ ?>
              <div id="breadcrumb">
                <?php if ($cf['show']['breadcrumb']){ ?>
                  <!--(begin_breadcrumb)-->
                  <?php print($breadcrumb); ?>
                  <!--(end_breadcrumb)-->
                <?php } ?>

                <?php if ($cf['show']['subtitle']){ ?>
                  <!--(begin_subtitle)-->
                  <div class="subtitle"><?php print($cf['data']['subtitle']['content']);?></div>
                  <!--(end_subtitle)-->
                <?php } ?>
              </div>
            <?php } ?>

            <?php if ($cf['show']['secondary_local_tasks']){ ?>
              <div id='secondary_local_tasks'>
                <!--(begin_secondary_tabs)-->
                <ul class='tabs secondary'><?php print($secondary_local_tasks); ?></ul>
                <!--(end_secondary_tabs)-->
              </div>
            <?php } ?>

            <?php if ($cf['show']['action_links']){ ?>
              <div id='action_links'>
                <!--(begin_action_links)-->
                <ul class="action-links"><?php print($action_links); ?></ul>
                <!--(end_action_links)-->
              </div>
            <?php } ?>

            <div id="content" class="drupal_content <?php print($cf['markup_css']['container']['class']); ?>">
              <!--(begin_content)-->
              <?php print($page['content']); ?>
              <!--(end_content)-->
            </div>
            <!--(end_main_content)-->
          </div>
        </div>
        <!--(end_content_column)-->
      </div>

      <?php if ($cf['show']['page']['sidebar_second']){ ?>
        <!--(begin_sidebar_second)-->
        <div id="sidebar-second" class="sidebar"><?php print($page['sidebar_second']); ?></div>
        <!--(end_sidebar_second)-->
      <?php } ?>
    </div>

    <?php if ($cf['show']['page']['tertiary_content'] && !$cf['is']['overlay']){ ?>
      <div id="tertiary-content" class="<?php print($cf['markup_css']['content']['class']); ?>">
        <!--(begin_tertiary_content)-->
        <?php print($page['tertiary_content']); ?>
        <!--(end_tertiary_content)-->
      </div>
    <?php } ?>
  </div>

  <?php if ($cf['show']['page']['footer'] && !$cf['is']['overlay']){ ?>
    <div id="footer" class="drupal_footer">
      <!--(begin_footer)-->
      <?php print($page['footer']); ?>
      <!--(end_footer)-->
    </div>
  <?php } ?>
