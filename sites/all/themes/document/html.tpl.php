<?php print($cf['agent']['doctype'] . "\n");?>
<html lang="<?php print($language->language); ?>" dir="<?php print $language->dir; ?>" version="HTML+RDFa 1.1" <?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <!--(begin_head)-->
  <?php print($head); ?>
  <title><?php print($head_title); ?></title>
  <?php print($styles); ?>
  <?php print($scripts);?>
  <?php print(cf_theme_generate_headers($cf)); ?>
</head>
<body class="document <?php print($cf['markup_css']['body']['class']); ?>" <?php print($attributes);?>>
  <!--(begin_body)-->
  <!--(begin_page_top)-->
  <?php print $page_top; ?>
  <!--(end_page_top)-->
  <!--(begin_page)-->
  <?php print($page); ?>
  <!--(end_page)-->
  <!--(begin_page_bottom)-->
  <?php print $page_bottom; ?>
  <!--(end_page_bottom)-->
  <!--(end_body)-->
</body>
</html>
