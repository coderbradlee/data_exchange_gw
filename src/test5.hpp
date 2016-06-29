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
	void test()
	{
        //test_design_model_template_method::test();
        test_design_model_strategy::test();
	}

}
#endif

