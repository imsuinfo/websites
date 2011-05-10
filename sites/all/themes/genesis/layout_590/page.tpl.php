<?php
// $Id: page.tpl.php,v 1.13 2011/01/14 03:34:24 jmburnz Exp $

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 * - $in_overlay: TRUE if the page is in the overlay.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlight']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
  <div id="container" class="<?php print $classes; ?> <?php print($page['is_front_css']);?>">
    <?php if (!$in_overlay): // hide in overlay ?>

      <?php if (!empty($page['leaderboard'])): ?>
        <div id="leaderboard" class="clearfix">
          <?php print($page['leaderboard']); ?>
        </div>
      <?php endif; ?>

      <?php if ($secondary_menu_links): ?>
        <div id="secondary-menu-wrapper" class="clearfix">
          <div class="secondary-menu-inner"><?php print $secondary_menu_links; ?></div>
        </div>
      <?php endif; ?>

      <div id="header" class="clearfix">

        <?php if ($site_logo || $site_name || $site_slogan): ?>
          <div id="branding">

            <?php if ($site_logo or $site_name): ?>
              <?php if ($title): ?>
                <div class="logo-site-name"><strong>
                  <?php if ($site_logo): ?><span id="logo"><?php print $site_logo; ?></span><?php endif; ?>
                  <?php if ($site_name): ?><span id="site-name"><?php print $site_name; ?></span><?php endif; ?>
                </strong></div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 class="logo-site-name">
                  <?php if ($site_logo): ?><span id="logo"><?php print $site_logo; ?></span><?php endif; ?>
                  <?php if ($site_name): ?><span id="site-name"><?php print $site_name; ?></span><?php endif; ?>
               </h1>
              <?php endif; ?>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>

          </div> <!-- /branding -->
        <?php endif; ?>

        <?php if (!empty($page['header'])): ?>
          <div id="header-blocks"><?php print($page['header']); ?></div>
        <?php endif; ?>

        <div id="header-horizontal_ruler"></div>

      </div> <!-- /header -->

      <?php if ($main_menu_links): ?>
        <div id="main-menu-wrapper" class="clearfix">
          <div class="main-menu-inner"><?php print $main_menu_links; ?></div>
        </div>
      <?php endif; ?>

    <?php endif; // end hide in overlay ?>
<div>
		
		<div id="featured" >
		<ul class="ui-tabs-nav">
	        <li class="ui-tabs-nav-item ui-tabs-selected" id="nav-fragment-1"><a href="#fragment-1"><img src="/sites/all/themes/genesis/layout_586/images/image1-small.jpg" alt="" /></a></li>
	        <li class="ui-tabs-nav-item" id="nav-fragment-2"><a href="#fragment-2"><img src="/sites/all/themes/genesis/layout_586/images/image2-small.jpg" alt="" /></a></li>
	        <li class="ui-tabs-nav-item" id="nav-fragment-3"><a href="#fragment-3"><img src="/sites/all/themes/genesis/layout_586/images/image3-small.jpg" alt="" /></a></li>
	        <li class="ui-tabs-nav-item" id="nav-fragment-4"><a href="#fragment-4"><img src="/sites/all/themes/genesis/layout_586/images/image4-small.jpg" alt="" /></a></li>
		<li class="ui-tabs-nav-item" id="nav-fragment-5"><a href="#fragment-5"><img src="/sites/all/themes/genesis/layout_586/images/image1-small.jpg" alt="" /></a></li>

	      </ul>

	    <!-- First Content -->
	    <div id="fragment-1" class="ui-tabs-panel" style="">
			<img src="/sites/default/files/styles/home_slider/public/nivo_03.png" alt=""  height="350" width="700" />
			 <div class="info" >
				<h2><a href="#" >And here the LONG version --> Excellent High Speed Website!!</a></h2>
				<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla tincidunt condimentum lacus. Pellentesque ut diam....<a href="#" >read more</a></p>
			 </div>
	    </div>

	    <!-- Second Content -->
	    <div id="fragment-2" class="ui-tabs-panel ui-tabs-hide" style="">
			<img src="/sites/default/files/styles/home_slider/public/nivo_01.png" alt=""  height="350" width="700" />
			 <div class="info" >
				<h2><a href="#" >Super Awesome Stuff Going on yeah!</a></h2>
				<p>Vestibulum leo quam, accumsan nec porttitor a, euismod ac tortor. Sed ipsum lorem, sagittis non egestas id, suscipit....<a href="#" >read more</a></p>
			 </div>
	    </div>

	    <!-- Third Content -->
	    <div id="fragment-3" class="ui-tabs-panel ui-tabs-hide" style="">
			<img src="/sites/default/files/styles/home_slider/public/nivo_05.png" alt=""  height="350" width="700" />
			 <div class="info" >
				<h2><a href="#" >Demo of Different Title! Amazing Web Designs</a></h2>
				<p>liquam erat volutpat. Proin id volutpat nisi. Nulla facilisi. Curabitur facilisis sollicitudin ornare....<a href="#" >read more</a></p>
	         </div>
	    </div>

	    <!-- Fourth Content -->
	    <div id="fragment-4" class="ui-tabs-panel ui-tabs-hide" style="">
			<img src="/sites/default/files/styles/home_slider/public/nivo_04.png" alt="" height="350" width="700" />
			 <div class="info" >
				<h2><a href="#" >LONGER Example of The Best Web Team!</a></h2>
				<p>Quisque sed orci ut lacus viverra interdum ornare sed est. Donec porta, erat eu pretium luctus, leo augue sodales....<a href="#" >read more</a></p>
	         </div>
	    </div>
 <!-- Fifth Content -->
        <div id="fragment-5" class="ui-tabs-panel ui-tabs-hide" style="">
        	<img src="/sites/default/files/styles/home_slider/public/nivo_02.png" alt="" height="350" width="700" />
        	<div class="info" >
        		<h2><a href="#" >Test Web Team!</a></h2>
        		<p>Quisque sed orci ut lacus viverra interdum ornare sed est. Donec porta, erat eu pretium      luctus, leo augue sodales....<a href="#" >read more</a></p>
        	</div>
        </div>
        
		</div>
	</div>

    <?php if (!empty($page['secondary_content']) && !$in_overlay): // hide in overlay ?>
      <div id="secondary-content">
        <?php print($page['secondary_content']); ?>
      </div>
    <?php endif; ?>

    <div id="columns" class="clear clearfix <?php print($page['sidebar_css']); ?>">
      <?php if (!empty($page['sidebar_first'])): ?>
        <div id="sidebar-first" class="sidebar"><?php print($page['sidebar_first']); ?></div>
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
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>

            <?php if (!empty($page['subboard']) || !empty($page['subboard_image'])): ?>
              <div id="subboard">
                <?php if (!empty($page['subboard_image'])) { ?>
                <div id="subboard-image"><?php print($page['subboard_image']);?></div>
                <?php } ?>
                <?php print($page['subboard']); ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($page['renderred_tabs'])): ?>
              <div class="local-tasks"><?php print($page['renderred_tabs']); ?></div>
            <?php endif; ?>

            <div id="content">
              <?php print($page['content']); ?>
            </div>
          </div>

        </div>
      </div>

      <?php if (!empty($page['sidebar_second'])): ?>
        <div id="sidebar-second" class="sidebar"><?php print($page['sidebar_second']); ?></div>
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
