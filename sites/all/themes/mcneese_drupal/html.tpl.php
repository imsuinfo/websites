<?php?><!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">
<html lang="<?php print($language->language); ?>" dir="<?php print $language->dir; ?>" version="HTML+RDFa 1.1" <?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <!--(begin_head)-->
  <?php print($head); ?>
  <title><?php print($head_title); ?></title>
  <?php print($styles); ?>
  <?php print($scripts);?>
  <!--(end_head)-->
</head>
<body id="mcneese_drupal-body" class="<?php print($classes); ?>" <?php print($attributes);?>>
  <?php if (!$in_overlay){ ?><div id="mcneese_drupal-skip_nav" class="clearfix mcneese_drupal-text_shadow">
    <!--(begin_skipnav)-->
    <ul id="mcneese_drupal-skip_nav-list">
      <li id="mcneese_drupal-skip_nav-list-content"><a id="mcneese_drupal-skip_nav-list-content-link" class="mcneese_drupal-skipnav-link element-invisible element-focusable" href="#mcneese_drupal-content"><?php print t("Skip to main content"); ?></a></li>
      <?php if (isset($search)){ ?><li id="mcneese_drupal-skip_nav-list-search"><a id="mcneese_drupal-skip_nav-list-search-link" class="mcneese_drupal-skipnav-link element-invisible element-focusable" href="#mcneese_drupal-search"><?php print t("Skip to search"); ?></a></li><?php } ?>
    </ul>
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
    <?php print($page); ?>
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
