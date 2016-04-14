#ifndef ORDERBOT_RESTSERVER_HPP
#define	ORDERBOT_RESTSERVER_HPP
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
using namespace restbed;

//////////////default
void default_handler(const std::shared_ptr< Session > session)
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
void faulty_method_handler(const std::shared_ptr< Session >)
{
	throw SERVICE_UNAVAILABLE;
}

void resource_error_handler(const int, const std::exception&, const std::shared_ptr< Session > session)
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

void service_error_handler(const int, const std::exception& ex, const std::shared_ptr< Session > session)
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
void authentication_handler(const std::shared_ptr< Session > session,
	const function< void(const std::shared_ptr< Session >) >& callback)
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

	if (authorization not_eq "Basic dGVzdGFwaUBvcmRlcmJvdC5jb206U2hpbnlFbGVwaGFudDIzMiM=")
	{
		session->close(UNAUTHORIZED, { { "WWW-Authenticate", "Basic realm=\"restbed\"" } });
	}
	else
	{
		callback(session);
	}
}
/////////////////regular
void get_general_func(const std::shared_ptr< Session > session)
{
	const auto request = session->get_request();
	//string order_num = request->get_path_parameter("name");
	string path = request->get_path();
	string param="";
	auto ret = request->get_query_parameters();
	for (auto& r : ret)
	{	
		//cout << r.first << ":" << r.second << endl;
		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		param+=r.first+"="+r.second+"&";
	}
	
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);

	session->fetch(content_length, [=](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		//const string temp_content( content_body.begin( ), content_body.end( ) );
		std::shared_ptr<std::string> temp_content(new std::string( content_body.begin(), content_body.end()));
		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
		order->request("GET", path, param, *temp_content);

		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		session->close(order->get_status(), *(order->get_data()), { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(order->get_length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<order->get_status()<<":"<<*(order->get_data());
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void get_orders_param_func(const std::shared_ptr< Session > session)
{
	//get_general_func(session);
	const auto request = session->get_request();
	string path = request->get_path();
	string param="";
	auto ret = request->get_query_parameters();
	for (auto& r : ret)
	{	
		//cout << r.first << ":" << r.second << endl;
		////?created_at_min=2015-01-01&limit=200&page=1&order_status=unconfirmed,unshipped,to_be_shipped&Sales_channels=dtc,wholesale
		param+=r.first+"="+r.second+"&";
	}

	
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	//cout<<__LINE__<<":"<<request->get_header( "SessionID" )<<endl;
	session->fetch(content_length, [=](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
		order->request("GET", path, param, "");
		
		//cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		cout<<"status:"<<order->get_status()<<endl;
		session->close(order->get_status(), *(order->get_data()), { {"Cache-Control","no-cache"},{"Pragma","no-cache"},{ "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(order->get_length()) } });
		
		////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<order->get_status()<<":"<<*(order->get_data());
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void get_orders_num_func(const std::shared_ptr< Session > session)
{	get_general_func(session);

}
void put_orders_num_func(const std::shared_ptr< Session > session)
{
	const auto request = session->get_request();
	//string order_num = request->get_path_parameter("name");
	string request_path = request->get_path();
	//cout<<__LINE__<<":"<<request_path<<endl;
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);

	session->fetch(content_length, [=](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		//cout<<__LINE__<<":"<<request_path<<endl;
		const string temp_content( content_body.begin( ), content_body.end( ) );
		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
		order->request("PUT", request_path, "", temp_content);
		cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		cout<<"status:"<<order->get_status()<<endl;
		session->close(order->get_status(), *(order->get_data()), { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(order->get_length()) } });
				////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<order->get_status()<<":"<<*(order->get_data());
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
}
void post_orders_param_func(const std::shared_ptr< Session > session)
{
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [=](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		//fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());
		//session->close(OK, "Hello, World!", { { "Content-Length", "13" }, { "Connection", "close" } });
		//orderbot 接口
		
		//string temp_content(content_body.data(),(int)content_body.size());
		// string temp_content;
		// sprintf(temp_content, "%.*s\n", (int)content_body.size(), content_body.data());
		const string temp_content( content_body.begin( ), content_body.end( ) );
		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
		order->request("POST", "/admin/orders.json/", "", temp_content);

		cout<<order->get_length()<<":"<<*(order->get_data())<<endl;
		// string body = "{\"response_code\": 1,\"message\": \"Success\",\"order_process_result\": [{\"response_code\": 0,\"orderbot_order_id\": 79,\"reference_order_id\": \"aabb15998966\",\"success\": true,\"message\": \"Order has been placed successfully!\"}]}";
		cout<<"status:"<<order->get_status()<<endl;
		session->close(order->get_status(), *(order->get_data()), { { "Content-Type", "application/json; charset=utf-8" },{ "Content-Length", ::to_string(order->get_length()) } });

		////////////////////////////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "response:"<<order->get_status()<<":"<<*(order->get_data());
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
	});
		
}


void get_products_num_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void put_products_num_func(const std::shared_ptr< Session > session)
{
	string product_num = session->get_request()->get_path_parameter("name");
	cout << "product_num:" << product_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());
		
		string body = "{  \"success\": true,  \"message\": \"Prouct update success\",  \"reference_product_id\": null,  \"orderbot_product_id\": 429229  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});


}
void get_products_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void post_products_param_func(const std::shared_ptr< Session > session)
{
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());
		
		string body = "[  {  \"success\": true,  \"message\": \"Product created success; BOM created success; Purchase unit created success\",  \"reference_product_id\": \"1000\",  \"orderbot_product_id\": 469317  },  {  \"success\": true,  \"message\": \"Product created success; BOM created success; Purchase unit created success\",  \"reference_product_id\": \"1001\",  \"orderbot_product_id\": 469318  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void get_customers_num_func(const std::shared_ptr< Session > session)
{	
	get_general_func(session);
}
void put_customers_num_func(const std::shared_ptr< Session > session)
{
	
	string product_num = session->get_request()->get_path_parameter("name");
	cout << "product_num:" << product_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"reference_customer_id\": \"7777777\",  \"orderbot_customer_id\": 2,  \"success\": true,  \"message\": \"Customer updated successfully\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});

}
void get_customers_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void post_customers_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"reference_account_id\": \"1000111\",  \"orderbot_account_id\": 36,  \"customers\": [  {  \"reference_customer_id\": \"10001\",  \"orderbot_customer_id\": 36,  \"success\": true,  \"message\": \"Customer successfully created\"  }  ],  \"success\": true,  \"message\": \"Account successfully created\"  },  {  \"reference_account_id\": \"1000113\",  \"orderbot_account_id\": 37,  \"customers\": [  {  \"reference_customer_id\": \"10001\",  \"orderbot_customer_id\": 37,  \"success\": true,  \"message\": \"Customer successfully created\"  }  ],  \"success\": true,  \"message\": \"Account successfully created\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void put_account_num_func(const std::shared_ptr< Session > session)
{
	
	string product_num = session->get_request()->get_path_parameter("name");
	//cout << "product_num:" << product_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"reference_customer_id\": \"7777777\",  \"orderbot_customer_id\": 2,  \"success\": true,  \"message\": \"Customer updated successfully\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});

}

void get_account_num_func(const std::shared_ptr< Session > session)
{	
	get_general_func(session);
}
void get_account_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void post_account_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"reference_account_id\": \"1000111\",  \"orderbot_account_id\": 36,  \"customers\": [  {  \"reference_customer_id\": \"10001\",  \"orderbot_customer_id\": 36,  \"success\": true,  \"message\": \"Customer successfully created\"  }  ],  \"success\": true,  \"message\": \"Account successfully created\"  },  {  \"reference_account_id\": \"1000113\",  \"orderbot_account_id\": 37,  \"customers\": [  {  \"reference_customer_id\": \"10001\",  \"orderbot_customer_id\": 37,  \"success\": true,  \"message\": \"Customer successfully created\"  }  ],  \"success\": true,  \"message\": \"Account successfully created\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void get_salesreps_num_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void put_salesreps_num_func(const std::shared_ptr< Session > session)
{ 
	string salesreps_num = session->get_request()->get_path_parameter("name");
	cout << "salesreps_num:" << salesreps_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"sales_rep_id\": 461,  \"sales_name\": \"Peter\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void get_salesreps_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void post_salesreps_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"sales_rep_id\": 560,  \"sales_name\": \"Peter\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void put_product_categories_num_func(const std::shared_ptr< Session > session)
{ 
	string product_categories_num = session->get_request()->get_path_parameter("name");
	cout << "product_categories_num:" << product_categories_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"is_valid\": true,  \"ref_category_id\": \"123\",  \"product_category_id\": 55,  \"product_category_name\": \"test\",  \"message\": \"Updated success\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void post_product_categories_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"is_valid\": true,  \"ref_category_id\": \"123\",  \"product_category_id\": 55,  \"product_category_name\": \"test\",  \"message\": \"Uploaded success\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void put_product_groups_num_func(const std::shared_ptr< Session > session)
{ 
	string put_product_groups_num = session->get_request()->get_path_parameter("name");
	cout << "put_product_groups_num:" << put_product_groups_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"is_valid\": true,  \"ref_group_id\": \"123\",  \"product_group_id\": 55,  \"product_group_name\": \"test\",  \"message\": \"Updated success\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void post_product_groups_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"is_valid\": true,  \"ref_group_id\": \"123\",\"product_group_id\": 55,  \"product_group_name\": \"test\",  \"message\": \"Uploaded success\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void put_variable_groups_num_func(const std::shared_ptr< Session > session)
{ 
	string put_variable_groups_num_func = session->get_request()->get_path_parameter("name");
	cout << "put_variable_groups_num_func:" << put_variable_groups_num_func << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"is_valid\": true,  \"variable_group_id\": 55,  \"variable_group_name\": \"test\",  \"message\": \"Updated success\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}


void post_product_variable_groups_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"is_valid\": true,  \"variable_group_id\": 55,  \"variable_group_name\": \"test\",  \"message\": \"Uploaded success\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void put_product_variables_num_func(const std::shared_ptr< Session > session)
{ 
	string put_product_variables_num = session->get_request()->get_path_parameter("name");
	cout << "put_product_variables_num:" << put_product_variables_num << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"is_valid\": true,  \"variable_group_id\": 55,  \"variable_name\": \"test\",  \"message\": \"Updated success\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void post_product_variables_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"is_valid\": true,  \"variable_group_id\": 55,  \"variable_name\": \"test\",  \"message\": \"Uploaded success\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}
void put_product_variable_values_num_func(const std::shared_ptr< Session > session)
{ 
	string put_product_variable_values_num_func = session->get_request()->get_path_parameter("name");
	cout << "put_product_variable_values_num_func:" << put_product_variable_values_num_func << endl;
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "{  \"success\": true,  \"is_valid\": true,  \"variable_id\": 55,  \"variable_value_name\": \"test\",  \"message\": \"Updated success\"  }";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}

void post_product_variable_values_param_func(const std::shared_ptr< Session > session)
{
	
	const auto request = session->get_request();
	size_t content_length = 0;
	request->get_header("Content-Length", content_length);
	cout << content_length << endl;
	//string ret = session->get_body();
	session->fetch(content_length, [&](const std::shared_ptr< Session > session, const Bytes & content_body)
	{
		fprintf(stdout, "%.*s\n", (int)content_body.size(), content_body.data());

		string body = "[  {  \"success\": true,  \"is_valid\": true,  \"variable_id\": 55,  \"variable_value_name\": \"test\",  \"message\": \"Uploaded success\"  }  ]";

		session->close(OK, body, { { "Content-Length", ::to_string(body.length()) } });
	});
}



void get_account_groups_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_units_of_measurement_types_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_order_guides_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_product_structure_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_product_variables_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}

void get_purchase_unit_categories_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_vendor_purchase_unit_of_measurements_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_salesrep_groups_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_Vendors_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_Countries_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_States_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_distribution_centers_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}
void get_websites_param_func(const std::shared_ptr< Session > session)
{
	get_general_func(session);
}

std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
    while(true)   {     
        std::string::size_type   pos(0);     
        if(   (pos=str.find(old_value))!=std::string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }     
    return   str;     
}     
std::string&   replace_all_distinct(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
    for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {     
        if(   (pos=str.find(old_value,pos))!=std::string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }     
    return   str;     
} 


#endif	