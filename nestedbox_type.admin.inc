<?php
use Drupal\nestedbox_core\NestedBoxType;

/**
 * Generates the nestedbox type editing form.
 *
 * @param array $form
 * @param array $form_state
 * @param \Drupal\nestedbox_core\NestedBoxType $nestedbox_type
 * @param string $op
 *
 * @return mixed
 */
function nestedbox_type_form(
  array $form,
  /** @noinspection PhpUnusedParameterInspection */ array &$form_state,
  NestedBoxType $nestedbox_type,
  $op = 'edit'
) {

  if ($op === 'clone') {
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

  if (!empty($nestedbox_type->type) && $op !== 'add') {
    if (!$nestedbox_type->hasStatus(ENTITY_IN_CODE)) {
      // Entity lives only in the database.
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete nestedbox type'),
        '#weight' => 45,
        '#limit_validation_errors' => array(),
        /* @see nestedbox_type_form_submit_redirect() */
        '#submit' => array('nestedbox_type_form_submit_redirect'),
        '#redirect_path' => 'admin/structure/nestedbox-types/manage/' . $nestedbox_type->type . '/delete',
      );
    }
    elseif ($nestedbox_type->hasStatus(ENTITY_OVERRIDDEN)) {
      // Entity lives in code, and is overridden.
      $form['actions']['revert'] = array(
        '#type' => 'submit',
        '#value' => t('Revert nestedbox type'),
        '#weight' => 45,
        '#limit_validation_errors' => array(),
        /* @see nestedbox_type_form_submit_redirect() */
        '#submit' => array('nestedbox_type_form_submit_redirect'),
        '#redirect_path' => 'admin/structure/nestedbox-types/manage/' . $nestedbox_type->type . '/revert',
      );
    }
  }

  return $form;
}

/**
 * Form API submit callback for the type form.
 *
 * @param array $form
 * @param array $form_state
 */
function nestedbox_type_form_submit(array &$form, array &$form_state) {
  /** @var \Drupal\nestedbox_core\NestedBoxType $nestedbox_type */
  $nestedbox_type = entity_ui_form_submit_build_entity($form, $form_state);
  $nestedbox_type->save();
  $form_state['redirect'] = 'admin/structure/nestedbox-types';
}

/**
 * Form API submit callback for the delete or revert button.
 *
 * @param array $form
 * @param array $form_state
 */
function nestedbox_type_form_submit_redirect(
  /** @noinspection PhpUnusedParameterInspection */ array &$form,
  array &$form_state
) {
  $form_state['redirect'] = $form_state['clicked_button']['#redirect_path'];
}
