#pragma once
#include <random>
#include  "include.hpp"
string rand_string(int len)
{
	string choice="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_y(0,35);  
	auto dice_y= std::bind(dis_y,gen);
    for(int i=0;i<len;++i)
    {
    	ret+=choice[dice_y()];
    }
    //cout<<ret<<endl;
   return ret;
}
class cskutop:public boost::enable_shared_from_this<cskutop>
{
	mysql_database m_mysql_database;
public:
	cskutop(mysql_database mysql_input):m_d_t(m_io_s),m_mysql_database(mysql_input),m_conn(nullptr)
	{
	}
private:
	bool get_sales_order(const string& company_id)
	{
		try
		{
			string start_time;
			string end_time;
			string accounting_period_id;
			if(!get_accounting_period_tuple(company_id,accounting_period_id,start_time,end_time))
			{
				return false;
			}
			string query_sql = "select sales_order_id,company_id from "+m_mysql_database.m_mysql_database + ".t_sales_order where order_date>'"+start_time+"' and status=4 and dr=0 and order_date<'"+end_time+"' and company_id='"+company_id+"'";
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
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	void item_master_sum()
	{   
		//company_id //sales_order_id 
	    //item_master_id //unit_price 
		//uom_id //quantity

		//company_id item_master_id quantity 
		map<string,int> item_master_quantity;
		for(const auto& i:m_all)
		{
			for(const auto& j:i.second)
			{
				string item_master_id=*(std::get<1>(j));
				int quantity=*(std::get<4>(j));
				//cout<<i.first<<":"<<<<<<endl;
				if(item_master_quantity.find(item_master_id)!=item_master_quantity.end())
				{
					item_master_quantity[item_master_id]+=quantity;
				}
				else
				{
					item_master_quantity[item_master_id]=quantity;
				}
			}
			sort_item_master(i.first,item_master_quantity);
			item_master_quantity.clear();
		}
	}
	void sort_item_master(const string& company_id,const map<string,int>& item_master_quantity)
	{
		std::vector<pair<string,int>> item_master_vector;
		
		for(const auto& i:item_master_quantity)
		{
			//cout<<i.first<<":"<<i.second<<endl;
			item_master_vector.push_back(pair<string,int>(i.first,i.second));
		}
		
		sort(item_master_vector.begin(),item_master_vector.end(),cmp_by_value());
		if(item_master_vector.size()>10)
		{
			item_master_vector.erase(item_master_vector.begin()+10,item_master_vector.end());
		}
		m_all_sorted[company_id]=item_master_vector;
	}
	void get_sales_order_detail()
	{
		for(const auto& i : m_sales_order_vector)
		{
			get_sales_order_detail(*(std::get<0>(i)),*(std::get<1>(i))) ;
			//cout<<m_sales_order_detail_vector.size()<<endl;
			
			// update_sales_statistics(*(std::get<1>(i)));
			// update_sales_statistics_detail();
			//m_sales_order_detail_vector.clear();
		}
		item_master_sum();
		
	}
	bool get_sales_order_detail(
		const string& sales_order_id,
		const string& company_id)
	{
		// std::cout<<sales_order_id<<":"<<company_id<<std::endl;
		try
		{
			string query_sql = "select sales_order_id,item_master_id,unit_price,uom_id,quantity,sales_order_detail_id from "+m_mysql_database.m_mysql_database + ".t_sales_order_detail where sales_order_id='"+sales_order_id+"' and dr=0";
			//cout << query_sql << endl;
			m_conn->runQuery(&m_all[company_id], query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			//boost_log->get_initsink()->flush();
			
			if(m_all[company_id].empty())
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
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	string get_product_category_id(const string& item_master_id,const string& company_id)
	{
		typedef tuple<
			unique_ptr<string>
			>anyonestring;
		std::vector<anyonestring> one_string_vector;
		string query_string="select item_basic_id from t_item_master where item_master_id='"+item_master_id+"' and company_id='"+company_id+"'";
		m_conn->runQuery(&one_string_vector, query_string.c_str());
		if(one_string_vector.size()>0)
		{
			query_string="select item_category_id from t_item_basic where item_basic_id='"+*(std::get<0>(one_string_vector[0]))+"'";
			one_string_vector.clear();
			m_conn->runQuery(&one_string_vector, query_string.c_str());
			if(one_string_vector.size()>0)
			{
				query_string="select product_category_id from t_product_category_item_category_link where item_category_id='"+*(std::get<0>(one_string_vector[0]))+"'";
				one_string_vector.clear();
				m_conn->runQuery(&one_string_vector, query_string.c_str());
				if(one_string_vector.size()>0)
				{
					return *(std::get<0>(one_string_vector[0]));
				}
			}
		}
		return "";
	}
	void update_sales_statistics()
	{
		for(const auto& i:m_all_sorted)
		{
			// for(const auto& j:i.second)
			// {
			// 	cout<<i.first<<":"<<j.first<<":"<<j.second<<endl;
			// }
			update_sales_statistics(i.first);
		}
	}
	string get_sales_uom_id(const string& company_id,const string& item_master_id)
	{
		for(const auto& i:m_all[company_id])
		{
			if(item_master_id==*(std::get<1>(i)))
			{
				return *(std::get<3>(i));
			}
		}
		return "";
	}
	bool is_exist(const string& company_id,const string& item_master_id,string& ret)
	{
		try
		{
			typedef tuple<string> userTuple;
		    vector<userTuple> users;
			string query_sql = "select sales_statistics_id from "+m_mysql_database.m_mysql_database + ".t_sales_statistics where company_id='"+company_id+"' and item_master_id='"+item_master_id+"'";
			//cout << query_sql << endl;
			m_conn->runQuery(&users, query_sql.c_str());

			if(users.empty())
			{
				return false;
			}
			ret=std::get<0>(users[0]);
			// *(std::get<0>(one_string_vector[0]))
			return true;
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	bool get_accounting_period()
	{
		try
		{

			vector<accounting_period> m_accounting_period;
			ptime now = second_clock::local_time();
			string p4 = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());

			string query_sql = "select accounting_period_id,code,company_id,opening_date,ending_date from "+m_mysql_database.m_mysql_database + ".t_accounting_period where '"+ p4+"'<ending_date and '"+p4+"' >opening_date";
			cout << query_sql << ":"<<__LINE__<<endl;
			m_conn->runQuery(&m_accounting_period, query_sql.c_str());

			if(m_accounting_period.empty())
			{
				return false;
			}
	// 		typedef tuple<
	// 		unique_ptr<string>, //accounting_period_id
	// 		unique_ptr<string>, //code
	// 		unique_ptr<string>, //company_id
	// 		unique_ptr<string>, //opening_date
	// 		unique_ptr<string> //ending_date
	// 		>accounting_period;
	// vector<accounting_period> m_accounting_period;
	// std::map<string,accounting_period> m_accounting_periods;//key is company_id
			for(const auto& i:m_accounting_period)
			{
				string company_id=std::get<2>(i);
				m_accounting_periods[company_id]=i;
			}
			//string ret=std::get<0>(users[0]);
			// *(std::get<0>(one_string_vector[0]))
			// return std::get<0>(m_accounting_period[0]);
			return true;
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	bool get_accounting_period_tuple(
		const string& company_id,
		string& accounting_period_id,
		string& start_time,
		string& end_time)
	{
		if(m_accounting_periods.find(company_id) != m_accounting_periods.end())
		{
			accounting_period_id=std::get<0>(m_accounting_periods[company_id]);
			start_time=std::get<3>(m_accounting_periods[company_id]);
			end_time=std::get<4>(m_accounting_periods[company_id]);
			return true;
		}
		else
		{
			accounting_period_id="data_gw";
			ptime now = second_clock::local_time();
			string p4 = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());
			start_time=p4.substr(0,7)+"-01 00:00:00";
			end_time=p4.substr(0,7)+"-31 23:59:59";
			return false;
		}
	}
	void update_sales_statistics(const string& company_id)
	{
		try
		{
			// typedef tuple<
			// unique_ptr<string>, //sales_order_id
			// unique_ptr<string>, //item_master_id
			// unique_ptr<string>, //unit_price
			// unique_ptr<string>, //uom_id
			// unique_ptr<int>, //quantity
			// unique_ptr<string>, //sales_order_detail_id
			// >m_sales_order_detail;
			// std::map<string,std::vector<m_sales_order_detail>> m_all;//key is company_id
			// std::map<string,vector<pair<string,int>>> m_all_sorted;//company_id item_master_id quantity
			// 		typedef tuple<
	// 		unique_ptr<string>, //accounting_period_id
	// 		unique_ptr<string>, //code
	// 		unique_ptr<string>, //company_id
	// 		unique_ptr<string>, //opening_date
	// 		unique_ptr<string> //ending_date
	// 		>accounting_period;
	// vector<accounting_period> m_accounting_period;
	// std::map<string,accounting_period> m_accounting_periods;//key is company_id
			vector<pair<string,int>> sorted=m_all_sorted[company_id];
			string start_time;
			string end_time;
			string accounting_period_id;
			get_accounting_period_tuple(company_id,accounting_period_id,start_time,end_time);
			
			//vector<m_sales_order_detail> sorted_detail=m_all[company_id];
			int sort_no=1;
			for(const auto& i:sorted)
			{
				string product_category_id=get_product_category_id(i.first,company_id);
				//cout<<product_category_id<<":"<<__LINE__<<endl;
				if(product_category_id.empty())
					continue;
				string sales_uom_id=get_sales_uom_id(company_id,i.first);
				ptime now = second_clock::local_time();
				string p4 = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());
				// string start_time=p4.substr(0,4)+"-01-01 00:00:00";
				string insert_sql,insert_statistics_detail;
				string sales_statistics_id;

				// string accounting_year=get_accounting_year(company_id,);
				// if(accounting_year.length()==0)
				//{
					string accounting_year=p4.substr(0,7);
				//}

				if(is_exist(company_id,i.first,sales_statistics_id))//存在返回sales_statistics_id
				{
					//cout<<"sales_statistics_id:"<<sales_statistics_id<<endl;
					insert_sql = "update t_sales_statistics set updateBy='data_exchange_gw',updateAt='"+p4+"',total_quantity_sold="+boost::lexical_cast<std::string>(i.second)+",sort_no="+boost::lexical_cast<std::string>(sort_no)+" where company_id='"+company_id+"' and item_master_id='"+i.first+"'";
					// insert_statistics_detail="insert into t_sales_statistics_detail(sales_statistics_detail_id,sales_statistics_id,sales_order_id,sales_order_quantity,unit_price,sales_id,owner_sales_id,customer_master_id,createAt,createBy,dr,data_version)values('"+sales_statistics_detail_id+"','"+sales_statistics_id+"','"+company_id+"','"+product_category_id+"','"+i.first+"',"+boost::lexical_cast<std::string>(i.second)+",'"+sales_uom_id+"','"+start_time+"','"+p4+"','"+p4.substr(0,4)+"',"+boost::lexical_cast<std::string>(sort_no)+",'"+p4+"','data_exchange_gw',0,1)";
				}
				else
				{
					sales_statistics_id=rand_string(20);
					
					insert_sql = "insert into t_sales_statistics(sales_statistics_id,company_id,product_category_id,item_master_id,total_quantity_sold,sales_uom_id,statistic_beginning_date,statistic_ending_date,accounting_period_id,sort_no,createAt,createBy,dr,data_version)values('"+sales_statistics_id+"','"+company_id+"','"+product_category_id+"','"+i.first+"',"+boost::lexical_cast<std::string>(i.second)+",'"+sales_uom_id+"','"+start_time+"','"+end_time+"','"+accounting_period_id+"',"+boost::lexical_cast<std::string>(sort_no)+",'"+p4+"','data_exchange_gw',0,1)";

					//insert_statistics_detail= "insert into t_sales_statistics_detail(sales_statistics_detail_id,sales_statistics_id,sales_order_id,sales_order_quantity,unit_price,sales_id,owner_sales_id,customer_master_id,createAt,createBy,dr,data_version)values('"+sales_statistics_detail_id+"','"+sales_statistics_id+"','"+company_id+"','"+product_category_id+"','"+i.first+"',"+boost::lexical_cast<std::string>(i.second)+",'"+sales_uom_id+"','"+start_time+"','"+p4+"','"+p4.substr(0,4)+"',"+boost::lexical_cast<std::string>(sort_no)+",'"+p4+"','data_exchange_gw',0,1)";
				}
				
				// cout<<insert_sql<<__LINE__<<endl;
				m_conn->runCommand(insert_sql.c_str());
				update_sales_statistics_detail(company_id,sales_statistics_id,i.first);
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) <<insert_sql<<":"<<__FILE__<<":"<<__LINE__;
				//boost_log->get_initsink()->flush();
				++sort_no;
			}
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
		}
	}
	string get_sales_id(const string& sales_order_id)
	{
		try
		{
			typedef tuple<string> userTuple;
		    vector<userTuple> users;
			string query_sql = "select sales_id from "+m_mysql_database.m_mysql_database + ".t_sales_order where sales_order_id='"+sales_order_id+"'";
			//cout << query_sql << endl;
			m_conn->runQuery(&users, query_sql.c_str());

			if(users.empty())
			{
				return "";
			}
			return std::get<0>(users[0]);
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return "";
		}
	}
	string get_owner_sales_id(const string& sales_id)
	{
		try
		{
			typedef tuple<string> userTuple;
		    vector<userTuple> users;
			string query_sql = "select employee_no from "+m_mysql_database.m_mysql_database + ".t_system_account where system_account_id='"+sales_id+"'";
			//cout << query_sql << endl;
			m_conn->runQuery(&users, query_sql.c_str());

			if(users.empty())
			{
				return "";
			}
			return std::get<0>(users[0]);
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return "";
		}
	}
	string get_customer_master_id(const string& sales_order_id)
	{
		try
		{
			typedef tuple<string> userTuple;
		    vector<userTuple> users;
			string query_sql = "select customer_master_id from "+m_mysql_database.m_mysql_database + ".t_sales_order where sales_order_id='"+sales_order_id+"'";
			//cout << query_sql << endl;
			m_conn->runQuery(&users, query_sql.c_str());

			if(users.empty())
			{
				return "";
			}
			return std::get<0>(users[0]);
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return "";
		}
	}
	bool is_exist_sales_order_id(
		const string& sales_order_id,
		const string& quantity,
		const string& sales_statistics_id)
	{
		try
		{
			typedef tuple<string> userTuple;
		    vector<userTuple> users;
			string query_sql = "select sales_order_id from "+m_mysql_database.m_mysql_database + ".t_sales_statistics_detail where sales_order_id='"+sales_order_id+"' and sales_order_quantity="+quantity+" and sales_statistics_id='"+sales_statistics_id+"'";
			cout << query_sql <<":"<<__LINE__<< endl;
			m_conn->runQuery(&users, query_sql.c_str());

			if(users.empty())
			{
				return false;
			}
			return true;
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	void update_sales_statistics_detail(
		const string& company_id,
		const string& sales_statistics_id,
		const string& item_master_id)
	{
		// typedef tuple<
			// unique_ptr<string>, //sales_order_id
			// unique_ptr<string>, //item_master_id
			// unique_ptr<string>, //unit_price
			// unique_ptr<string>, //uom_id
			// unique_ptr<int>, //quantity
			// unique_ptr<string>, //sales_order_detail_id
			// >m_sales_order_detail;
			// std::map<string,std::vector<m_sales_order_detail>> m_all;//key is company_id
		
		string insert_statistics_detail;
		ptime now = second_clock::local_time();
		string p4 = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());
		for(const auto& i:m_all[company_id])
		{
			string sales_statistics_detail_id=rand_string(20);
			if(*(std::get<1>(i))==item_master_id)
			{
				string sales_order_id=*(std::get<0>(i));
				string unit_price=*(std::get<2>(i));
				string sales_order_quantity=boost::lexical_cast<std::string>(*(std::get<4>(i)));

				if(is_exist_sales_order_id(sales_order_id,sales_order_quantity,sales_statistics_id))
					continue;

				string sales_id=get_sales_id(sales_order_id);
				// string owner_sales_id=get_owner_sales_id(sales_id);
				string owner_sales_id=sales_id;
				string customer_master_id=get_customer_master_id(sales_order_id);
				
				insert_statistics_detail= "insert into t_sales_statistics_detail(sales_statistics_detail_id,sales_statistics_id,sales_order_id,sales_order_quantity,unit_price,sales_id,owner_sales_id,customer_master_id,createAt,createBy,dr,data_version)values('"+sales_statistics_detail_id+"','"+sales_statistics_id+"','"+sales_order_id+"',"+sales_order_quantity+","+unit_price+",'"+sales_id+"','"+owner_sales_id+"','"+customer_master_id+"','"+p4+"','data_exchange_gw',0,1)";
				cout<<insert_statistics_detail<<":"<<__LINE__<<endl;
				m_conn->runCommand(insert_statistics_detail.c_str());
			}
		}
		
	}
public:	
	void start()
	{
		m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_sku_top10_request_interval));
		  
	    m_d_t.async_wait(boost::bind(&cskutop::handle_wait, shared_from_this(), boost::asio::placeholders::error));  
		m_io_s.run();
	}
private:
	void get_company_id(vector<tuple<string>>& company_ids)
	{
		try
		{
			string query_sql = "select DISTINCT company_id from "+m_mysql_database.m_mysql_database + ".t_sales_order";
			cout << query_sql <<":"<<__LINE__<< endl;
			m_conn->runQuery(&company_ids, query_sql.c_str());
		}
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
		}
	}
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
	    	get_accounting_period();
	    	vector<tuple<string>> company_ids;
	    	get_company_id(company_ids);
	    	for(const auto& i:company_ids)
	    	{
	    		string company_id=std::get<0>(i);
	    		if(get_sales_order(company_id))
				{
					get_sales_order_detail();
					update_sales_statistics();
				}
				m_sales_order_vector.clear();
	    	}
			m_accounting_periods.clear();
			m_all_sorted.clear();
			m_all.clear();
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
    	//cout<<"handle_wait "<<__FILE__<<":"<<__LINE__<<endl;
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
	boost::asio::io_service m_io_s;  
	deadline_timer m_d_t;
	typedef tuple<
			unique_ptr<string>, //sales_order_id
			unique_ptr<string> //company_id
			>m_sales_order;
	std::vector<m_sales_order> m_sales_order_vector;
	typedef tuple<
			unique_ptr<string>, //sales_order_id
			unique_ptr<string>, //item_master_id
			unique_ptr<string>, //unit_price
			unique_ptr<string>, //uom_id
			unique_ptr<int>, //quantity
			unique_ptr<string>//sales_order_detail_id
			>m_sales_order_detail;
	
  	struct cmp_by_value
  	{
  		bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs) 
  		{  
		  return lhs.second > rhs.second;  
		}
  	};
	std::map<string,std::vector<m_sales_order_detail>> m_all;//key is company_id
	std::map<string,vector<pair<string,int>>> m_all_sorted;//company_id item_master_id quantity
	
	typedef tuple<
			string, //accounting_period_id
			string, //code
			string, //company_id
			string, //opening_date
			string //ending_date
			>accounting_period;
	std::map<string,accounting_period> m_accounting_periods;//key is company_id
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

