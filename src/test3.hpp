#ifndef TEST3_HPP
#define	TEST3_HPP
#include "serverResource.hpp"
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include <bitset>
namespace x3
{
	namespace design_model
	{
    // namespace test_count_ref
    // {
    // 	class base
    // 	{
    // 	public:
    // 		base():m_ref_count(0),m_shareable(true)
    // 		{}
    // 		base(const base& r):m_ref_count(0),m_shareable(true)
    // 		{}
    // 		base& operator=(const base& r)
    // 		{
    // 			return *this;
    // 		}
    // 		virtual ~base()=0;
    // 		//{}
    // 		void add_ref()
    // 		{
    // 			++m_ref_count;
    // 			cout<<"add:"<<m_ref_count<<endl;
    // 		}
    // 		void remove_ref()
    // 		{
    // 			if(--m_ref_count==0) delete this;
    // 			cout<<"remove:"<<m_ref_count<<endl;
    // 		}
    // 		void mark_unshareable()
    // 		{
    // 			m_shareable=false;
    // 			cout<<"mark_unshareable"<<endl;
    // 		}
    // 		bool is_shareable()const
    // 		{
    // 			cout<<"is_shareable"<<endl;
    // 			return m_shareable;
    // 		}
    // 		bool is_shared()const
    // 		{
    // 			cout<<"is_shared"<<endl;
    // 			return m_ref_count>1;
    // 		}
    // 	private:
    // 		int m_ref_count;
    // 		bool m_shareable;
    // 	};
    // 	base::~base(){}
    // 	template<typename T>
    // 	class ptr
    // 	{
    // 	public:
    // 		ptr(T* p=nullptr):m_pointee(p)
    // 		{
    // 			init();
    // 		}
    // 		ptr(const ptr& r):m_pointee(r.m_pointee)
    // 		{
    // 			init();
    // 		}
    // 		~ptr()
    // 		{
    // 			if(m_pointee)
    // 				m_pointee->remove_ref();
    // 		}
    // 		ptr& operator=(const ptr& r)
    // 		{
    // 			if(m_pointee!=r.m_pointee)
    // 			{
    // 				if(m_pointee)
    // 				{
    // 					m_pointee->remove_ref();
    // 				}
    // 				m_pointee=r.m_pointee;
    // 				init();
    // 			}
    // 			return *this;
    // 		}
    // 		T* operator->()const
    // 		{
    // 			return m_pointee;
    // 		}
    // 		T& operator*()const
    // 		{
    // 			return *m_pointee;
    // 		}
    // 	private:
    // 		T* m_pointee;
    // 		void init()
    // 		{
    // 			if(m_pointee==nullptr)
    // 				return;
    // 			if(m_pointee->is_shareable()==false)
    // 				m_pointee=new T(*m_pointee);
    // 			m_pointee->add_ref();
    // 		}
    // 	};
    // 	class string
    // 	{
    // 	public:
    // 		class proxy
    // 		{
    // 		proxy(string& str,int index):m_string(str),m_char_index(index)
    // 		{

    // 		}
    // 		proxy& operator=(const proxy& r)
    // 		{
    // 			if(m_string.m_value->is_shared())
    // 			{
    // 				m_string.m_value=new string_data(m_string.m_value->data);
    // 			}
    // 			//m_string.m_value->mark_unshareable();
    // 			m_string.m_value->data[m_char_index]=r.m_string.m_value->data[r.m_char_index];
    // 			return *this;
    // 		}
    // 		proxy& operator=(char c)
    // 		{
				// if(m_string.m_value->is_shared())
    // 			{
    // 				m_string.m_value=new string_data(m_string.m_value->data);
    // 			}
    // 			//m_string.m_value->mark_unshareable();
    // 			m_string.m_value->data[m_char_index]=c;
    // 			return *this;
    // 		}
    // 		operator char()const
    // 		{
    // 			return m_string.m_value->data[m_char_index];
    // 		}
    // 	private:
    // 		string& m_string;
    // 		int m_char_index;
    // 		};
    // 		string(const char* v):m_value(new string_data(v))
    // 		{
    // 			cout<<"string constructor"<<endl;
    // 		}
    // 		const proxy operator[](int index)const
    // 		{
    // 			cout<<"string operator[]const"<<endl;
    // 			//return m_value->data[index];
    // 			return proxy(const_cast<string&>(*this),index);
    // 		}
    // 		proxy operator[](int index)
    // 		{
    // 			cout<<"string operator[]"<<endl;
    // 			// if(m_value->is_shared())
    // 			// {
    // 			// 	m_value=new string_data(m_value->data);
    // 			// }
    // 			// m_value->mark_unshareable();
    // 			// return m_value->data[index];
    // 			return proxy(*this,index);
    // 		}
    // 		// ostream operator<<(ostream& os)const
    // 		// {
    // 		// 	return os<<m_value->data<<endl;
    // 		// }
    // 		friend inline ostream & operator << (ostream & os, string &t1)
    // 		{  
		  //        cout << t1.m_value->data<< endl;  
		  //        return os; 
		  //   }
		  //   friend class proxy;
    // 	private:
    // 		struct  string_data:public base
    // 		{
    // 			char *data;
    // 			string_data(const char* v)
    // 			{
    // 				data=new char[strlen(v)+1];
    // 				strcpy(data,v);
    // 				cout<<"string_data constructor"<<endl;
    // 			}
    // 			string_data(const string_data& r)
    // 			{
    // 				data=new char[strlen(r.data)+1];
    // 				strcpy(data,r.data);
    // 				cout<<"string_data copy"<<endl;
    // 			}
    // 			~string_data()
    // 			{
    // 				delete[] data;
    // 				cout<<"~string_data"<<endl;
    // 			}
    // 		};
    // 		ptr<string_data> m_value;
    		
    // 	};
    // 	void test()
    // 	{
    // 		string x("hello");
    // 		cout<<"----------------------"<<endl;
    // 		string y=x;
    // 		cout<<"----------------------"<<endl;
    // 		string z("wo");
    // 		cout<<"----------------------"<<endl;
    // 		z=y;
    // 		cout<<"----------------------"<<endl;
    // 		z[1]='l';
    // 		cout<<"----------------------"<<endl;
    // 		cout<<x<<endl;
    // 		cout<<y<<endl;
    // 		cout<<z<<endl;
    // 	}
    // }
    namespace test_auto_ptr
    {
        template<typename T>
        class auto_ptr
        {
            template<typename U> friend class auto_ptr;
        public:
            explicit auto_ptr(T* p=nullptr):m_pointee(p)
            {}
            template<class U>
            auto_ptr(auto_ptr<U>& r):m_pointee(r.release())
            {}
            ~auto_ptr()
            {
                delete m_pointee;
            }
            template<class U>
            auto_ptr<T>& operator=(auto_ptr<U>& r)
            {
                if(this!=&r)
                    reset(r.release());
                return *this;
            }
            T& operator*()const
            {
                return *m_pointee;
            }
            T* operator->()const
            {
                return m_pointee;
            }
            T* get()const
            {
                return m_pointee;
            }
            T* release()
            {
                T* old=m_pointee;
                m_pointee=nullptr;
                return old;
            }
            void reset(T* p=nullptr)
            {
                if(m_pointee!=p)
                    delete m_pointee;
                m_pointee=p;
            }

        private:
            T* m_pointee;
            
        };
        class b
        {};
        class d:public b 
        {};     
        void test()
        {
            auto_ptr<int> p(new int(5));
            cout<<*p<<endl;
            auto_ptr<b> pb(new d());

        }
    }
    namespace test_operator_new_and_delete
    {
        class bad{};
        class  base
        {
        public:
             base():m_id(0)
             {
                cout<<"base default constructor,this="<<this<<" id="<<m_id<<endl;
             }
             base(int i):m_id(i)
             {
                cout<<"base constructor,this="<<this<<" id="<<m_id<<endl;
                throw bad();
             }
            ~ base()
            {
                cout<<"base destructor,this="<<this<<" id="<<m_id<<endl;
            }
            void *operator new(size_t size)
            {
                base* p=(base*)malloc(size);
                cout<<"operator new:"<<size<<endl;
                return p;
            }
            void *operator new[](size_t size)
            {
                base* p=(base*)malloc(size);
                cout<<"operator new[]:"<<size<<endl;
                return p;
            }
            void operator delete(void* v,size_t size)
            {
                cout<<"operator delete:"<<size<<endl;
                free(v);
            }
            void operator delete[](void* v,size_t size)
            {
                cout<<"operator delete[]:"<<size<<endl;
                free(v);
            }
            /////////////////////////
            void* operator new(size_t size,void *start)
            {
                return start;
            }
            void* operator new(size_t size,long extra)
            {
                return malloc(size+extra);
            }
            void* operator new(size_t size,long extra,char init)
            {
                return malloc(size+extra);
            }
            // void* operator new(long extra,char init)
            // {}
            
            void operator delete(void*,void*)
            {
                cout<<"operator delete(void*,void*)"<<endl;
            }
            void operator delete(void*,long)
            {
                cout<<"operator delete(void*,long)"<<endl;
            }
            void operator delete(void*,long,char)
            {
                cout<<"operator delete(void*,long,char)"<<endl;
            }
        private:
            int m_id;
            long m_data;
            string m_str;
        };
        void test()
        {
            // cout<<"sizeof string="<<sizeof(string)<<endl;
            // cout<<"sizeof base="<<sizeof(base)<<endl;
            // base* p=new base(7);
            // delete p;

            // base* p_array=new base[5];
            // delete[] p_array;

            // base* p=::new base(7);
            // ::delete p;
            // base* p_array=::new base[5];
            // ::delete[] p_array;
            base start;
            base* p1=new base;
            base* p2=new(&start)base;
            base* p3=new(100)base;
            base* p4=new(100,'a')base;
            //base* p5=new(100)base(1);
            //base* p6=new(100,'a')base(1);
            //base* p7=new(&start)base(1);
            base* p8=new base(1);
        }
    }
    namespace test_function_pointer
    {
        float func(int, int); 
        float func(int a, int b)
        {     
            return (float)(a + b); 
        }
        void test()
        {     
            float (*pf)(int, int);     
            pf = func;    
             printf("%fn", pf(1, 2));   
             cout<<endl;  
             printf("%.16Xn", (uint64_t)(pf));   cout<<endl;   
             printf("%.16Xn", (uint64_t)(func));  cout<<endl;    
             printf("%.16Xn", (uint64_t)(&func)); cout<<endl;     
             printf("%.16Xn", (uint64_t)(&pf));cout<<endl; 
             
        }

    }
    namespace test_string_reference_counting
    {
        void test()
        {
            string test1="12345";
            string test2=test1;
            char* p=&test1[3];
            *p='5';
            cout<<test1<<endl;
            cout<<test2<<endl;
        }
        
    }
	void test()
	{
		
		//test_count_ref::test();
        //test_auto_ptr::test();
		//test_operator_new_and_delete::test();
        //test_function_pointer::test();
        test_string_reference_counting::test();
	}
}
}
#endif	/* PAYPAL_HPP */

