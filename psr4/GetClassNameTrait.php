<?php


namespace Drupal\nestedbox;


trait GetClassNameTrait {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }
} 
