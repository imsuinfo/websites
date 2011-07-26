<?php ?><?xml version="1.0" encoding="UTF-8" ?>
<rss version="2.0">
<channel>
  <title><?php print($rss_title); ?></title>
  <description><?php print($rss_description); ?></description>
  <link><?php print($rss_link); ?></link>
  <?php if (!empty($rss_timestamp)) { ?><lastBuildDate><?php print($rss_timestamp); ?></lastBuildDate><?php } ?>

  <item>
    <?php print($page); ?>
  </item>
</channel>
</rss>

