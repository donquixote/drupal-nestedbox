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

  /* @see nestedbox_core_entity_operation_info() */
  /* @see EntityOperationsOperationAddGeneric::menu_item() */
  if (isset($items['admin/content/nestedbox/add'])) {
    // Replace MENU_CALLBACK with MENU_LOCAL_ACTION, so a link shows up above
    // the list of nested boxes.
    $items['admin/content/nestedbox/add']['type'] = MENU_LOCAL_ACTION;
    // Rename the link from "Add" to "Add nested box".
    $items['admin/content/nestedbox/add']['title'] = 'Add nested box';
  }

  /* @see EntityOperationsDefaultUIController::hook_menu() */
  if (isset($items[$k = 'admin/content/nestedbox/%entity_object'])) {
    // Set a dynamic title. Without this, the title provided by
    // entity_operations would be just 'View', which is not what we intend.
    /* @see entity_class_label() */
    $items[$k]['title callback'] = 'entity_class_label';
    $items[$k]['title arguments'] = array(3);
  }
}

/**
 * Implements hook_admin_menu_map()
 */
function nestedbox_core_admin_menu_map() {

  $map['admin/structure/nestedbox-types/manage/%nestedbox_type'] = array(
    'parent' => 'admin/structure/nestedbox-types',
    'arguments' => array(
      array(
        /* @see nestedbox_type_load() */
        '%nestedbox_type' => array_keys(nestedbox_get_types()),
      )
    )
  );

  return $map;
}
