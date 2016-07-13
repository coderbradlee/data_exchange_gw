2016.07.13
	1、在tag exchange_rate_api_depoyv1.4基础上，根据财务部要求更改数据源为雅虎
	2、增加PUT接口，针对近期要求美元/日元的汇率为110，在每天程序更新汇率的基础上将美元对日元的汇率更新为110
	3、同时PUT接口解决
		（1）平日人民币/美元汇率取中行9.30后第一笔交易的折算价
		（2）月末最后一天人民币/美元汇率取当天第一笔（EG. 6月30日早上0点后第一笔）现汇买入/卖出价的平均价

	curl -u test:test -X PUT http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&ratio=110\&database=js