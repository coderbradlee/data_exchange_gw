#ifndef BOOST_SIGNALSET_HPP
#define	BOOST_SIGNALSET_HPP
#include "include.hpp"
#include "sigslot.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
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
#define PRINT_DEBUG

static void print_reason(int sig)
{
    void *array[10];
    size_t size;
    size = backtrace(array, 10);
#ifdef PRINT_DEBUG
    char **strings;
    int i;
    strings = backtrace_symbols(array, size);
    printf("Obtained %d stack frames.\n", size);
    
    BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal,stack frames)" <<size;
	boost_log->get_initsink()->flush();
	

    for (i = 0; i < size; i++)
	 {
	 	printf("%s\n", strings[i]);
	 	BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal)" <<strings[i];
	 }   
	 boost_log->get_initsink()->flush();
    free(strings);

#endif
    BOOST_LOG_SEV(slg, severity_level::error) <<"(exception:signal)" <<sig;
	boost_log->get_initsink()->flush();
	cout<<"(exception:signal)"<<":"<<sig<<endl;
	exit(sig);  
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
    //for(int i=1;i<32;++i)
	{
		//std::signal(i, sig_handler);  
		// 	boost::asio::signal_set sig(service, i, i+16);
		// sig.async_wait(signal_handler);
	}
	struct sigaction myAction;
    myAction.sa_handler = print_reason;
    sigemptyset(&myAction.sa_mask);
    myAction.sa_flags = SA_RESTART | SA_SIGINFO;
    // sigaction(SIGSEGV, &myAction, NULL); // 无效内存引用
    // sigaction(SIGABRT, &myAction, NULL); // 异常终止
    for(int i=1;i<32;++i)
    {
    	sigaction(i, &myAction, NULL);
    }
    //sigaction(SIGINT, &myAction, NULL);

}

void die()
{
    char *test1;
    char *test2;
    char *test3;
    char *test4 = NULL;
    strcpy(test4, "ab");
}
void test1()
{
    die();
}
#endif	/* BOOST_SIGNALSET_HPP */

