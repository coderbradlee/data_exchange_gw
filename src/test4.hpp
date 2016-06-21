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
            // std::vector<int> v{3,4,2,8,15,15,267};
            // std::vector<int> v2{3,8};
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
            // std::vector<int> v3(v);
            
            // std::copy_backward(v3.begin(),v3.end()-2,v3.end());
            // for(const auto& n:v3)
            // {
            //     cout<<n<<endl;
            // }
            // std::vector<int> from_vector;
            // for (int i = 0; i < 10; i++) 
            // {
            //     from_vector.push_back(i);
            // }
         
            // std::vector<int> to_vector(15);
         
            // std::copy_backward(from_vector.begin(), from_vector.end(), to_vector.end());
         
            // std::cout << "to_vector contains: ";
            // for (unsigned int i = 0; i < to_vector.size(); i++) 
            // {
            //     std::cout << to_vector[i] << " ";
            // }
            // std::vector<int> v{1,2,3,4,5};
            // std::vector<int> v2{11,22,33,44};
            // std::swap(v,v2);
            // for(const auto& n:v)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // std::swap_ranges(v.begin(),v.end(),v2.begin());
            // for(const auto& n:v)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // std::vector<int> v{1,2,3,4};
            // std::vector<int> v2(v.size());
            // std::transform(v.begin(),v.end(),v2.begin(),std::negate<int>());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // std::vector<int> v1{1,2,3};
            // std::vector<int> v2{10,20,30};
            // std::transform(v1.begin(),v1.end(),v2.begin(),v2.begin(),std::plus<int>());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // std::replace_if(v2.begin(),v2.end(),std::bind(std::greater_equal<int>(),std::placeholders::_1,20),0);
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // std::vector<int> v1{1,2,3,4};
            // std::vector<int> v2(v1.size());
            // std::replace_copy_if(v1.begin(),v1.end(),v2.begin(),std::bind(std::greater_equal<int>(),std::placeholders::_1,3),0);
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            std::vector<int> v(5);
            std::fill(v.begin(),v.end(),10);
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
            std::vector<int> v2(5);
            std::generate(v2.begin(),v2.end(),rand);
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            reverse(v2.begin(),v2.end());
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            rotate(v2.begin(),v2.begin()+2,v2.end());
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            random_shuffle(v2.begin(),v2.end());
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            partition(v2.begin(),v2.end(),bind(std::less<int>(),std::placeholders::_1,1804289383));
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            sort(v2.begin(),v2.end());
            for(const auto& n:v2)
            {
                cout<<n<<" ";
            }
            cout<<endl;
        }
    }
	void test()
	{

        test_for_each::test();
	}

}
#endif	/* PAYPAL_HPP */

