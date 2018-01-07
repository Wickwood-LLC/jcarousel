<?php

/**
 * @file
 * Drush integration for jcarousel.
 */

/**
 * The jCarousel plugin URI.
 */
define('JCAROUSEL_DOWNLOAD_URI', 'https://github.com/jsor/jcarousel/archive/0.3.5.zip');

/**
 * Implements hook_drush_command().
 */
function jcarousel_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  // No bootstrap.
  $items['jcarousel-plugin'] = array(
    'callback' => 'drush_jcarousel_lib_plugin',
    'description' => dt('Download and install the jCarousel plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the jCarousel plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('jcarouselplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function jcarousel_drush_help($section) {
  if ($section == 'drush:jcarousel-plugin') {
    return dt('Download and install the jCarousel plugin from https://github.com/jsor/jcarousel, default location is /libraries.');
  }
}

/**
 * Command to download the jCarousel plugin.
 */
function drush_jcarousel_lib_plugin() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(JCAROUSEL_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Remove any existing jCarousel plugin directory.
    if (is_dir($dirname) || is_dir('jcarousel')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('jcarousel', TRUE);
      drush_log(dt('A existing jCarousel plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname);

    // Change the directory name to "jCarousel" if needed.
    if ($dirname != 'jcarousel') {
      drush_move_dir($dirname, 'jcarousel', TRUE);
      $dirname = 'jcarousel';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('jCarousel plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the jCarousel plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
