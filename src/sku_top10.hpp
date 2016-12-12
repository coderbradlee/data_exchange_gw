#pragma once

#include  "include.hpp"
string rand_string(int len)
{
	string choice="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string ret;
    for(int i=0;i<len;++i)
    {
    	ret+=choice[rand()%62];
    }
    cout<<ret<<endl;
   return ret;
}
class cskutop:public boost::enable_shared_from_this<cskutop>
{
	mysql_database m_mysql_database;
public:
	cskutop(mysql_database mysql_input):m_d_t(m_io_s),m_product_all(nullptr),m_mysql_database(mysql_input),m_conn(nullptr)
	{
	}
private:
	bool get_sales_order()
	{
		try
		{
			string query_sql = "select sales_order_id,company_id from "+m_mysql_database.m_mysql_database + ".t_sales_order";
			cout << query_sql << endl;
			m_conn->runQuery(&m_sales_order_vector, query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			boost_log->get_initsink()->flush();
			
			if(m_sales_order_vector.empty())
			{
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "nothing select from t_currency";
				boost_log->get_initsink()->flush();
				cout<<"nothing select from t_currency"<<endl;
				return false;
			}
			return true;
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			return false;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			return false;
		}
	}
	void get_sales_order_detail()
	{
		for(const auto& i : m_sales_order_vector)
		{
			get_sales_order_detail(*(std::get<0>(i))) ;
			cout<<m_sales_order_detail_vector.size()<<endl;
			update_sales_statistics();
			update_sales_statistics_detail();
			m_sales_order_detail_vector.clear();
		}
	}
	bool get_sales_order_detail(const string& sales_order_id)
	{
		// std::cout<<sales_order_id<<":"<<company_id<<std::endl;
		try
		{
			string query_sql = "select item_master_id,unit_price,uom_id,quantity from "+m_mysql_database.m_mysql_database + ".t_sales_order_detail where sales_order_id='"+sales_order_id+"'";
			cout << query_sql << endl;
			m_conn->runQuery(&m_sales_order_detail_vector, query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			boost_log->get_initsink()->flush();
			
			if(m_sales_order_detail_vector.empty())
			{
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "nothing select from t_currency";
				boost_log->get_initsink()->flush();
				cout<<"nothing select from t_currency"<<endl;
				return false;
			}
			return true;
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
			return false;
		}
	}
	void update_sales_statistics()
	{
		try
		{
			ptime now = second_clock::local_time();
			string p4 = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());

			string insert_sql = "insert into t_sales_statistics(sales_statistics_id,company_id,product_category_id,item_master_id,total_quantity_sold,sales_uom_id,statistic_beginning_date,statistic_ending_date,accounting_year,sort_no,createAt,createBy,updateAt,updateBy,dr,data_version)values("+rand_string(20)+",'"+"source_currency_id"+"','"+"target_currency_id"+"',0,7,0,'"+p4+"','','"+p4+"','',0,1)";
			cout << insert_sql << endl;
			//m_conn->runCommand(insert_sql.c_str());
			BOOST_LOG_SEV(slg, boost_log->get_log_level()) <<insert_sql<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
		}
	}
	void update_sales_statistics_detail()
	{

	}
public:	
	void start()
	{
		m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_sku_top10_request_interval));
		  
	    m_d_t.async_wait(boost::bind(&cskutop::handle_wait, shared_from_this(), boost::asio::placeholders::error));  
		m_io_s.run();
	}
private:
	void handle_wait_method()
	{
		cout<<"handle_wait_method"<<":"<<__FILE__<<":"<<__LINE__<<endl;
		try
		{
	    	if(m_conn==nullptr)
	    	{
	    		cout<<__FILE__<<":"<<__LINE__<<endl;
	    		m_conn=boost::shared_ptr<MySql>(new MySql(m_mysql_database.m_mysql_ip.c_str(), m_mysql_database.m_mysql_username.c_str(), m_mysql_database.m_mysql_password.c_str(), m_mysql_database.m_mysql_database.c_str(), m_mysql_database.m_mysql_port));
	    		cout<<__FILE__<<":"<<__LINE__<<endl;
	    	}
			if(get_sales_order())
			{
				get_sales_order_detail();
				update_sales_statistics();
				update_sales_statistics_detail();
			}
			
			m_conn->close();
			m_conn=nullptr;
			boost::this_thread::sleep(boost::posix_time::millisec(30000));
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
	}
	void handle_wait(const boost::system::error_code& error)  
    {  
    	cout<<"handle_wait "<<__FILE__<<":"<<__LINE__<<endl;
        if(!error)  
        {  
        	ptime now = second_clock::local_time();			
			string hour_minute_second = to_simple_string(now.time_of_day());
			std::vector<std::string> hms;
 			
			boost::split(hms,hour_minute_second , boost::is_any_of(":"));
 			
 			string hour=hms[0];
 			string minute=hms[1];
 			
        	string hour_minute=hour+":"+minute;
        	//if(hour_minute==get_config->m_sku_top10_insert_time)
        	{   
        		handle_wait_method();
        	}
        	
            // m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_sku_top10_request_interval));  
            // m_d_t.async_wait(boost::bind(&cskutop::handle_wait,shared_from_this(), boost::asio::placeholders::error));                 
    	}
	}  
	

private:
	boost::shared_ptr<MySql> m_conn;
	string m_today_string;
	boost::shared_ptr<string> m_product_all;
	std::stringstream m_ss;
	boost::asio::io_service m_io_s;  
	deadline_timer m_d_t;
	typedef tuple<
			unique_ptr<string>, //sales_order_id
			unique_ptr<string> //company_id
			>m_sales_order;
	std::vector<m_sales_order> m_sales_order_vector;
	typedef tuple<
			unique_ptr<string>, //item_master_id
			unique_ptr<string>, //unit_price
			unique_ptr<string>, //uom_id
			unique_ptr<string> //quantity
			>m_sales_order_detail;
	std::vector<m_sales_order_detail> m_sales_order_detail_vector;
};

void sku_top10()
{
		// mysql_database mysql_os;
		// mysql_os.m_mysql_ip=get_config->m_mysql_ip;
		// mysql_os.m_mysql_port=get_config->m_mysql_port;
		// mysql_os.m_mysql_username=get_config->m_mysql_username;
		// mysql_os.m_mysql_password=get_config->m_mysql_password;
		// mysql_os.m_mysql_database=get_config->m_mysql_database;

		// mysql_database mysql_js;
		// mysql_js.m_mysql_ip=get_config->m_mysql_js_ip;
		// mysql_js.m_mysql_port=get_config->m_mysql_js_port;
		// mysql_js.m_mysql_username=get_config->m_mysql_js_username;
		// mysql_js.m_mysql_password=get_config->m_mysql_js_password;
		// mysql_js.m_mysql_database=get_config->m_mysql_js_database;

		// mysql_database mysql_eu;
		// mysql_eu.m_mysql_ip=get_config->m_mysql_eu_ip;
		// mysql_eu.m_mysql_port=get_config->m_mysql_eu_port;
		// mysql_eu.m_mysql_username=get_config->m_mysql_eu_username;
		// mysql_eu.m_mysql_password=get_config->m_mysql_eu_password;
		// mysql_eu.m_mysql_database=get_config->m_mysql_eu_database;

		// mysql_database mysql_as;
		// mysql_as.m_mysql_ip=get_config->m_mysql_as_ip;
		// mysql_as.m_mysql_port=get_config->m_mysql_as_port;
		// mysql_as.m_mysql_username=get_config->m_mysql_as_username;
		// mysql_as.m_mysql_password=get_config->m_mysql_as_password;
		// mysql_as.m_mysql_database=get_config->m_mysql_as_database;
		
		mysql_database mysql_eu_erp;
		mysql_eu_erp.m_mysql_ip=get_config->m_mysql_eu_erp_ip;
		mysql_eu_erp.m_mysql_port=get_config->m_mysql_eu_erp_port;
		mysql_eu_erp.m_mysql_username=get_config->m_mysql_eu_erp_username;
		mysql_eu_erp.m_mysql_password=get_config->m_mysql_eu_erp_password;
		mysql_eu_erp.m_mysql_database=get_config->m_mysql_eu_erp_database;

		// boost::shared_ptr<cskutop> pos(new cskutop(mysql_os));
		// boost::shared_ptr<cskutop> pjs(new cskutop(mysql_js));
		// boost::shared_ptr<cskutop> peu(new cskutop(mysql_eu));
		// boost::shared_ptr<cskutop> pas(new cskutop(mysql_as));
		boost::shared_ptr<cskutop> perp(new cskutop(mysql_eu_erp));
		// thread pos_Thread([&pos](){pos->start();});
		// thread pjs_Thread([&pjs](){pjs->start();});
		// thread peu_Thread([&peu](){peu->start();});
		// thread pas_Thread([&pas](){pas->start();});
		thread perp_Thread([&perp](){perp->start();});
		// pos_Thread.join();
// 		pjs_Thread.join();
// 		peu_Thread.join();
// 		pas_Thread.join();
		perp_Thread.join();
}

