<?php if ($cf['show']['messages'] || $cf['show']['page']['messages']){ ?>
  <aside id='document-message_region' tabindex="2">
    <!--(begin_messages)-->
    <h2>Messages</h2>
    <div>
      <?php if ($cf['show']['messages']) print($messages); ?>
      <?php if ($cf['show']['page']['messages']) print($page['messages']); ?>
    </div>
    <!--(end_messages)-->
  </aside>
<?php if ($cf['show']['page']['website_menu']){ ?>
  <nav id='document-website_menu_region' tabindex="3">
    <!--(begin_website_menu)-->
    <h2>Website Menu</h2>
    <?php if ($cf['show']['page']['website_menu']) print($page['website_menu']); ?>
    <!--(end_website_menu)-->
  </nav>
<?php } ?>
<?php if ($cf['show']['editing']){ ?>
  <aside id='document-editing_region' tabindex="4">
    <!--(begin_editing)-->
    <h2>Editing</h2>
    <?php if ($cf['show']['primary_local_tasks']){ ?>
      <div id='document-primary_tabs'>
        <!--(begin_primary_tabs)-->
        <ul class='tabs primary'><?php print($primary_local_tasks); ?></ul>
        <!--(end_primary_tabs)-->
      </div>
    <?php } ?>
    <?php if ($cf['show']['breadcrumb']){ ?>
      <!--(begin_breadcrumb)-->
      <?php print($breadcrumb); ?>
      <!--(end_breadcrumb)-->
    <?php } ?>
    <!--(end_editing)-->
  </aside>
<?php } ?>
<?php if ($cf['show']['page']['help']){ ?>
  <aside id='document-help_region' tabindex="5">
    <!--(begin_help)-->
    <h2>Help</h2>
    <div>
      <div id='document-help_region-icon'></div>
      <?php print($page['help']); ?>
    </div>
    <!--(end_help)-->
  </aside>
<?php } ?>
<?php } ?>
<?php if ($cf['show']['node_information']){ ?>
  <aside id='document-node_information_region' tabindex="6">
    <!--(begin_node_information)-->
    <h2>Node Information</h2>
    <div>
      <div id='document-node_information_region-icon'></div>
      <?php print($node_information); ?>
    </div>
    <!--(end_node_information)-->
  </aside>
<?php } ?>
<?php if ($cf['show']['page']['content']){ ?>
  <!--(begin_content)-->
  <?php print($page['content']); ?>
  <!--(end_content)-->
<?php } ?>
