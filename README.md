2016.12.22
t_sales_order中条件增加status==4的，只取订单状态为“确认”的
按公司对累加的item_master_id的数量进行排序，是否更改为数量乘以价格？
2016.12.09
增加统计sku top10功能
每晚根据当天新增 SalesOrder 更新 t_sales_statistic（销售统计表），t_sales_statistic_detail（销售统计明细表）；

2016.12.06
 更新线上环境，今日美元对日元汇率为112
curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-12-06\&ratio=112\&database=js
curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-12-06\&ratio=112\&database=os
curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-12-06\&ratio=112\&database=as
curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-12-06\&ratio=112\&database=eu

curl -u test:test -X GET http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-12-06\&database=os

curl -u test:test -X GET http://127.0.0.1:8688/exchange_rate/?source=USD\&target=CNY\&time=2016-12-06\&database=as
2016.10.21
curl -u test:test -X PUT http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-10-21\&ratio=105\&database=js
curl -u test:test -X GET http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-10-21\&database=js
curl -u test:test -X POST http://127.0.0.1:8688/exchange_rate/?source=USD\&target=JPY\&time=2016-10-21\&ratio=105\&database=as

2016.08.19 更新svn版本到线上，tag exchange_rate_api_depoyv1.9

2016.08.09
增加写入svn版本的接口
//curl -u test:test -X PUT http://172.18.100.87:8688/deploy_software_version/?ver=v1.0\&url=http://au.renesola.com/\&database=apollo_js

2016.08.05 解决172.18.100.87上程序启动时发生signal 27,重新拉取代码并编译

2016.07.27 tag exchange_rate_api_depoyv1.8
(error:):src/exchange_rate.hpp:1302  去掉此地方日志写入
2016.07.20 tag exchange_rate_api_depoyv1.7
解决最近几天插入的数据每天都增加一条的问题，1.6没有解决，现在找到问题：
是m_exchage_rate_data_array，这个数据每次都从数据库拉取一次，导致每天数据会double


GET http://172.18.100.87:8088/flowNo/us/so

本地数据库:172.18.22.202
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