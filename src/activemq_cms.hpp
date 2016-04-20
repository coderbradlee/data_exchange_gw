#ifndef ACTIVEMQ_CMS_HPP
#define	ACTIVEMQ_CMS_HPP

#include  "include.hpp"
#include <sys/mman.h>
#include <activemq/core/ActiveMQConnection.h>
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
        cleanup();
    }

    void close() 
    {
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
                TextMessage* message = session->createTextMessage( m_messages );

                message->setIntProperty( "Integer", ix );
                message->setStringProperty("testkey","testvalue");
                // Tell the producer to send the message
                printf( "Sent message #%d from thread %s\n", ix+1, threadIdStr.c_str() );
                producer->send( message );

                delete message;
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
            if( destination != NULL ) delete destination;
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        destination = NULL;

        try
        {
            if( producer != NULL ) delete producer;
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
            if( session != NULL ) delete session;
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        session = NULL;

        try
        {
            if( connection != NULL ) delete connection;
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        connection = NULL;
    }
};
////////////////////////////////////////////////////////////////////////////////
class activemq_cms_consumer : public ExceptionListener,
                            public MessageListener,
                            public DefaultTransportListener 
                            {
private:

    Connection* connection;
    cms::Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    bool useTopic;
    std::string brokerURI;
    std::string destURI;
    bool clientAck;

private:

    activemq_cms_consumer( const activemq_cms_consumer& );
    activemq_cms_consumer& operator= ( const activemq_cms_consumer& );

public:

    activemq_cms_consumer( const std::string& brokerURI,
                         const std::string& destURI,
                         bool useTopic = false,
                         bool clientAck = false ) :
        connection(NULL),
        session(NULL),
        destination(NULL),
        consumer(NULL),
        useTopic(useTopic),
        brokerURI(brokerURI),
        destURI(destURI),
        clientAck(clientAck) {
    }

    virtual ~activemq_cms_consumer() {
        this->cleanup();
    }

    void close() {
        this->cleanup();
    }

    void runConsumer() {

        try {

            // Create a ConnectionFactory
            ActiveMQConnectionFactory* connectionFactory =
                new ActiveMQConnectionFactory( brokerURI );

            // Create a Connection
            connection = connectionFactory->createConnection();
            delete connectionFactory;

            ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection );
            if( amqConnection != NULL ) {
                amqConnection->addTransportListener( this );
            }

            connection->start();

            connection->setExceptionListener(this);

            // Create a Session
            if( clientAck ) {
                session = connection->createSession( cms::Session::CLIENT_ACKNOWLEDGE );
            } else {
                session = connection->createSession( cms::Session::AUTO_ACKNOWLEDGE );
            }

            // Create the destination (Topic or Queue)
            if( useTopic ) {
                destination = session->createTopic( destURI );
            } else {
                destination = session->createQueue( destURI );
            }

            // Create a MessageConsumer from the Session to the Topic or Queue
            consumer = session->createConsumer( destination );
            consumer->setMessageListener( this );

        } catch (CMSException& e) {
            e.printStackTrace();
        }
    }

    // Called from the consumer since this class is a registered MessageListener.
    virtual void onMessage( const Message* message ) {

        static int count = 0;

        try
        {
            count++;
            const TextMessage* textMessage =
                dynamic_cast< const TextMessage* >( message );
            string text = "";

            if( textMessage != NULL ) {
                text = textMessage->getText();
            } else {
                text = "NOT A TEXTMESSAGE!";
            }

            if( clientAck ) {
                message->acknowledge();
            }

            printf( "Message #%d Received: %s\n", count, text.c_str() );
        } catch (CMSException& e) {
            e.printStackTrace();
        }
    }

    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException( const CMSException& ex AMQCPP_UNUSED ) {
        printf("CMS Exception occurred.  Shutting down client.\n");
        exit(1);
    }

    virtual void transportInterrupted() {
        std::cout << "The Connection's Transport has been Interrupted." << std::endl;
    }

    virtual void transportResumed() {
        std::cout << "The Connection's Transport has been Restored." << std::endl;
    }

private:

    void cleanup(){

        //*************************************************
        // Always close destination, consumers and producers before
        // you destroy their sessions and connection.
        //*************************************************

        // Destroy resources.
        try{
            if( destination != NULL ) delete destination;
        }catch (CMSException& e) {}
        destination = NULL;

        try{
            if( consumer != NULL ) delete consumer;
        }catch (CMSException& e) {}
        consumer = NULL;

        // Close open resources.
        try{
            if( session != NULL ) session->close();
            if( connection != NULL ) connection->close();
        }catch (CMSException& e) {}

        // Now Destroy them
        try{
            if( session != NULL ) delete session;
        }catch (CMSException& e) {}
        session = NULL;

        try{
            if( connection != NULL ) delete connection;
        }catch (CMSException& e) {}
        connection = NULL;
    }
};

////////////////////////////////////////////////////////////////////////////////
#endif

