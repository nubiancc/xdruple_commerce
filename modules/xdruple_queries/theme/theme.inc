<?php

/**
 * @keeps theme callback
 */

/**
 * Preprocess variables for products-page.tpl.php
 */
function template_preprocess_products_page(&$variables) {
  $products = $variables['products'];

  $column_size = 3;
  $column = array();
  foreach ($products as $i => $product) {
    $column[] = $product;
    if (($i + 1) % $column_size === 0) {
      $variables['columns'][] = $column;
      $column = array();
    }
  }
  if (!empty($column)) {
    $variables['columns'][] = $column;
    unset($column);
  }
}

/**
 * Preprocess variables for products-page-item.tpl.php
 */
function template_preprocess_products_page_item(&$variables) {
  $product = $variables['product'];
  $params = $variables['params'];

  $variables['id']          = $product->product_id;
  $variables['price']       = _xdruple_queries_price($product);
  $variables['title']       = l($product->title, "products/{$product->product_id}");
  $variables['subtitle']    = $product->subTitle;
  $variables['img']         = _xdruple_queries_product_image($product, 'ft_commerce_product_thumb');

  if (!empty($params['fields']['add_to_cart'])) {
    $show_qty = FALSE;
    if (!empty($params['fields']['add_to_cart']['show_quantity'])) {
      $show_qty = $params['fields']['add_to_cart']['show_quantity'];
    }
    $variables['add_to_cart'] = _xdruple_queries_add_to_cart_button($product, $show_qty);
  }

  $variables['read_more']   = l('Read more', "products/$product->product_id", array(
    'attributes' => array(
      'class' => array('btn', 'btn-info'),
    ),
  ));
}

/**
 * Helper function to show correct Product price
 */
function _xdruple_queries_price($product) {
  $line_item = commerce_product_line_item_new($product, 1, 0, array(), NULL);
  xdruple_commerce_get_customer_line_item_price($line_item);
  $unit_price = $line_item->commerce_unit_price[LANGUAGE_NONE]['0'];

  return commerce_currency_format($unit_price['amount'], $unit_price['currency_code']);
}

/**
 * Helper function to build "Add to cart" button
 */
function _xdruple_queries_add_to_cart_button($product, $show_qty = FALSE, $context = array()) {
  $product_ids = array($product->product_id);
  $form_id     = commerce_cart_add_to_cart_form_id($product_ids);

  $line_item                                         = commerce_product_line_item_new($product, 1, 0, array());
  $line_item->data['context']['product_ids']         = $product_ids;
  $line_item->data['context']['add_to_cart_combine'] = TRUE;

  $form = drupal_get_form($form_id, $line_item, $show_qty, $context);
  return drupal_render($form);
}

/**
 * Helper function to build Image field
 */
function _xdruple_queries_product_image($product, $style = '') {
  $img = !empty($product->field_product_image[LANGUAGE_NONE][0]['uri']) ? l(theme('image_style', array(
    'style_name' => $style,
    'path'       => $product->field_product_image[LANGUAGE_NONE][0]['uri'],
  )), "products/{$product->product_id}", array(
    'html' => TRUE,
  )) : '';

  return $img;
}