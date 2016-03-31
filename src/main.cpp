//#include "include.hpp"
#include "config.hpp"
#include "serverResource.hpp"
#include <boost/asio/yield.hpp>
#include <boost/asio/coroutine.hpp>
#include "redis/redis_source.hpp"
#include "mysql/mysql_api.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>   
#include <boost/any.hpp>
#include "orderbot.hpp"
#define BOOST_DATE_TIME_SOURCE   


using namespace boost::asio;
using namespace std;
int main() {
	try
	{
	
			try
			{

				MySql conn(get_config->m_mysql_ip.c_str(), get_config->m_mysql_username.c_str(), get_config->m_mysql_password.c_str(), get_config->m_mysql_database.c_str(), get_config->m_mysql_port);

				//`customer_credit_flow_id` char(20) NOT NULL COMMENT ' /*Ö÷¼ü*/',
				//`company_id` char(20) NOT NULL COMMENT ' /*¹«Ë¾id*/',
				//`customer_master_id` char(20) NOT NULL COMMENT '/*¿Í»§¹ÜÀíµµ°¸id*/',
				//`transaction_no` varchar(128) NOT NULL COMMENT '/*½»Ò×Á÷Ë®ºÅ*/',
				//`transaction_type` tinyint(4) NOT NULL COMMENT ' /*½»Ò×ÀàÐÍ, 0:ÐÅÓÃ¿ª»§; 1:ÐÅÓÃÏú»§; 2:ÐÅÓÃÖ§³ö; 3:ÐÅÓÃ´æÈë*/',
				//`insurer_id` char(20) DEFAULT NULL COMMENT ' /*±£ÏÕÉÌid*/',
				//`effective_date` char(19) DEFAULT NULL COMMENT '/*ÉúÐ§ÈÕÆÚ*/',
				//`expire_date` char(19) DEFAULT NULL COMMENT ' /*Ê§Ð§ÈÕÆÚ*/',
				//`transaction_amount` decimal(20, 8) NOT NULL COMMENT ' /*½»Ò×½ð¶î*/',
				//`credit_days` smallint(6) DEFAULT NULL COMMENT '/*ÕÊÆÚ, µ±transaction_typeÎª¡¯0¡¯Ê±ÌîÐ´*/',
				//`invoice_no` varchar(128) DEFAULT NULL COMMENT '/*·¢Æ±±àÂë*/',
				//`ar_voucher_no` varchar(128) DEFAULT NULL COMMENT ' /*ÊÕ¿îÆ¾Ö¤±àÂë*/',
				//`ar_accountant_id` char(20) DEFAULT NULL COMMENT ' /*ÊÕ¿î»á¼Æid*/',
				//`balance` decimal(20, 8) NOT NULL COMMENT '/*ÐÅÓÃÓà¶î*/',
				//`transaction_date` char(19) NOT NULL COMMENT ' /*½»Ò×ÈÕÆÚ*/',
				//`note` text COMMENT ' /*±¸×¢*/',
				//`createAt` char(19) NOT NULL COMMENT '/*´´½¨Ê±¼ä, YYYY-MM-DD HH:MM:SS*/',
				//`createBy` char(20) DEFAULT NULL COMMENT '/*´´½¨ÈË*/',
				//`updateAt` char(19) DEFAULT NULL COMMENT '/*×î½üÐÞ¸ÄÊ±¼ä, YYYY-MM-DD HH:MM:SS*/',
				//`updateBy` char(20) DEFAULT NULL COMMENT '/*ÐÞ¸ÄÈË*/',
				//`deleteAt` char(19) DEFAULT NULL COMMENT '/*É¾³ýÊ±¼ä, YYYY-MM-DD HH:MM:SS*/',
				//`deleteBy` char(20) DEFAULT NULL COMMENT '/*É¾³ýÈË*/',
				//`dr` smallint(2) NOT NULL DEFAULT '0' COMMENT '/*Âß¼­É¾³ý±êÖ¾ 0:Î´É¾³ý;1:ÒÑÉ¾³ý*/',
				//`data_version` int(11) NOT NULL DEFAULT '1' COMMENT '/*°æ±¾ºÅ, ´Ó0¿ªÊ¼*/',

				/*std::string strTime = boost::posix_time::to_iso_string(\
					boost::posix_time::second_clock::local_time());
				int pos = strTime.find('T');
				strTime.replace(pos, 1, std::string("-"));
				strTime.replace(pos + 3, 0, std::string(":"));
				strTime.replace(pos + 6, 0, std::string(":"));

				std::cout << strTime << std::endl;*/

				boost::gregorian::date today = boost::gregorian::day_clock::local_day();
				std::string str1(to_iso_extended_string(today));
				std::cout << str1 << std::endl;

				//typedef tuple<unique_ptr<string, string, string, string, double, string, string, string, double, double, string, string, string, double, string, string, string, string, string, string, string, string, double, double> credit_tuple;
				//typedef tuple<unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<double>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<double>, unique_ptr<double>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<double>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<string>, unique_ptr<double>, unique_ptr<double>> credit_tuple;
				//vector<credit_tuple> credits;//2099-12-31
				//str1 = "2099-12-31";
				//string query_sql = "SELECT * FROM " + database+"."+table + " where expire_date='" + str1 + "'";
				////cout << query_sql << endl;
				//conn.runQuery(&credits, query_sql.c_str());

				///********************************/
				//cout.setf(ios::showpoint); cout.setf(ios::fixed); cout.precision(8);
				///********************************/

				//for (const auto& item : credits)
				//{
				//		cout << item << endl;
				//}

				//str1 = "2099-12-31";
				typedef tuple<unique_ptr<string>, unique_ptr<double> ,unique_ptr<string>> credit_tuple;
				
				//typedef tuple<string,double> credit_tuple;
				vector<credit_tuple> credits;
				string query_sql = "SELECT customer_credit_flow_id,balance,customer_master_id FROM " + get_config->m_mysql_database + "." + get_config->m_mysql_table + " where expire_date='" + str1 + "' and balance>0 and dr=0 and transaction_type=0";
				cout << query_sql << endl;
				conn.runQuery(&credits, query_sql.c_str());

				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
				boost_log->get_initsink()->flush();
				/********************************/
				cout.setf(ios::showpoint); cout.setf(ios::fixed); cout.precision(8);
				/********************************/

				for (const auto& item : credits)
				{
					cout << item << endl;
					//cout << std::get<0>(item) << endl;
					
					//unique_ptr<string> data=std::move((std::get<0>(item)));
					
					//UPDATE ±íÃû³Æ SET ÁÐÃû³Æ = ÐÂÖµ WHERE ÁÐÃû³Æ = Ä³Öµ
					string update_sql = "update " + get_config->m_mysql_database + "." + get_config->m_mysql_table + " set balance=0 where customer_credit_flow_id='" + *(std::get<0>(item))+"'";
					cout << update_sql << endl;
					string update_sql2;
					try
					{
						conn.runCommand(update_sql.c_str());
						//¸üÐÂÁíÒ»¸ö±í
						update_sql2 = "update " + get_config->m_mysql_database + "." + get_config->m_mysql_table2 + " set credit_balance=0 where customer_master_id='" + *(std::get<2>(item))+"'";
						cout << update_sql2 << endl;
						conn.runCommand(update_sql2.c_str());
						
						BOOST_LOG_SEV(slg, boost_log->get_log_level()) << update_sql;
						BOOST_LOG_SEV(slg, boost_log->get_log_level()) << update_sql2;
						boost_log->get_initsink()->flush();
					}
					catch (const MySqlException& e)
					{
						BOOST_LOG_SEV(slg, severity_level::error) << "(1)" << update_sql << "(2)" << update_sql2 << "(exception:)" << e.what();
						boost_log->get_initsink()->flush();
					}
					

					
				}

				//ÐèÒª¸üÐÂt_customer_master.credit_balance×Ö¶Î
				
				credits.clear();


				

			}
			catch (const MySqlException& e) 
			{
				cout << e.what() << endl;
				BOOST_LOG_SEV(slg, severity_level::error)<<"query_sql(exception:)" << e.what();
				boost_log->get_initsink()->flush();
				
			}
	
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}
		{
			boost::timer::cpu_timer pass;
			pass.start();

			//orderbot 接口
			boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			//order->request("GET", "/admin/products.json/", "class_type=sales&category_name=Rings", "");

			//cout<<order->get_data().length()<<":"<<order->get_data()<<endl;
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}

	}
	catch (std::exception& e) {
		//cout << diagnostic_information(e) << endl;
		cout << e.what() << endl;
        }
	catch(...) 
	{
         
	}
    return 0;
}