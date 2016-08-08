<?php
use Drupal\nestedbox_core\NestedBoxType;

/**
 * Generates the nestedbox type editing form.
 *
 * @param array $form
 * @param array $form_state
 * @param NestedBoxType $nestedbox_type
 * @param string $op
 *
 * @return mixed
 */
function nestedbox_type_form($form, &$form_state, NestedBoxType $nestedbox_type, $op = 'edit') {

  if ($op == 'clone') {
    $nestedbox_type->label .= ' (cloned)';
    $nestedbox_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $nestedbox_type->label,
    '#description' => t('The human-readable name of this Nested Box type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($nestedbox_type->type) ? $nestedbox_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $nestedbox_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'nestedbox_get_type',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this Nested Box type. '
      . 'It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Nested Box type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$nestedbox_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete nestedbox type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('nestedbox_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function nestedbox_type_form_submit(&$form, &$form_state) {
  /** @var NestedBoxType $nestedbox_type */
  $nestedbox_type = entity_ui_form_submit_build_entity($form, $form_state);
  $nestedbox_type->save();
  $form_state['redirect'] = 'admin/structure/nestedbox-types';
}

/**
 * Form API submit callback for the delete button.
 */
function nestedbox_type_form_submit_delete(&$form, &$form_state) {
  /** @var NestedBoxType $nestedbox_type */
  $nestedbox_type = $form_state['nestedbox_type'];
  $form_state['redirect'] = 'admin/structure/nestedbox-types/manage/' . $nestedbox_type->type . '/delete';
}

/**
 * Nestedbox type delete form.
 */
function nestedbox_type_form_delete_confirm($form, &$form_state, $task_type) {
  if (!empty($task_type)) {
    $form_state['task_type'] = $task_type;
    $form['task_type_id'] = array(
      '#type' => 'value',
      '#value' => entity_id('nestedbox_type', $task_type)
    );
    return confirm_form($form,
      t('Are you sure you want to delete task type %title?', array('%title' => entity_label('nestedbox_type', $task_type))),
      'task/' . entity_id('nestedbox_type', $task_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
  else {
    $form_state['redirect'] = 'admin/structure/nestedbox-types';
  }
}

/**
 * Nestedbox type delete form submit handler.
 */
function nestedbox_type_form_delete_confirm_submit($form, &$form_state) {
  $nestedbox_type = $form_state['task_type'];
  entity_delete('nestedbox_type', entity_id('nestedbox_type' ,$nestedbox_type));
  watchdog('nestedbox_type', '@type: deleted %title.', array('@type' => $nestedbox_type->type, '%title' => $nestedbox_type->label));
  drupal_set_message(t('%title has been deleted.', array('%title' => $nestedbox_type->label)));
  $form_state['redirect'] = 'admin/structure/nestedbox-types';
}
