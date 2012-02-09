<?php

/**
 * @file
 * Default theme implementation to display the document node information region.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $title: The node title.
 * - $nid: The node id.
 * - $vid: The node revision id.
 * - $uid: The user id.
 * - $username: The user name.
 * - $revision_uid: The revision user id.
 * - $revision_username: The revision user name.
 * - $created: The date the node was created.
 * - $changed: The date the node was changed.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 */
?>
<div class="<?php print($base_class); ?>-wrapper">
  <div class="<?php print($base_class); ?>-group-title <?php print($base_class); ?>-group-odd <?php print($base_class); ?>-group">
    <div class="<?php print($base_class); ?>-title-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-title-label <?php print($base_class); ?>-item-label">
        <?php print(t("Title")); ?>
      </div>
      <div class="<?php print($base_class); ?>-title-information <?php print($base_class); ?>-item-value">
        <?php print($title); ?>
      </div>
    </div>
  </div>

  <div class="<?php print($base_class); ?>-group-ids <?php print($base_class); ?>-group-even <?php print($base_class); ?>-group">
    <div class="<?php print($base_class); ?>-nid-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-nid-label <?php print($base_class); ?>-item-label">
        <?php print(t("Node ID")); ?>
      </div>
      <div class="<?php print($base_class); ?>-nid-information <?php print($base_class); ?>-item-value">
        <?php print($nid); ?>
      </div>
    </div>
    <div class="<?php print($base_class); ?>-vid-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-vid-label <?php print($base_class); ?>-item-label">
        <?php print(t("Revision ID")); ?>
      </div>
      <div class="<?php print($base_class); ?>-vid-information <?php print($base_class); ?>-item-value">
        <?php print($vid); ?>
      </div>
    </div>
  </div>

  <div class="<?php print($base_class); ?>-group-create <?php print($base_class); ?>-group-even <?php print($base_class); ?>-group">
    <div class="<?php print($base_class); ?>-user-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-user-label <?php print($base_class); ?>-item-label">
        <?php print(t("Created By")); ?>
      </div>
      <div class="<?php print($base_class); ?>-user-information <?php print($base_class); ?>-item-value">
        <?php print($username); ?>
      </div>
    </div>
    <div class="<?php print($base_class); ?>-created-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-created-label <?php print($base_class); ?>-item-label">
        <?php print(t("Created On")); ?>
      </div>
      <div class="<?php print($base_class); ?>-created-information <?php print($base_class); ?>-item-value">
        <?php print($created); ?>
      </div>
    </div>
  </div>

  <div class="<?php print($base_class); ?>-group-update <?php print($base_class); ?>-group-even <?php print($base_class); ?>-group">
    <div class="<?php print($base_class); ?>-revision_user-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-revision_user-label <?php print($base_class); ?>-item-label">
        <?php print(t("Updated By")); ?>
      </div>
      <div class="<?php print($base_class); ?>-revision_user-information <?php print($base_class); ?>-item-value">
        <?php print($revision_username); ?>
      </div>
    </div>
    <div class="<?php print($base_class); ?>-changed-wrapper <?php print($base_class); ?>-item-wrapper">
      <div class="<?php print($base_class); ?>-changed-label <?php print($base_class); ?>-item-label">
        <?php print(t("Updated On")); ?>
      </div>
      <div class="<?php print($base_class); ?>-changed-information <?php print($base_class); ?>-item-value">
        <?php print($changed); ?>
      </div>
    </div>
  </div>
</div>
