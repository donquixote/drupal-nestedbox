<?php
use Drupal\nestedbox_core\NestedBox;

/**
 * Form API submit callback for the delete button.
 */
function nestedbox_form_submit_delete(&$form, &$form_state) {
  /** @var NestedBox $nestedbox */
  $nestedbox = $form_state['nestedbox_type'];
  $form_state['redirect'] = 'admin/content/nestedbox/' . $nestedbox->nestedbox_id . '/delete';
}
