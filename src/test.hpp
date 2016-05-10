#ifndef TEST_HPP
#define	TEST_HPP
#include "serverResource.hpp"
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include <bitset>
namespace x
{
	void rotate(std::vector<std::vector<string>>& vec)
	{
		for (int layer = 0; layer < 6 / 2; ++layer)
		{
			int first = layer;
			int last = 6 - 1 - layer;
			for (int i = first; i < last; ++i)
			{
				/*int offset = i - first;
				string top = vec[first][i];
				vec[first][i] = vec[last - offset][first];
				vec[last - offset][first] = vec[last][last - offset];
				vec[last][last - offset] = vec[i][last];
				vec[i][last] = top;*/
				string top = vec[first][i];
				vec[first][i] = vec[last - i][first];
				vec[last - i][first] = vec[last][last - i];
				vec[last][last - i] = vec[i][last];
				vec[i][last] = top;
			}
		}
	}
	void print(std::vector<std::vector<string>> vec)
	{
		for (int i = 0; i < 6; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				cout << vec[i][j] << " ";
			}
			cout << endl;
		}
	}
	void test()
	{
		std::vector<std::vector<string>> vec(6);
		for (int i = 0; i < 6; ++i)
			vec[i].resize(6);
		for (int i = 0; i < 6; ++i)
		{
			for (int j = 0; j < 6; ++j)
				vec[i][j] = boost::lexical_cast<string>(i)+boost::lexical_cast<string>(j);
		}
		print(vec);
		rotate(vec);
		cout << "==================================" << endl;
		print(vec);
	}

	class tree
	{	
	public:
		tree(int data) :data(data), left(nullptr), right(nullptr)
		{}
		int data;
		boost::shared_ptr<tree> left;
		boost::shared_ptr<tree> right;
	};
	bool covers(boost::shared_ptr<tree> root, boost::shared_ptr<tree> p)
	{
		if (root == nullptr) return false;
		if (root == p) return true;
		return covers(root->left, p) || covers(root->right, p);
	}
	boost::shared_ptr<tree> common_ancestor_helper(boost::shared_ptr<tree> root, boost::shared_ptr<tree> p, boost::shared_ptr<tree> q)
	{
		if (root == nullptr) return nullptr;
		if (root == p || root == q) return root;
		bool p_in_left = covers(root->left, p);
		bool q_in_left = covers(root->left, q);

		if (p_in_left != q_in_left) return root;
		else
		{
			boost::shared_ptr<tree> child = p_in_left ? root->left : root->right;
			return common_ancestor_helper(child, p, q);
		}
	}
	boost::shared_ptr<tree> common_ancestor(boost::shared_ptr<tree> root, boost::shared_ptr<tree> p, boost::shared_ptr<tree> q)
	{
		if (!covers(root, p) || !covers(root, q))
			return nullptr;
		return common_ancestor_helper(root, p, q);
	}
	void mid_travel(boost::shared_ptr<tree> root)
	{
		if (root == nullptr)
		{
			cout << "root is nullptr" << endl;
		}
		else
		{
			mid_travel(root->left);
			cout << root->data << endl;
			mid_travel(root->right);
		}
	}
	void test_tree()
	{
		//中序遍历为：123654
		boost::shared_ptr<tree> node1 = boost::shared_ptr<tree>(new tree(1));
		boost::shared_ptr<tree> node3 = boost::shared_ptr<tree>(new tree(3));
		boost::shared_ptr<tree> node4 = boost::shared_ptr<tree>(new tree(4));
		
		boost::shared_ptr<tree> node2 = boost::shared_ptr<tree>(new tree(2));
		node2->left = node1;
		node2->right = node3;
		boost::shared_ptr<tree> node6 = boost::shared_ptr<tree>(new tree(6));
		node6->left = node2;
		boost::shared_ptr<tree> node5 = boost::shared_ptr<tree>(new tree(5));
		node6->right = node5;
		node5->right = node4;

		mid_travel(node6);
		boost::shared_ptr<tree> test1=common_ancestor(node6, node1, node3);
		cout << test1->data << endl;
		boost::shared_ptr<tree> test2=common_ancestor(node6, node3, node4);
		cout << test2->data << endl;
		boost::shared_ptr<tree> test3 = common_ancestor(node6, node4, node5);
		cout << test3->data << endl;
		boost::shared_ptr<tree> test4 = common_ancestor(node6, node2, node3);
		cout << test4->data << endl;
	}
	int length(int x)
	{
		int ret = 0;
		while(x > 0)
		{
			ret++;
			x=x >> 1;
			//cout << __LINE__ << ":"<<x << endl;
		}
		return ret;
	}
	void merge_bit(int n, int m, int j, int i)
	{
		int space = j - i + 1;
		int len = length(m);
		/*int min = len <space ? len : space;
		cout << "len:" << len << endl;
		cout << "min:" << min << endl;*/
		int mask = 0;
		for (int i = 0; i < space; ++i)
			mask += (i +1)* 2;
		n &= ~mask;
		cout << "n:" << hex << n << endl;
		int temp = m << (j-len+1);
		cout << "temp:" << hex << temp << endl;
		cout << hex << n + temp << endl;
	}
	int update_bit(int n, int m, int i, int j)
	{
		int all = ~0;
		int left = all << (j + 1);
		int right = ((1 << i) - 1);
		int mask = left | right;
		int clear = n&mask;
		int shift = m << i;
		return clear | shift;
	}
	void test_merge_bit()
	{
		//01101100 010 3 1 结果输出100
		//merge_bit(108, 2, 3, 1);
		merge_bit(0x400, 0x13, 6, 2);
		cout << hex << update_bit(0x400, 0x13, 2, 6) << endl;
	}
	void test_bit_set()
	{
		auto str1 = "0011";
		auto str2 = "0110";
		bitset<10> one(str1);
		bitset<10> two(str2);
		auto three = one&two;
		cout << three << endl;
		three <<= 4;
		cout << three << endl;
	}

	void test_nlohmann_fifo_map()
	{
		using json = nlohmann_fifo_map::json;
		 json j =
	    {
	        {"pi", 3.141},
	        {"happy", true},
	        {"name", "Niels"},
	        {"nothing", nullptr},
	        {
	            "answer", {
	                {"everything", 42}
	            }
	        },
	        {"list", {1, 0, 2}},
	        {
	            "object", {
	                {"currency", "USD"},
	                {"value", 42.99}
	            }
	        }
	    };

	    // add new values
	    j["new"]["key"]["value"] = {"another", "list"};

	    // count elements
	    j["size"] = j.size();

	    // pretty print with indent of 4 spaces
	    std::cout << j << '\n';
	}
}
namespace brad
{ 
	//geekband homework1
	class Date
	{
		public:
		Date(int year,int month,int day):m_year(year),m_month(month),m_day(day)
		{
		
		}
		int get_year() const
		{
			return m_year;
		}
		int get_month() const
		{
			return m_month;
		}
		int get_day() const
		{
			return m_day;
		}
		void print() const
		{
			cout<<m_year<<"-"<<setw(2)<< setfill ('0')<<m_month<<"-"<<setw(2)<< setfill ('0')<<m_day<<endl;
		}
		private:
		int m_year;
		int m_month;
		int m_day;	
	};

	inline bool operator<(const Date& l,const Date& r)
	{
		//return 	(l.get_year()<r.get_year())||(l.get_month()<r.get_month())||(l.get_day()<r.get_day());
		if(l.get_year()>r.get_year())
		{
			return false;
		}
		else if(l.get_year()<r.get_year())
		{
			return true;
		}
		else if(l.get_month()>r.get_month())
		{
			return false;
		}
		else if(l.get_month()<r.get_month())
		{
			return true;
		}
		else if(l.get_day()>=r.get_day())
		{
			return false;
		}
		else
		{	
			return true;
		}

	}
	inline bool operator==(const Date& l,const Date& r)
	{
		return (l.get_year()==r.get_year())&&(l.get_month()==r.get_month())&&(l.get_day()==r.get_day());
	}
	inline bool operator>(const Date& l,const Date& r)
	{
		return ~(l<r||l==r);
	}
	int get_random_int(int head, int tail) 
	{  
	    static std::default_random_engine e{std::random_device{}()}; 
	    static std::uniform_int_distribution<int> u;  
	  
	    return u(e, decltype(u)::param_type(head, tail));  
	}
	 class comp
	 {
	 public:
	     bool operator() (const Date &a , const Date  &b) const
	     {
	         return a<b;
	     }
	}; 
	std::vector<Date> CreatePoints()
	{
		std::vector<Date> v;
		for(int i=0;i<10;++i)
		{
			v.push_back(Date(get_random_int(1900,2099),get_random_int(1,12),get_random_int(1,31)));
		}
		return v;
	}
	void Sort(std::vector<Date>& v)
	{
		//std::map<Date,int,comp> m;
		std::map<Date,int> m;
		int i=0;
		for(auto& date:v)
		{
			m.emplace(std::make_pair(date,i++));
		}
		v.clear();

		for(auto& date:m)
		{
			v.push_back(date.first);
		}
	} 
	void test()
	{
		std::vector<brad::Date> v=brad::CreatePoints();
			for(auto& d:v)
			{
				d.print();
			}
			cout<<"--------------------"<<endl;
			brad::Sort(v);
			for(auto& d:v)
			{
				d.print();
			}

	}
}
namespace effective_cplusplus
{
	template<typename T>
	class reference_class
	{
	public:
		reference_class(std::string& name,const T& value):m_name(name),m_value(value){}
		void print()
		{
			cout<<m_name<<":"<<m_value<<endl;
		}
		reference_class& operator=(const reference_class& r)
		{
			m_name=r.m_name;
			m_value=r.m_value;
		}
	private:
		std::string& m_name;
		T m_value;
	};
	void test()
	{
		std::string ps="p";
		std::string ss="s";
		reference_class<int> p(ps,1);
		reference_class<int> s(ss,2);
		reference_class<string> twostring(ss,"2");
		//twostring.print();
		//p=twostring;
		p=s;
		p.print();
	}
}
#endif	/* PAYPAL_HPP */

