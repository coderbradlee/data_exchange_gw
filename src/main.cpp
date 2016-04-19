#include "config.hpp"
#include "orderbot.hpp"
#include "credit_ontime.hpp"
#include "activemq.hpp"
#include "orderbot_restserver_resource.hpp"
#include "product_inventory.hpp"
#include "activemq_cms.hpp"

int main()
{
	try
	{
		{
			send_messge_to_activemq("");
		}
		{
			product_inventory t;
			//t.start_update();
			//t.start_update();
		}
		{
			//orderbot server
			//orderbot_server_start();
		}
		
		{
			boost::timer::cpu_timer pass;
			pass.start();
			credit_ontime test;
			//test.start_update();

			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
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