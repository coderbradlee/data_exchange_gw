

本地:172.18.22.202
2016.07.18 tag exchange_rate_api_depoyv1.6
解决最近几天插入的数据每天都增加一条的问题，今天线上数据库同样的数据插入了四条，昨天三条
2016.07.13 tag exchange_rate_api_depoyv1.5
	1、在tag exchange_rate_api_depoyv1.4基础上，根据财务部要求更改数据源为雅虎
	2、增加PUT接口，针对近期要求美元/日元的汇率为110，在每天程序更新汇率的基础上将美元对日元的汇率更新为110
	3、同时PUT接口解决
		（1）平日人民币/美元汇率取中行9.30后第一笔交易的折算价
		（2）月末最后一天人民币/美元汇率取当天第一笔（EG. 6月30日早上0点后第一笔）现汇买入/卖出价的平均价
	4、利用curl对自身请求，更新日元汇率为他们临时要求的110,配置增加usd_jpy=110，当设置为0时不update
	curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&ratio=110\&database=js

yahoo.finance.xchange接口限制如下：
https://developer.yahoo.com/yql/guide/usage_info_limits.html


               Public	        OAuth with API Key
YQL Endpoint	/v1/public/*	/v1/yql/*
Hourly Cap	2,000 requests/hour per IP	20,000 requests/hour per IP
Daily Cap	None	100,000 total requests/day per API Key


	curl -u test:test -X GET http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&database=js

	curl -u test:test -X POST http://172.18.100.87:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-07-13\&ratio=110\&database=js

	curl -X GET "http://query.yahooapis.com/v1/public/yql?q=select%20id,Rate%20from%20yahoo.finance.xchange%20where%20pair%20in%20(%22USDEUR%22,%20%22USDJPY%22,%20%22USDBGN%22,%20%22USDGBP%22,%20%22USDPLN%22,%20%22USDRON%22,%20%22USDSEK%22,%20%22USDHRK%22,%20%22USDRUB%22,%20%22USDTRY%22,%20%22USDAUD%22,%20%22USDBRL%22,%20%22USDCAD%22,%20%22USDCNY%22,%20%22USDHKD%22,%20%22USDIDR%22,%20%22USDINR%22,%20%22USDKRW%22,%20%22USDMXN%22,%20%22USDNZD%22,%20%22USDPHP%22,%20%22USDSGD%22,%20%22USDTHB%22,%20%22USDZAR%22,%22USDTWD%22,%22USDCNY%22,%22USDPHP%22)&format=json&env=store://datatables.org/alltableswithkeys"

	http://query.yahooapis.com/v1/public/yql?q=select id,Rate from yahoo.finance.xchange where pair in ("USDEUR", "USDJPY", "USDBGN", "USDGBP", "USDPLN", "USDRON", "USDSEK", "USDHRK", "USDRUB", "USDTRY", "USDAUD", "USDBRL", "USDCAD", "USDCNY", "USDHKD", "USDIDR", "USDINR", "USDKRW", "USDMXN", "USDNZD", "USDPHP", "USDSGD", "USDTHB", "USDZAR","USDTWD","USDCNY","USDPHP")&format=json&env=store://datatables.org/alltableswithkeys