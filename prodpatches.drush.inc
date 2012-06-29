<?php

/**
 * Implementation of hook_drush_command().
 */
function prodpatches_drush_command() {
  $items = array();

  $items['production-patches'] = array(
    'description' => 'Apply production patches',
    'drupal dependencies' => array(),
    'aliases' => array('pp'),
  );

  return $items;
}

function drush_prodpatches_production_patches() {
  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $command = drush_get_context('command');
  $patches = $command['path'];
  foreach(file_scan_directory(realpath($patches . '/patches/'), '\.patch$') as $file) {
    $path = array_filter(explode('/', $file->filename));
    array_pop($path);
    $project = array_pop($path);
    if ($project == 'drupal') {
      $path = $drupal_root . '/';
      $type = '';
    }
    else {
      $type = array_pop($path);
      $path = $drupal_root . '/' . drupal_get_path($type, $project) . '/';
    }

    if (drush_shell_exec("patch -p1 -d %s < %s", $path, $file->filename)) {
      drush_print('Applied patch to ' . $project . ($type ? ' of type ' . $type : '') . ' with filename ' . $file->name);
    }
    else {
      drush_print('Failed to apply patch to ' . $project . ' of type ' . $type . ' with filename ' . $file->name);
    }
  }
}
