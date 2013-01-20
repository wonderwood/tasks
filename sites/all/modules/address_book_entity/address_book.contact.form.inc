<?php

/**
 * Implements contact add/edit form
 */
function address_book_contactform($form, $form_state, $contact = NULL) {
  drupal_set_title(t('Add contact'));
  $breadcrumbs   = drupal_get_breadcrumb();
  $breadcrumbs[] = drupal_get_title();
  drupal_set_breadcrumb($breadcrumbs);

  if (isset($contact->cid)) {
    drupal_set_title(t("Edit contact #@contact_id", array('@contact_id' => $contact->cid)));
    $breadcrumbs   = array();
    $breadcrumbs[] = l(t('Home'), url());
    $breadcrumbs[] = l(t('Address book'), 'address_book');
    $breadcrumbs[] = l($contact->first_name . ' ' . $contact->last_name, "address_book/$contact->cid");
    $breadcrumbs[] = t('Edit');
    drupal_set_breadcrumb($breadcrumbs);
  }

  $form['form_intro_text'] = array(
    '#markup' => t('Contact add form intro text, what explains something to user.'),
    '#weight' => 0,
  );

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Add contact'),
    '#weight' => 90,
  );


  /**
   * check for $cid is present
   */
  if (!is_null($contact)) {
    /**
     * Query to the DB and check if contact in DB
     * Set default values for form
     * Add hidden field with $cid
     * Add delete link
     * Rewrite submit button text
     */
//    if (isset($contact->cid)) {
    // Add custom fields with their values into form
    field_attach_form('contact', $contact, $form, $form_state);

    $form['cid'] = array(
      '#type'   => 'hidden',
      '#value'  => $contact->cid,
      '#weight' => 150,
    );

    $form['delete'] = array(
      '#markup' => l(t('Delete contact'), "admin/address_book/$contact->cid/delete"),
      '#weight' => 100,
    );

//      $form['first_name']['#default_value'] = $contact->first_name;
//      $form['last_name']['#default_value']  = $contact->last_name;
//      $form['email']['#default_value']      = $contact->email;
//      $form['phone']['#default_value']      = $contact->phone;
//      $form['photo']['#default_value']      = $contact->photo;
//      $form['comment']['#default_value']    = $contact->comment;
//      $form['birthday']['#default_value']   = $contact->birthday;
//      $form['category']['#default_value']   = $contact->category;

    $form['submit']['#value'] = t('Edit contact');
//    }
  }
  else {
    field_attach_form('contact', '', $form, $form_state);
  }

  return $form;
}

function address_book_contactform_validate(&$form, &$form_state) {
}

/**
 *  Implements contact add form submit
 */
function address_book_contactform_submit($form, &$form_state) {
  dpm($form);
  dpm($form_state);

//  $entity = (object) $form_state['values'];
  $entity = new stdClass();
  if(isset($form_state['values']['cid'])){
    $entity->cid = $form_state['values']['cid'];
  }
  $entity->bundle = 'contact';
  field_attach_submit('contact', $entity, $form, $form_state);
  entity_form_submit_build_entity('contact', $entity, $form, $form_state);
  contact_save($entity);
  /**
   * if $cid isset - update entity
   * if $cid not set - save new entity
   */
  if (isset($form_values['values']['cid'])) {
    drupal_set_message(
      t('Contact %cid updated.'),
      array(
        '%cid' => $form_values['values']['cid'],
      ));
    drupal_goto('admin/structure/address_book');
    drupal_set_message(t('Contact edited'));
  }
  else {
    drupal_set_message(t('Contact #%cid successfully added',
      array(
        '%cid' => $entity->cid,
      )));
  }

}