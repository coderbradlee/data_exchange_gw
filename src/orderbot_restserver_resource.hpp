#ifndef ORDERBOT_RESTSERVER_RESOURCE_HPP
#define	ORDERBOT_RESTSERVER_RESOURCE_HPP

#include "orderbot_restserver.hpp"
void orderbot_server_start()
{
	/////////////////////////////////////////////////////////
		auto default_index = std::make_shared< Resource >();
		//default_index->set_path({ "/{filename: .*}", "/" });
		default_index->set_path("/.*");
		default_index->set_method_handler("GET", default_handler);
		auto default_index2 = std::make_shared< Resource >();
		//default_index->set_path({ "/{filename: .*}", "/" });
		default_index2->set_path("/");
		default_index2->set_method_handler("GET", default_handler);
		////////////////////////////////////////////////////////
		//curl -u testapi@orderbot.com:ShinyElephant232# -X GET http://172.18.100.85:8688/admin/orders.json/233
		auto get_orders_num = std::make_shared< Resource >();
		get_orders_num->set_path("/admin/orders.json/{name: ([0-9]+)}");
		get_orders_num->set_method_handler("GET", get_orders_num_func);
		get_orders_num->set_method_handler("PUT", put_orders_num_func);
		get_orders_num->set_error_handler(&resource_error_handler);
		//------------------------------------------
		/*auto put_orders_num = std::make_shared< Resource >();
		put_orders_num->set_path("/admin/orders.json/{name: ([0-9]+)}");
		put_orders_num->set_method_handler("PUT", put_orders_num_func);
		put_orders_num->set_error_handler(&resource_error_handler);*/
		
		/////////////////////////////////////
		auto get_orders_param = std::make_shared< Resource >();
		get_orders_param->set_path("/admin/orders.json/");
		//curl -u testapi@orderbot.com:ShinyElephant232# -X GET http://172.18.100.85:8688/admin/orders.json/?created_at_min=2015-01-01\&limit=200\&page=1\&order_status=unconfirmed,unshipped,to_be_shipped\&Sales_channels=dtc,wholesale
		//http://172.18.100.85:8688/admin/orders.json/?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		get_orders_param->set_method_handler("GET", get_orders_param_func);
		get_orders_param->set_method_handler("POST", post_orders_param_func);
		get_orders_param->set_error_handler(&resource_error_handler);

		////////////////////////////////////////////////////////////

		//http://172.18.100.85:8688/admin/products.json/1223
		auto get_products_num = std::make_shared< Resource >();
		get_products_num->set_path("/admin/products.json/{name: ([0-9]+)}");
		get_products_num->set_method_handler("GET", get_products_num_func);
		get_products_num->set_method_handler("PUT", put_products_num_func);
		get_products_num->set_error_handler(&resource_error_handler);

////////////////////////////////////////////////////////////

		//http://172.18.100.85:8688/admin/products.json/?class_type=sales&category_name=Rings
		auto get_products_param = std::make_shared< Resource >();
		get_products_param->set_path("/admin/products.json/");
		get_products_param->set_method_handler("GET", get_products_param_func);
		get_products_param->set_method_handler("POST", post_products_param_func);
		get_products_param->set_error_handler(&resource_error_handler);
		////////////////////////////////////////////////////////////

		//http://172.18.100.85:8688/admin/customers.json/1223
		auto get_customers_num = std::make_shared< Resource >();
		get_customers_num->set_path("/admin/customers.json/{name: ([0-9]+)}");
		get_customers_num->set_method_handler("GET", get_customers_num_func);
		get_customers_num->set_method_handler("PUT", put_customers_num_func);
		get_customers_num->set_error_handler(&resource_error_handler);
		/////////////////////////////////////
		//http://172.18.100.85:8688/admin/Customers.json/?active=true&sales_channel_name=wholesale
		auto get_customers_param = std::make_shared< Resource >();
		get_customers_param->set_path("/admin/customers.json/");
		get_customers_param->set_method_handler("GET", get_customers_param_func);
		get_customers_param->set_method_handler("POST", post_customers_param_func);
		get_customers_param->set_error_handler(&resource_error_handler);
////////////////////////////////////////////////////////////

		//http://172.18.100.85:8688/admin/salesreps.json/461
		auto get_salesreps_num = std::make_shared< Resource >();
		get_salesreps_num->set_path("/admin/salesreps.json/{name: ([0-9]+)}");
		get_salesreps_num->set_method_handler("GET", get_salesreps_num_func);
		get_salesreps_num->set_method_handler("PUT", put_salesreps_num_func);
		get_salesreps_num->set_error_handler(&resource_error_handler);
		/////////////////////////////////////
		//http://172.18.100.85:8688/admin/salesreps.json/
		auto get_salesreps_param = std::make_shared< Resource >();
		get_salesreps_param->set_path("/admin/salesreps.json/");
		get_salesreps_param->set_method_handler("GET", get_salesreps_param_func);
		get_salesreps_param->set_method_handler("POST", post_salesreps_param_func);
		get_salesreps_param->set_error_handler(&resource_error_handler);

		///////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_categories.json/461
		auto put_product_categories_num = std::make_shared< Resource >();
		put_product_categories_num->set_path("/admin/product_categories.json/{name: ([0-9]+)}");		
		put_product_categories_num->set_method_handler("PUT", put_product_categories_num_func);
		put_product_categories_num->set_error_handler(&resource_error_handler);
		////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_categories.json/
		auto post_product_categories_param = std::make_shared< Resource >();
		post_product_categories_param->set_path("/admin/product_categories.json/");
		post_product_categories_param->set_method_handler("POST", post_product_categories_param_func);
		post_product_categories_param->set_error_handler(&resource_error_handler);
///////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_groups.json/461
		auto put_product_groups_num = std::make_shared< Resource >();
		put_product_groups_num->set_path("/admin/product_groups.json/{name: ([0-9]+)}");
		put_product_groups_num->set_method_handler("PUT", put_product_groups_num_func);
		put_product_groups_num->set_error_handler(&resource_error_handler);

		////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_groups.json/
		auto post_product_groups_param = std::make_shared< Resource >();
		post_product_groups_param->set_path("/admin/product_groups.json/");
		post_product_groups_param->set_method_handler("POST", post_product_groups_param_func);
		post_product_groups_param->set_error_handler(&resource_error_handler);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
//http://172.18.100.85:8688/admin/variable_groups.json/461
		auto put_variable_groups_num = std::make_shared< Resource >();
		put_variable_groups_num->set_path("/admin/variable_groups.json/{name: ([0-9]+)}");
		put_variable_groups_num->set_method_handler("PUT", put_variable_groups_num_func);
		put_variable_groups_num->set_error_handler(&resource_error_handler);

////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_variable_groups.json/
		auto post_product_variable_groups_param = std::make_shared< Resource >();
		post_product_variable_groups_param->set_path("/admin/product_variable_groups.json/");
		post_product_variable_groups_param->set_method_handler("POST", post_product_variable_groups_param_func);
		post_product_variable_groups_param->set_error_handler(&resource_error_handler);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_variables.json/461
		auto put_product_variables_num = std::make_shared< Resource >();
		put_product_variables_num->set_path("/admin/product_variables.json/{name: ([0-9]+)}");
		put_product_variables_num->set_method_handler("PUT", put_product_variables_num_func);
		put_product_variables_num->set_error_handler(&resource_error_handler);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_variable_values.json/461
		auto put_product_variable_values_num = std::make_shared< Resource >();
		put_product_variable_values_num->set_path("/admin/product_variable_values.json/{name: ([0-9]+)}");
		put_product_variable_values_num->set_method_handler("PUT", put_product_variable_values_num_func);
		put_product_variable_values_num->set_error_handler(&resource_error_handler);
		
		////////////////////////////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_variable_values.json/
		auto post_product_variable_values_param = std::make_shared< Resource >();
		post_product_variable_values_param->set_path("/admin/product_variable_values.json/");
		post_product_variable_values_param->set_method_handler("POST", post_product_variable_values_param_func);
		post_product_variable_values_param->set_error_handler(&resource_error_handler);


/////////////////////////////////////
		//http://172.18.100.85:8688/admin/account_groups.json/
		auto get_account_groups_param = std::make_shared< Resource >();
		get_account_groups_param->set_path("/admin/account_groups.json/");
		get_account_groups_param->set_method_handler("GET", get_account_groups_param_func);
		get_account_groups_param->set_error_handler(&resource_error_handler);
/////////////////////////////////////
		//http://172.18.100.85:8688/admin/units_of_measurement_types.json/
		auto get_units_of_measurement_types_param = std::make_shared< Resource >();
		get_units_of_measurement_types_param->set_path("/admin/units_of_measurement_types.json/");
		get_units_of_measurement_types_param->set_method_handler("GET", get_units_of_measurement_types_param_func);
		get_units_of_measurement_types_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/order_guides.json/
		auto get_order_guides_param = std::make_shared< Resource >();
		get_order_guides_param->set_path("/admin/order_guides.json/");
		get_order_guides_param->set_method_handler("GET", get_order_guides_param_func);
		get_order_guides_param->set_error_handler(&resource_error_handler);
/////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_structure.json/
		auto get_product_structure_param = std::make_shared< Resource >();
		get_product_structure_param->set_path("/admin/product_structure.json/");
		get_product_structure_param->set_method_handler("GET", get_product_structure_param_func);
		get_product_structure_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/product_variables.json/
		auto get_product_variables_param = std::make_shared< Resource >();
		get_product_variables_param->set_path("/admin/product_variables.json/");
		get_product_variables_param->set_method_handler("GET", get_product_variables_param_func);
		get_product_variables_param->set_method_handler("POST", post_product_variables_param_func);
		get_product_variables_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/purchase_unit_categories.json/
		auto get_purchase_unit_categories_param = std::make_shared< Resource >();
		get_purchase_unit_categories_param->set_path("/admin/purchase_unit_categories.json/");
		get_purchase_unit_categories_param->set_method_handler("GET", get_purchase_unit_categories_param_func);
		get_purchase_unit_categories_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/vendor_purchase_unit_of_measurements.json/
		auto get_vendor_purchase_unit_of_measurements_param = std::make_shared< Resource >();
		get_vendor_purchase_unit_of_measurements_param->set_path("/admin/vendor_purchase_unit_of_measurements.json/");
		get_vendor_purchase_unit_of_measurements_param->set_method_handler("GET", get_vendor_purchase_unit_of_measurements_param_func);
		get_vendor_purchase_unit_of_measurements_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/salesrep_groups.json/
		auto get_salesrep_groups_param = std::make_shared< Resource >();
		get_salesrep_groups_param->set_path("/admin/salesrep_groups.json/");
		get_salesrep_groups_param->set_method_handler("GET", get_salesrep_groups_param_func);
		get_salesrep_groups_param->set_error_handler(&resource_error_handler);
/////////////////////////////////////
		//http://172.18.100.85:8688/admin/Vendors.json/
		auto get_Vendors_param = std::make_shared< Resource >();
		get_Vendors_param->set_path("/admin/Vendors.json/");
		get_Vendors_param->set_method_handler("GET", get_Vendors_param_func);
		get_Vendors_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/Countries.json/
		auto get_Countries_param = std::make_shared< Resource >();
		get_Countries_param->set_path("/admin/Countries.json/");
		get_Countries_param->set_method_handler("GET", get_Countries_param_func);
		get_Countries_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/States.json/
		auto get_States_param = std::make_shared< Resource >();
		get_States_param->set_path("/admin/States.json/");
		get_States_param->set_method_handler("GET", get_States_param_func);
		get_States_param->set_error_handler(&resource_error_handler);
/////////////////////////////////////
		//http://172.18.100.85:8688/admin/distribution_centers.json/
		auto get_distribution_centers_param = std::make_shared< Resource >();
		get_distribution_centers_param->set_path("/admin/distribution_centers.json/");
		get_distribution_centers_param->set_method_handler("GET", get_distribution_centers_param_func);
		get_distribution_centers_param->set_error_handler(&resource_error_handler);

/////////////////////////////////////
		//http://172.18.100.85:8688/admin/websites.json/
		auto get_websites_param = std::make_shared< Resource >();
		get_websites_param->set_path("/admin/websites.json/");
		get_websites_param->set_method_handler("GET", get_websites_param_func);
		get_websites_param->set_error_handler(&resource_error_handler);



		////////////////////////////////////////////////////////
		auto settings = std::make_shared< Settings >();
		settings->set_port(get_config->m_port);
		settings->set_worker_limit(get_config->m_threads);
		settings->set_default_header("Connection", "close");
		cout << "server started with port:" << get_config->m_port << ",thread num:" << get_config->m_threads << endl;
		Service service;
		service.publish(default_index);
		service.publish(default_index2);
		service.publish(get_orders_num);
		//service.publish(put_orders_num);
		service.publish(get_orders_param);
		

		service.publish(get_products_num);
		//service.publish(put_products_num);
		service.publish(get_products_param);


		service.publish(get_customers_num);
		service.publish(get_customers_param);
		service.publish(get_salesreps_num);
		service.publish(get_salesreps_param);

		service.publish(put_product_categories_num);
		service.publish(post_product_categories_param);
		service.publish(put_product_groups_num);
		service.publish(post_product_groups_param);
		service.publish(put_variable_groups_num);
		service.publish(post_product_variable_groups_param);
		service.publish(put_product_variables_num);
		service.publish(put_product_variable_values_num);
		service.publish(post_product_variable_values_param);

		service.publish(get_account_groups_param);
		service.publish(get_units_of_measurement_types_param);
		service.publish(get_order_guides_param);
		service.publish(get_product_structure_param);
		service.publish(get_product_variables_param);
		service.publish(get_purchase_unit_categories_param);
		service.publish(get_vendor_purchase_unit_of_measurements_param);

		service.publish(get_salesrep_groups_param);
		service.publish(get_Vendors_param);
		service.publish(get_Countries_param);
		service.publish(get_States_param);
		service.publish(get_distribution_centers_param);
		service.publish(get_websites_param);



		service.set_authentication_handler(authentication_handler);
		
		service.set_error_handler(service_error_handler);
		service.set_logger(std::make_shared< CustomLogger >());
		service.start(settings);

}

#endif	