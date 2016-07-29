<?php


namespace Drupal\nestedbox_core;


class NestedBox extends \Entity {

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
  public function __construct(array $values = array()) {
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
    return array('path' => 'admin/content/nestedbox/' . $this->nestedbox_id);
  }

} 
