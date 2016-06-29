#ifndef EXCHANGE_RATE_RESTSERVER_HPP
#define	EXCHANGE_RATE_RESTSERVER_HPP
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
#include "exchange_rate.hpp"
#include "shared.hpp"
using namespace restbed;
namespace exchange_rate_namespace
{
//////////////default
void default_handler(const std::shared_ptr< restbed::Session > session)
{
	try
	{
		//cout << "default handler" << endl;
		/*string body = "this is default page";
		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });	*/
		const auto request = session->get_request();
		string filename = request->get_path_parameter("filename");
		cout << filename << endl;
		if (filename.length() == 0)
		{
			filename = "index.html";
		}
		ifstream stream("./resource/" + filename, ifstream::in);

		if (stream.is_open())
		{
			const string body = string(istreambuf_iterator< char >(stream), istreambuf_iterator< char >());

			const multimap< string, string > headers
			{
				{ "Content-Type", "text/html" },
				{ "Content-Length", ::to_string(body.length()) }
			};

			session->close(OK, body, headers);
		}
		else
		{
			session->close(NOT_FOUND);
		}
	}
	catch (std::exception& ex)
	{
		cout << ex.what() << endl;
		session->close(NOT_FOUND);
	}
}
/////////////error
void faulty_method_handler(const std::shared_ptr< restbed::Session >)
{
	throw SERVICE_UNAVAILABLE;
}

void resource_error_handler(const int, const std::exception&, const std::shared_ptr< restbed::Session > session)
{
	if (session->is_open())
	{
		session->close(6000, "Custom Resource Internal Server Error", { { "Content-Length", "37" } });
	}
	else
	{
		session->close();
		fprintf(stderr, "Custom Resource Internal Server Error\n");
	}
}

void service_error_handler(const int, const std::exception& ex, const std::shared_ptr< restbed::Session > session)
{
	if (session->is_open())
	{
		cout << ex.what() << endl;
		session->close(5000, "Custom Service Internal Server Error", { { "Content-Length", "36" } });
	}
	else
	{
		session->close();
		fprintf(stderr, "Custom Service Internal Server Error\n");
	}
}

////////////////////auth
void authentication_handler(const std::shared_ptr< restbed::Session > session,
	const function< void(const std::shared_ptr< restbed::Session >) >& callback)
{
	const auto request = session->get_request();
	auto authorization = request->get_header("Authorization");
	// curl -w'\n' -v -XGET 'http://testapi@orderbot.com:ShinyElephant232#@localhost:8688/resource'
	// curl -u testapi@orderbot.com:ShinyElephant232# -X GET http://localhost:8688/resource
	//cout << authorization << endl;

	/*BOOST_LOG_SEV(slg, boost_log->get_log_level()) << authorization;
	boost_log->get_initsink()->flush();*/



	cout << request->get_path() << endl;
	{
		/*string body = "";
		for (const auto key : session->keys())
		{
			string value = session->get(key);
			body += key + ":" + value + "\n";

		}
		cout << body << endl;

		auto ret = request->get_query_parameters();
		for (auto& r : ret)
			cout << r.first << ":" << r.second << endl;*/

		/*size_t content_length = 0;
		request->get_header("Content-Length", content_length);
		cout << "Content-Length:" << content_length << endl;*/
		//cout << "url:" << request->get_header("url").get_query_parameters() << endl;


		//session->fetch(content_length, [request](const std::shared_ptr< Session > session, const Bytes & body)
		//{
		//	fprintf(stdout, ":%.*s\n", (int)body.size(), body.data());
		//	//session->close(OK, "Hello, World!", { { "Content-Length", "13" }, { "Connection", "close" } });
		//});
	}
	string temp;
	SimpleWeb::Base64Encode(get_config->m_exchange_rate_username+":"+get_config->m_exchange_rate_password,&temp);
	if (authorization not_eq "Basic "+temp)//dGVzdGFwaUBvcmRlcmJvdC5jb206U2hpbnlFbGVwaGFudDIzMiM=")
	{
		//session->close(UNAUTHORIZED, { { "WWW-Authenticate", "Basic realm=\"restbed\"" } });
		string tip="UNAUTHORIZED";
		session->close(UNAUTHORIZED, tip, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(tip.length()) } });
	}
	else
	{
		callback(session);
	}
}
/////////////////regular
void get_general_func(const std::shared_ptr< restbed::Session > session)
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
		if(r.first=="source")
			source=r.second;
		else if(r.first=="target")
			target=r.second;
		else if(r.first=="time")
			which_day=r.second;	
		else if(r.first=="database")
			database_name=r.second;	
	}
	
	session->fetch(0, [=](const std::shared_ptr< restbed::Session > session, const Bytes & content_body)
	{		
		boost::shared_ptr<mysql_database_base> mysql_xx;
		if(database_name=="os")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_os());
		}
		else if(database_name=="js")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_js());
		}
		else if(database_name=="eu")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_eu());
		}
		else// if(database_name=="eu")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_as());
		}
		boost::shared_ptr<exchange_rate_rest> exchange_rate_rest(new exchange_rate_rest(mysql_xx));
		string rate=exchange_rate_rest->get_rate(source,target,which_day);
		cout<<__FILE__<<":"<<__LINE__<<":"<<rate<<endl;
		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		session->close(OK, rate, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(rate.length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<OK<<":"<<rate;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void post_exchange_rate_func(const std::shared_ptr< restbed::Session > session)
{
	const auto request = session->get_request();
	string path = request->get_path();
	auto ret = request->get_query_parameters();
	string source="USD",target,which_day,database_name,ratio;

	for (auto& r : ret)
	{	
		//cout << r.first << ":" << r.second << endl;
		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		//param+=r.first+"="+r.second+"&";
		if(r.first=="source")
			source=r.second;
		else if(r.first=="target")
			target=r.second;
		else if(r.first=="time")
			which_day=r.second;	
		else if(r.first=="database")
			database_name=r.second;	
		else if(r.first=="ratio")
			ratio=r.second;	
	}
	
	session->fetch(0, [=](const std::shared_ptr< restbed::Session > session, const Bytes & content_body)
	{
		boost::shared_ptr<mysql_database_base> mysql_xx;
		if(database_name=="os")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_os());
		}
		else if(database_name=="js")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_js());
		}
		else if(database_name=="eu")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_eu());
		}
		else// if(database_name=="eu")
		{
			mysql_xx=boost::shared_ptr<mysql_database_base>(new mysql_database_as());
		}
		boost::shared_ptr<exchange_rate_rest> exchange_rate_rest(new exchange_rate_rest(mysql_xx));
		exchange_rate_rest->update_rate(source,target,which_day,ratio);
		cout<<__FILE__<<":"<<__LINE__<<":"<<endl;
		string temp="update ok";
		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		session->close(OK, temp, { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(temp.length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<OK<<":"<<temp;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
// void get_orders_param_func(const std::shared_ptr< restbed::Session > session)
// {
// 	//get_general_func(session);
// 	const auto request = session->get_request();
// 	string path = request->get_path();
// 	string param="";
// 	auto ret = request->get_query_parameters();
// 	for (auto& r : ret)
// 	{	
// 		//cout << r.first << ":" << r.second << endl;
// 		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
// 		param+=r.first+"="+r.second+"&";
// 	}

	
// 	size_t content_length = 0;
// 	request->get_header("Content-Length", content_length);
// 	//cout<<__LINE__<<":"<<request->get_header( "SessionID" )<<endl;
// 	session->fetch(content_length, [=](const std::shared_ptr< Session > session, const Bytes & content_body)
// 	{
// 		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
// 		order->request("GET", path, param, "");
		
// 		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
// 		cout<<"status:"<<order->get_status()<<endl;
// 		session->close(order->get_status(), *(order->get_data()), { {"Cache-Control","no-cache"},{"Pragma","no-cache"},{ "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(order->get_length()) } });
		
// 		////////////////////////////////////////////////////////////
// 		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<order->get_status()<<":"<<*(order->get_data());
// 		boost_log->get_initsink()->flush();
// 		/////////////////////////////////////////////////////
// 	});
// }
void get_exchange_rate_func(const std::shared_ptr< restbed::Session > session)
{	
	cout<<__FILE__<<":"<<__LINE__<<endl;
	get_general_func(session);
}

}

#endif	