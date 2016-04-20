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

	void start_consume_listener()
	{
		std::string brokerURI1 =
        "failover://(tcp://"+get_config->m_activemq_url+
//        "?wireFormat=openwire"
//        "&connection.useAsyncSend=true"
//        "&transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true"
//        "&wireFormat.tightEncodingEnabled=true"
        ")";
        std::string brokerURI=brokerURI1;
	    bool useTopics = false;

	    //============================================================
	    // set to true if you want the consumer to use client ack mode
	    // instead of the default auto ack mode.
	    //============================================================
	    bool clientAck = false;
	    std::string read_queue_name=get_config->m_activemq_read_order_queue;
	    // Create the consumer
	    activemq_cms_consumer consumer( brokerURI, read_queue_name, useTopics, clientAck );
	    //cout<<__FILE__<<":"<<__LINE__<<endl;
	    // Start it up and it will listen forever.
	    consumer.runConsumer();

	    // Wait to exit.
	    std::cout << "Press 'q' to quit" << std::endl;
	    while( std::cin.get() != 'q') {}

	    // All CMS resources should be closed before the library is shutdown.
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

