#ifndef SCM_DATA_HPP
#define SCM_DATA_HPP

namespace scm_namespace
{
string rand_strings(int len)
{
  string choice="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis_y(0,35);  
  auto dice_y= std::bind(dis_y,gen);
    for(int i=0;i<len;++i)
    {
      ret+=choice[dice_y()];
    }
    //cout<<ret<<endl;
   return ret;
}
struct supplier_basic
{
public:
  string supplier_id;
  string supplier_no;
  string company_name_en;
  string status;
  string country_id;
  string city_name;
  string province;
  string address;
  string zip_code;
  string tel;
  string fax;
  string delivery_terms;
  string payment_terms;
  string trade_term_id;
  string billing_company_name;
  string billing_address;
  string billing_bank_name;
  string billing_bank_account_no;
  string note;
  string owner_purchaser_id;
  void print()
  { cout<<"{";
    cout<<supplier_id<<":";
    cout<<supplier_no<<":";
    cout<<company_name_en<<":";
    cout<<status<<":";
    cout<<country_id<<":";
    cout<<city_name<<"}";
    cout<<":"<<__FILE__<<":"<<__LINE__<<endl;
  }
};
struct vendor_basic
{
  
};
// vendor_basic_id` char(20) NOT NULL COMMENT '供应商基本档案ID',
//   `vendor_no` varchar(64) NOT NULL COMMENT '/*供应商编码*/',
//   `name1` varchar(256) DEFAULT NULL COMMENT '供应商名称1',
//   `name2` varchar(256) DEFAULT NULL COMMENT '供应商名称2',
//   `short_name` varchar(128) DEFAULT NULL COMMENT '简称',
//   `status` tinyint(4) NOT NULL COMMENT ' /*状态, 0:Active;1:Inactive*/',
//   `buyer_id` char(20) DEFAULT NULL COMMENT '/*采购员帐号id*/',
//   `website_url` varchar(128) DEFAULT NULL COMMENT '供应商网址',
//   `type` tinyint(4) NOT NULL COMMENT '/*供应商类型,0:服务商;1:制造商;2:经销商;3:零售商;4:临时供应商;5:其他*/',
//   `register_date` char(19) NOT NULL COMMENT '注册时间   YYYY-MM-DD HH:MM:SS',
//   `register_address` varchar(256) NOT NULL COMMENT '注册地址',
//   `register_capital` decimal(20,2) NOT NULL COMMENT '注册资金',
//   `business_register_number` varchar(128) DEFAULT NULL COMMENT '工商注册号',
//   `tax_number` varchar(128) DEFAULT NULL COMMENT '税号',
//   `company_scale` smallint(6) DEFAULT NULL COMMENT '公司规模',
//   `annual_revenue_last_year` decimal(20,2) DEFAULT NULL COMMENT '去年年产值',
//   `annual_revenue_year_before_last_year` decimal(20,2) DEFAULT NULL COMMENT '前年-年产值',
//   `area_id` char(20) DEFAULT NULL,
//   `country_id` char(20) DEFAULT NULL,
//   `city_id` char(20) DEFAULT NULL,
//   `zip_code` varchar(64) DEFAULT NULL COMMENT '邮编',
//   `createAt` char(19) NOT NULL,
//   `createBy` varchar(24) NOT NULL,
//   `updateAt` char(19) DEFAULT NULL,
//   `updateBy` varchar(24) DEFAULT NULL,
//   `deleteAt` char(19) DEFAULT NULL,
//   `deleteBy` varchar(24) DEFAULT NULL,
//   `dr` smallint(6) NOT NULL DEFAULT '0' COMMENT '逻辑删除 0:未删除;1:已删除',
//   `data_version` int(11) DEFAULT NULL,
//   `rank` tinyint(4) NOT NULL COMMENT ' /*供应商等级, 0:A;1:B;2:C;3:D;4:E*/',

  }
#endif