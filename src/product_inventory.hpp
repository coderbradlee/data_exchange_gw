#ifndef PRODUCT_INVENTORY_HPP
#define	PRODUCT_INVENTORY_HPP

#include  "include.hpp"
//#define DEBUG
#include "mysql/mysql_api.hpp"
#include "orderbot.hpp"
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
		boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
		order->request("GET", "/admin/products.json/", "", "");

		m_product_all=order->m_data;
		//cout<<*(order->m_data)<<":"<<__FILE__<<":"<<__LINE__<<endl;
	}
	void start_update()
	{
		try
		{
			get_product_all();
			//cout<<*m_product_all<<":"<<__FILE__<<":"<<__LINE__<<endl;
			//
			 ptree pt;
			 std::istringstream is(*m_product_all);
			read_json(is, pt);
			for(auto& sub:pt)
			{

	   			string product_category_id=sub.second.get<string>("product_category_id");
				int product_id=sub.second.get<int>("product_id");
				string product_name=sub.second.get<string>("product_name");
				string sku=sub.second.get<string>("sku");

				cout<<product_name<<":"<<sku<<endl;
				
				ptree child = sub.second.get_child("inventory_quantities");
				for(auto& subchild:child)
				{
					int distribution_center_id=subchild.second.get<int>("distribution_center_id");
					string distribution_center_name=subchild.second.get<string>("distribution_center_name");
					int inventory_quantity=subchild.second.get<int>("inventory_quantity");
					cout<<distribution_center_name<<":"<<inventory_quantity<<endl;
				}
			}
			

			// typedef tuple<unique_ptr<string>, unique_ptr<double> ,unique_ptr<string>> credit_tuple;
			
			// //typedef tuple<string,double> credit_tuple;
			// vector<credit_tuple> credits;
			// string query_sql = "SELECT customer_credit_flow_id,balance,customer_master_id FROM " + get_config->m_mysql_database + "." + get_config->m_mysql_table + " where expire_date='" + m_today_string + "' and balance>0 and dr=0 and transaction_type=0";
			// cout << query_sql << endl;
			// m_conn->runQuery(&credits, query_sql.c_str());

			// BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			// boost_log->get_initsink()->flush();
			// /********************************/
			// cout.setf(ios::showpoint); cout.setf(ios::fixed); cout.precision(8);
			// /********************************/
			// if(credits.empty())
			// {
			// 	BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "nothing need to update";
			// 	boost_log->get_initsink()->flush();
			// 	cout<<"nothing need to update"<<endl;
			// }
			// for (const auto& item : credits)
			// {
			// 	cout << item << endl;

			// 	string update_sql = "update " + get_config->m_mysql_database + "." + get_config->m_mysql_table + " set balance=0 where customer_credit_flow_id='" + *(std::get<0>(item))+"'";
			// 	cout << update_sql << endl;
			// 	string update_sql2;
			// 	try
			// 	{
			// 		m_conn->runCommand(update_sql.c_str());
			// 		//¸üÐÂÁíÒ»¸ö±í
			// 		update_sql2 = "update " + get_config->m_mysql_database + "." + get_config->m_mysql_table2 + " set credit_balance=0 where customer_master_id='" + *(std::get<2>(item))+"'";
			// 		cout << update_sql2 << endl;
			// 		m_conn->runCommand(update_sql2.c_str());
					
			// 		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << update_sql;
			// 		BOOST_LOG_SEV(slg, boost_log->get_log_level()) << update_sql2;
			// 		boost_log->get_initsink()->flush();
			// 	}
			// 	catch (const MySqlException& e)
			// 	{
			// 		BOOST_LOG_SEV(slg, severity_level::error) << "(1)" << update_sql << "(2)" << update_sql2 << "(exception:)" << e.what();
			// 		boost_log->get_initsink()->flush();
			// 	}
				

				
			// }

			// credits.clear();
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
private:
	boost::shared_ptr<MySql> m_conn;
	string m_today_string;
	boost::shared_ptr<string> m_product_all;
};

#endif
