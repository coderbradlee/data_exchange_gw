sql：
SELECT customer_credit_flow_id,balance,customer_master_id FROM apollo_os.t_customer_credit_flow where expire_date='2016-03-18' and balance>0 and dr=0 and transaction_type=0


update apollo_os.t_customer_credit_flow set balance=0 where customer_credit_flow_id='JAVVUWVBFV5QJIPNY79T'

update apollo_os.t_customer_master set credit_balance=0 where customer_master_id='JAVVUWUGS2OBYQ381HNP'


curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[  {  \"orderbot_account_id\": null,  \"order_date\": \"2016-04-11\",  \"ship_date\": \"2016-04-11\",  \"orderbot_customer_id\": 10,  \"reference_customer_id\": \"98000234\",  \"reference_order_id\": \"aabb15998965\",  \"customer_po\": \"12345678\",  \"order_status\": \"unshipped\",  \"order_notes\": \"test\",  \"internal_notes\": \"test internal\",  \"bill_third_party\": false,  \"distribution_center_id\": 1,  \"account_group_id\": 0,  \"order_guide_id\": 0,  \"insure_packages\": false,  \"shipping_code\": \"A1\",  \"email_confirmation_address\": \"test@orderbot.com\",  \"subtotal\": 10.89,  \"shipping\": 3,  \"order_discount\": 0,  \"order_total\": 19.73,  \"shipping_tax\": [  {  \"tax_name\": \"TAX\",  \"tax_rate\": 0.05,  \"amount\": 0.15  }  ],  \"shipping_address\": {  \"store_name\": \"Test Store\",  \"first_name\": \"f\",  \"last_name\": \"x\",  \"address1\": \"350 5th Avenue\",  \"address2\": \"\",  \"city\": \"New York\",  \"state\": \"NY\",  \"postal_code\": \"10118\",  \"country\": \"US\",  \"phone_number\": \"+1 800-428-4322\",  \"email\": \"\"  },  \"billing_address\": {  \"account_name\": \"Test Store\",  \"first_name\": \"x\",  \"last_name\": \"x\",  \"address1\": \"350 5th Avenue\",  \"address2\": \"\",  \"city\": \"New York\",  \"state\": \"NY\",  \"postal_code\": \"10118\",
\"country\": \"US\",  \"phone_number\": \"+1 800-428-4322\",  \"email\": null  },  \"payments\": [  {  \"payment_type\": \"visa\",  \"payment_date\": \"2015-08-26\",  \"amount_paid\": 19.73,  \"check_no\": null,  \"notes\": null,  \"credit_card_info\": {  \"transaction_id\": \"2239012792\",  \"authorization_code\": \"YT0VX3\",  \"last_four_digits\": \"0002\",  \"gateway_customer_profile_id\": \"36724516\",  \"gateway_customer_payment_profile_id\": \"33204462\",  \"pay_by_cim\": true  }  }  ],  \"order_lines\": [  {  \"line_number\": 1,  \"product_sku\": \"123\",  \"custom_description\": \"Test Product\",  \"quantity\": 1,  \"price\": 10.89,  \"product_discount\": 0,  \"product_taxes\": [  {  \"tax_name\": \"TAX\",  \"tax_rate\": 0.05,  \"amount\": 0.54  }  ]  }  ],  \"other_charges\": [  {  \"other_charge_id\": 60,  \"amount\": 5,  \"other_charge_taxes\": [  {  \"tax_name\": \"TAX\",  \"tax_rate\": 0.05,  \"amount\": 0.15  }  ]  }  ]  }  ]' 'http://api.devbot.ca/admin/orders.json/'













1、put_orders_num_func：

curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{  \"ship_date\": \"2015-09-26\",  \"order_status\": \"to_be_shipped\",  \"order_notes\": \"Test notes77777\",  \"internal_notes\": \"Test notes\",  \"shipping_total\":10.00,\"payments\": [  {  \"payments\":\"payments\",  \"amount_paid\":10.00,  \"pre_auth_id\":81  }  ] ,\"tracking_numbers\": [  {  \"tracking_number\": \"3333333\",  \"cost\": 10,  \"weight\": 5  }  ]  }' 'http://api.devbot.ca/admin/orders.json/1'

2、post_orders_param_func:

curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{\"orderbot_account_id\": null, \"order_date\" : \"2015-08-26\", \"ship_date\" : \"2015-08-26\", \"orderbot_customer_id\" : 10, \"reference_customer_id\" : \"98000234\", \"reference_order_id\" : \"aabb15998966\", \"customer_po\" : \"12345678\", \"order_status\" : \"unshipped\", \"order_notes\" : \"test\", \"internal_notes\" : \"test internal\", \"bill_third_party\" : false, \"distribution_center_id\" : 1, \"account_group_id\" : 0, \"order_guide_id\" : 0, \"insure_packages\" : false, \"shipping_code\" : \"A1\", \"email_confirmation_address\" : \"test@orderbot.com\", \"subtotal\" : 10.89, \"shipping\" : 3, \"order_discount\" : 0, \"order_total\" : 19.73, \"shipping_tax\" : [{\"tax_name\": \"TAX\", \"tax_rate\" : 0.05, \"amount\" : 0.15}], \"shipping_address\" : {\"store_name\": \"Test Store\", \"first_name\" : \"f\", \"last_name\" : \"x\", \"address1\" : \"350 5th Avenue\", \"address2\" : \"\", \"city\" : \"New York\", \"state\" : \"NY\", \"postal_code\" : \"10118\", \"country\" : \"US\", \"phone_number\" : \"+1 800-428-4322\", \"email\" : \"\"}, \"billing_address\" : {\"account_name\": \"Test Store\", \"first_name\" : \"x\", \"last_name\" : \"x\", \"address1\" : \"350 5th Avenue\", \"address2\" : \"\", \"city\" : \"New York\", \"state\" : \"NY\", \"postal_code\" : \"10118\", \"country\" : \"US\", \"phone_number\" : \"+1 800-428-4322\", \"email\" : null}, \"payments\" : [{\"payment_type\": \"visa\", \"payment_date\" : \"2015-08-26\", \"amount_paid\" : 19.73, \"check_no\" : null, \"notes\" : null, \"credit_card_info\" : {\"transaction_id\": \"2239012792\", \"authorization_code\" : \"YT0VX3\", \"last_four_digits\" : \"0002\", \"gateway_customer_profile_id\" : \"36724516\", \"gateway_customer_payment_profile_id\" : \"33204462\", \"pay_by_cim\" : true}}], \"order_lines\" : [{\"line_number\": 1, \"product_sku\" : \"123\", \"custom_description\" : \"Test Product\", \"quantity\" : 1, \"price\" : 10.89, \"product_discount\" : 0, \"product_taxes\" : [{\"tax_name\": \"TAX\", \"tax_rate\" : 0.05, \"amount\" : 0.54}]}], \"other_charges\" : [{\"other_charge_id\": 60, \"amount\" : 5, \"other_charge_taxes\" : [{\"tax_name\": \"TAX\", \"tax_rate\" : 0.05, \"amount\" : 0.15}]}]}]' 'http://172.18.100.85:8688/admin/orders.json/'

3、put_products_num_func

curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"group_id\": 1, \"name\" : \"TestAPIUpdate\", \"description\" : \"TestAPIUpdate\", \"sku\" : \"Test Sku Update\", \"base_price\" : 0, \"units_of_measure_type\" : 1, \"weight\" : 1, \"shipping_unit_of_measure\" : 1, \"taxable\" : true, \"active\" : true, \"is_parent\" : false, \"cost\" : 10, \"vendorId\" : null, \"upc\" : \"11111\", \"variable_value1_id\" : null, \"variable_value2_id\" : null, \"leadtimes\" : 10, \"product_visibilities\" : [{  \"websites_id\": 35, \"visibility_type_id\" : 2, \"available_on_date\" : \"2015-10-11T16:49:41.6168812-07:00\", \"on_back_order\" : false  }], \"optional_description_fields\" : {  \"description1\": \"test1\", \"description2\" : null, \"description3\" : null, \"description4\" : null, \"description5\" : null, \"description6\" : \"test6\"  } }' 'http://172.18.100.85:8688/admin/products.json/429229'

4、post_products_param_func

curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"reference_product_id\": \"100011\", \"reference_parent_product_id\" : null, \"group_id\" : 1, \"component_group_id\" : 2631, \"name\" : \"TestAPIProduct\", \"description\" : \"TestAPIProduct\", \"sku\" : \"Test Sku\", \"base_price\" : 0, \"units_of_measure\" : 1, \"units_of_measure_type_id\" : 1, \"weight\" : 1, \"shipping_units_of_measure_type_id\" : 1, \"taxable\" : true, \"min_quantity\" : 0, \"sequence\" : 1, \"active\" : true, \"is_parent\" : false, \"quantity\" : 1, \"cost\" : 10, \"vendor_id\" : 1840, \"upc\" : \"11111\", \"variable_Value1_id\" : 0, \"variable_Value2_id\" : 0, \"bom_quantity\" : 1, \"purchase_unit_of_measure_type_id\" : 61, \"purchase_category_id\" : 1, \"purchase_unit_quantity\" : 1, \"create_bom\" : true, \"create_purchase_unit\" : true, \"lead_times\" : null  }, { \"reference_product_id\": \"100111\", \"reference_parent_product_id\" : null, \"group_id\" : 1, \"component_group_id\" : 2631, \"name\" : \"TestAPIProduct1\", \"description\" : \"TestAPIProduct1\", \"sku\" : \"Test Sku1\", \"base_price\" : 0, \"units_of_measure\" : 1, \"units_of_measure_type_id\" : 1, \"weight\" : 1, \"shipping_units_of_measure_type_id\" : 1, \"taxable\" : true, \"min_quantity\" : 0, \"sequence\" : 1, \"active\" : true, \"is_parent\" : false, \"quantity\" : 1, \"cost\" : 10, \"vendor_id\" : 1840, \"upc\" : \"2222222\", \"variable_Value1_id\" : 0, \"variable_Value2_id\" : 0, \"bom_quantity\" : 1,
\"purchase_unit_of_measure_type_id\" : 61, \"purchase_category_id\" : 1, \"purchase_unit_quantity\" : 1, \"create_bom\" : true, \"create_purchase_unit\" : true, \"lead_times\" : null }]' 'http://api.devbot.ca/admin/products.json/'

api.devbot.ca

5、put_customers_num_func

curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"account_id\": 3, \"first_name\" : \"Peter\", \"last_name\" : \"Lin\", \"address\" : \"122 sdgdfg\", \"address2\" : \"12342\", \"city\" : \"sfsdf\", \"state_id\" : 1, \"state_name\" : \"\", \"country\" : \"US\", \"postal_code\" : \"98001\", \"phone\" : \"11111111\", \"sales_rep_id\" : 461, \"email\" : \"Test@orderbot.com\", \"active\" : true, \"commission_rate\" : 10 }' 'http://172.18.100.85:8688/admin/customers.json/2'

6、post_customers_param_func

curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"reference_account_id\": \"1000111\", \"account_group_id\" : 18, \"order_guide_id\" : 364, \"account_name\" : \"Test\", \"account\" : {  \"first_name\": \"Peter\", \"last_name\" : \"Lin\", \"address\" : \"122 sdgdfg\", \"address2\" : \"12342\", \"city\" : \"sfsdf\", \"state_id\" : 1, \"state_name\" : \"\", \"country_id\" : 226, \"postal_code\" : \"98001\", \"email\" : \"Test@orderbot.com\", \"phone\" : \"11111111\", \"fax\" : \"222222\"  }, \"customers\" : [{  \"reference_customer_id\": \"10001\", \"store_name\" : \"Test store\", \"customer\" : {  \"first_name\": \"Peter\", \"last_name\" : \"Lin\", \"address\" : \"122 sdgdfg\", \"address2\" : \"12342\", \"city\" : \"sfsdf\", \"state_id\" : 1, \"state_name\" : \"\", \"country_id\" : 226, \"postal_code\" : \"98001\", \"email\" : \"Test@orderbot.com\", \"phone\" : \"11111111\", \"fax\" : \"222222\"  }  }]  }, { \"reference_account_id\": \"1000113\", \"account_group_id\" : 18, \"order_guide_id\" : 364, \"account_name\" : \"Test\", \"account\" : {  \"first_name\": \"Peter\", \"last_name\" : \"Lin\", \"address\" : \"122 sdgdfg\", \"address2\" : \"12342\", \"city\" : \"sfsdf\", \"state_id\" : 1, \"state_name\" : \"\", \"country_id\" : 226, \"postal_code\" : \"98001\", \"email\" : \"Test@orderbot.com\", \"phone\" : \"11111111\", \"fax\" : \"222222\"  }, \"customers\" : [{  \"reference_customer_id\": \"10001\", \"store_name\" : \"Test store\", \"customer\" : {  \"first_name\": \"Peter\", \"last_name\" : \"Lin\", \"address\" : \"122 sdgdfg\", \"address2\" : \"12342\", \"city\" : \"sfsdf\", \"state_id\" : 1, \"state_name\" : \"\", \"country_id\" : 226, \"postal_code\" : \"98001\", \"email\" : \"Test@orderbot.com\", \"phone\" : \"11111111\", \"fax\" : \"222222\"  }  }] }]' 'http://172.18.100.85:8688/admin/customers.json/'

7、put_salesreps_num_func

curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"sales_person_name\": \"Peter\", \"abbreviation\" : \"Peter\", \"default_commission_rate\" : 10, \"sales_person_group_id\" : 1, \"email_invoices\" : \"Peter@orderbot.com\" }' 'http://172.18.100.85:8688/admin/salesreps.json/461'

8、post_salesreps_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[  {  "sales_person_name": "Peter",  "abbreviation": "Peter",  "default_commission_rate": 10,  "sales_person_group_id": 1,  "email_invoices": "Peter@orderbot.com"  }  ]' 'http://172.18.100.85:8688/admin/salesreps.json/'


9、put_product_categories_num_func
curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"category_name\": \"test\", \"category_abbreviation\" : \"Test\", \"category_active\" : true, \"category_sequence\" : 1, \"product_class_id\" : 122 }' 'http://172.18.100.85:8688/admin/product_categories.json/461'

10、post_product_categories_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"ref_category_id\": 10098, \"category_name\" : \"test\", \"category_sequence\" : 1, \"product_class_id\" : 99, \"category_active\" : true, \"category_abbreviation\" : \"test\"  }]' 'http://172.18.100.85:8688/admin/product_categories.json/'


11、 put_product_groups_num
curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"group_name\": \"Test\", \"category_id\" : 256, \"group_abbreviation\" : \"Test\", \"group_active\" : false, \"group_sequence\" : 2 }' 'http://172.18.100.85:8688/admin/product_groups.json/461'


12、post_product_groups_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"ref_group_id\": 9008, \"group_name\" : \"Test\", \"category_id\" : 166, \"group_abbreviation\" : \"Test\", \"group_active\" : true, \"group_sequence\" : 1, }]' 'http://172.18.100.85:8688/admin/product_groups.json/'

13、put_variable_groups_num_func
curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"variable_group_name\": \"Test\", \"variable_group_active \" : true, \"variable_group_sequence\" : 1 }' 'http://172.18.100.85:8688/admin/variable_groups.json/461'

14、post_product_variable_groups_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"variable_group_name\": \"Test\", \"variable_group_active\" : \"test\", \"variable_group_sequence\" : 1  }]' 'http://172.18.100.85:8688/admin/product_variable_groups.json/'


15、put_product_variables_num_func
curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"variable_group_id\": 211, \"variable_name\" : \"Test\", \"variable_active\" : true, \"sequence\" : 1 }' 'http://172.18.100.85:8688/admin/product_variables.json/461'

16、post_product_variables_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"variable_group_id\": 565, \"variable_name\" : \"Test\", \"variable_active\" : true, \"sequence\" : 1  }]' 'http://172.18.100.85:8688/admin/product_variables.json/'


17、put_product_variable_values_num_func
curl -u testapi@orderbot.com:ShinyElephant232# -X PUT --data '{ \"variable_id\": 235, \"variable_value_name\" : \"Test\", \"variable_value_active\" : true, \"sequence\" : 1 }' 'http://172.18.100.85:8688/admin/product_variable_values.json/461'

18、post_product_variable_values_param_func
curl -u testapi@orderbot.com:ShinyElephant232# -X POST --data '[{  \"variable_id\": 25, \"variable_value_name\" : \"Test\", \"default_commission_rate\" : 10, \"variable_value_active\" : true, \"sequence\" : 1  }]' 'http://172.18.100.85:8688/admin/product_variable_values.json/'







