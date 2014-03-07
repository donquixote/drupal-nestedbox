<?php


namespace Drupal\nestedbox;


class NestedBoxTypeController  extends \EntityAPIControllerExportable {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }

  /**
   * Create a model type - we first set up the values that are specific
   * to our model type schema but then also go through the EntityAPIController
   * function.
   *
   * @param array $values
   *   The machine-readable type of the model.
   *
   * @return NestedBoxType
   *   A model type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
      'id' => '',
      'is_new' => TRUE,
      'data' => '',
    );
    $nestedbox_type = parent::create($values);
    return $nestedbox_type;
  }

} 
