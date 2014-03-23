<?php

/**
 * @file keeps callbacks for /products/categories/% page
 */

/**
 * Page callback
 *
 * @param $group_id
 *
 * @return string
 */
function xdruple_queries_products_by_category_page($group_id) {
  $output = '';

  $catalog = entity_load('xtuple_xdruplecatalog');
  foreach ($catalog as $cat) {
    if ($cat->id == $group_id) {
      drupal_set_title($cat->name);
      break;
    }
  }

  $params = array(
    'fields' => array(
      'add_to_cart' => array(
        'show_quantity' => FALSE,
      ),
    ),
  );
  drupal_alter(array('product_page_item', 'products_by_category_page_item'), $params);

  $list           = array();
  $group_products = entity_load('xtuple_xdrupleitemgroupitem', NULL, array(
    'parentGroup' => $group_id,
  ));
  foreach ($group_products as $item) {
    $list[] = theme('products_page_item', array(
      'product' => entity_load_single('commerce_product', $item->Product),
      'params'  => $params,
    ));
  }

  $output .= theme('products_page', array(
    'products' => $list,
  ));

  return $output;
}