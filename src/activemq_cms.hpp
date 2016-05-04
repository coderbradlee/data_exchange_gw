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
	friend class boost::signals2::deconstruct_access;  // give boost::signals2::deconstruct access to private constructor
    // private constructor forces use of boost::signals2::deconstruct to create objects.
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

/* This adl_predestruct friend function will be found by
    via argument-dependent lookup when using boost::signals2::deconstruct. */
    friend void adl_predestruct(activemq_cms_producer *)
    {
      std::cout << "Goodbye, ";
    }
    /* boost::signals2::deconstruct always requires an adl_postconstruct function
    which can be found via argument-dependent, so we define one which does nothing. */
    template<typename T> friend
      void adl_postconstruct(const boost::shared_ptr<T> &, activemq_cms_producer *)
    {}


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
        	// cout<<brokerURI<<endl;
        	// cout<<destURI<<endl;
        }

    virtual ~activemq_cms_producer()
    {
    	//cout<<"~activemq_cms_producer"<<endl;
        cleanup();
    }

    void close() 
    {
    	//cout<<"producer close"<<endl;
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

                // message->setIntProperty( "Integer", ix );
                // message->setStringProperty("testkey","testvalue");
                // Tell the producer to send the message
                printf( "Sent message #%d from thread %s\n", ix+1, threadIdStr.c_str() );
                BOOST_LOG_SEV(slg, severity_level::error) <<"(message send to activemq:)" << m_messages;
				boost_log->get_initsink()->flush();
                producer->send( message.get() );

                //delete message;
            }

        }
        // catch ( CMSException& e ) 
        // {
        //     e.printStackTrace();
        // } 
        catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
				boost_log->get_initsink()->flush();
				cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
        }
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
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
            	cout<<__FILE__<<":"<<__LINE__<<":activemq_cms_producer delete destination"<<endl;
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
            	cout<<__FILE__<<":"<<__LINE__<<":activemq_cms_producer delete producer"<<endl;
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
            	cout<<__FILE__<<":"<<__LINE__<<":activemq_cms_producer delete session"<<endl;
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
            	cout<<__FILE__<<":"<<__LINE__<<":activemq_cms_producer delete connection"<<endl;
            } 
        }
        catch ( CMSException& e ) 
        { 
        	e.printStackTrace(); 
        }
        connection = NULL;
    }
};

class activemq_cms_consumer : public ExceptionListener,
                            public MessageListener,
                            public activemq::transport::DefaultTransportListener {
private:

    Connection* connection;
    cms::Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    bool useTopic;
    std::string brokerURI;
    std::string destURI;
    bool clientAck;
    //std::stringstream m_ss;
    std::stringstream m_ss_tomq;
    string m_ss;
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
            //count++;
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
            decode_request_orderbot(text);
            cout<<"Message Received:"<<text<<endl;
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
	void parser_json_write_ss(const string& texts)
    {
        try
        {
        string text="{    \"sales_order_id\": \"1\",    \"so_no\": \"2\",    \"po_no\": \"3\",    \"status\": 0,    \"order_date\": \"2016-05-03\",    \"company_id\": \"4\",    \"sales_id\": \"5\",    \"currency_id\": \"6\",    \"ss_currency_daily_exchange_rate\": 6.45,    \"tax_schedule_id\": \"7\",    \"ss_tax_rate\": 7.2,    \"customer_master_id\": \"8\",    \"customer_contact_id\": \"9\",    \"customer_invoice_address_id\": \"10\",    \"ship_to_customer_name\": \"11\",    \"ship_to_address\": \"12\",    \"ship_to_state\": \"NY\",    \"ship_to_city\": \"13\",    \"ship_to_zip_code\": \"10118\",    \"ship_to_contact_name\": \"14\",    \"ship_to_contact_phone_number\": \"+1 800-428-4322\",    \"ship_to_contact_email\": \"test@orderbot.com\",    \"trade_term_id\": \"17\",    \"ss_landed_cost_coefficient\": 3.3,    \"dispatch_warehouse_id\": 1,    \"requested_delivery_date\": \"2016-05-03\",    \"promotion_code\": \"\",    \"company_bank_account_id\": \"\",    \"shipping_cost_total\": 25.48,    \"saving_total\": 3.56,    \"tax_total\": 22.51,    \"sub_total\": 180.37,    \"grand_total\": 218.67,    \"note\": \"note\",    \"detail\": [        {            \"sales_order_detail_id\": 44,            \"item_master_id\": \"\",            \"ss_guidance_price\": 5.46,            \"ss_promotion_price\": 5.41,            \"unit_price\": 5.43,            \"uom_id\": \"\",            \"quantity\": 12,            \"sub_total\": 63.67,            \"sub_tax\": 4.33,            \"sub_shipping_cost\": 5.68,            \"sub_discount\": 0,            \"note\": \"detail.note\"        }    ]}";
        using namespace json_spirit;
        json_spirit::Value value;
cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
        json_spirit::read_or_throw( text, value );
cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
        string sales_order_id;
        string so_no;
        string po_no;
        int status;
        string order_date;
        string company_id;
        string sales_id;
        string currency_id;
        double ss_currency_daily_exchange_rate;
        string tax_schedule_id;
        double ss_tax_rate;
        string customer_master_id;
        string customer_contact_id;
        string customer_invoice_address_id;
        string ship_to_customer_name;
        string ship_to_address;
        string ship_to_state;
        string ship_to_city;
        string ship_to_zip_code;
        string ship_to_contact_name;
        string ship_to_contact_phone_number;
        string ship_to_contact_email;
        string trade_term_id;
        double ss_landed_cost_coefficient;
        int dispatch_warehouse_id;
        string requested_delivery_date;
        string promotion_code;
        string company_bank_account_id;
        double shipping_cost_total;
        double saving_total;
        double tax_total;
        double sub_total;
        double grand_total;
        string note;
        json_spirit::Object ret_json_all,order_lines,product_taxes;
        json_spirit::Array detail_child;
        if (value.type() != obj_type)
        {
            //return;
            //m_ss.str("");
            cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
        }
        else
        {
            cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
            const json_spirit::Object &obj = value.get_obj();
            for(unsigned int i = 0; i < obj.size(); ++i )
            {                
                const json_spirit::Pair& pair = obj[i];
                const string& name  = pair.name_;
                const json_spirit::Value&  pt = pair.value_;
                //cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                if( name == "sales_order_id")
                {
                    sales_order_id = pt.get_str();
                    cout<<sales_order_id<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "so_no")
                {
                    so_no = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "po_no")
                {
                    po_no = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "status")
                {
                    status = pt.get_int();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "order_date")
                {
                    order_date = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "company_id")
                {
                    company_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "sales_id")
                {
                    sales_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "currency_id")
                {
                    currency_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ss_currency_daily_exchange_rate")
                {
                    ss_currency_daily_exchange_rate = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "tax_schedule_id")
                {
                    tax_schedule_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ss_tax_rate")
                {
                    ss_tax_rate = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "customer_master_id")
                {
                    customer_master_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "customer_contact_id")
                {
                    customer_contact_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "customer_invoice_address_id")
                {
                    customer_invoice_address_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_customer_name")
                {
                    ship_to_customer_name = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_address")
                {
                    ship_to_address = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_state")
                {
                    ship_to_state = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_city")
                {
                    ship_to_city = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_zip_code")
                {
                    ship_to_zip_code = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_contact_name")
                {
                    ship_to_contact_name = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_contact_phone_number")
                {
                    ship_to_contact_phone_number = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "ship_to_contact_email")
                {
                    ship_to_contact_email = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }                
                else if( name == "trade_term_id")
                {
                    trade_term_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }  
                else if( name == "ss_landed_cost_coefficient")
                {
                    ss_landed_cost_coefficient = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "dispatch_warehouse_id")
                {
                    dispatch_warehouse_id = pt.get_int();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "requested_delivery_date")
                {
                    requested_delivery_date = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "promotion_code")
                {
                    promotion_code = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }                
                else if( name == "company_bank_account_id")
                {
                    company_bank_account_id = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                } 
                else if( name == "shipping_cost_total")
                {
                    shipping_cost_total = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "saving_total")
                {
                    saving_total = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "tax_total")
                {
                    tax_total = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "sub_total")
                {
                    sub_total = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "grand_total")
                {
                    grand_total = pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                }
                else if( name == "note")
                {
                    note = pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                } 
                else if( name == "detail")
                {
                    int sales_order_detail_id;
                    string item_master_id;
                    double ss_guidance_price;
                    double ss_promotion_price;
                    double unit_price;
                    string uom_id;
                    double quantity;
                    double sub_total;
                    double sub_tax;
                    double sub_shipping_cost;
                    double sub_discount;
                    string note;
                    cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                    detail_child = pt.get_array();
                    for(unsigned int j = 0; i < detail_child.size(); ++j )
                    {
                        const json_spirit::Object &detail_child_obj = detail_child[j].get_obj();
                        for(unsigned int x = 0; x < detail_child_obj.size(); ++x )
                        {                
                            const json_spirit::Pair& detail_child_obj_pair = detail_child_obj[x];
                            const string& detail_child_obj_name  = detail_child_obj_pair.name_;
                            const json_spirit::Value&  detail_child_obj_pt = detail_child_obj_pair.value_;
                            cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                           
                        if( detail_child_obj_name == "sales_order_detail_id")
                        {
                            sales_order_detail_id = detail_child_obj_pt.get_int();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "sales_order_detail_id")
                        {
                            item_master_id = detail_child_obj_pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "ss_guidance_price")
                        {
                            ss_guidance_price = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "ss_promotion_price")
                        {
                            ss_promotion_price = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "unit_price")
                        {
                            unit_price = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "ss_guidance_price")
                        {
                            ss_guidance_price = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "quantity")
                        {
                            quantity = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "sub_total")
                        {
                            sub_total = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "sub_tax")
                        {
                            sub_tax = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "sub_shipping_cost")
                        {
                            sub_shipping_cost = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "sub_discount")
                        {
                            sub_discount = detail_child_obj_pt.get_real();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "uom_id")
                        {
                            uom_id = detail_child_obj_pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
                        else if( detail_child_obj_name == "note")
                        {
                            note = detail_child_obj_pt.get_str();cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
                        }
             }//for
            }//for
            //     order_lines.push_back( Pair("line_number", sales_order_detail_id));
            //     order_lines.push_back( Pair("product_sku", "123"));
            //     order_lines.push_back( Pair("custom_description", note));
            //     order_lines.push_back( Pair("quantity", quantity));
            //     order_lines.push_back( Pair("price", unit_price));
            //     order_lines.push_back( Pair("product_discount",sub_discount));

            //     product_taxes.push_back( Pair("tax_name", "TAX"));
            //     product_taxes.push_back( Pair("tax_rate",sub_tax));
            //     product_taxes.push_back( Pair("amount", quantity));
                
            //     order_lines.push_back( Pair("product_taxes",product_taxes));
                
            }//else if
        }//for
        //     ret_json_all.push_back( Pair("orderbot_account_id", 1));
        //     ret_json_all.push_back( Pair("order_date",order_date));
        //     ret_json_all.push_back( Pair("orderbot_account_id",Value()));
        //     ret_json_all.push_back( Pair("order_date",order_date));
        //     ret_json_all.push_back( Pair("ship_date",requested_delivery_date));
        //     ret_json_all.push_back( Pair("orderbot_customer_id","need get from orderbot"));//need get from orderbot
        //     ret_json_all.push_back( Pair("reference_customer_id",customer_master_id));
        //     ret_json_all.push_back( Pair("reference_order_id",sales_order_id));
        //     ret_json_all.push_back( Pair("customer_po", sales_order_id));
            
        //     //"0", "OrderByCustomer", "1", "OrderBysales","2","Canceled","3","UnConfirmed"); 
        //     if(status==0||status==1)
        //     {
        //        ret_json_all.push_back( Pair("order_status", "unshipped"));
        //     }
        //     else if(status==2)
        //     {
        //         ret_json_all.push_back( Pair("order_status", "do_not_ship"));
        //     }
        //     else
        //     {
        //         ret_json_all.push_back( Pair("order_status", "unconfirmed"));
        //     }  
        //     ret_json_all.push_back( Pair("order_notes", note));
        //     ret_json_all.push_back( Pair("internal_notes", "test internal"));
        //     ret_json_all.push_back( Pair("bill_third_party", false));
        //     ret_json_all.push_back( Pair("distribution_center_id", dispatch_warehouse_id));//need get from orderbot
        //     ret_json_all.push_back( Pair("account_group_id", "null"));//need get from orderbot
        //     ret_json_all.push_back( Pair("order_guide_id", "null"));//need get from orderbot
        //     ret_json_all.push_back( Pair("insure_packages", false));//not sure
        //     ret_json_all.push_back( Pair("shipping_code", "A1"));//need get from orderbot
        //     ret_json_all.push_back( Pair("email_confirmation_address", "test@orderbot.com"));
        //     ret_json_all.push_back( Pair("subtotal", sub_total));
        //     ret_json_all.push_back( Pair("shipping", shipping_cost_total));
        //     ret_json_all.push_back( Pair("order_discount", 0));
        //     ret_json_all.push_back( Pair("order_total", grand_total));

        //     json_spirit::Object shipping_tax;
        //     shipping_tax.push_back( Pair("tax_name","TAX"));
        //     shipping_tax.push_back( Pair("tax_rate",0.05));
        //     shipping_tax.push_back( Pair("amount",0.15));
              
        //     //ret_json_all.push_back(std::make_pair("shipping_tax", shipping_tax));

        //     json_spirit::Object shipping_address;
        //     shipping_address.push_back( Pair("tax_name","TAX"));
            
        //     shipping_address.push_back( Pair("store_name", "Test Store"));
        //     shipping_address.push_back( Pair("first_name", ship_to_contact_name));
        //     shipping_address.push_back( Pair("last_name", "x"));
        //     shipping_address.push_back( Pair("address1", ship_to_address));
        //     shipping_address.push_back( Pair("address2", ""));
        //     shipping_address.push_back( Pair("city", ship_to_city));
        //     shipping_address.push_back( Pair("state", ship_to_state));
        //     shipping_address.push_back( Pair("postal_code",ship_to_zip_code));
        //     shipping_address.push_back( Pair("country", "US"));
        //     shipping_address.push_back( Pair("phone_number", ship_to_contact_phone_number));
        //     shipping_address.push_back( Pair("email",ship_to_contact_email));

        //     ret_json_all.push_back(Pair("shipping_address", shipping_address));

        //     json_spirit::Object billing_address;
        //     billing_address.push_back( Pair("tax_name","TAX"));
            
        //     billing_address.push_back( Pair("store_name", "Test Store"));
        //     billing_address.push_back( Pair("first_name", ship_to_contact_name));
        //     billing_address.push_back( Pair("last_name", "x"));
        //     billing_address.push_back( Pair("address1", ship_to_address));
        //     billing_address.push_back( Pair("address2", ""));
        //     billing_address.push_back( Pair("city", ship_to_city));
        //     billing_address.push_back( Pair("state", ship_to_state));
        //     billing_address.push_back( Pair("postal_code",ship_to_zip_code));
        //     billing_address.push_back( Pair("country", "US"));
        //     billing_address.push_back( Pair("phone_number", ship_to_contact_phone_number));
        //     billing_address.push_back( Pair("email",ship_to_contact_email));
            
        //     ret_json_all.push_back(Pair("shipping_address", billing_address));
        //     //ret_json_all.push_back(Pair("order_lines", order_lines));
            
        }//else
        // cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
        // m_ss=write(ret_json_all);
    }
    catch(json_spirit::Error_position& e)
    {
        cout<<e.reason_<<":"<<__FILE__<<":"<<__LINE__<<endl;
        BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)"<<":"<<__FILE__<<":"<<__LINE__<<":" << e.reason_;
        boost_log->get_initsink()->flush();//cout<<e.what()<<endl;
    }
    catch(std::exception& e)
    {
        BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)"<<":"<<__FILE__<<":"<<__LINE__<<":" << e.what();
        boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
    }
    }
    void decode_request_orderbot(const string& texts)
    {
    	try
    	{
            parser_json_write_ss(texts);
            // string request_content="["+m_ss.str()+"]";
            // request_content=replace_all_distinct(request_content,"\"null\"","null");
            // request_content.erase(remove(request_content.begin(), request_content.end(), '\n'), request_content.end());
            // request_content=replace_all_distinct(request_content,"\"false\"","false");
            // request_content=replace_all_distinct(request_content,"\"true\"","true");
            cout<<m_ss<<":"<<__FILE__<<":"<<__LINE__<<endl;
			
            boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			order->request("POST", "/admin/orders.json/", "",m_ss);

			// return_json.put<std::string>("orders1",*(order->m_data));
			
			// write_json(m_ss,return_json);
            // 
            
	        m_ss_tomq<<*(order->m_data);
    		send_message_to_activemq();
			cout<<__FILE__<<":"<<__LINE__<<endl;   

    	}
    	catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
				boost_log->get_initsink()->flush();
				cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
        }
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
    }
    void send_message_to_activemq()
	{
		try
		{
			cout<<__FILE__<<":"<<__LINE__<<endl;
			string message(m_ss_tomq.str());
			message.erase(remove(message.begin(), message.end(), '\n'), message.end());
            m_ss_tomq.str("");
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
			cout<<__FILE__<<":"<<__LINE__<<endl;
		    //activemq::library::ActiveMQCPP::shutdownLibrary();
	    }
	    catch(json_parser_error& e) 
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
				boost_log->get_initsink()->flush();
				cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch (CMSException& e) 
        {
            BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
        }
		catch (const MySqlException& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
		catch(std::exception& e)
		{
			BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:)" << e.what()<<":"<<__FILE__<<":"<<__LINE__;
			boost_log->get_initsink()->flush();cout<<e.what()<<":"<<__FILE__<<":"<<__LINE__<<endl;
		}
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
#endif

