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
		get_exchange_rate->set_method_handler("POST", post_exchange_rate_func);
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


		service.set_authentication_handler(authentication_handler);
		
		service.set_error_handler(service_error_handler);
		service.set_logger(std::make_shared< CustomLogger >());
		service.start(settings);

}

// void update()
// {
// 	boost::this_thread::sleep(boost::posix_time::millisec(100));
// 	std::vector<string> update_time{"2016-06-03","2016-06-07","2016-06-08","2016-06-13","2016-06-15"};
// 			std::vector<string> currency_name{"SGD","AUD","NZD","CAD","EUR","GBP","INR","JPY","KRW","ZAR","IDR","TRY","MXN","CNY","THB","BRL","HKD","RUR"};
// 			string ratio[18][5] 
// 			{
// 				{"1.3780","1.3603","1.3549","1.3637","1.3602"},
// 				{"1.3853","1.3600","1.3440","1.3586","1.3640"},
// 				{"1.4692","1.4517","1.4353","1.4232","1.4355"},
// 				{"1.3115","1.2856","1.2762","1.2828","1.2897"},
// 				{"0.8986","0.8819","0.8812","0.8913","0.8946"},
// 				{"0.6951","0.6920","0.6885","0.7056","0.7090"},
// 				{"67.2305","66.8850","66.7600","66.9700","67.3165"},
// {"109.2666","107.8300","107.1333","106.1800","106.3632"},
// {"1191.5473","1166.0384","1159.6851","1173.7500","1181.4024"},
// {"15.5827","14.9551","14.9198","15.2549","15.3459"},
// {"13610.0000","13335.0000","13210.0000","13340.0000","13405.0000"},
// {"2.9502","2.9066","2.8967","2.9315","2.9373"},
// {"18.6890","18.7049","18.3901","18.6822","18.9471"},
// {"6.5690","6.5578","6.5557","6.5760","6.5875"},
// {"35.6236","35.3330","35.2647","35.3548","35.3976"},
// {"3.5831","3.4874","3.4404","3.4197","3.4795"},			
// {"7.7703","7.7681","7.7665","7.7630","7.7619"},
// {"66.9623","65.6436","64.7796","65.4980","66.2726"}
// 			};
// 			for(int i=0;i<18;++i)
// 			{
// 				for(int j=0;j<5;++j)
// 				{
// 					boost::shared_ptr<exchange_rate_rest_client> t(new exchange_rate_rest_client(currency_name[i], ratio[i][j],update_time[j],"os"));
// 					boost::this_thread::sleep(boost::posix_time::millisec(1000));
// 					cout<<__FILE__<<":"<<__LINE__<<":"<<t->get_data()<<endl;
					
// 				}
// 			}
// }
		void start()
		{
			boost::thread_group pool;  
		    pool.create_thread(boost::bind(exchange_rate_server_start));  
		    
		    //pool.create_thread(boost::bind(update));  
		  	pool.create_thread(boost::bind(start_exchange_rate_thread));
		    pool.join_all();  
			
			
			//test
			//curl -X GET http://172.18.100.87:8688/exchange_rate/?target=SGD\&time=2016-06-03\&database=eu
			//curl -X POST http://172.18.100.87:8688/exchange_rate/?target=SGD\&time=2016-06-03\&ratio=1.3708\&database=eu
			
		}	
}
#endif	