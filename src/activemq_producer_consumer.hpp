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
public:

	void start()
	{
		try
		{
			activemq::library::ActiveMQCPP::initializeLibrary();

			boost::shared_ptr<product_inventory> producer(new product_inventory);
	        boost::shared_ptr<order_activemq> consumer(new order_activemq);

		    // ///////////Start the consumer thread.
		    thread consumerThread([&consumer](){consumer->start();});

		    thread producerThread([&producer](){producer->start();});

		    producerThread.join();
		    consumerThread.join();
			activemq::library::ActiveMQCPP::shutdownLibrary();
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
};

#endif

