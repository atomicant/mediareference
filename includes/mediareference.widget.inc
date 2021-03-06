<?php

/**
 * @file
 * CCK widget hook implementations for mediareference widget
 */

//=========================================
//    CCK WIDGET
//=========================================

/**
 * Implementation of FAPI hook_elements()
 */
function mediareference_elements() {

  $elements = array(
    'mediareference_widget' => array(
      '#input' => TRUE,
      '#columns' => array('nid', 'fid', 'title', 'description', 'link', 'colour', 'data'),
      '#delta' => 0,
      '#process' => array('mediareference_widget_process'),
      '#value_callback' => 'mediareference_widget_value',
      '#autocomplete_path' => FALSE,
    )
  );

  return $elements;
}

/**
 * Implementation of CCK hook_widget_info()
 */
function mediareference_widget_info() {
  return array(
    'mediareference_widget' => array(
      'label' => t('Mediareference'),
      'field types' => array('mediareference'),
      'multiple values' => CONTENT_HANDLE_CORE,
      'callbacks' => array('default value' => CONTENT_CALLBACK_CUSTOM),
      'description' => t('An edit widget for media nodereference fields that allows upload/preview of the media content and its description.' ),
    ),
  );
}

/**
 * Implementation of hook_widget()
 */
function mediareference_widget(&$form, &$form_state, $field, $items, $delta = 0) {
  // Value callback taken from nodereference_widget - type: nodereference_autocomplete.
  $element = array(
    '#type' => $field['widget']['type'],
    '#default_value' => isset($items[$delta]) ? $items[$delta] : NULL,
    '#after_build' => array('mediareference_widget_after_build'),
  );

  $subfields = mediareference_field_subfields();

  // apply Mediareference special fields settings from CCK settings form
  foreach ($field['mediareference_fields'] as $type => $mode) {
    if (isset($subfields[$type]) && ($mode != MEDIAREF_FIELD_DISABLED)) {
      if ($subfields[$type]['data']) {
        $element['data'][$type]['#required'] = ($mode == MEDIAREF_FIELD_REQUIRED ? TRUE : FALSE);
      }
      else {
        $element[$type]['#required'] = ($mode == MEDIAREF_FIELD_REQUIRED ? TRUE : FALSE);
      }
    }
  }

  return $element;
}

/**
 * After build FAPI form builder
 *
 * We use after build handler because it is load always, even when form is loaded from cache.
 */
function mediareference_widget_after_build($form, &$form_state) {

  // preload colorpicker JS and CSS
  if (module_exists('colorpicker')) {
    $colorpicker_path = drupal_get_path('module', 'colorpicker');

    // Add Farbtastic color picker
    drupal_add_css('misc/farbtastic/farbtastic.css');
    drupal_add_js('misc/farbtastic/farbtastic.js');

    // Add our custom js and css for our colorpicker
    drupal_add_js("$colorpicker_path/js/colorpicker.js");
    drupal_add_css("$colorpicker_path/css/colorpicker.css");
  }


  // attach our CSS and JS files after dependendencies
  // (it allows us to override theirs CSS/JS if needed)
  $module_path = drupal_get_path('module', 'mediareference');
  drupal_add_css($module_path .'/includes/mediareference.css');
  drupal_add_js($module_path .'/includes/mediareference.js');

  return $form;
}

/**
 * The #value_callback for the mediareference_widget type element.
 *
 * NOT USED CURRENTLY
 * check mediareference_elements or mediareference_widget
 */
function mediareference_widget_value($element, $edit = FALSE) {
  $item = array();

  if (isset($element['#default_value']) && is_array($element['#default_value'])) {
    $item = &$element['#default_value'];
    mediareference_field_unserialize($item);
  }

  if ($edit) {
    $item = array_merge($item, $edit);
  }

//  dpm($item, 'mediareference_widget_value()');
  return $item;
}

/**
 * Implementation of hook_widget_settings()
 */
function mediareference_widget_settings($op, $widget) {
  switch ($op) {
    case 'form':
      $form = array();

      $rows = (isset($widget['rows']) && is_numeric($widget['rows'])) ? $widget['rows'] : 5;

      $form['description_field'] = array(
        '#type' => 'fieldset',
        '#title' => 'Settings for Description field widget',
        '#collapsible' => TRUE,
      );
      $form['description_field']['rows'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of rows in Description field'),
        '#default_value' => $rows,
        '#element_validate' => array('_text_widget_settings_row_validate'),
        '#required' => TRUE,
        '#weight' => 8,
      );

      $cols = (isset($widget['cols']) && is_numeric($widget['cols'])) ? $widget['cols'] : 40;
      $form['description_field']['cols'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of columns in Description field'),
        '#default_value' => $cols,
        '#element_validate' => array('_text_widget_settings_row_validate'),
        '#required' => TRUE,
        '#weight' => 9,
      );

      $form['thumbnails'] = array(
        '#type' => 'fieldset',
        '#title' => t('Thumbnail preview'),
        '#description' => 'At each content type select one field which will be used as a source for displaing thubnail preview in a mediareference widget.',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $options = array();
      $imagecache_presets = imagecache_presets();
      foreach ($imagecache_presets as $id => $preset) {
      	$options[$id] = $preset['presetname'];
      }

      $form['thumbnails']['thumbnail_imagecache_preset'] = array(
        '#type' => 'select',
        '#title' => t('Imagecache profile'),
        '#options' => $options,
        '#default_value' => !empty($widget['thumbnail_imagecache_preset']) ? $widget['thumbnail_imagecache_preset'] : MEDIAREF_IMAGECACHE_THUMB_PROFILE,
      );

      $form['thumbnails']['thumbnail_type_fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Content types'),
        '#description' => 'At each content type select one field which will be used as a source for displaing thubnail preview in a mediareference widget.',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );

      // $node_types = node_get_types('names');
      // $info = content_fields(NULL, 'story');
      $info = _content_type_info();
      $content_types = $info['content types'];
      unset($info);

      foreach ($content_types as $type => $type_properties) {
        $options = array('' => t('Disabled'));
        foreach ($type_properties['fields'] as $name => $fields_properties) {
          if ($fields_properties['type'] == 'filefield') {
            $options[$name] = t('Use field @field', array('@field' => $fields_properties['widget']['label']));
          }
        }

        $form['thumbnails']['thumbnail_type_fields'][$type] = array(
          '#type' => 'radios',
          '#title' => t('Thumbnail support for type %type', array('%type' => $type_properties['name'])),
          '#options' => $options,
          '#default_value' => !empty($widget['thumbnail_type_fields'][$type]) ? $widget['thumbnail_type_fields'][$type] : '',
        );
      }

      return array_merge($form, nodereference_widget_settings('form', $widget));

    case 'save':
      $settings = array();
      $settings[] = 'rows';
      $settings[] = 'cols';

      $settings[] = 'thumbnail_imagecache_preset';
      $settings[] = 'thumbnail_type_fields';

      return array_merge($settings, nodereference_widget_settings('save', $widget));
  }
}

/**
 * Process of our FAPI elements/widget
 */
function mediareference_widget_process($element, $edit, &$form_state, $form) {
//  dpm('mediareference_widget_process()');
  $defaults =& $element['#value'];
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $thumb_field = NULL;
  $imagecache_preset = NULL;
  $preview = '';

  // DEBUG
//  if (is_array($edit) && !empty($edit)) {
//    static $counter = 1;
//    dpm('---- LAUNCH #' . $counter++);
//    dpm($element, 'element #' . $element['#delta']);
//    dpm($element['#default_value'], 'defaults #' . $element['#delta']);
//    dpm($element['#value'], 'values #' . $element['#delta']);
//    dpm($edit, 'edit #' . $element['#delta']);
//  }

  if ($field['widget']['insert']) {
    $element['#after_build'][] = 'insert_process';
    $element['#after_build'][] = 'mediareference_widget_insert_after_build';
  }

  if (empty($defaults['nid'])) {
    $element['info'] = array('#value' => t('None <em>(use Browse button to attach some media)</em>'));
  }
  else {

    if (empty($defaults['filepath']) || empty($defaults['fid']) || empty($defaults['title']) || empty($defaults['type'])) {
      $node = node_load(array('nid' => $defaults['nid']));
      $defaults['type'] = !empty($defaults['type']) ? $defaults['type'] : $node->type;
      $defaults['title'] = !empty($defaults['title']) ? $defaults['title'] : $node->title;
    }

    $type = $defaults['type'];

    if ($type && (empty($defaults['filepath']) || empty($defaults['fid']))) {
      if ($thumb_field = $field['widget']['thumbnail_type_fields'][$type]) {
        // change also $element['#value']['filepath'] because it is processed in Insert module
        $defaults['filepath'] = $element['#value']['filepath'] = $node->{$thumb_field}[0]['filepath'];
        $defaults['fid'] = $element['#value']['fid'] = $node->{$thumb_field}[0]['fid'];
      }
    }

    if (!empty($defaults['filepath'])) {
      if ($imagecache_preset = imagecache_preset($field['widget']['thumbnail_imagecache_preset'])) {
        $preview = theme('imagecache', $imagecache_preset['presetname'], $defaults['filepath']);
      }
    }

    if (empty($element['data'])) {
      $element['data'] = array();
    }
    else {
      // make sure that data is unserialized
      mediareference_field_unserialize($element);
    }

    $element['type'] = array(
      '#type' => 'hidden',
      '#value' => $defaults['type'],
    );

    $element['nid'] = array(
      '#type' => 'hidden',
      '#value' => $defaults['nid'],
    );

    $element['fid'] = array(
      '#type' => 'hidden',
      '#value' => $defaults['fid'],
    );

    $element['filepath'] = array(
      '#type' => 'hidden',
      '#value' => $defaults['filepath'],
    );

    if (isset($element['title'])) {
      $element['title'] += array(
        '#title' => t('Title'),
        '#type' => 'textfield',
        '#default_value' => $defaults['title'],
        '#weight' => 1,
      );
    }

    if (isset($element['link'])) {
      $element['link'] += array(
        '#title' => t('Link'),
        '#type' => 'textfield',
        '#default_value' => $defaults['link'],
        '#weight' => 2,
      );
    }

    if (isset($element['colour'])) {
      $element['colour'] += array(
        '#title' => t('Color'),
        '#type' => 'colorpicker_textfield',
        '#default_value' => $defaults['colour'],
        '#weight' => 3,
      );
    }

    if (isset($element['description'])) {
      $element['description'] += array(
        '#title' => t('Description'),
        '#type' => 'textarea',
        '#rows' => $field['widget']['rows'],
        '#cols' => $field['widget']['cols'],
        '#default_value' => $defaults['description'],
        '#weight' => 4,
      );

      if (! empty($field['description_text_processing'])) {
        $filter = isset($defaults['format']) ? $defaults['format'] : FILTER_FORMAT_DEFAULT;
        $parents = array_merge($element['#parents'], array('format'));
        $element['format'] = filter_form($filter, 5, $parents);
      }
    }

    if ($preview) {
      $element['preview'] = array(
        '#type' => 'item',
        '#title' => t('Preview'),
        '#value' => $preview,
        '#weight' => -1,
      );
    }

    if ($type) {
      $element['type_info'] = array(
        '#type' => 'item',
        '#title' => t('Type'),
        '#value' => $type,
        '#weight' => -2,
      );
    }
    // placeholder.. will be serialized into the data column. this is a place for widgets
    // to put additional data.
    $element['data'] = array_merge(array('#tree' => 'true', '#weight' => 18), $element['data']);

    $element['data']['type'] = array(
      '#type' => 'hidden',
      '#value' => $type,
    );

    if (isset($element['data']['slide_animation'])) {
      $element['data']['slide_animation'] += array(
        '#title' => t('Slide animation'),
        '#type' => 'select',
        '#options' => array(
          'slide' => 'slide',
          'fade' => 'fade',
         ),
        '#default_value' => $defaults['data']['slide_animation'],
      );
    }

    if (isset($element['data']['slide_delay'])) {
      $element['data']['slide_delay'] += array(
        '#title' => t('Slide delay'),
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 4,
        '#field_suffix' => 's',
        '#default_value' => !empty($defaults['data']['slide_delay']) ? $defaults['data']['slide_delay'] : $field['mediareference_formatters']['slideshow']['slide_delay_default'],
        '#description' => t('Delay between automatically moving to the next.'),
      );
    }
  }

  $element['actions'] = array(
    '#weight' => 20,
  );

  $field_name_css = str_replace('_', '-', $element['#field_name']);

  $element['actions']['remove'] = array(
    '#type' => 'button',
    '#value' => t('Remove'),
    '#ahah' => array(
      'path' => 'mediareference_js/'. $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'] . '/remove',
      'wrapper' => $field_name_css .'-items',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#weight' => 8,
  );

  $element['actions']['browse'] = array(
    '#type' => 'button',
    '#value' => t('Browse'),
    '#ahah' => array(
      'path' => 'mediareference_js/'. $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'] . '/update',
      'wrapper' => $field_name_css .'-items',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => 'mrf-browse-button'),
    '#weight' => 4,
  );

  // single element fix
  if (($field['multiple'] == 0) && ! mediareference_is_ahah_request()) {
    $element['#prefix'] = '<div id="'. $field_name_css .'-items">';
    $element['#suffix'] = '</div>';
  }

  return $element;
}

/**
 * Process function for insert-enabled fields.
 */
function mediareference_widget_insert_after_build($element) {
  if (isset($element['insert'])) {
    $insert = $element['insert'];
    // place it before Browse and Remove buttons
    $insert['#weight'] = 0;
    $element['actions']['insert'] = $insert;
    unset($element['insert']);
  }
  return $element;
}

//=====================================
//   MENU CALLBACKS
//=====================================

/**
 * AHAH menu callback
 *
 * @param string_type $type_name
 * @param string $field_name
 * @param int $delta_action
 * @param string $action
 */
function mediareference_widget_js($type_name, $field_name, $delta_action, $action) {

  // set the global property
  mediareference_is_ahah_request(TRUE);

  // load the field definition and check if arguments from url is right
  $field = content_fields($field_name, $type_name);
  $delta_action = intval($delta_action);
  if (empty($type_name) || empty($field_name) || empty($action) || empty($field)) {
    return drupal_not_found();
  }

  // check access
  if (!user_access("create $type_name content")) {
    return drupal_access_denied();
  }

  // Add the new elements to the stored form. Without adding the element
  // to the form, Drupal is not aware of this new elements existence and
  // will not process it. We retreive the cached form, add the element,
  // and resave.
  $form_build_id = $_POST['form_build_id'];
  $form_state = array('submitted' => FALSE);
  if (!($form = form_get_cache($form_build_id, $form_state))) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }

  // The form that we get from the cache is unbuilt. We need to build it so that
  // _value callbacks can be executed and $form_state['values'] populated.
  // We only want to affect $form_state['values'], not the $form itself
  // (built forms aren't supposed to enter the cache) nor the rest of $form_data,
  // so we use copies of $form and $form_data.
  $form_copy = $form;
  $form_state_copy = $form_state;
  $form_copy['#post'] = array();
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Just grab the data we need.
  $form_state['values'] = $form_state_copy['values'];
  unset($form_copy, $form_state_copy);
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  unset($form_state['values'][$field_name][$field['field_name'] .'_add_more']);

  if ($action == 'remove') {
    unset($form[$field_name][$delta_action]);
    unset($form_state['values'][$field_name][$delta_action]);
    unset($_POST[$field_name][$delta_action]);

    // reindex form ie. 1,2,4 => 1,2,3
    $items = mediareference_filter_children($form[$field_name]);
    $new_key = 0;
    $bak = array();
    foreach ($items as $old_key => $item) {
      // reindex'em all, do not use $old_key >= $delta_action
      if ($old_key != $new_key) {
        $bak = $form[$field_name][$old_key];
        unset($form[$field_name][$old_key]);
        $form[$field_name][$new_key] = $bak;

        $bak = $form_state['values'][$field_name][$old_key];
        unset($form_state['values'][$field_name][$old_key]);
        $form_state['values'][$field_name][$new_key] = $bak;

        $bak = $_POST[$field_name][$old_key];
        unset($_POST[$field_name][$old_key]);
        $_POST[$field_name][$new_key] = $bak;
      }
      $new_key++;
    }
    unset($bak);

    if (count($items) == 0) {
      module_load_include('inc', 'content', 'includes/content.node_form');
      // Build our new form element for the whole field, asking for one more element.
      $form_state['item_count'] = array($field_name => 0);
      // Create temporary form copy, because when we updating the node and there
      // is no items, content_field_form() will try to assign values from original node,
      // but we want empty form element.
      $form_copy = $form;
      if (isset($form_copy['#node']) && !empty($form_copy['#node']->{$field_name})) {
        unset($form_copy['#node']->{$field_name});
      }
      $form_element = content_field_form($form_copy, $form_state, $field);
      // we dont need $form_copy anymore
      unset($form_copy);
      $form[$field_name][0] = $form_element[$field_name][0];
    }
  }

  if ($action == 'update') {
    if (isset($_SESSION['lightframe']) && isset($_SESSION['lightframe']['nid']) && ($nid = intval($_SESSION['lightframe']['nid']))) {
      unset($_SESSION['lightframe']['nid']);
      $node = node_load(array('nid' => $nid));
      if ($node) {
        // TODO: rethink this
        $form_state['values'][$field_name][$delta_action]['nid'] = $nid;
        $form_state['values'][$field_name][$delta_action]['type'] = $node->type;
        $form_state['values'][$field_name][$delta_action]['title'] = $node->title;
        if ($thumb_field = $field['widget']['thumbnail_type_fields'][$node->type]) {
          $form_state['values'][$field_name][$delta_action]['fid'] = $node->{$thumb_field}[0]['fid'];
          $form_state['values'][$field_name][$delta_action]['filepath'] = $node->{$thumb_field}[0]['filepath'];
        }

        $form_state['values'][$field_name][$delta_action]['data']['type'] = $node->type;
        $form_state['values'][$field_name][$delta_action]['data']['slide_animation'] = 'slide';
        $form_state['values'][$field_name][$delta_action]['data']['slide_delay'] = $field['mediareference_formatters']['slideshow']['slide_delay_default'];

        $_POST[$field_name][$delta_action] = $form_state['values'][$field_name][$delta_action];

        module_load_include('inc', 'content', 'includes/content.node_form');
        $form_element = content_field_form($form, $form_state, $field);
        // Let other modules alter it.
        drupal_alter('form', $form_element, array(), 'mediareference_widget_js');
        $form[$field_name][$delta_action] = $form_element[$field_name][$delta_action];
      }
    }
  }

  //dpm($_POST[$field_name], 'POST before sort');

  // Sort the $form_state['values'] we just built *and* the incoming $_POST data
  // according to d-n-d reordering.
  foreach ($_POST[$field_name] as $delta => $item) {
    $form_state['values'][$field_name][$delta]['_weight'] = $item['_weight'];
  }

  // no we construct vector $old_key => $new_key
  $vector = $form_state['values'][$field_name];
  uasort($vector, '_content_sort_items_helper');
  $vector = array_flip(array_flip(array_keys($vector)));
  $form_field_copy = $form[$field_name];
  $max = count($vector) + 1;
  foreach ($vector as $old_delta => $delta) {
    if ($old_delta !== $delta) {
      $form[$field_name][$delta] = $form_field_copy[$old_delta];
    }

    $form[$field_name][$delta]['#delta'] = $delta;
    $form[$field_name][$delta]['#weight'] = $delta;
    if (isset($form[$field_name][$delta]['_weight'])) {
      $form[$field_name][$delta]['_weight']['#delta'] = $max;
      $form[$field_name][$delta]['_weight']['#default_value'] = $delta;
    }
  }
  unset($form_field_copy);
  //dpm($vector, 'vector');

  $form_state['values'][$field_name] = _content_sort_items($field, $form_state['values'][$field_name]);
  $_POST[$field_name] = _content_sort_items($field, $_POST[$field_name]);

  //dpm($form[$field_name], 'unbuild field_form');

  // Save rebuilded (but unprocessed!) form back to form cache.
  form_set_cache($form_build_id, $form, $form_state);

  // Rebuild the form, filter only needed elements and render the new output
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);
  $field_form = $form[$field_name];
  unset($field_form['#prefix'], $field_form['#suffix']);
  //dpm($form[$field_name], 'field_form');
  //dpm($form_state['values'][$field_name], 'form_values');
  //dpm($_POST[$field_name], 'POST');

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages');
  $output .= drupal_render($field_form) . $output_js;

  // Using drupal_json() breaks filefield's file upload, because the jQuery
  // Form plugin handles file uploads in a way that is not compatible with
  // 'text/javascript' response type.
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}