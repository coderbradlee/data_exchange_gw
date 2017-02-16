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
struct supplier_basic
{
public:
	string supplier_id;//CAD
	string supplier_no;//0.772558
	string company_name_en;//0.772558
	string status;//J4YVQ3USQNO3U430EKE1
	string country_id;//TFTBLZNSNBNAZAZGC2RW
	string city_name;//TFTBLZNSNBNAZAZGC2RW
	void print()
	{	cout<<"{";
		cout<<supplier_id<<":";
		cout<<supplier_no<<":";
		cout<<company_name_en<<":";
		cout<<status<<":";
		cout<<country_id<<":";
		cout<<city_name<<"}";
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
	string ip;
	string port;
	string username;
	string password;
	string database;
};
class scm_supplier_from
{
public:
	scm_supplier_from(boost::shared_ptr<mysql_info_> in)
	{
		m_driver = get_driver_instance();
		m_con = boost::shared_ptr<sql::Connection>(m_driver->connect("tcp://"+in->ip+":"+in->port, in->username, in->password));
		
		m_con->setSchema(in->database);
	}
	void get_vendor(boost::shared_ptr<std::vector<supplier_basic>> v)
	{
		get_vendor_to_myql(v);
	}
	
private:
	void get_vendor_to_myql(boost::shared_ptr<std::vector<supplier_basic>> v)
	{
		get_supplier_basic(v);
	}
	
	bool get_supplier_basic(boost::shared_ptr<std::vector<supplier_basic>> v)
	{
		try
		{
			string query_sql = "select " 
				"supplier_id,"
				"supplier_no,"
				"company_name_en,"
				"status,"
				"country_id,"
				"city_name "
				"from t_supplier_basic where dr=0";
			cout << query_sql << endl;
			query(query_sql);
			while(m_res->next())
			{
			 	boost::shared_ptr<supplier_basic> temp(new supplier_basic());
			 	temp->supplier_id=m_res->getString("supplier_id");
			 	temp->supplier_no=m_res->getString("supplier_no");
			 	temp->company_name_en=m_res->getString("company_name_en");
			 	temp->status=m_res->getString("status");
			 	temp->country_id=m_res->getString("country_id");
			 	temp->city_name=m_res->getString("city_name");
			 	v->push_back(temp);
			 }
			return true;
		}
		catch (sql::SQLException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			cout<<e.what()<<":"<<__LINE__<<":"<<__FILE__<<endl;
			return false;
		}
	}
	void query(const std::string& query_sql)
	{
		try 
		{
		  //std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement(query_sql));
		  m_res = boost::shared_ptr<sql::ResultSet>(m_pstmt->executeQuery());

		} 
		catch (sql::SQLException &e) 
		{
		  BOOST_LOG_SEV(slg, severity_level::error) <<"# ERR: " << e.what();
		  BOOST_LOG_SEV(slg, severity_level::error) <<" (MySQL error code: " << e.getErrorCode();
		  BOOST_LOG_SEV(slg, severity_level::error) <<", SQLState: " << e.getSQLState();
		}
	}
private:
	boost::shared_ptr<sql::ResultSet> m_res;
	boost::shared_ptr<sql::Statement> m_stmt;
	boost::shared_ptr<sql::PreparedStatement> m_pstmt;
	boost::shared_ptr<sql::Driver> m_drivers;
	boost::shared_ptr<sql::Connection> m_con;

	sql::Driver* m_driver;
};
class scm_supplier_to
{
public:
	scm_supplier_to(boost::shared_ptr<mysql_info_> in)
	{
		m_driver = get_driver_instance();
		m_con = boost::shared_ptr<sql::Connection>(m_driver->connect("tcp://"+in->ip+":"+in->port, in->username, in->password));
		
		m_con->setSchema(in->database);
	}
	void update_vendor()
	{	
		update_vendor_to_mysql();	
	}
	
private:
	
	void update_vendor_to_mysql()
	{	
		std::cout<<"update_vendor_to_mysql"<<std::endl;
	}
	void query(const std::string& query_sql)
	{
		try 
		{
		  //std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement(query_sql));
		  m_res = boost::shared_ptr<sql::ResultSet>(m_pstmt->executeQuery());

		} 
		catch (sql::SQLException &e) 
		{
		  BOOST_LOG_SEV(slg, severity_level::error) <<"# ERR: " << e.what();
		  BOOST_LOG_SEV(slg, severity_level::error) <<" (MySQL error code: " << e.getErrorCode();
		  BOOST_LOG_SEV(slg, severity_level::error) <<", SQLState: " << e.getSQLState();
		}
	}
private:
	boost::shared_ptr<sql::ResultSet> m_res;
	boost::shared_ptr<sql::Statement> m_stmt;
	boost::shared_ptr<sql::PreparedStatement> m_pstmt;
	boost::shared_ptr<sql::Driver> m_drivers;
	boost::shared_ptr<sql::Connection> m_con;

	sql::Driver* m_driver;
};
class scm_supplier_rest
{
public:
	scm_supplier_rest(
		boost::shared_ptr<mysql_info_> from,
		boost::shared_ptr<mysql_info_> to):
	m_from_database(boost::make_shared<scm_supplier_from>(from)),
	m_to_database(boost::make_shared<scm_supplier_to>(to)),
	m_supplier_basic(boost::make_shared<std::vector<supplier_basic>>())
	{
		
	}
	void update_vendor()
	{
		update_vendor_to_myql();
	}
	
private:
	void update_vendor_to_myql()
	{
		m_from_database->get_vendor(m_supplier_basic);
		print_supplier_basic();
		m_to_database->update_vendor();
	}
	void print_supplier_basic()
	{
		for(const auto& i:*m_supplier_basic)
		{
			i.print();
		}
	}
private:
	boost::shared_ptr<scm_supplier_from> m_from_database;
	boost::shared_ptr<scm_supplier_to> m_to_database;
	boost::shared_ptr<std::vector<supplier_basic>> m_supplier_basic;
};

}
#endif

