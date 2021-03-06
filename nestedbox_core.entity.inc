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
    'view modes' => array(
      'default' => array(
        'label' => t('Default'),
        'custom settings' => FALSE,
      ),
    ),
    'entity class' => NestedBox::class,
    'controller class' => NestedBoxController::class,
    'metadata controller class' => '',
    'fieldable' => TRUE,
    /* @see entity_operations_entity_uri() */
    'uri callback' => 'entity_operations_entity_uri',
    /* @see entity_class_label() */
    'label callback' => 'entity_class_label',
    'module' => 'nestedbox_core',
    /* @see nestedbox_access() */
    'access callback' => 'nestedbox_access',
    'admin ui' => array(
      'path' => 'admin/content/nestedbox',
      // We need our own controller for this, because we're using generic
      // entity form operations.
      'controller class' => EntityOperationsDefaultAdminUIController::class,
      // @todo All the paths use entity_object_load(). A nestedbox_load() does not exist.
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
      'controller' => NestedBoxInlineEntityFormController::class,
    );
  }

  $info['nestedbox_type'] = array(
    'label' => t('Nested Box type'),
    'entity class' => NestedBoxType::class,
    'controller class' => NestedBoxTypeController::class,
    'base table' => 'nestedbox_type',
    'fieldable' => FALSE,
    'bundle of' => 'nestedbox',
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'name' => 'type',
      'label' => 'label',
    ),
    /* @see nestedbox_type_access() */
    'access callback' => 'nestedbox_type_access',
    'module' => 'nestedbox_core',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/nestedbox-types',
      'file' => 'nestedbox_type.admin.inc',
      'controller class' => NestedBoxTypeUIController::class,
      /* @see nestedbox_type_load() */
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
 *
 * @param array $entity_info
 */
function nestedbox_core_entity_info_alter(array &$entity_info) {
  foreach (nestedbox_get_types() as $type => $info) {
    $entity_info['nestedbox']['bundles'][$type] = array(
      'label' => $info->label,
      'admin' => array(
        /* @see nestedbox_type_load() */
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
      /* @see nestedbox_core_menu_alter() */
      'handler' => EntityOperationsOperationAddGeneric::class,
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'view' => array(
      // Or try EntityOperationsOperationEntityViewOperations!
      'handler' => EntityOperationsOperationEntityView::class,
      'provision' => array(
        'menu' => array(
          'default' => TRUE,
        ),
        'views field' => TRUE,
      ),
    ),
    'edit' => array(
      'handler' => EntityOperationsOperationEditGeneric::class,
      'provision' => array(
        'menu' => TRUE,
        'views field' => TRUE,
      ),
    ),
    'delete' => array(
      'handler' => EntityOperationsOperationDelete::class,
      'provision' => array(
        'menu' => TRUE,
        'views field' => TRUE,
      ),
    ),
    'devel/devel' => array(
      'handler' => EntityOperationsOperationDevel::class,
      'provision' => array(
        'menu' => array(
          'default secondary' => TRUE,
        ),
      ),
    ),
    'devel/token' => array(
      // Note this only shows if you have Entity Token module enabled.
      'handler' => EntityOperationsOperationToken::class,
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'devel/metadata' => array(
      'handler' => EntityOperationsOperationMetadata::class,
      'provision' => array(
        'menu' => TRUE,
      ),
    ),
    'author' => array(
      'handler' => EntityOperationsOperationSetOwner::class,
      'provision' => array(
        'menu' => TRUE,
        //'entity view' => TRUE,
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
    /* @see entity_property_verbatim_set() */
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['admin_title'] = array(
    'type' => 'text',
    'schema_field' => 'admin_title',
    'required' => FALSE,
    'label' => t('Administrative title'),
    /* @see entity_property_verbatim_set() */
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['type'] = array(
    'type' => 'text',
    'schema field' => 'type',
    'required' => TRUE,
    'label' => t('Nested Box type (bundle)'),
    /* @see entity_property_verbatim_set() */
    'setter callback' => 'entity_property_verbatim_set',
  );
  return $info;
}
