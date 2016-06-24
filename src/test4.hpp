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
        void test1()
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
            // std::vector<int> v(5);
            // std::fill(v.begin(),v.end(),10);
            // for(const auto& n:v)
            // {
            //     cout<<n<<" ";
            // }cout<<endl;
            // std::vector<int> v2{1,2,3,4,5};
            // //std::generate(v2.begin(),v2.end(),rand);
            // for(const auto& n:v2)
            // {
            //     //cout<<n<<" ";
            // }
            // cout<<endl;
            // // reverse(v2.begin(),v2.end());
            // // for(const auto& n:v2)
            // // {
            // //     cout<<n<<" ";
            // // }
            // // cout<<endl;
            // // rotate(v2.begin(),v2.begin()+2,v2.end());
            // // for(const auto& n:v2)
            // // {
            // //     cout<<n<<" ";
            // // }
            // // cout<<endl;
            // random_shuffle(v2.begin(),v2.end());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // partition(v2.begin(),v2.end(),bind(std::less<int>(),std::placeholders::_1,4));
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // // sort(v2.begin(),v2.end());
            // // for(const auto& n:v2)
            // // {
            // //     cout<<n<<" ";
            // // }
            // // cout<<endl;
            // partial_sort(v2.begin(),v2.begin()+2,v2.end());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // make_heap(v2.begin(),v2.end());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }
            // cout<<endl;
            // // std::vector<int> v3{66,77,99};
            
            // // copy_backward(v3.begin(), v3.end(), v2.end());
            // v2.push_back(200);
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }cout<<endl;
            // push_heap(v2.begin(),v2.end());
            // for(const auto& n:v2)
            // {
            //     cout<<n<<" ";
            // }cout<<endl;
            int elem[]={100,90,99,70,80,30,45,20,35,10,95};
            const int n=sizeof(elem)/sizeof(int);
            std::vector<int> v(elem,elem+n-1);
            
            make_heap(v.begin(),v.end());
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
            v.push_back(95);
            push_heap(v.begin(),v.end());
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
            //v.erase(v.begin()+1);
            pop_heap(v.begin(),v.end());
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
            v.erase(v.begin()+v.size()-1);
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
            sort_heap(v.begin(),v.end());
            for(const auto& n:v)
            {
                cout<<n<<" ";
            }cout<<endl;
        }
        void test()
        {
            // std::vector<int> v(100);
            // std::iota(v.begin(),v.end(),1);
            // cout<<accumulate(v.begin(),v.end(),0)<<endl;
            // std::vector<int> v(10);
            // std::iota(v.begin(),v.end(),1);
            // cout<<std::accumulate(v.begin(),v.end(),1,std::multiplies<int>())<<endl;
            std::vector<int> v1{1,2,3};
            std::vector<int> v2{4,5,6};
            cout<<inner_product(v1.begin(),v1.end(),v2.begin(),0)<<endl;
            cout<<inner_product(v1.begin(),v1.end(),v2.begin(),1,std::multiplies<int>(),std::plus<int>())<<endl;
            partial_sum(v1.begin(),v1.end(),v1.begin());
            for(const auto& n:v1)
            {
                cout<<n<<" ";
            }
            cout<<endl;
            adjacent_difference(v1.begin(),v1.end(),v1.begin(),multiplies<int>());
            for(const auto& n:v1)
            {
                cout<<n<<" ";
            }
            cout<<endl;
        }
    }
    namespace test_allocator
    {
        template<typename T>
        class allocator
        {
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef int difference_type;
            template<typename U>
            struct  rebind
            {
                typedef allocator<U> other;
            };
            allocator()
            {
                cout<<"allocator construct"<<endl;
            }
            allocator(allocator<T> const&)
            {
                cout<<"allocator copy construct"<<endl;
            }
            template<typename U>
            allocator(allocator<U> const&)
            {
                cout<<"allocator template copy construct"<<endl;
            }
            pointer allocate(size_type n,const void* p=nullptr)
            {
                T* b=(T*) ::operator new((size_t)(n*sizeof(T)));
                cout<<hex<<(size_t)b<<":"<< n*sizeof(T)<<endl;
                return b;
            }
            void deallocate(pointer (p),size_type n)
            {
                if(p!=nullptr)
                {
                    ::operator delete(p);
                    cout<<"operator delete"<<endl;
                }
            }
            void construct(pointer p,const T& v)
            {
                new(p) T(v);
                cout<<"placement new"<<endl;
            }
            void destroy(pointer p,size_type n)
            {
                p->~T();
                cout<<"destroy"<<endl;
            }
            size_type max_size()const
            {
                return size_type(1000);
            }
            pointer address(reference x)
            {
                return (pointer)&x;
            }
            const_pointer const_address(const_reference x)
            {
                return (const_pointer)&x;
            }
        };
        void test()
        {
            std::vector<int,allocator<int>> v{1,2,3};
            for_each(v.begin(),v.end(),[](int x){cout<<x<<endl;});

                string s("abc");
               string t;
               char & c(s[1]);

               t = s;   // Data typically shared between s and t.
               c = 'z';     // How many strings does this modify?
               if (t[1] == 'z') {
                    printf("wrong\n");
               } else {
                    printf("right\n");
               }
            cout<<t<<endl;
            cout<<s<<endl;
            cout<<sizeof(string)<<endl;
            string ss("adfaf");
            cout<<sizeof(ss)<<endl;
        }

    }
    namespace test_stl
    {
        void print(const std::vector<int>& v)
        {
            for(const auto& n:v)
                cout<<n<<" ";
            cout<<endl;
        }
        void test()
        {
            int x=3;
            int temp(x);
            std::vector<int> v;
            std::vector<int> v2;
            //transform(v.begin(),v.end(),back_inserter(v2),temp);
            print(v);
            print(v2);

        }
    }
    namespace test_permutation
    {
        template<typename T>
        void print(const std::vector<T>& v)
        {
            // for(const auto& n:v)
            //     cout<<n<<" ";
            copy(v.begin(),v.end(),std::ostream_iterator<T>(cout,","));
            cout<<endl<<"***************************"<<endl;
        }
        void test()
        {
            std::vector<int> v{3,2,1};
            int count=0;
            do
            {
                cout<<++count<<":";
                copy(v.begin(),v.end(),std::ostream_iterator<int>(cout,","));
                cout<<endl;
            }while(prev_permutation(v.begin(),v.end()));

            cout<<"------------------"<<endl;
            //print<int>(v);
            ////////////////////////////////////////////
            std::vector<int> v1{1,2,3};
            count=0;
            do
            {
                cout<<++count<<":";
                copy(v1.begin(),v1.end(),std::ostream_iterator<int>(cout,","));
                cout<<endl;
            }while(next_permutation(v1.begin(),v1.end()));

            cout<<"------------------"<<endl;
            //print<int>(v);
            ///////////////////////////////////////////
            std::vector<char> v2{'a','a','b','b','b'};
            count=0;
            do
            {
                cout<<++count<<":";
                copy(v2.begin(),v2.end(),std::ostream_iterator<char>(cout,","));
                cout<<endl;
            }while(prev_permutation(v2.begin(),v2.end()));
            cout<<"------------------"<<endl;
           // print<char>(v2);
            
        }
    }
	void test()
	{
        //test_allocator::test();
        //test_for_each::test();
        //test_stl::test();
        test_permutation::test();
	}

}
#endif	/* PAYPAL_HPP */

