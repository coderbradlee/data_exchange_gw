﻿#ifndef TEST4_HPP
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
        template<typename T>
        struct search_double
        {
            bool operator()(const T& l,const T& r)
            {
                return r==l*2;
            }
        };
        void test()
        {
            // std::vector<int> v{3,4,2,8,15,15,267};
            // for(const auto &n:v)
            // {
            //     cout<<' '<<n;
            // }
            // cout<<endl;
            // std::for_each(v.begin(),v.end(),[](int& n){n++;});
            // for(const auto&n:v)
            // {
            //     cout<<' '<<n;
            // }
            // cout<<endl;
            // sum s=std::for_each(v.begin(),v.end(),sum());
            // for(const auto&n:v)
            // {
            //     cout<<' '<<n;
            // }
            // cout<<endl<<s.m_sum<<endl;

            // auto it=std::find_if(v.begin(),v.end(),std::bind(std::greater<int>(),std::placeholders::_1,10));
            // cout<<*it<<endl;
            // int n=std::count(v.begin(),v.end(),16);
            // cout<<n<<endl;
            // n=std::count_if(v.begin(),v.end(),std::bind(std::less<int>(),std::placeholders::_1,10));
            // cout<<n<<endl;
            std::vector<int> v{3,4,2,8,15,15,267};
            std::vector<int> v2{3,8};
            // typedef std::vector<int>::iterator it_type;
            // std::pair<it_type,it_type> p=std::mismatch(v.begin(),v.end(),v2.begin(),v2.end());
            // cout<<*p.first<<":"<<*p.second<<endl;
            // cout<<std::equal(v.begin(),v.end(),v2.begin(),v2.end())<<endl;
            // cout<<std::equal(v.begin(),v.begin()+4,v2.begin(),v2.begin()+4)<<endl;
            // auto it=std::search(v.begin(),v.end(),v2.begin(),v2.end(),search_double<int>());
            // cout<<*it<<endl;
            // std::vector<int> v3(v.size());
            // std::copy_n(v.begin(),4,v3.begin());
            // for(const auto& n:v3)
            // {
            //     cout<<n<<endl;
            // }
            std::vector<int> v3(v1);
            std::copy_backward(v3.begin(),v3.end()-1,v2.end());
            for(const auto& n:v3)
            {
                cout<<n<<endl;
            }
        }
    }
	void test()
	{

        test_for_each::test();
	}

}
#endif	/* PAYPAL_HPP */

