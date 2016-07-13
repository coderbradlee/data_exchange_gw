2016.07.13
	1、在tag exchange_rate_api_depoyv1.4基础上，根据财务部要求更改数据源为雅虎
	2、增加PUT接口，针对近期要求美元/日元的汇率为110，在每天程序更新汇率的基础上将美元对日元的汇率更新为110
	3、同时PUT接口解决
		（1）平日人民币/美元汇率取中行9.30后第一笔交易的折算价
		（2）月末最后一天人民币/美元汇率取当天第一笔（EG. 6月30日早上0点后第一笔）现汇买入/卖出价的平均价

	curl -u test:test -X PUT http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&ratio=110\&database=js

	curl -u test:test -X GET http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&database=js

	curl -u test:test -X POST http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&ratio=110\&database=js

	curl -X GET "http://query.yahooapis.com/v1/public/yql?q=select%20id,Rate%20from%20yahoo.finance.xchange%20where%20pair%20in%20(%22USDEUR%22,%20%22USDJPY%22,%20%22USDBGN%22,%20%22USDGBP%22,%20%22USDPLN%22,%20%22USDRON%22,%20%22USDSEK%22,%20%22USDHRK%22,%20%22USDRUB%22,%20%22USDTRY%22,%20%22USDAUD%22,%20%22USDBRL%22,%20%22USDCAD%22,%20%22USDCNY%22,%20%22USDHKD%22,%20%22USDIDR%22,%20%22USDINR%22,%20%22USDKRW%22,%20%22USDMXN%22,%20%22USDNZD%22,%20%22USDPHP%22,%20%22USDSGD%22,%20%22USDTHB%22,%20%22USDZAR%22,%22USDTWD%22,%22USDCNY%22,%22USDPHP%22)&format=json&env=store://datatables.org/alltableswithkeys"