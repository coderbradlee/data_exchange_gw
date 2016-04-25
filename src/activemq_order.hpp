#ifndef ACTIVEMQ_ORDER_HPP
#define	ACTIVEMQ_ORDER_HPP

#include  "include.hpp"
//#define DEBUG
#include "mysql/mysql_api.hpp"
#include "orderbot.hpp"
#include "activemq_cms.hpp"

class order_activemq
{
public:
	order_activemq(){}
	virtual void start() 
	{
        try 
        {
        	//start_consume_listener_multithread();
        	start_consume_listener();
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
	void start_consume_listener_multithread()
	{
		try
		{
		//activemq::library::ActiveMQCPP::initializeLibrary();
		std::string brokerURI =
        "failover://(tcp://"+get_config->m_activemq_url+
//        "?wireFormat=openwire"
//        "&connection.useAsyncSend=true"
//        "&transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true"
//        "&wireFormat.tightEncodingEnabled=true"
        ")";
		 bool useTopics = false;
	    bool sessionTransacted = false;
	    int numMessages = 1;

        activemq_cms_consumer_multithread consumer(brokerURI, numMessages, useTopics, sessionTransacted);

	    // Start the consumer thread.
	    Thread consumerThread(&consumer);
	    consumerThread.start();

	    // Wait for the consumer to indicate that its ready to go.
	    consumer.waitUntilReady();

	    consumerThread.join();

	    consumer.close();

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
	void start_consume_listener()
	{
		try
		{
		//activemq::library::ActiveMQCPP::initializeLibrary();
		std::string brokerURI =
        "failover://(tcp://"+get_config->m_activemq_url+
//        "?wireFormat=openwire"
//        "&connection.useAsyncSend=true"
//        "&transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true"
//        "&wireFormat.tightEncodingEnabled=true"
        ")";

		    // Create the consumer
		    activemq_cms_consumer consumer( brokerURI, get_config->m_activemq_read_order_queue);

		    // Start it up and it will listen forever.
		    consumer.runConsumer();

		    // Wait to exit.
		    std::cout << "Press 'q' to quit" << std::endl;
		    while( std::cin.get() != 'q') {}

		    // All CMS resources should be closed before the library is shutdown.
		    consumer.close();

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
		//boost::shared_ptr<MySql> m_conn;
		string m_today_string;
		//boost::shared_ptr<string> m_product_all;
		std::stringstream m_ss;
};

#endif

