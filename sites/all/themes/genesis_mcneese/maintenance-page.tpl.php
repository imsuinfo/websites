<?php print($cf['agent']['doctype'] . "\n");?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>">
<head>
  <!--(begin_head)-->
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php print(cf_theme_generate_headers($cf)); ?>
  <!--(end_head)-->
</head>

<body id="genesis_mcneese-body" class="<?php print($cf['markup_css']['body']['class']); ?>" <?php print($attributes);?>>
  <?php if (!$cf['is']['overlay']){ ?>
    <span>
      <div id="genesis_mcneese-skip_nav">
        <!--(begin_skipnav)-->
        <a href="#content-column"><?php print t("Skip to main content"); ?></a>
        <!--(end_skipnav)-->
      </div>
    </span>
  <?php } ?>

  <?php if ($cf['is']['unsupported']){ ?>
    <div id="unsupported" class="clearfix">
      <!--(begin_unsupported)-->
      <?php print($cf['is_data']['unsupported']['message']); ?>
      <!--(end_unsupported)-->
    </div>
  <?php } ?>

  <!--(begin_page_top)-->
  <?php print $page_top; ?>
  <!--(end_page_top)-->

  <!--(begin_page)-->
  <div id="container" class="<?php print $classes; ?>">
    <?php if (!$cf['is']['overlay']){ ?>
      <?php if ($cf['show']['leaderboard']){ ?>
        <div id="leaderboard" class="clearfix">
          <!--(begin_leaderboard)-->
          <?php print($leaderboard); ?>
          <!--(end_leaderboard)-->
        </div>
      <?php } ?>

      <div id="header" class="clearfix">
        <!--(begin_header)-->
          <div id="branding">
            <!--(begin_branding)-->
            <h1 class="logo-site-name">
              <!--(begin_site_name)-->
              <span id="logo"><a rel="home" title="Home page" href="/"><img alt=" logo" src="<?php print(path_to_theme());?>/images/web_logo.png"></a></span>
              <!--(end_site_name)-->
            </h1>
            <!--(end_branding)-->
          </div>

        <?php if ($cf['show']['header']){ ?>
          <!--(begin_header_blocks)-->
          <div id="header-blocks"><?php print($header); ?></div>
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

    <?php if ($cf['show']['secondary_content'] && !$cf['is']['overlay']){ ?>
      <div id="secondary-content">
        <!--(begin_secondary_content)-->
        <?php print($secondary_content); ?>
        <!--(end_secondary_content)-->
      </div>
    <?php } ?>

    <div id="columns" class="clear clearfix <?php print($cf['markup_css']['container']['class']); ?>">
      <?php if ($cf['show']['sidebar_first']){ ?>
        <div id="sidebar-first" class="sidebar">
          <!--(begin_sidebar_first)-->
          <?php print($sidebar_first); ?>
          <!--(end_sidebar_first)-->
        </div>
      <?php } ?>

      <div id="content-column">
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

        <?php if ($cf['show']['help']){ ?>
          <!--(begin_help)-->
          <?php print($help); ?>
          <!--(end_help)-->
        <?php } ?>

        <?php if ($cf['show']['renderred_action_links']){ ?>
          <!--(begin_renderred_action_links)-->
          <ul class="action-links"><?php print($renderred_action_links); ?></ul>
          <!--(end_renderred_action_links)-->
        <?php } ?>

        <div class="content-inner">
          <?php if ($cf['show']['highlighted']){ ?>
            <!--(begin_highlighted)-->
            <div id="highlighted"><?php print ($highlighted); ?></div>
            <!--(end_highlighted)-->
          <?php } ?>

          <div id="main-content">
            <!--(begin_main_content)-->
            <?php if ($cf['show']['subboard'] || $cf['show']['subboard_image']){ ?>
              <div id="subboard">
                <?php if ($cf['show']['subboard_image']){ ?>
                  <div id="subboard-image">
                    <!--(begin_subboard_image)-->
                    <?php print($cf['data']['subboard_image']['content']);?>
                    <!--(end_subboard_image)-->
                  </div>
                <?php } ?>

                <?php if ($cf['show']['subboard']){ ?>
                  <!--(begin_subboard)-->
                  <?php print($subboard); ?>
                  <!--(end_subboard)-->
                <?php } ?>
              </div>
            <?php } ?>

            <?php if ($cf['show']['title_prefix']){ ?>
              <!--(begin_title_prefix)-->
              <?php print($title_prefix); ?>
              <!--(end_title_prefix)-->
            <?php } ?>

            <?php if ($cf['show']['title']){ ?>
              <!--(begin_title)-->
              <h1 id="page-title" class="drupal_page_title"><?php print($title); ?></h1>
              <!--(end_title)-->
            <?php } ?>

            <?php if ($cf['show']['title_suffix']){ ?>
              <!--(begin_title_suffix)-->
              <?php print($title_suffix); ?>
              <!--(end_title_suffix)-->
            <?php } ?>

            <?php if ($cf['show']['renderred_tabs']){ ?>
              <div class="local-tasks">
                <!--(begin_renderred_tabs)-->
                <?php print($renderred_tabs); ?>
                <!--(end_renderred_tabs)-->
              </div>
            <?php } ?>

            <?php if ($cf['show']['breadcrumb'] || $cf['show']['subtitle']){ ?>
              <div id="breadcrumb">
                <?php if ($cf['show']['breadcrumb']){ ?>
                  <!--(begin_breadcrumb)-->
                  <?php print $breadcrumb; ?>
                  <!--(end_breadcrumb)-->
                <?php } ?>

                <?php if ($cf['show']['subtitle']){ ?>
                  <!--(begin_subtitle)-->
                  <div class="subtitle"><?php print($cf['data']['subtitle']['content']);?></div>
                  <!--(end_subtitle)-->
                <?php } ?>
              </div>
            <?php } ?>

            <div id="content" class="drupal_content <?php print($cf['markup_css']['container']['class']); ?>">
              <!--(begin_content)-->
              <?php if (!empty($cf['is']['emergency'])){ ?>
                <div class="emergency-content"><?php print(check_markup($cf['is_data']['emergency']['body']['value'], $cf['is_data']['emergency']['body']['format'])); ?></div>
              <?php } else { ?>
                <?php print($content); ?>
              <?php } ?>
            </div>
            <!--(end_main_content)-->
          </div>
        </div>
        <!--(end_content_column)-->
      </div>

      <?php if ($cf['show']['sidebar_second']){ ?>
        <!--(begin_sidebar_second)-->
        <div id="sidebar-second" class="sidebar"><?php print($sidebar_second); ?></div>
        <!--(end_sidebar_second)-->
      <?php } ?>
    </div>

    <?php if ($cf['show']['tertiary_content'] && !$cf['is']['overlay']){ ?>
      <div id="tertiary-content" class="<?php print($cf['markup_css']['content']['class']); ?>">
        <!--(begin_tertiary_content)-->
        <?php print($tertiary_content); ?>
        <!--(end_tertiary_content)-->
      </div>
    <?php } ?>
  </div>

  <?php if ($cf['show']['footer'] && !$cf['is']['overlay']){ ?>
    <div id="footer" class="drupal_footer">
      <!--(begin_footer)-->
      <?php print ($footer); ?>
      <!--(end_footer)-->
    </div>
  <?php } ?>
  <!--(end_page)-->

  <!--(begin_page_bottom)-->
  <?php print $page_bottom; ?>
  <!--(end_page_bottom)-->
</body>
</html>
