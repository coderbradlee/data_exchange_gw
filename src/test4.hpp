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
           //  std::vector<int> v{3,2,1};
           //  int count=0;
           //  do
           //  {
           //      cout<<++count<<":";
           //      copy(v.begin(),v.end(),std::ostream_iterator<int>(cout,","));
           //      cout<<endl;
           //  }while(prev_permutation(v.begin(),v.end()));

           //  cout<<"------------------"<<endl;
           //  //print<int>(v);
           //  ////////////////////////////////////////////
           //  std::vector<int> v1{1,2,3};
           //  count=0;
           //  do
           //  {
           //      cout<<++count<<":";
           //      copy(v1.begin(),v1.end(),std::ostream_iterator<int>(cout,","));
           //      cout<<endl;
           //  }while(next_permutation(v1.begin(),v1.end()));

           //  cout<<"------------------"<<endl;
           //  //print<int>(v);
           //  ///////////////////////////////////////////
           //  std::vector<char> v2{'a','a','b','b','b'};
           //  count=0;
           //  do
           //  {
           //      cout<<++count<<":";
           //      copy(v2.begin(),v2.end(),std::ostream_iterator<char>(cout,","));
           //      cout<<endl;
           //  }while(prev_permutation(v2.begin(),v2.end()));
           //  cout<<"------------------"<<endl;
           // // print<char>(v2);
            std::vector<int> v{1,2,3,4};
            std::vector<int> selector{1,1,0,0};
            int count=0;
            do
            {
                cout<<++count<<":";
                for(int i=0;i<4;++i)
                {
                    if(selector[i]==1)
                        cout<<v[i]<<",";
                }
                cout<<endl;
            }while(prev_permutation(selector.begin(),selector.end()));
        }
    }
    namespace test_unique
    {

        void test()
        {
            string str="a  b   c     d";
            auto it=unique(str.begin(),str.end(),[](char x,char y){return x==' '&&y==' ';});
            str.erase(it,str.end());
            cout<<str<<endl;
            std::vector<int> v{1,2,3,4,5,6};
            stable_partition(v.begin(),v.end(),[](int i){return i%2==0;});
            copy(v.begin(),v.end(),ostream_iterator<int>(cout,","));
            cout<<endl;
        }
    }
    namespace test_merge_n
    {
        typedef int record;
        typedef std::vector<record> File;
        struct input
        {
            record value;
            size_t index;
            const File* file;
            explicit input(const File* f):value(-1),index(0),file(f)
            {
            }
            bool next()
            {
                if(index<file->size())
                {
                    value=(*file)[index];
                    ++index;
                    return true;
                }
                else
                    return false;
            }
            bool operator<(const input& r)const
            {
                return value<r.value;
            }
        };
        File merge_n(const std::vector<File>& files)
        {
            File output;
            std::vector<input> inputs;
            for(size_t i=0;i<files.size();++i)
            {
                input in(&files[i]);
                if(in.next())
                {
                    inputs.push_back(in);
                }
            }
            make_heap(inputs.begin(),inputs.end());
            while(!inputs.empty())
            {
                pop_heap(inputs.begin(),inputs.end());
                output.push_back(inputs.back().value);
                if(inputs.back().next())
                {
                    push_heap(inputs.begin(),inputs.end());
                }
                else
                    inputs.pop_back();
            }
            return output;
        }
        void test()
        {
            const int k_files=32;
            std::vector<File> files(k_files);
            for(size_t i=0;i<k_files;++i)
            {
                File file(rand()%1000);
                generate(file.begin(),file.end(),&rand);
                sort(file.begin(),file.end());
                files[i].swap(file);
            }
            File out=merge_n(files);
            copy(out.begin(),out.end(),ostream_iterator<record>(cout," "));
        }
    }
    namespace test_ip_range
    {
        struct  ip_range
        {
            uint32_t start;
            uint32_t end;
            int value;
            bool operator<(const ip_range& r)const
            {
                return start<r.start;
            }
        };
        int find_ip(const std::vector<ip_range>& ranges,uint32_t ip)
        {
            int result=-1;
            if(!ranges.empty())
            {
                ip_range needle={ip,0,0};
                auto it=lower_bound(ranges.begin(),ranges.end(),needle);
                if(it==ranges.end())
                {
                    --it;
                }
                else if(it!=ranges.begin()&&it->start>ip)
                {
                    --it;
                }
                if(it->start<ip&&it->end>=ip)
                {
                    result=it->value;
                }

            }
            return result;
        }
        bool operator==(const ip_range& l,const ip_range& r)
        {
            return l.start==r.start;
        }
        void test()
        {
            std::vector<ip_range> ranges;
            ip_range r1={123,234,999};
            ranges.push_back(r1);
            sort(ranges.begin(),ranges.end());
            int v=find_ip(ranges,0);
            cout<<v<<endl;
            v = find_ip(ranges, 122);
            cout<<v<<endl;

              v = find_ip(ranges, 123);
            cout<<v<<endl;

              v = find_ip(ranges, 234);
            cout<<v<<endl;

              v = find_ip(ranges, 235);
            cout<<v<<endl;

              ip_range r2 = { 1000, 2000, 7777 };
              ranges.push_back(r2);

              sort(ranges.begin(), ranges.end());
              assert(adjacent_find(ranges.begin(), ranges.end()) == ranges.end());

              v = find_ip(ranges, 0);
            cout<<v<<endl;

              v = find_ip(ranges, 122);
            cout<<v<<endl;

              v = find_ip(ranges, 123);
            cout<<v<<endl;

              v = find_ip(ranges, 234);
            cout<<v<<endl;

              v = find_ip(ranges, 235);
            cout<<v<<endl;

              v = find_ip(ranges, 999);
            cout<<v<<endl;

              v = find_ip(ranges, 1000);
            cout<<v<<endl;

              v = find_ip(ranges, 1500);
            cout<<v<<endl;

              v = find_ip(ranges, 2000);
            cout<<v<<endl;

              v = find_ip(ranges, 2001);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824);
            cout<<v<<endl;

              ip_range r3 = { 1073741824U*3, 1073741824U*3+1073741823U, 5555 };
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*2);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*3);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*3+1073741822U);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*3+1073741823U);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*3+1073741824U);
            cout<<v<<endl;

              ip_range r4 = { 1073741824U*3+1073741823U, 1073741824U*3+1073741823U, 3333 };
              ranges.push_back(r4);

              v = find_ip(ranges, 1073741824U*3+1073741822U);
            cout<<v<<endl;

              v = find_ip(ranges, 1073741824U*3+1073741823U);
            cout<<v<<endl;
        }
    }
    namespace test_static_const
    {
        class init
        {
            static int d=0;
            static const int i=0;
            static const double e=1.0f;
            static const char* const f="e";
        };
    }
	void test()
	{
        //test_allocator::test();
        //test_for_each::test();
        //test_stl::test();
        //test_permutation::test();
        //test_unique::test();
        //test_merge_n::test();
        test_ip_range::test();
	}

}
#endif	/* PAYPAL_HPP */

