#ifndef TEST4_HPP
#define	TEST4_HPP
#include "serverResource.hpp"
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include <bitset>
namespace x3
{
	namespace test_for_each
    {
        class sum
        {
        public:
            sum():m_sum(0){}
            void operator()(int n)
            {
                m_sum+=n;
            }
       
            int m_sum;
        };
        void test()
        {
            std::vector<int> v{3,4,2,8,15,267};
            for(const auto &n:v)
            {
                cout<<' '<<n;
            }
            cout<<endl;
            std::for_each(v.begin(),v.end(),[](int& n){n++;});
            for(const auto&n:v)
            {
                cout<<' '<<n;
            }
            cout<<endl;
            sum s=std::for_each(v.begin(),v.end(),sum());
            for(const auto&n:v)
            {
                cout<<' '<<n;
            }
            cout<<endl<<s.m_sum<<endl;

            auto it=std::find_if(v.begin(),v.end(),std::bind(std::greater<int>(),std::placeholders::_1,10);
                cout<<*it<<endl;
        }
    }
	void test()
	{

        test_for_each::test();
	}

}
#endif	/* PAYPAL_HPP */

