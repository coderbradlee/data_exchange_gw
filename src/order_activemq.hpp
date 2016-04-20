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
	void start_consume_listener()
	{
		std::string brokerURI =
        "failover://(tcp://"+get_config->m_activemq_url+
//        "?wireFormat=openwire"
//        "&connection.useAsyncSend=true"
//        "&transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true"
//        "&wireFormat.tightEncodingEnabled=true"
        ")";
    bool useTopics = false;

    //============================================================
    // set to true if you want the consumer to use client ack mode
    // instead of the default auto ack mode.
    //============================================================
    bool clientAck = false;

    // Create the consumer
    activemq_cms_consumer consumer( brokerURI, get_config->m_activemq_read_order_queue, useTopics, clientAck );
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
};

#endif

