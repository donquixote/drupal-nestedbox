<?php


namespace Drupal\nestedbox;


class NestedBox extends \Entity {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }

  /**
   * @var int
   */
  public $nestedbox_id;

  /**
   * @var string|null
   */
  public $admin_title;

  /**
   * @var string
   */
  public $type;

  /**
   * @param array $values
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'nestedbox');
  }

  /**
   * @return int|null
   */
  protected function defaultLabel() {
    return !empty($this->admin_title) ? $this->admin_title : $this->nestedbox_id;
  }

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => 'nestedbox/' . $this->nestedbox_id);
  }

} 
