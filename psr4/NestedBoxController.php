<?php


namespace Drupal\nestedbox_core;


class NestedBoxController extends \EntityAPIController {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }

  /**
   * Create a model - we first set up the values that are specific
   * to our model schema but then also go through the EntityAPIController
   * function.
   *
   * @param array $values
   *
   * @return NestedBox
   *   A model object with all default fields initialized.
   */
  public function _create(array $values = array()) {
    // Add values that are specific to our Nestedbox
    $values += array(
      'model_id' => '',
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
      'data' => '',
    );

    $model = parent::create($values);
    return $model;
  }

  /**
   * @param NestedBox $entity
   * @param string $view_mode
   * @param string|null $langcode
   *
   * @return array
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL) {
    $content = parent::buildContent($entity, $view_mode, $langcode);
    $content['#theme'] = 'nestedbox';
    return $content;
  }
} 
