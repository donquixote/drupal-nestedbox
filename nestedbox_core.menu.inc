<?php

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
