<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<?php // modify the layout by changing the id, see layout.css ?>
<body id="maintenance_mode-body" <?php print $attributes;?>>

  <?php if (!$in_overlay): // Hide the skip-link in overlay ?>
    <div id="skip-link">
      <a href="#content-column"><?php print t('Skip to main content'); ?></a>
    </div>
  <?php endif; ?>

  <?php print $page_top; ?>

  <div id="container" class="<?php print $classes; ?>">
    <?php if (!$in_overlay): // hide in overlay ?>

      <?php if (!empty($page['leaderboard'])): ?>
        <div id="leaderboard" class="clearfix">
          <?php print($page['leaderboard']); ?>
        </div>
      <?php endif; ?>

      <div id="header" class="clearfix">
        <div id="branding">
          <h1 class="logo-site-name">
            <span id="logo"><a rel="home" title="Home page" href="/"><img alt=" logo" src="<?php print(path_to_theme());?>/images/web_logo.png"></a></span>
          </h1>
        </div> <!-- /branding -->

        <?php if (!empty($page['header'])): ?>
          <div id="header-blocks"><?php print($page['header']); ?></div>
        <?php endif; ?>

        <div id="header-horizontal_ruler"></div>

      </div> <!-- /header -->

    <?php endif; // end hide in overlay ?>

    <?php if (!empty($page['secondary_content']) && !$in_overlay): // hide in overlay ?>
      <div id="secondary-content">
        <?php print($page['secondary_content']); ?>
      </div>
    <?php endif; ?>

    <div id="columns" class="clear clearfix sidebar-left<?php print($page['subboard_image_css']); ?>">
      <?php if (!empty($sidebar_first)): ?>
        <div id="sidebar-first" class="sidebar"><?php print($sidebar_first); ?></div>
      <?php endif; ?>

      <div id="content-column">
        <?php print $messages; ?>
        <?php print($page['help']); ?>

        <?php if (!empty($page['renderred_action_links'])): ?>
          <ul class="action-links"><?php print($page['renderred_action_links']); ?></ul>
        <?php endif; ?>

        <div class="content-inner">

          <?php if (!empty($page['highlighted'])): ?>
            <div id="highlighted"><?php print ($page['highlighted']); ?></div>
          <?php endif; ?>

          <div id="main-content">
            <?php if (!empty($page['subboard']) || !empty($page['subboard_image'])): ?>
              <div id="subboard">
                <?php if (!empty($page['subboard_image'])) { ?>
                <div id="subboard-image"><?php print($page['subboard_image']);?></div>
                <?php } ?>
                <?php print($page['subboard']); ?>
              </div>
            <?php endif; ?>

            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>

            <?php if (!empty($breadcrumb)){ ?>
              <div id="breadcrumb">
                <?php print $breadcrumb; ?>
                <?php if (!empty($page['subtitle'])){ ?>
                  <div class="subtitle"><?php print($page['subtitle']);?></div>
                <?php } ?>
              </div>
            <?php } ?>

            <?php if (!empty($page['renderred_tabs'])): ?>
              <div class="local-tasks"><?php print($page['renderred_tabs']); ?></div>
            <?php endif; ?>

            <div id="content">
              <?php print($content); ?>
            </div>
          </div>

        </div>
      </div>

      <?php if (!empty($sidebar_second)): ?>
        <div id="sidebar-second" class="sidebar"><?php print($sidebar_second); ?></div>
      <?php endif; ?>

    </div> <!-- /columns -->

    <?php if (!$in_overlay){ // hide in overlay ?>

      <?php if (!empty($page['tertiary_content'])): ?>
        <div id="tertiary-content" style="<?php print($page['is_front_css']);?>">
          <?php print($page['tertiary_content']); ?>
        </div>
      <?php endif; ?>
    <?php } ?>
  </div>

  <?php if (!$in_overlay): // hide in overlay ?>

  <div id="footer" style="<?php print($page['is_front_css']);?>">
    <?php if (!empty($page['footer']) || $feed_icons): ?>
      <?php print ($page['footer']); ?>
      <?php print $feed_icons; ?>
    <?php endif; ?>
  </div>

  <?php endif; // end hide in overlay ?>

  <?php print $page_bottom; ?>

</body>
</html>
