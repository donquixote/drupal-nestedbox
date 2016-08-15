<?php


namespace Drupal\nestedbox_core;


class NestedBoxTypeUIController extends \EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   *
   * @return array[]
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage nestedbox entity types, '
      . 'including adding and removing fields and the display of fields.';
    return $items;
  }

  /**
   * Builds the operation form.
   *
   * For the export operation a serialized string of the entity is directly
   * shown in the form (no submit function needed).
   *
   * @param array $form
   * @param array $form_state
   * @param \Drupal\nestedbox_core\NestedBoxType|null $nestedbox_type
   * @param string $op
   *
   * @return array[]
   */
  public function operationForm($form, &$form_state, $nestedbox_type, $op) {

    $list_path = 'admin/structure/nestedbox-types';
    $base_path = 'admin/structure/nestedbox-types/manage/' . $nestedbox_type->type;

    if ($op === 'delete' || $op === 'revert') {
      if (!$nestedbox_type->hasStatus(ENTITY_IN_CODE)) {
        // Entity lives only in the database.
        if ($op === 'revert') {
          // Revert is not allowed, but delete is.
          $text = t(
            'The entity cannot be reverted, but it can be !deleted.',
            array('!deleted' => l(t('deleted'), $base_path . '/delete')));
        }
      }
      elseif ($nestedbox_type->hasStatus(ENTITY_OVERRIDDEN)) {
        // Entity lives in code, and is overridden.
        if ($op === 'delete') {
          // Delete is not allowed, but revert is.
          $text = t(
            'The entity cannot be deleted, but it can be !reverted.',
            array('!reverted' => l(t('reverted'), $base_path . '/revert')));
        }
      }
      else {
        // Entity lives in code, and is not overridden.
        if ($op === 'delete') {
          $text = t('The nested box type is in its default state, and cannot be reverted or deleted.');
        }
        elseif ($op === 'revert') {
          $text = t('The nested box type is in its default state, and cannot be reverted.');
        }
      }

      if (isset($text)) {
        $form['text'] = [
          '#markup' => '<p>' . $text . '</p>',
        ];
        $form['back'] = [
          '#markup' => '<p>' . l(t('Back to list'), $list_path) . '</p>',
        ];
        return $form;
      }
    }

    return parent::operationForm($form, $form_state, $nestedbox_type, $op);
  }
} 
