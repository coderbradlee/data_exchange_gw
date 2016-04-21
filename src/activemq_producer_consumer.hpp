#ifndef ACTIVEMQ_PRODUCER_CONSUMER_HPP
#define	ACTIVEMQ_PRODUCER_CONSUMER_HPP

#include  "include.hpp"
//#define DEBUG
#include "mysql/mysql_api.hpp"
#include "orderbot.hpp"
#include "activemq_cms.hpp"
#include "activemq_order.hpp"
#include "activemq_product_inventory.hpp"
class activemq_producer_consumer
{
	void start()
	{
		activemq::library::ActiveMQCPP::initializeLibrary();
		// boost::shared_ptr<order_activemq> consume(new order_activemq);		
		// thread consume_thread([&consume](){consume->start();});	
		//////////////////////////
		// boost::shared_ptr<product_inventory> produce(new product_inventory);
		// thread produce_thread([&produce](){produce->start();});	
		
		// produce_thread.join();
		//consume_thread.join();
		

		boost::shared_ptr<product_inventory> producer(new product_inventory);
        boost::shared_ptr<order_activemq> consumer(new order_activemq);

	    // Start the consumer thread.
	    thread consumerThread([&consumer](){consumer->start();});
	    //consumerThread.start();

	    // Wait for the consumer to indicate that its ready to go.
	    //consumer->waitUntilReady();

	    // Start the producer thread.
	    thread producerThread([&producer](){producer->start();});
	    //producerThread.start();

	    // Wait for the threads to complete.
	    producerThread.join();
	    consumerThread.join();
		activemq::library::ActiveMQCPP::shutdownLibrary();
	}
};

#endif

