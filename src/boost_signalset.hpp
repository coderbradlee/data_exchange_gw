#ifndef BOOST_SIGNALSET_HPP
#define	BOOST_SIGNALSET_HPP
#include "include.hpp"
#include "sigslot.hpp"
//boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
//signals.add(SIGUSR1); // 也可以直接用add函数添加信号
// boost::asio::signal_set signal_set_all(SIGTERM);
// signal_set_all.async_wait(boost::bind(handler, _1, _2));

// void handler(
//   const boost::system::error_code& error,
//   int signal_number // 通过这个参数获取当前触发的信号值
// )
// {
// 	BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal)" <<signal_number<<",error:"<<error ;
// 	boost_log->get_initsink()->flush();
// 	cout<<"(exception:signal)" <<signal_number<<",error:"<<error<<endl;
// }
void sig_handler (int signal_number)
{
	BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal)" <<signal_number;
	boost_log->get_initsink()->flush();
	cout<<"(exception:signal)" <<signal_number<<endl;
	exit(signal_number);  
}
void signal_handler(const boost::system::error_code & err, int signal_number)
{
  BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal)" <<signal_number;
	boost_log->get_initsink()->flush();
	cout<<"(exception:signal)" <<err<<":"<<signal_number<<endl;
	exit(signal_number);  
}
void register_signal()
{
	// mp::signal<const char*> sig;
 //    CCC a;
 //    {
 //        mp::scoped_connection conn = sig.connect(&CCC::fn1, &a);
 //        auto conn2 = sig.connect(&CCC::fn, &a);
 //        sig("step 1");
 //        conn2.disconnect();
 //        sig("step 2");
 //        sig.connect(&CCC::fn, &a);
 //        sig("step 3");
 //        sig.disconnect(&CCC::fn, &a);
 //        sig("step 4");
 //    }
 //    sig("step 5");
	//boost::asio::io_service service;
    for(int i=1;i<32;++i)
	{
		std::signal(i, sig_handler);  
		// 	boost::asio::signal_set sig(service, i, i+16);
		// sig.async_wait(signal_handler);
	}

}


#endif	/* BOOST_SIGNALSET_HPP */

