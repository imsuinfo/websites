<?php print($cf['agent']['doctype'] . "\n");?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>" <?php print $rdf_namespaces; ?>>
<head profile="<?php print $grddl_profile; ?>">
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
  <?php print $page; ?>
  <!--(end_page)-->

  <!--(begin_page_bottom)-->
  <?php print $page_bottom; ?>
  <!--(end_page_bottom)-->
</body>
</html>
