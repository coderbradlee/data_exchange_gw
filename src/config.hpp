#ifndef CONFIG_HPP
#define	CONFIG_HPP

#include <curl/curl.h>
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
//#define DEBUG
#include "serverResource.hpp"
#include "renesolalog.hpp"
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
			m_orderbot_username = m_pt.get<std::string>("orderbot.username");
			m_orderbot_password = m_pt.get<std::string>("orderbot.password");
			m_orderbot_url = m_pt.get<std::string>("orderbot.url");	
			m_redis_host=m_pt.get<std::string>("redis.host");	
			m_redis_password=m_pt.get<std::string>("redis.password");
			m_redis_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("redis.port"));
			m_webserver_url=m_pt.get<std::string>("webserver.url");
			m_webserver_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("webserver.port"));
		}
	public:
		boost::property_tree::ptree m_pt;
		string m_mysql_ip;
		unsigned short m_mysql_port;
		unsigned short m_webserver_port;
		string m_mysql_username;
		string m_mysql_password;
		string m_mysql_database;
		string m_mysql_table;
		string m_mysql_table2;
		size_t m_threads;
		string m_orderbot_username;
		string m_orderbot_password;
		string m_orderbot_url;	
		static boost::mutex m_mu;	
		static boost::shared_ptr<iconfig> m_ps;
		string m_redis_host;
		string m_redis_password;
		unsigned short m_redis_port;
		string m_webserver_url;
};
boost::shared_ptr<iconfig> iconfig::m_ps = nullptr;
boost::mutex iconfig::m_mu;
boost::shared_ptr<iconfig> get_config = iconfig::get_instance("config.ini");
#endif

