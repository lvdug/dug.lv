<?php

/**
 * Override or insert variables into the block template.
 */
function lvdug_preprocess_block(&$variables) {
  // In the "featured" region visually hide block titles.
  if ($variables['block']->region == 'featured') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}
