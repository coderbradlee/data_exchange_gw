#ifndef TEST5_HPP
#define	TEST5_HPP
#include "serverResource.hpp"
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include <bitset>
namespace x5
{
	namespace test_design_model_template_method
    {
        class library
        {
        public:
            void run()
            {
                step1();

                if (step2()) 
                { //支持变化 ==> 虚函数的多态调用
                    step3(); 
                }

                for (int i = 0; i < 4; i++)
                {
                    step4(); //支持变化 ==> 虚函数的多态调用
                }

                step5();
            }
            virtual ~library()
            {}
        protected:
            virtual void step1()
            {
                cout<<"base step1"<<endl;
            }
            virtual void step3()
            {
                cout<<"base step3"<<endl;
            }
            virtual void step5()
            {
                cout<<"base step5"<<endl;
            }
            virtual bool step2()=0;
            virtual void step4()=0;
        };
        class aplication:public library
        {
        public:
            ~aplication(){}
            bool step2()
            {
                cout<<"aplication step2"<<endl;
                return true;
            }
            void  step4()
            {
                cout<<"aplication step4"<<endl;
            }
        };
        void test()
        {
            boost::shared_ptr<aplication> t(new aplication());
            t->run();
        }
    }  
    namespace  test_design_model_strategy
    {
        class tax
        {
        public:
            virtual ~tax(){}
        
            virtual double caculate_tax()=0;

        };
        class cn_tax:public tax
        {
        public:
            double caculate_tax()
            {
                return 10.234;
            }
        };
        class sales_order
        {
        public:
            sales_order(boost::shared_ptr<tax> t):m_tax(t)
            {}
            ~sales_order(){}
            double caculate_tax()
            {
                return m_tax->caculate_tax();
            }
        private:
            boost::shared_ptr<tax> m_tax;
        };
        void test()
        {
            boost::shared_ptr<tax> t(new cn_tax());
            boost::shared_ptr<sales_order> s(new sales_order(t));
            cout<<s->caculate_tax()<<endl;
        }
    }
    namespace test_design_model_observer
    {
        class i_progress
        {
        public:
            virtual void do_progress(int ratio)=0;
            virtual ~i_progress(){}
        };
        class dot_progress:public i_progress
        {
        public:
            void do_progress(int ratio)
            {
                //cout<<ratio<<endl;
                for(size_t i=0;i<ratio;++i)
                cout<<".";
                cout<<endl;
            }
        };
        class pound_progress:public i_progress
        {
        public:
            void do_progress(int ratio)
            {
                //cout<<ratio<<endl;
                for(size_t i=0;i<ratio;++i)
                cout<<"#";
                cout<<endl;
            }
            
        };
        class file_split
        {
        public:
            file_split(int file_number):m_file_number(file_number){}
            void split()
            {
                for(size_t i=0;i<m_file_number;++i)
                {
                    this_thread::sleep_for(chrono::seconds(2));
                    double temp=(i+1)/m_file_number;
                    on_progress(temp);
                }
            }
            void add_iprogress(boost::shared_ptr<i_progress> i)
            {
                m_i_progress.push_back(i);
            }
            void remove_iprogress(boost::shared_ptr<i_progress> i)
            {
                m_i_progress.remove(i);
            }
        private:
            void on_progress(double ratio)
            {
                cout<<ratio<<endl;
                for(const auto& i:m_i_progress)
                {
                    i->do_progress(ratio*m_file_number);
                }
            }
            std::list<boost::shared_ptr<i_progress>> m_i_progress;
            int m_file_number;
        };
        void test()
        {
            boost::shared_ptr<file_split> f(new file_split(10));
            boost::shared_ptr<i_progress> d(new dot_progress());
            boost::shared_ptr<i_progress> p(new pound_progress());
            f->add_iprogress(d);
            f->add_iprogress(p);
            f->split();
        }
    }
	void test()
	{
        //test_design_model_template_method::test();
        //test_design_model_strategy::test();
        test_design_model_observer::test();
	}

}
#endif

