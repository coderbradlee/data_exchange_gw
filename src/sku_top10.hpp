#pragma once

#include  "include.hpp"

class cskutop:public boost::enable_shared_from_this<cskutop>
{
	mysql_database m_mysql_database;
public:
	cskutop(mysql_database mysql_input):m_d_t(m_io_s),m_product_all(nullptr),m_mysql_database(mysql_input),m_conn(nullptr)
	{
	}
	
	void get_info_from_myql()
	{
		try
		{
		typedef tuple<unique_ptr<string>, unique_ptr<string>> t_currency_tuple;
		//select code,currency_id from t_currency
			//typedef tuple<string,double> credit_tuple;
			std::vector<t_currency_tuple> t_currency_tuple_vector;
			string query_sql = "select code,currency_id from "+m_mysql_database.m_mysql_database + ".t_currency";
			cout << query_sql << endl;
			m_conn->runQuery(&t_currency_tuple_vector, query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			boost_log->get_initsink()->flush();
			
			if(t_currency_tuple_vector.empty())
			{
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "nothing select from t_currency";
				boost_log->get_initsink()->flush();
				cout<<"nothing select from t_currency"<<endl;
				return;
			}
			for (const auto& item : t_currency_tuple_vector)
			{
				//cout<<item<<":"<<__FILE__<<":"<<__LINE__<<endl;
				//cout<<*(std::get<0>(item))<<":"<<__FILE__<<":"<<__LINE__<<endl;
				if(*(std::get<0>(item))=="USD")
				{
					m_usd_info.code="USD";
					m_usd_info.currency_id=*(std::get<1>(item));
					cout<<m_usd_info.code<<":"<<m_usd_info.currency_id<<":"<<__FILE__<<":"<<__LINE__<<endl;
				}			
			}
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
	
	void start()
	{
		m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_sku_top10_request_interval));
		  
	    m_d_t.async_wait(boost::bind(&cskutop::handle_wait, shared_from_this(), boost::asio::placeholders::error));  
		m_io_s.run();
	}
	void handle_wait_method()
	{
		cout<<"handle_wait_method"<<":"<<__FILE__<<":"<<__LINE__<<endl;
    	if(m_conn==nullptr)
    	{
    		cout<<__FILE__<<":"<<__LINE__<<endl;
    		m_conn=boost::shared_ptr<MySql>(new MySql(m_mysql_database.m_mysql_ip.c_str(), m_mysql_database.m_mysql_username.c_str(), m_mysql_database.m_mysql_password.c_str(), m_mysql_database.m_mysql_database.c_str(), m_mysql_database.m_mysql_port));
    		cout<<__FILE__<<":"<<__LINE__<<endl;
    	}
		get_info_from_myql();
		
		m_conn->close();
		m_conn=nullptr;
		boost::this_thread::sleep(boost::posix_time::millisec(30000));
	}
	void handle_wait(const boost::system::error_code& error)  
    {  
    	cout<<"handle_wait"<<__FILE__<<":"<<__LINE__<<endl;
        if(!error)  
        {  
        	ptime now = second_clock::local_time();			
			string hour_minute_second = to_simple_string(now.time_of_day());
			std::vector<std::string> hms;
 			
			boost::split(hms,hour_minute_second , boost::is_any_of(":"));
 			
 			string hour=hms[0];
 			string minute=hms[1];
 			
        	string hour_minute=hour+":"+minute;
        	if(hour_minute==get_config->m_sku_top10_insert_time)
        	{   
        		handle_wait_method();
        	}
        	
            m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_sku_top10_request_interval));  
            m_d_t.async_wait(boost::bind(&cskutop::handle_wait,shared_from_this(), boost::asio::placeholders::error));                 
    	}
	}  
	

private:
	boost::shared_ptr<MySql> m_conn;
	string m_today_string;
	boost::shared_ptr<string> m_product_all;
	std::stringstream m_ss;
	boost::asio::io_service m_io_s;  
	deadline_timer m_d_t;
	std::vector<exchage_rate_data> m_exchage_rate_data_array;
	exchage_rate_data m_usd_info;
	vector<general_rate_data> m_general_rate_data;

};

void sku_top10()
{
		mysql_database mysql_os;
		mysql_os.m_mysql_ip=get_config->m_mysql_ip;
		mysql_os.m_mysql_port=get_config->m_mysql_port;
		mysql_os.m_mysql_username=get_config->m_mysql_username;
		mysql_os.m_mysql_password=get_config->m_mysql_password;
		mysql_os.m_mysql_database=get_config->m_mysql_database;

		mysql_database mysql_js;
		mysql_js.m_mysql_ip=get_config->m_mysql_js_ip;
		mysql_js.m_mysql_port=get_config->m_mysql_js_port;
		mysql_js.m_mysql_username=get_config->m_mysql_js_username;
		mysql_js.m_mysql_password=get_config->m_mysql_js_password;
		mysql_js.m_mysql_database=get_config->m_mysql_js_database;

		mysql_database mysql_eu;
		mysql_eu.m_mysql_ip=get_config->m_mysql_eu_ip;
		mysql_eu.m_mysql_port=get_config->m_mysql_eu_port;
		mysql_eu.m_mysql_username=get_config->m_mysql_eu_username;
		mysql_eu.m_mysql_password=get_config->m_mysql_eu_password;
		mysql_eu.m_mysql_database=get_config->m_mysql_eu_database;

		mysql_database mysql_as;
		mysql_as.m_mysql_ip=get_config->m_mysql_as_ip;
		mysql_as.m_mysql_port=get_config->m_mysql_as_port;
		mysql_as.m_mysql_username=get_config->m_mysql_as_username;
		mysql_as.m_mysql_password=get_config->m_mysql_as_password;
		mysql_as.m_mysql_database=get_config->m_mysql_as_database;

		boost::shared_ptr<cskutop> pos(new cskutop(mysql_os));
		boost::shared_ptr<cskutop> pjs(new cskutop(mysql_js));
		boost::shared_ptr<cskutop> peu(new cskutop(mysql_eu));
		boost::shared_ptr<cskutop> pas(new cskutop(mysql_as));
		
		thread pos_Thread([&pos](){pos->start();});
		thread pjs_Thread([&pjs](){pjs->start();});
		thread peu_Thread([&peu](){peu->start();});
		thread pas_Thread([&pas](){pas->start();});
		pos_Thread.join();
		pjs_Thread.join();
		peu_Thread.join();
		pas_Thread.join();
}

