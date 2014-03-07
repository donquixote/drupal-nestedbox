<?php


namespace Drupal\nestedbox;


class NestedBoxType extends \Entity {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }

  /**
   * @var string
   */
  public $type;

  /**
   * @var string
   */
  public $label;

  /**
   * @var string
   */
  public $title_label;

  /**
   * @param array $values
   */
  public function __construct(array $values = array()) {
    $this->title_label = t('Administrative title');
    parent::__construct($values, 'nestedbox_type');
  }

} 
