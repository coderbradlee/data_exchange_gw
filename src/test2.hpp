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
	namespace test_count_object
    {
    	template<typename being_counted>
    	class counted
    	{
    	public:
    		class too_many_objects{};
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
    		}
    	private:
    		static int m_num_objects;
    		static const size_t max_objects=10;
    		void init()
    		{
    			if( m_num_objects>max_objects)
    			{
    				//throw too_many_objects();
    			}	
    			++m_num_objects;
    		}
    	};
    	class printer:private counted<printer>
    	{
    	public:
    		static printer* make_printer()
    		{

    		}
    		static printer* make_printer(const printer& r)
    		{

    		}
    		virtual ~printer()
    		{

    		}
    	private:
    		printer()
    		{}
    		printer(const printer& r)
    		{}
    	};
    	void test()
    	{

    	}

    }
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
	void test()
	{
		//test_count_object::test();
		test_template::test();
	}
}
}
#endif	/* PAYPAL_HPP */

