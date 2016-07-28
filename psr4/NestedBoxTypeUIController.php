<?php


namespace Drupal\nestedbox_core;


class NestedBoxTypeUIController extends \EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage nestedbox entity types, '
      . 'including adding and removing fields and the display of fields.';
    return $items;
  }
} 
