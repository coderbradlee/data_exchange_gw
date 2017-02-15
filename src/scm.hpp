#ifndef SCM_HPP
#define	SCM_HPP

#include  "include.hpp"
#include "mysql_connection.h"
#include "config.hpp"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
namespace scm_namespace
{
string rand_strings(int len)
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
class exchage_rate_data
{
public:
	string code;//CAD
	string to_usd_exchange_rate;//0.772558
	string from_usd_exchange_rate;//0.772558
	string currency_id;//J4YVQ3USQNO3U430EKE1
	string to_usd_exchange_rate_id;//TFTBLZNSNBNAZAZGC2RW
	string from_usd_exchange_rate_id;//TFTBLZNSNBNAZAZGC2RW
	void print()
	{	cout<<"{";
		cout<<code<<":";
		cout<<to_usd_exchange_rate<<":";//0.772558
		cout<<from_usd_exchange_rate<<":";//0.772558
		cout<<currency_id<<":";//J4YVQ3USQNO3U430EKE1
		cout<<to_usd_exchange_rate_id<<":";//TFTBLZNSNBNAZAZGC2RW
		cout<<from_usd_exchange_rate_id<<"}";//TFTBLZNSNBNAZAZGC2RW
		cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
	}
};
class general_rate_data
{
public:
	string first_code;
	string second_code;
	string first_currency_id;
	string second_currency_id;
	string first_second_currency_exchange_rate_id;
	string second_first_currency_exchange_rate_id;
	string first_second_exchange_rate;
	void print()
	{
		cout<<"{"<<first_code<<":"<<second_code<<":"<<first_second_currency_exchange_rate_id<<":"<<second_first_currency_exchange_rate_id<<":"<<first_second_exchange_rate<<"}"<<endl;
	}
};
class mysql_info_
{
public:
	string m_mysql_ip;
	unsigned short m_mysql_port;
	string m_mysql_username;
	string m_mysql_password;
	string m_mysql_database;
};

class scm_supplier_rest
{
public:
	scm_supplier_rest(boost::shared_ptr<mysql_info_> in)
	{
		m_driver = get_driver_instance();
		m_con = boost::shared_ptr<sql::Connection>(m_driver->connect("tcp://"+in->ip+":"+in->port, in->username, in->password));
		
		m_con->setSchema(in->database);
	}
	string get_vendor()
	{
		return get_vendor_to_myql();
	}
	void update_vendor()
	{	
		update_vendor_to_mysql();	
	}
	void insert_vendor()
	{
		insert_vendor_to_mysql();
	}
private:
	string get_vendor_to_myql()
	{
		return "get_vendor_to_myql";
	}
	void update_vendor_to_mysql()
	{	
		std::cout<<"update_vendor_to_mysql"<<std::endl;
	}
	void insert_vendor_to_mysql()
	{
		std::cout<<"insert_vendor_to_mysql"<<std::endl;
	}
private:
	boost::shared_ptr<sql::ResultSet> m_res;
	boost::shared_ptr<sql::Statement> m_stmt;
	boost::shared_ptr<sql::PreparedStatement> m_pstmt;
	boost::shared_ptr<sql::Driver> m_drivers;
	boost::shared_ptr<sql::Connection> m_con;

	sql::Driver* m_driver;
};
}
#endif

