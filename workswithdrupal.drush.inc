<?php

function drush_workswithdrupal_module_compatibility() {

  $api_url = 'http://drupal.balaclark.com';
  $versions = array(8, 7, 6);
  $modules = implode('+', module_list());

  $options = array(
    'headers' => array('Content-Type' => 'application/json')
  );

  $compatibility = array();

  foreach ($versions as $version) {

    $url = sprintf('%s/%d/%s', $api_url, $version, $modules);
    $result = drupal_http_request($url, $options);

    if ($result->code != 200) {
      drush_set_error('HTTP error, please try again');
      break;
    }

    $compatibility[$version] = json_decode($result->data);
  }

  // TODO: different view modes

  _workswithdrupal_drush_view_plain($compatibility);
}

function _workswithdrupal_drush_view_plain(array $compatibility) {

  $notfound = current($compatibility)->modules->notfound;

  // $unknown = current(count($compatibility)->notfound);
  $rows = array();

  foreach ($compatibility as $version => $data) {
    $rows[] = array(
      'Drupal ' . $version,
      $data->percentage . '%'
    );
  }

  drush_print('');
  drush_print_table($rows);
  drush_print('');
  drush_print(
    count($notfound) .
    ' modules were unknown and have been ignored. These are normally custom or sub-modules.'
  );
}
