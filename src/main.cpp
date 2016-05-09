#include "config.hpp"
#include "orderbot.hpp"
#include "credit_ontime.hpp"
#include "activemq.hpp"
#include "orderbot_restserver_resource.hpp"
#include "activemq_producer_consumer.hpp"
#include "boost_signalset.hpp"
void test()
{
	using json = nlohmann_fifo_map::json;
	 json j =
    {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {
            "answer", {
                {"everything", 42}
            }
        },
        {"list", {1, 0, 2}},
        {
            "object", {
                {"currency", "USD"},
                {"value", 42.99}
            }
        }
    };

    // add new values
    j["new"]["key"]["value"] = {"another", "list"};

    // count elements
    j["size"] = j.size();

    // pretty print with indent of 4 spaces
    std::cout << j << '\n';
}
namespace brad
{
	class Date
	{
		public:
		Date(int year,int month,int day):m_year(year),m_month(month),m_day(day)
		{
		
		}
		int get_year() const
		{
			return m_year;
		}
		int get_month() const
		{
			return m_month;
		}
		int get_day() const
		{
			return m_day;
		}
		void print() const
		{
			cout<<m_year<<"-"<<setw(2)<< setfill ('0')<<m_month<<"-"<<setw(2)<< setfill ('0')<<m_day<<endl;
		}
		private:
		int m_year;
		int m_month;
		int m_day;	
	};

	inline bool operator<(const Date& l,const Date& r)
	{
		return 	(l.get_year()<r.get_year())||
				(l.get_month()<r.get_month())||
				(l.get_day()<r.get_day());
	}
	inline bool operator==(const Date& l,const Date& r)
	{
		return (l.get_year()==r.get_year())&&(l.get_month()==r.get_month())&&(l.get_day()==r.get_day());
	}
	inline bool operator>(const Date& l,const Date& r)
	{
		return ~(l<r||l==r);
	}
	int get_random_int(int a, int b) 
	{  
	    static std::default_random_engine e{std::random_device{}()}; 
	    static std::uniform_int_distribution<int> u;  
	  
	    return u(e, decltype(u)::param_type(a, b));  
	}
	 class comp
	 {
	 public:
	     bool operator() (const Date &a , const Date  &b) const
	     {
	         return a<b;
	     }
	}; 
	std::vector<Date> CreatePoints()
	{
		std::vector<Date> v;
		for(int i=0;i<10;++i)
		{
			v.push_back(Date(get_random_int(1900,2099),get_random_int(1,12),get_random_int(1,31)));
		}
		return v;
	}
	void Sort(std::vector<Date>& v)
	{
		std::map<Date,int,comp> m;
		int i=0;
		for(auto& date:v)
		{
			m.emplace(std::make_pair(date,i++));
		}
		v.clear();

		for(auto& date:m)
		{
			v.push_back(date.first);
		}
	} 
}
int main()
{
	for(int i=1;i<64;++i)
	std::signal(i, sig_handler);  
	//std::raise(SIGINT);
	try
	{
		{
			std::vector<brad::Date> v=brad::CreatePoints();
			for(auto& d:v)
			{
				d.print();
			}
			cout<<"--------------------"<<endl;
			brad::Sort(v);
			for(auto& d:v)
			{
				d.print();
			}
		}
		{
			//test();
			// credit_ontime test;
			// test.start_update();
		}
		{	
			boost::shared_ptr<activemq_producer_consumer> producer_consumer(new activemq_producer_consumer);
			thread producer_consumer_thread([&producer_consumer](){producer_consumer->start();});
			producer_consumer_thread.join();
		}
		{
			//orderbot server
			//orderbot_server_start();
		}
		
		
		{
			boost::timer::cpu_timer pass;
			pass.start();

			//orderbot 接口
			boost::shared_ptr<orderbot> order = boost::shared_ptr<orderbot>(new orderbot(get_config->m_orderbot_username, get_config->m_orderbot_password, get_config->m_orderbot_url));
			// order->request("GET", "/admin/products.json/", "class_type=sales&category_name=Rings", "");

			// cout<<order->get_data().length()<<":"<<order->get_data()<<endl;
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}
		{
			boost::timer::cpu_timer pass;
			pass.start();

			//orderbot 接口
			boost::shared_ptr<activemq_rest> am = boost::shared_ptr<activemq_rest>(new activemq_rest(get_config->m_activemq_username, get_config->m_activemq_password, get_config->m_activemq_url));
			//am->request("POST", "/api/message/TEST", "type=queue", "body={message:\"test\"}");
			//cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			//am->request("GET", "/api/message/TEST", "type=queue&clientId=consumerA", "");
			//cout<<am->get_data().length()<<":"<<am->get_data()<<endl;
			this_thread::sleep_for(chrono::seconds(10));
			std::cout << "now time elapsed:" << pass.format(6) << std::endl;
		}

	}
	catch (std::exception& e)
	{
		//cout << diagnostic_information(e) << endl;
		cout << e.what() << endl;
	}
	catch (...)
	{

	}
	return 0;
}