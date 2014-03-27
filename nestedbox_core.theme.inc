<?php

/**
 * Implements hook_theme()
 *
 * @return array
 */
function nestedbox_core_theme() {
  return array(
    'nestedbox' => array(
      'render element' => 'elements',
      'template' => 'nestedbox',
      # 'path' => drupal_get_path('module', 'nestedbox_core') . '/theme',
      'file' => 'nestedbox_core.theme.inc',
    ),
  );
}

/**
 * Preprocess variables for theme('nestedbox').
 *
 * @param array $variables
 */
function template_preprocess_nestedbox(&$variables) {
  module_load_include('theme.inc', 'entity', 'theme/entity');
  template_preprocess_entity($variables);
  unset($variables['title']);
}
