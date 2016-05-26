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
    		
			printer *a=printer::make_printer();
			printer *b=printer::make_printer(*a);
			
    		
    		printer *c=printer::make_printer();
    		printer *d=printer::make_printer(*c);
    		delete a;
			delete b;
			delete c;
			delete d;
    	}

    }
	void test()
	{
		test_count_object::test();
		//test_template::test();
	}
}
}
#endif	/* PAYPAL_HPP */

