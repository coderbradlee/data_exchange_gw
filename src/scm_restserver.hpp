#ifndef SCM_RESTSERVER_HPP
#define	SCM_RESTSERVER_HPP
#define BOOST_SPIRIT_THREADSAFE
#include <boost/regex.hpp>
#include "renesolalog.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <condition_variable>
#include <assert.h>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
//Added for the default_resource example
#include<fstream>
using namespace std;
//Added for the json:
using namespace boost::property_tree;
using namespace boost::posix_time;

#include <restbed>
#include "config.hpp"
#include "scm.hpp"
#include "shared.hpp"
using namespace restbed;
namespace scm_namespace
{

void post_scm_func(const std::shared_ptr< restbed::Session > session)
{
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	
	session->fetch(content_length, [=](const std::shared_ptr< restbed::Session > session, const Bytes & content_body)
	{
		string temp_content(content_body.begin(), content_body.end());
		std::cout<<temp_content<<std::endl;
		boost::shared_ptr<mysql_info_> info(new mysql_info_());
		info->ip=get_config->m_mysql_js_ip;
		info->username=get_config->m_mysql_js_username;
		info->password=get_config->m_mysql_js_password;
		info->database=get_config->m_mysql_js_database;
		info->port=boost::lexical_cast<std::string>(get_config->m_mysql_js_port);
		
		boost::shared_ptr<scm_supplier_rest> producer_scm_supplier_rest_js(new scm_supplier_rest(info));
		producer_scm_supplier_rest_js->insert_vendor();
		string temp="insert ok";
		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		session->close(OK, temp, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(temp.length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<OK<<":"<<temp;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void put_scm_func(const std::shared_ptr< restbed::Session > session)
{
	const auto request = session->get_request();
	string path = request->get_path();
	auto ret = request->get_query_parameters();
	//string source="USD",target,which_day,database_name,ratio;

	for (auto& r : ret)
	{	
		//cout << r.first << ":" << r.second << endl;
		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		//param+=r.first+"="+r.second+"&";
		// if(r.first=="source")
		// 	source=r.second;
		// else if(r.first=="target")
		// 	target=r.second;
		// else if(r.first=="time")
		// 	which_day=r.second;	
		// else if(r.first=="database")
		// 	database_name=r.second;	
		// else if(r.first=="ratio")
		// 	ratio=r.second;	
	}
	
	session->fetch(0, [=](const std::shared_ptr< restbed::Session > session, const Bytes & content_body)
	{
		boost::shared_ptr<mysql_info_> info(new mysql_info_());
		info->ip=get_config->m_mysql_js_ip;
		info->username=get_config->m_mysql_js_username;
		info->password=get_config->m_mysql_js_password;
		info->database=get_config->m_mysql_js_database;
		info->port=boost::lexical_cast<std::string>(get_config->m_mysql_js_port);
		boost::shared_ptr<scm_supplier_rest> producer_scm_supplier_rest_js(new scm_supplier_rest(info));
		producer_scm_supplier_rest_js->update_vendor();
		string temp="update ok";
		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		session->close(OK, temp, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(temp.length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<OK<<":"<<temp;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void get_scm_general_func(const std::shared_ptr< restbed::Session > session)
{
	const auto request = session->get_request();
	string path = request->get_path();
	auto ret = request->get_query_parameters();
	string source="USD",target,which_day,database_name;

	for (auto& r : ret)
	{	
		//cout << r.first << ":" << r.second << endl;
		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		//param+=r.first+"="+r.second+"&";
		// if(r.first=="source")
		// 	source=r.second;
		// else if(r.first=="target")
		// 	target=r.second;
		// else if(r.first=="time")
		// 	which_day=r.second;	
		// else if(r.first=="database")
		// 	database_name=r.second;	
	}
	
	session->fetch(0, [=](const std::shared_ptr< restbed::Session > session, const Bytes & content_body)
	{		
		boost::shared_ptr<mysql_info_> info(new mysql_info_());
		info->ip=get_config->m_mysql_js_ip;
		info->username=get_config->m_mysql_js_username;
		info->password=get_config->m_mysql_js_password;
		info->database=get_config->m_mysql_js_database;
		info->port=boost::lexical_cast<std::string>(get_config->m_mysql_js_port);
		boost::shared_ptr<scm_supplier_rest> p(new scm_supplier_rest(info));
		
		string rate=p->get_vendor();
		cout<<rate<<endl;
		session->close(OK, rate, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(rate.length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<OK<<":"<<rate;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void get_scm_func(const std::shared_ptr< restbed::Session > session)
{	
	get_scm_general_func(session);
}

}

#endif	