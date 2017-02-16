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
		boost::shared_ptr<mysql_info_> from(new mysql_info_());
		from->ip=get_config->m_mysql_scm_from_ip;
		from->username=get_config->m_mysql_scm_from_username;
		from->password=get_config->m_mysql_scm_from_password;
		from->database=get_config->m_mysql_scm_from_database;
		from->port=boost::lexical_cast<std::string>(get_config->m_mysql_scm_from_port);

		boost::shared_ptr<mysql_info_> to(new mysql_info_());
		to->ip=get_config->m_mysql_scm_to_ip;
		to->username=get_config->m_mysql_scm_to_username;
		to->password=get_config->m_mysql_scm_to_password;
		to->database=get_config->m_mysql_scm_to_database;
		to->port=boost::lexical_cast<std::string>(get_config->m_mysql_scm_to_port);

		boost::shared_ptr<scm_supplier_rest> producer_scm_supplier_rest_js(new scm_supplier_rest(from,to));
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


}

#endif	