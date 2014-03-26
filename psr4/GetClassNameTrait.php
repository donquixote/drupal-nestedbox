<?php


namespace Drupal\nestedbox_core;


trait GetClassNameTrait {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }
} 
