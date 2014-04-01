<?php

use Drupal\nestedbox_core\NestedBox;
use Drupal\nestedbox_core\NestedBoxController;
use Drupal\nestedbox_core\NestedBoxInlineEntityFormController;
use Drupal\nestedbox_core\NestedBoxType;
use Drupal\nestedbox_core\NestedBoxTypeController;
use Drupal\nestedbox_core\NestedBoxTypeUIController;

/**
 * Implements hook_entity_info().
 *
 * Inspired by entity_operations/example_entity, and typical_entity_example.
 */
function nestedbox_core_entity_info() {
  $info = array();

  $info['nestedbox'] = array(
    'label' => t('Nested Box'),
    'plural label' => t('Nested Boxes'),
    'base table' => 'nestedbox',
    'translation' => array(
      'entity_translation' => array(
        'base path' => 'admin/content/nestedbox/%nestedbox',
      ),
    ),
    'entity keys' => array(
      'id' => 'nestedbox_id',
      'bundle' => 'type',
      'label' => 'admin_title',
    ),
    'bundle keys' => array(
      'bundle' => 'type',
    ),
    'entity class' => NestedBox::getClassName(),
    'controller class' => NestedBoxController::getClassName(),
    'metadata controller class' => '',
    'fieldable' => TRUE,
    'uri callback' => 'entity_operations_entity_uri',
    'label callback' => 'entity_class_label',
    'module' => 'nestedbox_core',
    'access callback' => 'nestedbox_access',
    'admin ui' => array(
      'path' => 'admin/content/nestedbox',
      // We need our own controller for this, because we're using generic
      // entity form operations.
      'controller class' => 'EntityOperationsDefaultAdminUIController',
      'menu wildcard' => '%nestedbox',
    ),
    // Entity Operations API
    'operations ui' => array(
      // The base path for your entities. This is the same as your entity's URI
      // but without the ID suffix. (In fact, you can set
      // entity_operations_entity_uri() as your URI callback, which will use the
      // value here).
      'path' => 'admin/content/nestedbox',
    ),
  );

  if (module_exists('inline_entity_form')) {
    $info['nestedbox']['inline entity form'] = array(
      'controller' => NestedBoxInlineEntityFormController::getClassName(),
    );
  }

  $info['nestedbox_type'] = array(
    'label' => t('Nested Box type'),
    'entity class' => NestedBoxType::getClassName(),
    'controller class' => NestedBoxTypeController::getClassName(),
    'base table' => 'nestedbox_type',
    'fieldable' => FALSE,
    'bundle of' => 'nestedbox',
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'name' => 'type',
      'label' => 'label',
    ),
    'access callback' => 'nestedbox_type_access',
    'module' => 'nestedbox_core',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/nestedbox-types',
      'file' => 'nestedbox_type.admin.inc',
      'controller class' => NestedBoxTypeUIController::getClassName(),
      'menu wildcard' => '%nestedbox_type',
    ),
  );

  return $info;
}

/**
 * Implements hook_entity_info_alter().
 *
 * We are adding the info about the nestedbox types via a hook to avoid a
 * recursion issue, as loading the nestedbox types requires the entity info as
 * well.
 */
function nestedbox_core_entity_info_alter(&$entity_info) {
  foreach (nestedbox_get_types() as $type => $info) {
    $entity_info['nestedbox']['bundles'][$type] = array(
      'label' => $info->label,
      'admin' => array(
        'path' => 'admin/structure/nestedbox-types/manage/%nestedbox_type',
        'real path' => 'admin/structure/nestedbox-types/manage/' . $type,
        'bundle argument' => 4,
        'access arguments' => array('administer nestedbox types'),
      ),
    );
  }
}

/**
 * Implements hook_entity_operation_info().
 */
function nestedbox_core_entity_operation_info() {
  $info = array();

  $info['nestedbox'] = array(
    'add' => array(
      'handler' => 'EntityOperationsOperationAddGeneric',
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'view' => array(
      // Or try EntityOperationsOperationEntityViewOperations!
      'handler' => 'EntityOperationsOperationEntityView',
      'provision' => array(
        'menu' => array(
          'default' => TRUE,
        ),
        'views field' => TRUE,
      ),
    ),
    'edit' => array(
      'handler' => 'EntityOperationsOperationEditGeneric',
      'provision' => array(
        'menu' => TRUE,
        'views field' => TRUE,
      ),
    ),
    'delete' => array(
      'handler' => 'EntityOperationsOperationDelete',
      'provision' => array(
        'menu' => TRUE,
        'views field' => TRUE,
      ),
    ),
    'devel/devel' => array(
      'handler' => 'EntityOperationsOperationDevel',
      'provision' => array(
        'menu' => array(
          'default secondary' => TRUE,
        ),
      ),
    ),
    'devel/token' => array(
      // Note this only shows if you have Entity Token module enabled.
      'handler' => 'EntityOperationsOperationToken',
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'devel/metadata' => array(
      'handler' => 'EntityOperationsOperationMetadata',
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'author' => array(
      'handler' => 'EntityOperationsOperationSetOwner',
      'provision' => array(
        'menu' => TRUE,
        //'entity view' => TRUE,
      ),
    ),
  );

  $info['_nestedbox_type'] = array(
    'add' => array(
      'handler' => 'EntityOperationsOperationAddGeneric',
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'edit' => array(
      'handler' => 'EntityOperationsOperationEditGeneric',
      'provision' => array(
        'menu' => array(
          'default' => TRUE,
        ),
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_entity_property_info().
 *
 * This implementation is necessary for dealing with Entity Metadata Wrappers.
 * @see http://drupal.org/node/1021466
 */
function nestedbox_core_entity_property_info() {
  $info = array();
  $properties = &$info['nestedbox']['properties'];
  $properties['nestedbox_id'] = array(
    'type' => 'integer',
    'schema field' => 'nestedbox_id',
    'label' => t('The primary identifier.'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['admin_title'] = array(
    'type' => 'text',
    'schema_field' => 'admin_title',
    'required' => FALSE,
    'label' => t('Administrative title'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['type'] = array(
    'type' => 'text',
    'schema field' => 'type',
    'required' => TRUE,
    'label' => t('Nested Box type (bundle)'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  return $info;
}
