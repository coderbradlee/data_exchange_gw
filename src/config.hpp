#ifndef CONFIG_HPP
#define	CONFIG_HPP

#include <curl/curl.h>
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
//#define DEBUG
#include "include.hpp"
#include "renesolalog.hpp"
using std::string;
class iconfig:public boost::enable_shared_from_this<iconfig>, boost::noncopyable
{
	public:
		static boost::shared_ptr<iconfig> get_instance(const std::string& filename)
		{
			boost::mutex::scoped_lock t(m_mu);
			if (m_ps == nullptr)
			{
				m_ps = boost::shared_ptr<iconfig>(new iconfig(filename));
			}
			
			return m_ps;
		}
	private:
		iconfig(const std::string& filename)
		{			
			boost::property_tree::ini_parser::read_ini(filename, m_pt);
			m_mysql_ip = m_pt.get<std::string>("mysql.ip");
			m_mysql_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql.port"));
			m_mysql_username = m_pt.get<std::string>("mysql.username");
			m_mysql_password = m_pt.get<std::string>("mysql.password");
			m_mysql_database = m_pt.get<std::string>("mysql.database");
			m_mysql_table = m_pt.get<std::string>("mysql.table");
			m_mysql_table2 = m_pt.get<std::string>("mysql.table2");
			m_threads = boost::lexical_cast<size_t>(m_pt.get<std::string>("webserver.threads"));
			m_port = boost::lexical_cast<size_t>(m_pt.get<std::string>("webserver.port"));
			m_orderbot_username = m_pt.get<std::string>("orderbot.username");
			m_orderbot_password = m_pt.get<std::string>("orderbot.password");
			m_orderbot_url = m_pt.get<std::string>("orderbot.url");	
			m_activemq_username = m_pt.get<std::string>("activemq.username");
			m_activemq_password = m_pt.get<std::string>("activemq.password");
			m_activemq_url = m_pt.get<std::string>("activemq.url");	
			// m_activemq_read_queue = m_pt.get<std::string>("activemq.read_queue");
			// m_activemq_write_queue = m_pt.get<std::string>("activemq.write_queue");	
			m_activemq_write_product_queue= m_pt.get<std::string>("activemq.write_product_queue");
			m_write_product_interval = boost::lexical_cast<size_t>(m_pt.get<std::string>("activemq.write_product_interval"));	
			m_activemq_read_order_queue= m_pt.get<std::string>("activemq.read_order_queue");	
			m_activemq_write_order_queue= m_pt.get<std::string>("activemq.write_order_queue");	
			m_exchange_rate_url=m_pt.get<std::string>("exchange_rate.url");
			m_exchange_rate_request_interval=m_pt.get<size_t>("exchange_rate.request_interval");
			m_exchange_rate_key=m_pt.get<std::string>("exchange_rate.key");
			m_exchange_rate_write_rate_queue=m_pt.get<std::string>("exchange_rate.write_rate_queue");
			/////////////////////////////////////////
			m_mysql_eu_ip = m_pt.get<std::string>("mysql_eu.ip");
			m_mysql_eu_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql_eu.port"));
			m_mysql_eu_username = m_pt.get<std::string>("mysql_eu.username");
			m_mysql_eu_password = m_pt.get<std::string>("mysql_eu.password");
			m_mysql_eu_database = m_pt.get<std::string>("mysql_eu.database");

			m_mysql_js_ip = m_pt.get<std::string>("mysql_js.ip");
			m_mysql_js_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql_eu.port"));
			m_mysql_js_username = m_pt.get<std::string>("mysql_js.username");
			m_mysql_js_password = m_pt.get<std::string>("mysql_js.password");
			m_mysql_js_database = m_pt.get<std::string>("mysql_js.database");
		}
	public:
		boost::property_tree::ptree m_pt;
		string m_mysql_ip;
		unsigned short m_mysql_port;
		string m_mysql_username;
		string m_mysql_password;
		string m_mysql_database;
		///////////////////
		string m_mysql_eu_ip;
		unsigned short m_mysql_eu_port;
		string m_mysql_eu_username;
		string m_mysql_eu_password;
		string m_mysql_eu_database;
		//////////////////
		string m_mysql_js_ip;
		unsigned short m_mysql_js_port;
		string m_mysql_js_username;
		string m_mysql_js_password;
		string m_mysql_js_database;
		//////////////////////////
		string m_mysql_table;
		string m_mysql_table2;
		size_t m_threads;
		size_t m_port;
		string m_orderbot_username;
		string m_orderbot_password;
		string m_orderbot_url;	
		string m_activemq_username;
		string m_activemq_password;
		string m_activemq_url;
		string m_activemq_read_queue;
		string m_activemq_write_queue;	
		string m_activemq_write_product_queue;	
		size_t m_write_product_interval;
		string m_activemq_read_order_queue;	
		string m_activemq_write_order_queue;	
		string m_exchange_rate_url;	
		size_t m_exchange_rate_request_interval;	
		string m_exchange_rate_key;
		string m_exchange_rate_write_rate_queue;
		static boost::mutex m_mu;	
		static boost::shared_ptr<iconfig> m_ps;
};
boost::shared_ptr<iconfig> iconfig::m_ps = nullptr;
boost::mutex iconfig::m_mu;
boost::shared_ptr<iconfig> get_config = iconfig::get_instance("config.ini");
#endif

