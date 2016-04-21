#ifndef ORDER_ACTIVEMQ_HPP
#define	ORDER_ACTIVEMQ_HPP

#include  "include.hpp"
//#define DEBUG
#include "mysql/mysql_api.hpp"
#include "orderbot.hpp"
#include "activemq_cms.hpp"

class order_activemq
{
public:
	order_activemq(){}

	void start_consume_listener_multithread()
	{
		activemq::library::ActiveMQCPP::initializeLibrary();
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

	    activemq::library::ActiveMQCPP::shutdownLibrary();
	}
		private:
		//boost::shared_ptr<MySql> m_conn;
		string m_today_string;
		//boost::shared_ptr<string> m_product_all;
		std::stringstream m_ss;
};

#endif

