#ifndef ACTIVEMQ_CMS_HPP
#define	ACTIVEMQ_CMS_HPP

#include  "include.hpp"


#include <activemq/transport/DefaultTransportListener.h>
#include <decaf/lang/Integer.h>
#include <activemq/util/Config.h>
#include <decaf/util/Date.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;
//using namespace cms::Session;
// using namespace cms::Connection;
// using namespace cms::Destination;
// using namespace cms::MessageProducer;
////////////////////////////////////////////////////////////////////////////////
class activemq_cms_producer : public Runnable
{
private:

    Connection* connection;
    cms::Session* session;
    Destination* destination;
   	MessageProducer* producer;
    bool useTopic;
    bool clientAck;
    unsigned int numMessages;
    std::string brokerURI;
    std::string destURI;
    std::string m_messages;
private:

    activemq_cms_producer( const activemq_cms_producer& );
    activemq_cms_producer& operator= ( const activemq_cms_producer& );

public:

    activemq_cms_producer(const std::string& message, const std::string& brokerURI, unsigned int numMessages,
                    const std::string& destURI, bool useTopic = false, bool clientAck = false ) :
        connection(NULL),
        session(NULL),
        destination(NULL),
        producer(NULL),
        useTopic(useTopic),
        clientAck(clientAck),
        numMessages(numMessages),
        brokerURI(brokerURI),
        destURI(destURI),m_messages(message) 
        {
        	cout<<brokerURI<<endl;
        	cout<<destURI<<endl;
        }

    virtual ~activemq_cms_producer()
    {
    	cout<<"~activemq_cms_producer"<<endl;
        cleanup();
    }

    void close() 
    {
    	cout<<"producer close"<<endl;
        this->cleanup();
    }

    virtual void run() 
    {
        try {

            // Create a ConnectionFactory
            boost::shared_ptr<ActiveMQConnectionFactory> connectionFactory(
                new ActiveMQConnectionFactory( brokerURI ) );

            // Create a Connection
            try
            {
                connection = connectionFactory->createConnection();
                connection->start();
            } 
            catch( CMSException& e ) 
            {
                e.printStackTrace();
                throw e;
            }

            // Create a Session
            if( clientAck ) 
            {
                session = connection->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
            } 
            else 
            {
                session = connection->createSession( cms::Session::AUTO_ACKNOWLEDGE );
            }

            // Create the destination (Topic or Queue)
            if( useTopic ) 
            {
                destination = session->createTopic( destURI );
            } 
            else 
            {
                destination = session->createQueue( destURI );
            }

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::PERSISTENT );
            //producer->setTimeToLive(10000);
            //producer->setPriority(5);
            // Create the Thread Id String
            string threadIdStr = Long::toString( Thread::currentThread()->getId() );

            // Create a messages
            //string text = (string)"Hello world! from thread " + threadIdStr;

            for( unsigned int ix=0; ix<numMessages; ++ix )
            {
                boost::shared_ptr<TextMessage> message(session->createTextMessage( m_messages ));

                message->setIntProperty( "Integer", ix );
                message->setStringProperty("testkey","testvalue");
                // Tell the producer to send the message
                printf( "Sent message #%d from thread %s\n", ix+1, threadIdStr.c_str() );
                producer->send( message.get() );

                //delete message;
            }

        }
        catch ( CMSException& e ) 
        {
            e.printStackTrace();
        }
    }

private:

    void cleanup()
    {

        // Destroy resources.
        try
        {
            if( destination != NULL )
            {
            	delete destination;
            	cout<<__FILE__<<":"<<__LINE__<<":delete destination"<<endl;
            } 
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        destination = NULL;

        try
        {
            if( producer != NULL )
            {
            	delete producer;
            	cout<<__FILE__<<":"<<__LINE__<<":delete producer"<<endl;
            } 
        }
        catch ( CMSException& e )
        {
          e.printStackTrace();
        }
        producer = NULL;

        // Close open resources.
        try
        {
            if( session != NULL ) session->close();
            if( connection != NULL ) connection->close();
        }
        catch ( CMSException& e ) 
        {
         	e.printStackTrace(); 
     	}

        try
        {
            if( session != NULL )
            {
            	delete session;
            	cout<<__FILE__<<":"<<__LINE__<<":delete session"<<endl;
            } 
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        session = NULL;

        try
        {
            if( connection != NULL )
            {
            	delete connection;
            	cout<<__FILE__<<":"<<__LINE__<<":delete connection"<<endl;
            } 
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        connection = NULL;
    }
};
class activemq_cms_consumer_multithread : public ExceptionListener,
                           public MessageListener,
                           public Runnable {

private:

    CountDownLatch latch;
    CountDownLatch doneLatch;
    Connection* connection;
    cms::Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    long waitMillis;
    bool useTopic;
    bool sessionTransacted;
    std::string brokerURI;
    std::stringstream m_ss;
private:

    activemq_cms_consumer_multithread(const activemq_cms_consumer_multithread&);
    activemq_cms_consumer_multithread& operator=(const activemq_cms_consumer_multithread&);

public:

    activemq_cms_consumer_multithread(const std::string& brokerURI, int numMessages, bool useTopic = false, bool sessionTransacted = false, int waitMillis = 30000) :
        latch(1),
        doneLatch(numMessages),
        connection(NULL),
        session(NULL),
        destination(NULL),
        consumer(NULL),
        waitMillis(waitMillis),
        useTopic(useTopic),
        sessionTransacted(sessionTransacted),
        brokerURI(brokerURI) {
    }

    virtual ~activemq_cms_consumer_multithread() 
    {
    	cout<<"~activemq_cms_consumer_multithread"<<endl;
        cleanup();
    }

    void close() 
    {
    	cout<<"activemq_cms_consumer_multithread close"<<endl;
        this->cleanup();
    }

    void waitUntilReady() {
        latch.await();
    }

    virtual void run() {

        try {

            // Create a ConnectionFactory
            boost::shared_ptr<ConnectionFactory> connectionFactory(
                ConnectionFactory::createCMSConnectionFactory(brokerURI));

            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();
            connection->setExceptionListener(this);

            // Create a Session
            if (this->sessionTransacted == true) {
                session = connection->createSession(cms::Session::SESSION_TRANSACTED);
            } else {
                session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
            }

            // Create the destination (Topic or Queue)
            if (useTopic) {
                destination = session->createTopic(get_config->m_activemq_read_order_queue);
            } else {
                destination = session->createQueue(get_config->m_activemq_read_order_queue);
            }

            // Create a MessageConsumer from the Session to the Topic or Queue
            consumer = session->createConsumer(destination);

            consumer->setMessageListener(this);

            std::cout.flush();
            std::cerr.flush();

            // Indicate we are ready for messages.
            latch.countDown();
			cout<<__FILE__<<":"<<__LINE__<<":"<<"consumer start to listen"<<endl;
            // Wait while asynchronous messages come in.
            //doneLatch.await(waitMillis);
			doneLatch.await();
        } 
        catch (CMSException& e) 
        {
            // Indicate we are ready for messages.
            latch.countDown();
            e.printStackTrace();
        }
    }

    // Called from the consumer since this class is a registered MessageListener.
    virtual void onMessage(const Message* message)
     {
        //static int count = 0;
        try 
        {
            //count++;
            const TextMessage* textMessage=dynamic_cast<const TextMessage*> (message);
            string text = "";

            if (textMessage != NULL) 
            {
                text = textMessage->getText();
            } 
            else
            {
                text = "NOT A TEXTMESSAGE!";
            }
            cout<<"Message Received: "<<text<<endl;

            //doneLatch.await();
            //decode text and request to orderbot,then put results to activemq
            decode_request_orderbot(text);
        } 
        catch (CMSException& e) 
        {
            e.printStackTrace();
        }

        // Commit all messages.
        if (this->sessionTransacted) 
        {
            session->commit();
        }

        // No matter what, tag the count down latch until done.
        //doneLatch.countDown();
    }

    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException(const CMSException& ex AMQCPP_UNUSED) 
    {
        printf("CMS Exception occurred.  Shutting down client.\n");
        ex.printStackTrace();
        exit(1);
    }

    void decode_request_orderbot(const string& text)
    {
    	try
    	{
    		ptree pt,ret_json_all;
			ptree return_json;
			std::istringstream is(*m_product_all);
			read_json(is, pt);
			// for(auto& sub:pt)
			// {
			//ptree ret_json;
	  //  			string product_category_id=sub.second.get<string>("product_category_id");
			// 	int product_id=sub.second.get<int>("product_id");
			// 	string product_name=sub.second.get<string>("product_name");
			// 	string sku=sub.second.get<string>("sku");

			// 	//cout<<product_id<<":"<<product_name<<":"<<sku<<endl;
			// 	ret_json.put<std::string>("product_code",get_product_id(product_name));
			// 	ret_json.put<std::string>("product_name",product_name);

			// 	ptree child = sub.second.get_child("inventory_quantities");
			// 	ret_json.add_child("inventory_quantities", child);

			// 	ret_json_all.push_back(std::make_pair("", ret_json));

			// }
			// 	return_json.push_back(std::make_pair("product", ret_json_all));
			// 	write_json(m_ss, return_json);
			boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			order->request("GET", "/admin/orders.json/1", "", "");

			return_json.put<std::string>("orders1",order->m_data);
			write_json(m_ss,return_json)
			send_message_to_activemq();
    			
    	}
    	catch (json_parser_error& e)
		{
			
		}
    	catch (std::exception& e)
		{
			//cout << diagnostic_information(e) << endl;
			cout << e.what() << endl;
		}
    }
    void send_message_to_activemq()
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

	    boost::shared_ptr<activemq_cms_producer> producer(new activemq_cms_producer(message,brokerURI, 1, get_config->m_activemq_write_order_queue, useTopics,true ));

	    producer->run();

	    producer->close();

	    //activemq::library::ActiveMQCPP::shutdownLibrary();
	}
private:

    void cleanup() 
    {
        if (connection != NULL) 
        {
            try 
            {
                connection->close();
                //connection=NULL;
            } 
            catch (cms::CMSException& ex) 
            {
                ex.printStackTrace();
            }
        }

        // Destroy resources.
        try {
	        	if (destination != NULL)
	           	{
	           	 	delete destination;
	           	 	destination=NULL;
	           	}
	            if (consumer != NULL)
	           	{
	           	 	delete consumer;
	           	 	consumer=NULL;
	           	}
	            if (session != NULL)
	           	{
	           	 	delete session;
	           	 	session=NULL;
	           	}
	            if (connection != NULL)
	           	{
	           	 	delete connection;
	           	 	connection=NULL;
	           	}
	           
        	} 
        	catch (CMSException& e) 
        	{
            	e.printStackTrace();
        	}
    }
};

#endif

