#ifndef TEST2_HPP
#define	TEST2_HPP
#include "serverResource.hpp"
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include <bitset>
namespace x2
{
	namespace design_model
	{
	
    namespace test_template
    {
		template<typename T>
		class base
		{
		protected:
			base(size_t n,T* p):m_size(n),m_p(p)
			{

			}
			void invert(size_t m)
			{
				cout<<"base invert"<<endl;
			}
		private:
			size_t m_size;
			T* m_p;
		};
		template<typename T,size_t n>
		class derived:private base<T>
		{
		private:
			using base<T>::invert;
		public:
			derived():base<T>(n,nullptr),m_p(new T[n*n])
			{

			}
			void invert()
			{
				//this->invert(n);
				invert(n);
				cout<<typeid(m_p).name()<<endl;
			}
		private:
			//T data[n*n];
			boost::scoped_array<T> m_p;
		};
		void test()
		{
			derived<int,4> d;
			d.invert();
		}
    }
    namespace test_count_object
    {
    	template<typename being_counted>
    	class counted
    	{
    	public:
    		class too_many_objects:public std::exception
    		{
    		public:
    			too_many_objects(const char* msg):m_msg(msg)
    			{

    			}
    			virtual ~too_many_objects()noexcept{}
    			virtual const char* what()const noexcept{return m_msg.c_str();}
    		protected:
    			string m_msg;
    		};
    		static int object_count()
    		{
    			return m_num_objects;
    		}
    	protected:
    		counted()
    		{
    			init();
    		}
    		counted(const counted&)
    		{
    			init();
    		}
    		virtual ~counted()
    		{
    			--m_num_objects;
    			cout<<"~counted:"<<m_num_objects<<endl;
    		}
    	private:
    		static int m_num_objects;
    		static const size_t max_objects=2;
    		void init()
    		{
    			cout<<"init:"<<m_num_objects<<endl;
    			if( m_num_objects>max_objects)
    			{
    				throw too_many_objects("too many printer");
    				
    			}	
    			++m_num_objects;		
    		}
    	};
    	class printer:private counted<printer>
    	{
    	public:
    		using counted<printer>::object_count;
    		static printer* make_printer()
    		{
    			return new printer();
    		}
    		static printer* make_printer(const printer& r)
    		{
    			return new printer(r);
    		}
    		virtual ~printer()
    		{
    			cout<<"printer destructor"<<endl;
    		}
    	private:
    		printer()
    		{
    			cout<<"printer constructor"<<endl;
    		}
    		printer(const printer& r)
    		{
    			cout<<"printer copy constructor"<<endl;
    		}
    	};
    	template<typename being_counted>
    	int counted<being_counted>::m_num_objects=0;
    	void test()
    	{
    		
			std::shared_ptr<printer> a(printer::make_printer());
			printer *b=printer::make_printer(*a);
			
    		
    		printer *c=printer::make_printer();
    		printer *d=printer::make_printer(*c);
			delete b;
			delete c;
			delete d;
    	}

    }
    namespace private_destructor
    {
    	class test_private_destructor
    	{
    	public:
    	private:
    		~test_private_destructor()
    		{}
    	};
    	void test()
    	{
    		//test_private_destructor d;//必须在堆上创建
    		test_private_destructor* d=new test_private_destructor();
    	}
    }
    namespace protected_destructor_base
    {
    	class base
    	{
    	public:
    		base()
    		{
    			if(!on_the_heap)
    				throw heap_constraint_violation("heap_constraint_violation");
    			on_the_heap=false;
    		}
    		class heap_constraint_violation:public exception
    		{
    			public:
    			heap_constraint_violation(const char* msg):m_msg(msg)
    			{

    			}
    			virtual ~heap_constraint_violation()noexcept{}
    			virtual const char* what()const noexcept{return m_msg.c_str();}
    		protected:
    			string m_msg;
    		};
    		static void* operator new(size_t size)
    		{
    			on_the_heap=true;
    			return ::operator new(size);
    		}
    		void destroy()
    		{
    			delete this;
    		}
    	protected:
    		virtual ~base()
    		{
    			cout<<"~base"<<endl;
    		}
    	private:
    		static bool on_the_heap;
    	};
    	bool base::on_the_heap=false;

    	class derived:public base
    	{
    	public:
    	};
    	class asset
    	{
    	public:
    		asset():m_value(new base())
    		{}
    		~asset()
    		{
    			m_value->destroy();
    		}
    	private:
    		base* m_value;
    	};
    	class heap_tracked
    	{
    	public:
    		class missing_address:public std::exception
    		{
    		public:
    			missing_address(const char* msg):m_msg(msg)
    			{

    			}
    			virtual ~missing_address()noexcept{}
    			virtual const char* what()const noexcept{return m_msg.c_str();}
    		protected:
    			string m_msg;
    		};
    		virtual ~heap_tracked()
    		{
    			cout<<"~heap_tracked"<<endl;
    		}
    		static void* operator new(size_t size)
    		{
    			void* ptr=::operator new(size);
    			m_addresses.push_front(ptr);
    			return ptr;
    		}
    		static void operator delete(void* ptr)
    		{
    			auto it=find(m_addresses.begin(),m_addresses.end(),ptr);
    			if(it!=m_addresses.end())
    			{
    				m_addresses.erase(it);
    				::operator delete(ptr);
    			}
    			else
    			{
    				throw missing_address("missing_address");
    			}
    		}
    		bool is_on_heap()const
    		{
    			const void* raw=dynamic_cast<const void*>(this);
    			auto it=find(m_addresses.begin(),m_addresses.end(),raw);
    			return it!=m_addresses.end();
    		}
    	private:
    		typedef const void* raw_address;
    		static std::list<raw_address> m_addresses;
    	};
    	std::list<const void*> heap_tracked::m_addresses;
    	class asset2:public heap_tracked
    	{
    	public:
    		~asset2()
    		{
    			cout<<"~asset2"<<endl;
    		}
    	private:

    	};
    	void test()
    	{
    		//derived d;
    		derived* dn=new derived();
    		delete dn;
    		asset2 a;
    		cout<<a.is_on_heap()<<endl;
    		asset2* b=new asset2();
    		cout<<b->is_on_heap()<<endl;
    		delete b;
    	}
    }
    namespace test_v_ptr_v_table
    {
    	class base
    	{
    	public:
    		virtual void f()
    		{
    			cout<<"base::f"<<endl;
    		}
    		virtual void g()
    		{
    			cout<<"base::g"<<endl;
    		}
    		virtual void h()
    		{
    			cout<<"base::h"<<endl;
    		}
    	};
    	class base2
    	{
    	public:
    		virtual void fff()
    		{
    			cout<<"base2::fff"<<endl;
    		}
    		
    	};
    	class derived:public base,public base2
    	{
    	public:
    		virtual void ff()
    		{
    			cout<<"derived::ff"<<endl;
    		}
    		virtual void g()
    		{
    			cout<<"derived::g"<<endl;
    		}
    		virtual void h1()
    		{
    			cout<<"derived::h1"<<endl;
    		}
    	};
    	void test()
    	{
    		typedef void(*fun)(void);
    		base *b=new base();
    		fun p_fun=nullptr;
    		cout<<"base v_table address is:"<<(intptr_t*)(b)<<endl;
    		cout<<"base first function in v_table is:"<<(intptr_t*)*(intptr_t*)b<<endl;
    		cout<<"base second function in v_table is:"<<(intptr_t*)*(intptr_t*)b+1<<endl;
    		cout<<"base third function in v_table is:"<<(intptr_t*)*(intptr_t*)b+2<<endl;
    		cout<<"base end is:"<<*((intptr_t*)*(intptr_t*)b+3)<<endl;
			for(int i=0;i<3;++i)
    		{
	    		p_fun = (fun)*((intptr_t*)*(intptr_t*)&(*b)+i);
	    		p_fun();//base::f
    		}
    		delete b;
    		cout<<"------------------------------------"<<endl;
    		derived d;
			cout<<"derived v_table address is:"<<(intptr_t*)(&d)<<endl;
    		
    		for(int i=0;i<5;++i)
    		{
    			cout<<"derived fun in v_table is:"<<(intptr_t*)*(intptr_t*)&d+i<<endl;
    			p_fun=(fun)*((intptr_t*)*(intptr_t*)&d+i);
    			p_fun();//dase::f
    		}
    		cout<<"derived end is:"<<*((intptr_t*)*(intptr_t*)&d+5)<<endl;
    		
    		cout<<"derived fun in v_table is:"<<(intptr_t*)*((intptr_t*)&d+1)+0<<endl;
    			p_fun=(fun)*((intptr_t*)*((intptr_t*)&d+1)+0);
    			p_fun();//dase::f
    			cout<<"end is:"<<*((intptr_t*)*((intptr_t*)&d+1)+1)<<endl;
    	}
    }
    namespace pointer_to_member_func
    {
    	struct A 
    	{
		    void foo() const {
		        std::cout << "A's this:\t" << this << std::endl;
		    }
		    char pad0[32];
		};
		 
		struct B 
		{
		    void bar() const {
		        std::cout << "B's this:\t" << this << std::endl;
		    }
		    char pad2[64];
		};
		 
		struct C : A, B
		{ };
		 
		void call_by_ptr(const C &obj, void (C::*mem_func)() const)
		{
		    void *data[2];
		    std::memcpy(data, &mem_func, sizeof(mem_func));
		    std::cout << "------------------------------\n"
		        "Object ptr:\t" << &obj <<
		        "\nFunction ptr:\t" << data[0] <<
		        "\nPointer adj:\t" << data[1] << std::endl;
		    (obj.*mem_func)();
		}
		void test()
		{
			C obj;
		    call_by_ptr(obj, &C::foo);
		    call_by_ptr(obj, &C::bar);
		}
    }
    namespace test_variadic_template
    {
    	void print()
    	{}
    	template<typename T,typename... types>
    	void print(const T& first,const types&... args)
    	{
    		cout<<first<<endl;
    		print(args...);
    	}
    	void test()
    	{
    		print(1,"2",bitset<16>(377),4.3);
    	}
    }
    namespace test_disable_new
    {
    	class test_class
    	{
    	public:
    		test_class()
    		{
    			cout<<"test_class constructor"<<endl;
    		}
    		virtual ~test_class()
    		{
    			cout<<"test_class destructor"<<endl;
    		}
    	private:
    		static void* operator new(size_t)
    		{
    			cout<<"base operator new"<<endl;
    		}
    		
    		static void operator delete(void*)
    		{
    			cout<<"base operator delete"<<endl;
    		}
    		
    	};
    	class derived:public test_class
    	{
    	public:
    		void* operator new(size_t s)
    		{
    			cout<<"derived operator new"<<endl;
    			return operator new(s);
    		}
    		void operator delete(void* v)
    		{
    			cout<<"derived operator delete"<<endl;
    			operator delete v;
    		}
    	};
    	void test()
    	{
    		test_class t;
    		static test_class t2;
    		//test_class* p=new test_class();
    		derived *p=new derived();
    		delete p;
    	}
    }
	void test()
	{
		//test_count_object::test();
		//test_template::test();
		//private_destructor::test();
		//protected_destructor_base::test();
		//test_v_ptr_v_table::test();
		// cout<<sizeof(int)<<endl;
		// cout<<sizeof(intptr_t)<<endl;
		// cout<<sizeof(int*)<<endl;
		// cout<<sizeof(intptr_t*)<<endl;
		//pointer_to_member_func::test();
		//test_variadic_template::test();
		test_disable_new::test();
	}
}
}
#endif	/* PAYPAL_HPP */

