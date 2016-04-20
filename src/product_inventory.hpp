#ifndef PRODUCT_INVENTORY_HPP
#define	PRODUCT_INVENTORY_HPP

#include  "include.hpp"
//#define DEBUG
#include "mysql/mysql_api.hpp"
#include "orderbot.hpp"
#include "activemq_cms.hpp"

class product_inventory
{
public:
	product_inventory()
	{
		// m_conn=boost::shared_ptr<MySql>(new MySql(get_config->m_mysql_ip.c_str(), get_config->m_mysql_username.c_str(), get_config->m_mysql_password.c_str(), get_config->m_mysql_database.c_str(), get_config->m_mysql_port));
		
		// m_today_string=to_iso_extended_string(boost::gregorian::day_clock::local_day());
	}
	void get_product_all()
	{
		try
		{
			boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			order->request("GET", "/admin/products.json/", "", "");

			m_product_all=order->m_data;
			//cout<<*(order->m_data)<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
		}
	}
	void start_update()
	{
		try
		{
			get_product_all();
			//cout<<*m_product_all<<":"<<__FILE__<<":"<<__LINE__<<endl;
			//
			 ptree pt,ret_json_all;
			 ptree return_json;
			 std::istringstream is(*m_product_all);
			read_json(is, pt);
			for(auto& sub:pt)
			{
				ptree ret_json;
	   			string product_category_id=sub.second.get<string>("product_category_id");
				int product_id=sub.second.get<int>("product_id");
				string product_name=sub.second.get<string>("product_name");
				string sku=sub.second.get<string>("sku");

				//cout<<product_id<<":"<<product_name<<":"<<sku<<endl;
				ret_json.put<std::string>("product_code",get_product_id(product_name));
				ret_json.put<std::string>("product_name",product_name);

				ptree child = sub.second.get_child("inventory_quantities");
				ret_json.add_child("inventory_quantities", child);
				

				// if(!child.empty())
				// {
				// 	for(auto& subchild:child)
				// 	{
					
				// 		double distribution_center_id=subchild.second.get<double>("distribution_center_id");
				// 		string distribution_center_name=subchild.second.get<string>("distribution_center_name");
				// 		double inventory_quantity=subchild.second.get<double>("inventory_quantity");
				// 		cout<<distribution_center_name<<":"<<inventory_quantity<<endl;
						
				// 	}
				// }

				ret_json_all.push_back(std::make_pair("", ret_json));

			}
				return_json.push_back(std::make_pair("product", ret_json_all));
				write_json(m_ss, return_json);
				send_messge_to_activemq();
			}
			catch(json_parser_error& e) 
			{
				BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
					boost_log->get_initsink()->flush();
					cout<<e.what()<<endl;
			}

			catch (const MySqlException& e)
			{
				BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			}
			catch(std::exception& e)
			{
				BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			}
	}
	void send_to_mq()
	{
		try
		{
			string message(m_ss.str());
			message.erase(remove(message.begin(), message.end(), '\n'), message.end());
			//orderbot 接口
			boost::shared_ptr<activemq_rest> am = boost::shared_ptr<activemq_rest>(new activemq_rest(get_config->m_activemq_username, get_config->m_activemq_password, get_config->m_activemq_url));
			am->request("POST", "/api/message/"+get_config->m_activemq_write_product_queue, "type=queue", "body="+message);
			cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			// am->request("GET", "/api/message/TEST", "type=queue&clientId=consumerA", "");
			// cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			// 
			// 
			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << am->get_data();
			boost_log->get_initsink()->flush();
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
		}
	}
	string get_product_id(const string& product_name)
	{
		try
		{
			m_conn=boost::shared_ptr<MySql>(new MySql(get_config->m_mysql_ip.c_str(), get_config->m_mysql_username.c_str(), get_config->m_mysql_password.c_str(), get_config->m_mysql_database.c_str(), get_config->m_mysql_port));
			typedef tuple<unique_ptr<string>> product_tuple;
				
			//typedef tuple<string,double> credit_tuple;
			vector<product_tuple> product;
			string query_sql = "SELECT product_code FROM " + get_config->m_mysql_database + ".t_item_master where product_name='" + product_name + "'";
			//cout << query_sql << endl;
			m_conn->runQuery(&product, query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			boost_log->get_initsink()->flush();
			/********************************/
			cout.setf(ios::showpoint); cout.setf(ios::fixed); cout.precision(8);
			/********************************/
			if(product.empty())
			{
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "get nothing from t_item_master";
				boost_log->get_initsink()->flush();
				cout<<"get nothing from t_item_master"<<endl;
				return "";
			}
			else
			{
				for (const auto& item : product)
				{
					cout << item << endl;

					return *(std::get<0>(item));			
				}
			}

		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			return "";
		}
	}
	void send_messge_to_activemq()
	{
		string message(m_ss.str());
		message.erase(remove(message.begin(), message.end(), '\n'), message.end());
		activemq::library::ActiveMQCPP::initializeLibrary();
		std::string brokerURI =
	        "failover://(tcp://"+get_config->m_activemq_url+
	       // "?wireFormat=openwire"
	       // "&connection.useAsyncSend=true"
	       // "&transport.commandTracingEnabled=true"
	       // "&transport.tcpTracingEnabled=true"
	       // "&wireFormat.tightEncodingEnabled=true"
	        ")";

	    bool useTopics = false;

	    activemq_cms_producer producer(message,brokerURI, 1, get_config->m_activemq_write_product_queue, useTopics,true );

	    producer.run();

	    producer.close();

	    activemq::library::ActiveMQCPP::shutdownLibrary();
	}
private:
	boost::shared_ptr<MySql> m_conn;
	string m_today_string;
	boost::shared_ptr<string> m_product_all;
	std::stringstream m_ss;
};

#endif

