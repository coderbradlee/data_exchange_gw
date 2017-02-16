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
#include "scm_data.hpp"
namespace scm_namespace
{

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
				"city_name,"
				"province,"
			    "address,"
			    "zip_code,"
			    "tel,"
			    "fax,"
			    "delivery_terms,"
			    "payment_terms,"
			    "trade_term_id,"
			    "billing_company_name,"
			    "billing_address,"
			    "billing_bank_name,"
			    "billing_bank_account_no,"
			    "note,"
			    "owner_purchaser_id "
				"from t_supplier_basic where dr=0";
			cout << query_sql << endl;
			query(query_sql);
			while(m_res->next())
			{
			 	supplier_basic temp;
			 	temp.supplier_id=m_res->getString("supplier_id");
			 	temp.supplier_no=m_res->getString("supplier_no");
			 	temp.company_name_en=m_res->getString("company_name_en");
			 	temp.status=m_res->getString("status");
			 	temp.country_id=m_res->getString("country_id");
			 	temp.city_name=m_res->getString("city_name");
			 	//////////////////////////////
			 	temp.province=m_res->getString("province");
			 	temp.address=m_res->getString("address");
			 	temp.zip_code=m_res->getString("zip_code");
			 	temp.tel=m_res->getString("tel");
			 	temp.fax=m_res->getString("fax");
			 	temp.delivery_terms=m_res->getString("delivery_terms");
			 	////////////////////////////////
			 	temp.payment_terms=m_res->getString("payment_terms");
			 	temp.trade_term_id=m_res->getString("trade_term_id");
			 	temp.billing_company_name=m_res->getString("billing_company_name");
			 	temp.billing_address=m_res->getString("billing_address");
			 	temp.billing_bank_name=m_res->getString("billing_bank_name");
			 	temp.billing_bank_account_no=m_res->getString("billing_bank_account_no");
			 	////////////////////////////
			 	temp.note=m_res->getString("note");
			 	temp.owner_purchaser_id=m_res->getString("owner_purchaser_id");

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
		for(auto& i:*m_supplier_basic)
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

