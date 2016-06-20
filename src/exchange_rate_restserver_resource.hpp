#ifndef EXCHANGE_RATE_RESTSERVER_RESOURCE_HPP
#define	EXCHANGE_RATE_RESTSERVER_RESOURCE_HPP

#include "exchange_rate_restserver.hpp"
namespace exchange_rate_namespace
{
void exchange_rate_server_start()
{
	/////////////////////////////////////////////////////////
		auto default_index = std::make_shared< Resource >();
		//default_index->set_path({ "/{filename: .*}", "/" });
		default_index->set_path("^/((?!/).)*$");
		default_index->set_method_handler("GET", default_handler);
		auto default_index2 = std::make_shared< Resource >();
		//default_index->set_path({ "/{filename: .*}", "/" });
		default_index2->set_path("/");
		default_index2->set_method_handler("GET", default_handler);
		////////////////////////////////////////////////////////
		//curl -u testapi@orderbot.com:ShinyElephant232# -X GET http://172.18.100.87:8688/exchange_rate/target/SGD
		// auto get_exchange_rate = std::make_shared< Resource >();
		// get_exchange_rate->set_path("/exchange_rate/target/{name: ([A-Z]+)}");
		// get_exchange_rate->set_method_handler("GET", get_exchange_rate_func);
		
		// get_exchange_rate->set_error_handler(&resource_error_handler);
		//------------------------------------------
		/*auto put_orders_num = std::make_shared< Resource >();
		put_orders_num->set_path("/admin/orders.json/{name: ([0-9]+)}");
		put_orders_num->set_method_handler("PUT", put_orders_num_func);
		put_orders_num->set_error_handler(&resource_error_handler);*/
		
		/////////////////////////////////////
		auto get_exchange_rate = std::make_shared< Resource >();
		//get_exchange_rate->set_path("/exchange_rate/{name: [\\?].*}");
		get_exchange_rate->set_path("/exchange_rate/");
		//curl -u testapi@orderbot.com:ShinyElephant232# -X GET http://172.18.100.85:8688/admin/orders.json/?created_at_min=2015-01-01\&limit=200\&page=1\&order_status=unconfirmed,unshipped,to_be_shipped\&Sales_channels=dtc,wholesale
		//http://172.18.100.85:8688/admin/orders.json/?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		get_exchange_rate->set_method_handler("GET", get_exchange_rate_func);
		//get_orders_param->set_method_handler("POST", post_orders_param_func);
		get_exchange_rate->set_error_handler(&resource_error_handler);


		

		////////////////////////////////////////////////////////
		auto settings = std::make_shared< Settings >();
		settings->set_port(get_config->m_port);
		settings->set_worker_limit(get_config->m_threads);
		settings->set_default_header("Connection", "keep-alive");
		cout << "server started with port:" << get_config->m_port << ",thread num:" << get_config->m_threads << endl;
		Service service;
		
		service.publish(get_exchange_rate);
		//service.publish(put_orders_num);
		//service.publish(get_orders_param);
		

		
		// service.publish(default_index);
		// service.publish(default_index2);


		//service.set_authentication_handler(authentication_handler);
		
		service.set_error_handler(service_error_handler);
		service.set_logger(std::make_shared< CustomLogger >());
		service.start(settings);

}
}
#endif	