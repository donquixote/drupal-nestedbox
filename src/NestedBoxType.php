<?php


namespace Drupal\nestedbox_core;


class NestedBoxType extends \Entity {

  /**
   * @var string|int|null
   *   E.g. '3' or 3, depending where the data is coming from.
   */
  public $id;

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
   * @var string|int|null
   */
  public $status;

  /**
   * @param array $values
   */
  public function __construct(array $values = array()) {
    $this->title_label = t('Administrative title');
    parent::__construct($values, 'nestedbox_type');
  }

} 
