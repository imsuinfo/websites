<?php

/**
 * @file
 * Default theme implementation to display node information in the accessibility tab page.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $node: The node whose information is to be displayed.
 */
?>
<div class="quail_api_node-node_information quail_api_node-items">
  <div class="quail_api_node-node_information-nid quail_api_node-node_information-item">
    <span class="quail_api_node-node_information-nid-object quail_api_node-node_information-object"><?php print(t("Node ID")); ?></span>
    <span class="quail_api_node-node_information-nid-content quail_api_node-node_information-content"><?php print($node->nid); ?></span>
  </div>
  <div class="quail_api_node-node_information-vid quail_api_node-node_information-item">
    <span class="quail_api_node-node_information-vid-object quail_api_node-node_information-object"><?php print(t("Revision ID")); ?></span>
    <span class="quail_api_node-node_information-vid-content quail_api_node-node_information-content"><?php print($node->vid); ?></span>
  </div>
  <div class="quail_api_node-node_information-author quail_api_node-node_information-item">
    <span class="quail_api_node-node_information-author-object quail_api_node-node_information-object"><?php print(t("Revision Author")); ?></span>
    <span class="quail_api_node-node_information-author-content quail_api_node-node_information-content"><?php print(theme('username', array('account' => $node))); ?></span>
  </div>
  <div class="quail_api_node-node_information-date quail_api_node-node_information-item">
    <span class="quail_api_node-node_information-date-object quail_api_node-node_information-object"><?php print(t("Revision Date")); ?></span>
    <span class="quail_api_node-node_information-date-content quail_api_node-node_information-content"><?php print(format_date($node->revision_timestamp, 'short')); ?></span>
  </div>
</div>
