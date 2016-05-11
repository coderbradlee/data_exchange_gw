#ifndef EXCHANGE_RATE_HPP
#define	EXCHANGE_RATE_HPP

#include  "include.hpp"
//#define DEBUG
//https://www.exchangerate-api.com/USD/GBP?k=d2408a508ec4f2bec554f465
class exchange_rate
{
public:
	exchange_rate(const std::string& url) : m_url(url),m_request_status(0),m_data(nullptr)
	{
		//register callback
		//register_callback();
		curl_global_init(CURL_GLOBAL_ALL);
		m_curl = curl_easy_init();
		curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
		
#ifdef DEBUG
		curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);
#endif
		//curl(m_download_url, "GET", filename, true);

		if (!share_handle)
		{
			share_handle = curl_share_init();
			curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
		}
		curl_easy_setopt(m_curl, CURLOPT_SHARE, share_handle);
		curl_easy_setopt(m_curl, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
		curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 100 );
		curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT,5);
	}
	virtual ~exchange_rate()
	{
		curl_easy_cleanup(m_curl);
		curl_global_cleanup();
	}
	void request(const std::string& method, const std::string& path, const std::string& param, const std::string& content)
	{
		/////////////////////////////////////
		BOOST_LOG_SEV(slg, boost_log->get_log_level()) <<"request:"<<method<<":"<<path<<":"<<param<<":"<<content;
		boost_log->get_initsink()->flush();
		/////////////////////////////////////////////////////
		//t_currency_exchange_rate 
		curl(path, method, param, content);
	}
	boost::shared_ptr<string> get_data()
	{

		return m_data;
	}
	long int get_status()
	{
		return m_request_status;
	}
	long int get_length()
	{
		return (*m_data).length();
	}
protected:

	static size_t request_callback(char *buffer, size_t size, size_t nmemb, void* thisPtr)
	{
		if (thisPtr)
		{
			//cout << __LINE__ << endl;
			return ((exchange_rate*)thisPtr)->request_write_data(buffer, size, nmemb);
		}

		else
		{
			//cout << __LINE__ << endl;
			return 0;
		}

	}
	size_t request_write_data(const char *buffer, size_t size, size_t nmemb)
	{
		int result = 0;
		if (buffer != 0)
		{
			//cout << __LINE__ << endl;
			//m_data.clear();
			(*m_data).append(buffer, size * nmemb);
			// response->content.read(&buffer[0], length);
            // content.write(&buffer[0], length);
            //m_content.read(buffer,size * nmemb)
			// cout<<"m_data:"<<m_data.size()<<endl;
			// cout<<"max_size:"<<m_data.max_size() <<endl;
			// cout<<"capacity:"<<m_data.capacity()<<endl;
			result = size * nmemb;
			// boost::asio::streambuf write_buffer;

		}
		return result;
	}

	void curl(const std::string& uri, const std::string& method = "GET", const std::string& param = "", const std::string& content = "")
	{
		set_url(m_url + uri + "?" + param);
		//cout << __LINE__ << ":" << uri << endl;

#ifdef DEBUG
		curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);
#endif
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, request_callback);
		curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 50L);
		curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
		curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, method.c_str());

		//curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS,0L);
		curl_easy_setopt(m_curl, CURLOPT_CLOSESOCKETFUNCTION, close_socket_callback);

		curl_easy_setopt(m_curl, CURLOPT_CLOSESOCKETDATA, this);
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, content.c_str());

  		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)content.length());
		if(on_request())
		{
			if(CURLE_OK==curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE,&m_request_status))
			{
				cout<<"get status success"<<endl;
			}
		}

	}
	void set_url(const std::string& url) const
	{
		curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
		cout<<url<<":"<<__FILE__<<":"<<__LINE__<<endl;
	}
	bool on_request()
	{
		if(m_data)
		{
			(*m_data).clear();
		}
		else
		{
			m_data=boost::shared_ptr<string>(new string());
		}
		return 0 == curl_easy_perform(m_curl);
	}
	static int close_socket_callback(void *clientp, curl_socket_t item)
	{
		if (clientp)
		{
			//cout << __LINE__ << endl;
			((exchange_rate*)clientp)->process_content();
		}

	}
	void process_content()
	{

		cout<<*m_data<<":"<<__FILE__<<":"<<__LINE__<<endl;		
	}
	
	
public:
	boost::shared_ptr<std::string> m_data;
protected:	
	CURL* m_curl;
	std::string m_url;
	static CURLSH* share_handle;	
	long int m_request_status;
	// boost::asio::streambuf m_content_buffer;
	// std::istream m_content;
};
CURLSH* exchange_rate::share_handle = NULL;

class exchage_rate_data
{
public:
	string code;//CAD
	double to_usd_exchange_rate;//0.772558
	double from_usd_exchange_rate;//0.772558
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
class exchange_rate_on_time:public boost::enable_shared_from_this<exchange_rate_on_time>
{
public:
	exchange_rate_on_time():m_d_t(m_io_s),m_product_all(nullptr)
	{
		
	}
	~exchange_rate_on_time()
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
			string query_sql = "select code,currency_id from "+get_config->m_mysql_database + ".t_currency";
			cout << query_sql << endl;
			m_conn->runQuery(&t_currency_tuple_vector, query_sql.c_str());

			BOOST_LOG_SEV(slg, boost_log->get_log_level()) << query_sql;
			boost_log->get_initsink()->flush();
			/********************************/
			cout.setf(ios::showpoint); cout.setf(ios::fixed); cout.precision(8);
			/********************************/
			if(t_currency_tuple_vector.empty())
			{
				BOOST_LOG_SEV(slg, boost_log->get_log_level()) << "nothing select from t_currency";
				boost_log->get_initsink()->flush();
				cout<<"nothing select from t_currency"<<endl;
			}
			for (const auto& item : t_currency_tuple_vector)
			{
				cout<<item<<":"<<__FILE__<<":"<<__LINE__<<endl;
				cout<<*(std::get<0>(item))<<":"<<__FILE__<<":"<<__LINE__<<endl;
				if(*(std::get<0>(item))=="USD")
				{
					m_usd_info.code="USD";
					m_usd_info.currency_id=*(std::get<1>(item));
					cout<<m_usd_info.code<<":"<<m_usd_info.currency_id<<":"<<__FILE__<<":"<<__LINE__<<endl;
				}
				
			}
			for (const auto& item : t_currency_tuple_vector)
			{
				// string code;//CAD
				// double to_usd_exchange_rate;//0.772558
				// double from_usd_exchange_rate;//0.772558
				// string currency_id;//J4YVQ3USQNO3U430EKE1
				// string to_usd_exchange_rate_id;//TFTBLZNSNBNAZAZGC2RW
				// string from_usd_exchange_rate_id;//TFTBLZNSNBNAZAZGC2RW
				exchage_rate_data temp;
				temp.code=*(std::get<0>(item));
				temp.currency_id=*(std::get<1>(item));
				if(temp.code=="USD")
					continue;
				cout << *(std::get<0>(item))<<":"<<*(std::get<1>(item))<<":"<<__FILE__<<":"<<__LINE__<<endl;

				string get_exchange_rate_id = "select currency_exchange_rate_id from t_currency_exchange_rate where source_currency_id=\'"+temp.currency_id+"\' and target_currency_id=\'"+m_usd_info.currency_id+"\'";
				cout << get_exchange_rate_id << endl;
				string get_exchange_rate_id2 = "select currency_exchange_rate_id from t_currency_exchange_rate where source_currency_id=\'"+m_usd_info.currency_id+"\' and target_currency_id=\'"+temp.currency_id+"\'";
				cout << get_exchange_rate_id2 << endl;
				try
				{
					{
						typedef tuple<unique_ptr<string>> c;
			
						vector<c> exchange_rate_ids;
						m_conn->runQuery(&exchange_rate_ids,get_exchange_rate_id.c_str());
						for(const auto& i : exchange_rate_ids)
						temp.to_usd_exchange_rate_id=*(std::get<0>(i));
						exchange_rate_ids.clear();
					}
					{
						typedef tuple<unique_ptr<string>> c;
			
						vector<c> exchange_rate_ids;
						m_conn->runQuery(&exchange_rate_ids,get_exchange_rate_id2.c_str());
						for(const auto& i : exchange_rate_ids)
						temp.from_usd_exchange_rate_id=*(std::get<0>(i));
						exchange_rate_ids.clear();
					}

				}
				catch (const MySqlException& e)
				{
					BOOST_LOG_SEV(slg, severity_level::error) << "(1)" << get_exchange_rate_id << "(2)" << get_exchange_rate_id2 << "(exception:)" << e.what();
					boost_log->get_initsink()->flush();
				}
				m_exchage_rate_data_array.push_back(temp);
			}	
			t_currency_tuple_vector.clear();
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
	void update_exchange_rate_to_mysql()
	{
		try
		{
			for(auto& item :m_exchage_rate_data_array)
			{
				
				item.print();
			}
		}	
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
	void get_exchange_rate()
	{
		try
		{
			//http://query.yahooapis.com/v1/public/yql?q=select * from yahoo.finance.xchange where pair in ("USDEUR", "USDJPY", "USDBGN", "USDCZK", "USDDKK", "USDGBP", "USDHUF", "USDLTL", "USDLVL", "USDPLN", "USDRON", "USDSEK", "USDCHF", "USDNOK", "USDHRK", "USDRUB", "USDTRY", "USDAUD", "USDBRL", "USDCAD", "USDCNY", "USDHKD", "USDIDR", "USDILS", "USDINR", "USDKRW", "USDMXN", "USDMYR", "USDNZD", "USDPHP", "USDSGD", "USDTHB", "USDZAR", "USDISK")&env=store://datatables.org/alltableswithkeys
			//KRW TRY USD HKD SKW INR SGD GBP TWD JPY BGN CNY EUR SEK TRL ZAR THB MXP HRK ROL CAD RUR PHP IDR BRL AUD PLZ MXN NZD

			get_info_from_myql();
			for(auto& item :m_exchage_rate_data_array)
			{
				boost::shared_ptr<exchange_rate> rate = boost::shared_ptr<exchange_rate>(new exchange_rate(get_config->m_exchange_rate_url));
				rate->request("GET", "/"+item.code+"/USD", "k="+get_config->m_exchange_rate_key, "");

				item.to_usd_exchange_rate=boost::lexical_cast<double>(*(rate->m_data));
				cout<<*(rate->m_data)<<":"<<__FILE__<<":"<<__LINE__<<endl;
			}
			for(auto& item :m_exchage_rate_data_array)
			{
				boost::shared_ptr<exchange_rate> rate = boost::shared_ptr<exchange_rate>(new exchange_rate(get_config->m_exchange_rate_url));
				rate->request("GET", "/USD/"+item.code, "k="+get_config->m_exchange_rate_key, "");

				item.from_usd_exchange_rate=boost::lexical_cast<double>(*(rate->m_data));
				cout<<*(rate->m_data)<<":"<<__FILE__<<":"<<__LINE__<<endl;
			}
			update_exchange_rate_to_mysql();
		}
		catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();
				cout<<e.what()<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
		try
		{
		    m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_exchange_rate_request_interval));
		  
		    m_d_t.async_wait(boost::bind(&exchange_rate_on_time::handle_wait, shared_from_this(), boost::asio::placeholders::error));  
			m_io_s.run();
		}
		catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();
				cout<<e.what()<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
	void handle_wait(const boost::system::error_code& error)  
    {  
        if(!error)  
        {  
        	start_update();
        	//cout<<"handle wait"<<endl;
            m_d_t.expires_from_now(boost::posix_time::seconds(get_config->m_write_product_interval));  
            m_d_t.async_wait(boost::bind(&exchange_rate_on_time::handle_wait,shared_from_this(), boost::asio::placeholders::error));                 
    	}   
	}  
	void start_update()
	{
		try
		{
			get_exchange_rate();
			//cout<<__FILE__<<":"<<__LINE__<<endl;

			ptree pt,ret_json_all;
			ptree return_json;
			if(m_product_all==nullptr||(*m_product_all).length()==0)
			{
				//*
				BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:):get nothing from exchange_rate";
				boost_log->get_initsink()->flush();
				cout<<":get nothing from exchange_rate"<<endl;
				*m_product_all="";
			}
			else
			{
				// std::istringstream is(*m_product_all);
				// read_json(is, pt);
				// for(auto& sub:pt)
				// {
				// 	ptree ret_json;
		  //  			string product_category_id=sub.second.get<string>("product_category_id");
				// 	int product_id=sub.second.get<int>("product_id");
				// 	string product_name=sub.second.get<string>("product_name");
				// 	string sku=sub.second.get<string>("sku");

				// 	//cout<<product_id<<":"<<product_name<<":"<<sku<<endl;
				// 	ret_json.put<std::string>("product_code",get_product_id(product_name));
				// 	ret_json.put<std::string>("product_name",product_name);

				// 	ptree child = sub.second.get_child("inventory_quantities");
				// 	ret_json.add_child("inventory_quantities", child);
					

				// 	// if(!child.empty())
				// 	// {
				// 	// 	for(auto& subchild:child)
				// 	// 	{
						
				// 	// 		double distribution_center_id=subchild.second.get<double>("distribution_center_id");
				// 	// 		string distribution_center_name=subchild.second.get<string>("distribution_center_name");
				// 	// 		double inventory_quantity=subchild.second.get<double>("inventory_quantity");
				// 	// 		cout<<distribution_center_name<<":"<<inventory_quantity<<endl;
							
				// 	// 	}
				// 	// }

				// 	ret_json_all.push_back(std::make_pair("", ret_json));

				//}
				// return_json.push_back(std::make_pair("product", ret_json_all));
				// if(!return_json.empty())
				// {
				// 	write_json(m_ss, return_json);
				// }
				// else
				// {
				// 	m_ss<<""<<endl;
				// }
				send_message_to_activemq();
			}
		}
		catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();
				cout<<e.what()<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
	
	
	void send_message_to_activemq()
	{
		try
		{
		string message(m_ss.str());
		message.erase(remove(message.begin(), message.end(), '\n'), message.end());
		//activemq::library::ActiveMQCPP::initializeLibrary();
		std::string brokerURI =
	        "failover://(tcp://"+get_config->m_activemq_url+
	       // "?wireFormat=openwire"
	       // "&connection.useAsyncSend=true"
	       // "&transport.commandTracingEnabled=true"
	       // "&transport.tcpTracingEnabled=true"
	       // "&wireFormat.tightEncodingEnabled=true"
	        ")";

	    bool useTopics = false;

	    boost::shared_ptr<activemq_cms_producer> producer(new activemq_cms_producer(message,brokerURI, 1, get_config->m_exchange_rate_write_rate_queue, useTopics,true ));

	    producer->run();

	    producer->close();

	    //activemq::library::ActiveMQCPP::shutdownLibrary();
	    }
	    catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
				boost_log->get_initsink()->flush();
				cout<<e.what()<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what();
			boost_log->get_initsink()->flush();cout<<e.what()<<endl;
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
	std::stringstream m_ss;
	boost::asio::io_service m_io_s;  
	deadline_timer m_d_t;
	std::vector<exchage_rate_data> m_exchage_rate_data_array;
	exchage_rate_data m_usd_info;
};
#endif

