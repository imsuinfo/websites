<?php print($cf['agent']['doctype'] . "\n");?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>" <?php print $rdf_namespaces; ?>>
<head profile="<?php print $grddl_profile; ?>">
  <!--(begin_head)-->
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php print(cf_theme_generate_headers($cf)); ?>
  <?php if ($cf['is']['front'] || $cf['at']['path'] == 'news' || $cf['at']['alias'] == 'news') { ?><link rel="alternate"  type="application/rss+xml" title="McNeese State University - News, Events, and More" href="/rss/feed/news_events_and_more" /><?php } ?>
  <?php if ($cf['at']['path'] == 'news/featured' || $cf['at']['alias'] == 'news/featured') { ?><link rel="alternate"  type="application/rss+xml" title="McNeese State University - Featured" href="/rss/feed/featured" /><?php } ?>
  <?php if ($cf['at']['path'] == 'news/events' || $cf['at']['alias'] == 'news/events') { ?><link rel="alternate"  type="application/rss+xml" title="McNeese State University - News & Events" href="/rss/feed/news" /><?php } ?>
  <?php if ($cf['at']['path'] == 'news/spotlight' || $cf['at']['alias'] == 'news/spotlight') { ?><link rel="alternate"  type="application/rss+xml" title="McNeese State University - Spotlight" href="/rss/feed/spotlight" /><?php } ?>
  <?php if ($cf['at']['path'] == 'calendar' || $cf['at']['alias'] == 'calendar') { ?><link rel="alternate"  type="application/rss+xml" title="McNeese State University - Events" href="/rss/feed/all/event" /><?php } ?>
  <!--(end_head)-->
</head>

<body id="genesis_mcneese-body" class="genesis_mcneese-body <?php print($cf['markup_css']['body']['class']); ?>" <?php print($attributes);?>>
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
  <?php print $page; ?>
  <!--(end_page)-->

  <!--(begin_page_bottom)-->
  <?php print $page_bottom; ?>
  <!--(end_page_bottom)-->
</body>
</html>
