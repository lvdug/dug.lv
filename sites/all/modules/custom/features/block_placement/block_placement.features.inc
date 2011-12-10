<?php
/**
 * @file
 * block_placement.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function block_placement_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
