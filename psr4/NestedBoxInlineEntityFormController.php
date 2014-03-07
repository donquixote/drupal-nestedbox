<?php


namespace Drupal\nestedbox;


class NestedBoxInlineEntityFormController extends \EntityInlineEntityFormController {

  /**
   * @return string
   */
  static function getClassName() {
    return get_called_class();
  }

  /**
   * Overrides \EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Nested Box'),
      'plural' => t('Nested Boxes'),
    );
    return $labels;
  }

  /**
   * Overrides \EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    /** @var NestedBox $nestedbox */
    $nestedbox = $entity_form['#entity'];
    # /** @var NestedBoxType $nestedbox_type */
    # $nestedbox_type = nestedbox_get_type($entity_form['#bundle']);
    # $extra_fields = field_info_extra_fields('nestedbox', $nestedbox->type, 'form');

    // Do some prep work on the node, similarly to node_form().
    if (!isset($nestedbox->admin_title)) {
      $nestedbox->admin_title = NULL;
    }
    # node_object_prepare($nestedbox);

    $entity_form['admin_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Administrative title'),
      '#required' => FALSE,
      '#default_value' => $nestedbox->admin_title,
      '#maxlength' => 255,
      // The label might be missing if the Title module has replaced it.
      '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
    );
    /*
    $entity_form['status'] = array(
      '#type' => 'radios',
      '#access' => user_access('administer nodes'),
      '#title' => t('Status'),
      '#default_value' => $nestedbox->status,
      '#options' => array(1 => t('Published'), 0 => t('Unpublished')),
      '#required' => TRUE,
      '#weight' => 99,
    );
    */

    $langcode = entity_language('node', $nestedbox);
    field_attach_form('nestedbox', $nestedbox, $entity_form, $form_state, $langcode);

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function _entityFormSubmit(&$entity_form, &$form_state) {
    parent::entityFormSubmit($entity_form, $form_state);

    node_submit($entity_form['#entity']);
    $child_form_state = form_state_defaults();
    $child_form_state['values'] = drupal_array_get_nested_value($form_state['values'], $entity_form['#parents']);
    foreach (module_implements('node_submit') as $module) {
      $function = $module . '_node_submit';
      $function($entity_form['#entity'], $entity_form, $child_form_state);
    }
  }
} 
