<?php

/**
 * Implements hook_menu().
 */
function nestedbox_core_menu() {
  $items = array();

  // URI for deleting entity bundles.
  $items['admin/structure/nestedbox-types/manage/%nestedbox_type/delete'] = array(
    'title' => 'Delete nested box type',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('nestedbox_type_form_delete_confirm', 4),
    'access arguments' => array('administer nestedbox types'),
    'weight' => 1,
    'type' => MENU_NORMAL_ITEM,
    'file' => 'nestedbox_type.admin.inc',
  );

  return $items;
}

/**
 * Implements hook_menu_alter()
 *
 * @param array $items
 */
function nestedbox_core_menu_alter(&$items) {

}

/**
 * Implements hook_admin_menu_map()
 */
function nestedbox_core_admin_menu_map() {

  $map['admin/structure/nestedbox-types/manage/%nestedbox_type'] = array(
    'parent' => 'admin/structure/nestedbox-types',
    'arguments' => array(
      array(
        '%nestedbox_type' => array_keys(nestedbox_get_types()),
      )
    )
  );

  return $map;
}
