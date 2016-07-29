<?php
/**
 * @file
 * Public API functions and callbacks for Nested box.
 * This file contains all functions that are not prefixed with "nestedbox_core".
 */

/**
 * Gets an array of all nestedbox types, keyed by the type name.
 *
 * @return \Drupal\nestedbox_core\NestedBoxType[]
 *   An array of nestedbox types. Array keys are the machine names.
 */
function nestedbox_get_types() {
  // entity_load() will get the Entity controller for our nestedbox entity and
  // call the load function of that object - we are loading entities by name
  // here.
  return entity_load_multiple_by_name('nestedbox_type');
}

/**
 * Gets an nestedbox type.
 *
 * @param string $type_name
 *   If set, the type with the given name is returned.
 *
 * @return \Drupal\nestedbox_core\NestedBoxType|null
 *   The nested box type entity with the given name, or NULL if not found.
 */
function nestedbox_get_type($type_name) {
  // entity_load() will get the Entity controller for our nestedbox entity and
  // call the load function of that object - we are loading entities by name
  // here.
  $types = entity_load_multiple_by_name('nestedbox_type', array($type_name));
  if (!isset($types[$type_name])) {
    return NULL;
  }
  return $types[$type_name];
}

/**
 * Menu argument loader; Load a nestedbox type by string.
 *
 * @param string $type_name
 *   The machine-readable name of a nestedbox type to load.
 *
 * @return \Drupal\nestedbox_core\NestedBoxType|false
 *   A nestedbox type array, or FALSE if $type does not exist.
 */
function nestedbox_type_load($type_name) {
  return nestedbox_get_type($type_name);
}

/**
 * Implements callback_entity_access().
 *
 * @param string $op
 * @param \Drupal\nestedbox_core\NestedBox $entity
 * @param object $account
 * @param string $entity_type
 *
 * @return bool
 */
function nestedbox_access(
  /** @noinspection PhpUnusedParameterInspection */ $op,
  /** @noinspection PhpUnusedParameterInspection */ $entity,
  /** @noinspection PhpUnusedParameterInspection */ $account,
  /** @noinspection PhpUnusedParameterInspection */ $entity_type
) {
  // TODO: add access control here.
  return TRUE;
}

/**
 * Load multiple instances of nestedbox_type.
 *
 * @param string $type_name
 *   Name of the entity type to load.
 *
 * @return mixed
 *   Found entity type, or false.
 *
 * @todo Is this even used anywhere?
 */
function nestedbox_type_load_multiple($type_name = NULL) {
  $types = entity_load_multiple_by_name('nestedbox_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Access callback for nestedbox_type.
 *
 * @param string $op
 * @param \Drupal\nestedbox_core\NestedBoxType $entity
 *
 * @return bool
 */
function nestedbox_type_access(
  /** @noinspection PhpUnusedParameterInspection */ $op,
  /** @noinspection PhpUnusedParameterInspection */ $entity = NULL
) {
  return user_access('administer nestedbox types');
}
