<?php

$view = new view;
$view->name = 'mediabrowser';
$view->description = 'Media browser view';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'field_image_fid_1' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'label_type' => 'none',
    'format' => 'mediareference_thumbs_default',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_image_fid_1',
    'table' => 'node_data_field_image',
    'field' => 'field_image_fid',
    'relationship' => 'none',
  ),
  'title' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 0,
      'ellipsis' => 0,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'type' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 0,
      'ellipsis' => 0,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'machine_name' => 0,
    'exclude' => 0,
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'delete_node' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'text' => '',
    'exclude' => 0,
    'id' => 'delete_node',
    'table' => 'node',
    'field' => 'delete_node',
    'relationship' => 'none',
  ),
  'edit_node' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 0,
      'ellipsis' => 0,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'text' => '',
    'exclude' => 0,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'relationship' => 'none',
  ),
  'null' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'embedded_form' => 'mediareference_attach_form',
    'exclude' => 0,
    'id' => 'null',
    'table' => 'views_embed_form',
    'field' => 'null',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'image' => 'image',
      'slideshow' => 'slideshow',
      'video' => 'video',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'type_op',
      'identifier' => 'type',
      'label' => 'Type',
      'optional' => 1,
      'single' => 0,
      'remember' => 0,
      'reduce' => 1,
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'title' => array(
    'operator' => 'contains',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'title_op',
      'identifier' => 'title',
      'label' => 'Title contains',
      'optional' => 1,
      'remember' => 0,
    ),
    'case' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'type_1' => array(
    'operator' => 'in',
    'value' => array(
      'image' => 'image',
      'slideshow' => 'slideshow',
      'video' => 'video',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type_1',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'perm' => 'create story content',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Media Browser');
$handler->override_option('css_class', 'mediabrowser-view');
$handler->override_option('header', '<strong>Add new media:</strong> <a href="/node/add/image?destination=mediabrowser">add image</a>, <a href="/node/add/video?destination=mediabrowser">add video</a>, <a href="/node/add/slideshow?destination=mediabrowser">add slideshow</a>');
$handler->override_option('header_format', '2');
$handler->override_option('header_empty', 1);
$handler->override_option('empty', 'No media were found.');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 8);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'grid');
$handler->override_option('style_options', array(
  'grouping' => '',
  'columns' => '4',
  'alignment' => 'horizontal',
));
