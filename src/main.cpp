#include "config.hpp"
#include "orderbot.hpp"
#include "credit_ontime.hpp"
#include "activemq.hpp"
#include "orderbot_restserver_resource.hpp"
#include "activemq_producer_consumer.hpp"
#include "boost_signalset.hpp"
void test()
{
	string text="{    \"sales_order_id\": \"1\",    \"so_no\": \"2\",    \"po_no\": \"3\",    \"status\": 0,    \"order_date\": \"2016-05-03\",    \"company_id\": \"4\",    \"sales_id\": \"5\",    \"currency_id\": \"6\",    \"ss_currency_daily_exchange_rate\": 6.45,    \"tax_schedule_id\": \"7\",    \"ss_tax_rate\": 7.2,    \"customer_master_id\": \"8\",    \"customer_contact_id\": \"9\",    \"customer_invoice_address_id\": \"10\",    \"ship_to_customer_name\": \"11\",    \"ship_to_address\": \"12\",    \"ship_to_state\": \"NY\",    \"ship_to_city\": \"13\",    \"ship_to_zip_code\": \"10118\",    \"ship_to_contact_name\": \"14\",    \"ship_to_contact_phone_number\": \"+1 800-428-4322\",    \"ship_to_contact_email\": \"test@orderbot.com\",    \"trade_term_id\": \"17\",    \"ss_landed_cost_coefficient\": 3.3,    \"dispatch_warehouse_id\": 1,    \"requested_delivery_date\": \"2016-05-03\",    \"promotion_code\": \"\",    \"company_bank_account_id\": \"\",    \"shipping_cost_total\": 25.48,    \"saving_total\": 3.56,    \"tax_total\": 22.51,    \"sub_total\": 180.37,    \"grand_total\": 218.67,    \"note\": \"note\",    \"detail\": [        {            \"sales_order_detail_id\": 44,            \"item_master_id\": \"\",            \"ss_guidance_price\": 5.46,            \"ss_promotion_price\": 5.41,            \"unit_price\": 5.43,            \"uom_id\": \"\",            \"quantity\": 12,            \"sub_total\": 63.67,            \"sub_tax\": 4.33,            \"sub_shipping_cost\": 5.68,            \"sub_discount\": 0,            \"note\": \"detail.note\"        }    ]}";
	ifstream is( text );

	using namespace json_spirit;
    json_spirit::Value value;

    json_spirit::read( text, value );

	if (value.type() != obj_type)
	{
	    //return;
	}
	else
	{
	    int id = 0;
	    std::string sales_order_id = "";
	    const json_spirit::Object &obj = value.get_obj();
	    for(unsigned int i = 0; i != obj.size(); ++i )
	    {
	        const json_spirit::Pair& pair = obj[i];

	        const string& name  = pair.name_;
	        const json_spirit::Value&  vvalue = pair.value_;

	        if( name == "sales_order_id" && vvalue.type() == int_type)
	        {
	            id = vvalue.get_int();
	            cout<<__FILE__<<":"<<__LINE__<<":"<<id<<endl;
	        }
	        else if( name == "sales_order_id" && vvalue.type() == str_type)
	        {
	            sales_order_id = vvalue.get_str();
	            cout<<__FILE__<<":"<<__LINE__<<":"<<sales_order_id<<endl;
	        }
	    }
	}


	json_spirit::Object robj;
	robj.push_back( Pair("id", 1));
	robj.push_back( Pair("username", "test"));
	std::string output = json_spirit::write_formatted(robj);
	cout<<__FILE__<<":"<<__LINE__<<":"<<output<<endl;
}
int main()
{
	for(int i=1;i<64;++i)
	std::signal(i, sig_handler);  
	//std::raise(SIGINT);
	try
	{
		{
			//test();
			// credit_ontime test;
			// test.start_update();
		}
		{	
			boost::shared_ptr<activemq_producer_consumer> producer_consumer(new activemq_producer_consumer);
			thread producer_consumer_thread([&producer_consumer](){producer_consumer->start();});
			producer_consumer_thread.join();
		}
		{
			//orderbot server
			//orderbot_server_start();
		}
		
		
		{
			boost::timer::cpu_timer pass;
			pass.start();

			//orderbot 接口
			boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			// order->request("GET", "/admin/products.json/", "class_type=sales&category_name=Rings", "");

			// cout<<order->get_data().length()<<":"<<order->get_data()<<endl;
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}
		{
			boost::timer::cpu_timer pass;
			pass.start();

			//orderbot 接口
			boost::shared_ptr<activemq_rest> am = boost::shared_ptr<activemq_rest>(new activemq_rest(get_config->m_activemq_username, get_config->m_activemq_password, get_config->m_activemq_url));
			//am->request("POST", "/api/message/TEST", "type=queue", "body={message:\"test\"}");
			//cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			//am->request("GET", "/api/message/TEST", "type=queue&clientId=consumerA", "");
			//cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			this_thread::sleep_for(chrono::seconds(10));
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}

	}
	catch (std::exception& e)
	{
		//cout << diagnostic_information(e) << endl;
		cout << e.what() << endl;
	}
	catch (...)
	{

	}
	return 0;
}