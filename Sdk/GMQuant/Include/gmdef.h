#ifndef GMCSDK_DEF_H
#define GMCSDK_DEF_H


const unsigned int LEN_ID = 64;
const unsigned int LEN_NAME = 64;
const unsigned int LEN_SYMBOL = 32;
const unsigned int LEN_INFO = 128;
const unsigned int LEN_EXCHANGE = 8;
const unsigned int LEN_ISO_DATATIME = 36;
const unsigned int LEN_FREQUENCY = 8;
const unsigned int DEPTH_OF_QUOTE = 10;
const unsigned int LEN_PARAM = 256;
const unsigned int LEN_COMMENT = 256;
const unsigned int LEN_MAX_ORDER_QUEUE = 50;
const unsigned int LEN_TYPE = 8;
const unsigned int LEN_TIME_LIST = 128;
const unsigned int MAX_TRADING_SESSION_NUM = 5;

#define IN_AT_OUT_NO_SPECIFIED 2147483647

enum StrategyMode
{
	MODE_UNDEF = 0,          //δ���壬 ���Բ�������
	MODE_LIVE = 1,           //ʵ�������ģʽ
	MODE_BACKTEST = 2        //�ز�ģʽ
};

enum OrderStatus
{
	OrderStatus_Unknown = 0,
	OrderStatus_New = 1,                   //�ѱ�
	OrderStatus_PartiallyFilled = 2,       //����
	OrderStatus_Filled = 3,                //�ѳ�
	OrderStatus_DoneForDay = 4,            //
	OrderStatus_Canceled = 5,              //�ѳ�
	OrderStatus_PendingCancel = 6,         //����
	OrderStatus_Stopped = 7,               //
	OrderStatus_Rejected = 8,              //�Ѿܾ�
	OrderStatus_Suspended = 9,             //����
	OrderStatus_PendingNew = 10,           //����
	OrderStatus_Calculated = 11,           //
	OrderStatus_Expired = 12,              //�ѹ���
	OrderStatus_AcceptedForBidding = 13,   //
	OrderStatus_PendingReplace = 14,       //
};

//ִ�лر�����
enum ExecType
{
	ExecType_Unknown = 0,
	ExecType_New = 1,                      //�ѱ�
	ExecType_DoneForDay = 4,               //
	ExecType_Canceled = 5,                 //�ѳ���
	ExecType_PendingCancel = 6,            //������
	ExecType_Stopped = 7,                  //
	ExecType_Rejected = 8,                 //�Ѿܾ�
	ExecType_Suspended = 9,                //����
	ExecType_PendingNew = 10,              //����
	ExecType_Calculated = 11,              //
	ExecType_Expired = 12,                 //����
	ExecType_Restated = 13,                //
	ExecType_PendingReplace = 14,          //
	ExecType_Trade = 15,                   //�ɽ�
	ExecType_TradeCorrect = 16,            //
	ExecType_TradeCancel = 17,             //
	ExecType_OrderStatus = 18,             //ί��״̬
	ExecType_CancelRejected = 19,          //�������ܾ�
};

//ί�оܾ�ԭ��
enum OrderRejectReason
{
	OrderRejectReason_Unknown = 0,                           //δ֪ԭ��
	OrderRejectReason_RiskRuleCheckFailed = 1,               //�����Ϸ�ع��� 
	OrderRejectReason_NoEnoughCash = 2,                      //�ʽ���
	OrderRejectReason_NoEnoughPosition = 3,                  //��λ����
	OrderRejectReason_IllegalAccountId = 4,                  //�Ƿ��˻�ID
	OrderRejectReason_IllegalStrategyId = 5,                 //�Ƿ�����ID
	OrderRejectReason_IllegalSymbol = 6,                     //�Ƿ����״���
	OrderRejectReason_IllegalVolume = 7,                     //�Ƿ�ί����
	OrderRejectReason_IllegalPrice = 8,                      //�Ƿ�ί�м�
	OrderRejectReason_AccountDisabled = 10,                  //�����˺ű���ֹ����
	OrderRejectReason_AccountDisconnected = 11,              //�����˺�δ����
	OrderRejectReason_AccountLoggedout = 12,                 //�����˺�δ��¼
	OrderRejectReason_NotInTradingSession = 13,              //�ǽ���ʱ��
	OrderRejectReason_OrderTypeNotSupported = 14,            //ί�����Ͳ�֧��
	OrderRejectReason_Throttle = 15,                         //��������
	OrderRejectReason_SymbolSusppended = 16,                 //���״���ͣ��
	OrderRejectReason_Internal = 999,                        //�ڲ�����

	CancelOrderRejectReason_OrderFinalized = 101,            //ί�������
	CancelOrderRejectReason_UnknownOrder = 102,              //δ֪ί��
	CancelOrderRejectReason_BrokerOption = 103,              //��̨����
	CancelOrderRejectReason_AlreadyInPendingCancel = 104,    //ί�г�����
};

//ί�з���
enum OrderSide
{
	OrderSide_Unknown = 0,
	OrderSide_Buy = 1,    //����
	OrderSide_Sell = 2,    //����
};

//ί������
enum OrderType
{
	OrderType_Unknown = 0,
	OrderType_Limit = 1,    //�޼�ί��
	OrderType_Market = 2,    //�м�ί��
	OrderType_Stop = 3,    //ֹ��ֹӯί��
};

//ί��ʱ������
enum OrderDuration
{
	OrderDuration_Unknown = 0,
	OrderDuration_FAK = 1,  //��ʱ�ɽ�ʣ�೷��(fill and kill)
	OrderDuration_FOK = 2,  //��ʱȫ��ɽ�����(fill or kill)
	OrderDuration_GFD = 3,  //������Ч(good for day)
	OrderDuration_GFS = 4,  //������Ч(good for section)
	OrderDuration_GTD = 5,  //ָ������ǰ��Ч(goodl till date)
	OrderDuration_GTC = 6,  //����ǰ��Ч(good till cancel)
	OrderDuration_GFA = 7,  //���Ͼ���ǰ��Ч(good for auction)
};

//ί�гɽ�����
enum OrderQualifier
{
	OrderQualifier_Unknown = 0,
	OrderQualifier_BOC = 1,  //�Է����ż۸�(best of counterparty)
	OrderQualifier_BOP = 2,  //�������ż۸�(best of party)
	OrderQualifier_B5TC = 3,  //�����嵵ʣ�೷��(best 5 then cancel)
	OrderQualifier_B5TL = 4,  //�����嵵ʣ��ת�޼�(best 5 then limit)
};

//�ֲַ���
enum PositionSide
{
	PositionSide_Unknown = 0,
	PositionSide_Long = 1,   //�෽��
	PositionSide_Short = 2,   //�շ���
};

//��ƽ��־
enum PositionEffect
{
	PositionEffect_Unknown = 0,
	PositionEffect_Open = 1,     //����
	PositionEffect_Close = 2,     //ƽ��,��������ȡ���ڶ�Ӧ�Ľ�����
	PositionEffect_CloseToday = 3,     //ƽ���
	PositionEffect_CloseYesterday = 4,     //ƽ���
};

//�ʽ�ֱֲ��ԭ��
enum CashPositionChangeReason
{
	CashPositionChangeReason_Unknown = 0,
	CashPositionChangeReason_Trade = 1,  //����
	CashPositionChangeReason_Inout = 2,  //�����/����ֲ�
	CashPositionChangeReason_Dividend = 3,  //�ֺ��͹�
};

enum AccountState
{
	State_UNKNOWN = 0,       //δ֪
	State_CONNECTING = 1,    //������
	State_CONNECTED = 2,     //������
	State_LOGGEDIN = 3,      //�ѵ�¼
	State_DISCONNECTING = 4, //�Ͽ���
	State_DISCONNECTED = 5,  //�ѶϿ�
	State_ERROR = 6          //����
};

//�㷨��״̬,��ͣ/�ָ��㷨��ʱ��Ч
enum AlgoOrderStatus 
{
	AlgoOrderStatus_Unknown = 0,
	AlgoOrderStatus_Resume = 1,                  //�ָ�ĸ��
	AlgoOrderStatus_Pause = 2,                   //��ͣĸ��
	AlgoOrderStatus_PauseAndCancelSubOrders = 3  //����ĸ�������ӵ�
};

//ͷ����Դ(��������ȯ��ȯ)
enum PositionSrc
{
	PositionSrc_Unknown = 0,
	PositionSrc_L1 = 1,          //��ͨ��
	PositionSrc_L2 = 2           //ר����
};

//֤ȯ����
enum SecurityType
{
	SecurityType_Unknown = 0,
	SecurityType_Stock = 1,  //��Ʊ
	SecurityType_Fund = 2,   //����
	SecurityType_Index = 3,  //ָ��
	SecurityType_Future = 4,  //�ڻ�
	SecurityType_Option = 5,  //��Ȩ
	SecurityType_Credit = 6,  //����
	SecurityType_Bond = 7,    //ծȯ
	SecurityType_Bond_Convertible = 8 //��ծȯ
};

//ҵ������
enum OrderBusiness
{
	OrderBusiness_NORMAL = 0,                         //��ͨ����                                                           
	OrderBusiness_IPO_BUY = 100,                      //�¹��깺                                                         
	OrderBusiness_CREDIT_BOM = 200,                   //��������(buying on margin)                                       
	OrderBusiness_CREDIT_SS = 201,                    //��ȯ����(short selling)                                          
	OrderBusiness_CREDIT_RSBBS = 202,                 //��ȯ��ȯ(repay share by buying share)                            
	OrderBusiness_CREDIT_RCBSS = 203,                 //��ȯ����(repay cash by selling share)                            
	OrderBusiness_CREDIT_DRS = 204,                   //ֱ�ӻ�ȯ(directly repay share)                                   
	OrderBusiness_CREDIT_DRC = 211,                   //ֱ�ӻ���(directly repay cash)                                    
	OrderBusiness_CREDIT_CPOM = 205,                  //����ƽ��(close position on margin)                               
	OrderBusiness_CREDIT_CPOSS = 206,                 //��ȯƽ��(close position on short selling)                        
	OrderBusiness_CREDIT_BOC = 207,                   //����Ʒ����(buying on collateral)                                 
	OrderBusiness_CREDIT_SOC = 208,                   //����Ʒ����(selling on collateral)                                
	OrderBusiness_CREDIT_CI = 209,                    //����Ʒת��(collateral in)                                        
	OrderBusiness_CREDIT_CO = 210,                    //����Ʒת��(collateral out)                                       
	OrderBusiness_ETF_BUY = 301,                      //ETF�깺(purchase)                                                
	OrderBusiness_ETF_RED = 302,                      //ETF���(redemption)                                              
	OrderBusiness_FUND_SUB = 303,                     //�����Ϲ�(subscribing)                                            
	OrderBusiness_FUND_BUY = 304,                     //�����깺(purchase)                                               
	OrderBusiness_FUND_RED = 305,                     //�������(redemption)                                             
	OrderBusiness_FUND_CONVERT = 306,                 //����ת��(convert)                                                
	OrderBusiness_FUND_SPLIT = 307,                   //����ֲ�(split)                                                  
	OrderBusiness_FUND_MERGE = 308,                   //����ϲ�(merge)                                                  
	OrderBusiness_BOND_RRP = 400,                     //ծȯ��ع�(reverse repurchase agreement (RRP) or reverse repo)   
	OrderBusiness_BOND_CONVERTIBLE_BUY = 401,         //��תծ�깺(purchase)                                             
	OrderBusiness_BOND_CONVERTIBLE_CALL = 402,        //��תծת��                                                       
	OrderBusiness_BOND_CONVERTIBLE_PUT = 403,         //��תծ����                                                       
	OrderBusiness_BOND_CONVERTIBLE_PUT_CANCEL = 404   //��תծ���۳���                                                   
};

typedef char Symbol[LEN_SYMBOL];
typedef char TradingDate[LEN_ISO_DATATIME];

struct Bar
{
	char                    symbol[LEN_SYMBOL];
	double                  bob;                          ///bar�Ŀ�ʼʱ��
	double                  eob;                          ///bar�Ľ���ʱ��
	float                   open;                         ///<���̼�
	float                   close;                        ///<���̼�
	float                   high;                         ///<��߼�
	float                   low;                          ///<��ͼ�
	double                  volume;                       ///<�ɽ���
	double                  amount;                       ///<�ɽ����
	float                   pre_close;                    ///�����̼ۣ�ֻ����Ƶ���ݸ�ֵ

	long long               position;                     ///<�ֲ���
	char                    frequency[LEN_FREQUENCY];     ///barƵ��
};

struct Quote
{
	float      bid_price;                            ///����ί���
	long long  bid_volume;                           ///����ί����
	float      ask_price;                            ///����ί����
	long long  ask_volume;                           ///����ί����
};

struct Tick
{
	char                   symbol[LEN_SYMBOL];
	double                 created_at;               ///<utcʱ�䣬��ȷ������
	float                  price;                    ///<���¼�
	float                  open;                     ///<���̼�
	float                  high;                     ///<��߼�
	float                  low;                      ///<��ͼ�
	double                 cum_volume;               ///<�ɽ�����
	double                 cum_amount;               ///<�ɽ��ܽ��/���³ɽ���,�ۼ�ֵ
	long long              cum_position;             ///<��Լ�ֲ���(��),�ۼ�ֵ
	double                 last_amount;              ///<˲ʱ�ɽ���
	int                    last_volume;              ///<˲ʱ�ɽ���
	int                    trade_type;               ///(����)��������,��Ӧ�࿪,��ƽ������
	Quote                  quotes[DEPTH_OF_QUOTE];   ///����, �±��0��ʼ��0-��ʾ��һ����1-��ʾ�ڶ�������������

};

struct L2Transaction
{
	char                   symbol[LEN_SYMBOL];
	double                 created_at;               ///�ɽ�ʱ�䣬utcʱ��
	float                  price;                    ///�ɽ���
	long long              volume;					 ///�ɽ���
	char                   side;                     ///�����̱��
	char                   exec_type;                ///�ɽ�����
	long long              exec_index;               ///�ɽ����
	long long              ask_order_index;          ///����ί�б��
	long long              bid_order_index;          ///����ί�б��
};

struct L2Order
{
	char                   symbol[LEN_SYMBOL];
	double                 created_at;               ///ί��ʱ�䣬utcʱ��
	float                  price;                    ///ί�м�
	long long              volume;					 ///ί����
	char                   side;                     ///��������
	char                   order_type;               ///ί������
	long long              order_index;              ///ί�б��
};

struct L2OrderQueue
{
	char                   symbol[LEN_SYMBOL];
	double                 created_at;                                  ///����ʱ�䣬utcʱ��
	float                  price;                                       ///����ί�м�
	long long              volume;					                    ///ί����
	char                   side;                                        ///��������
	int                    queue_orders;                                ///ί����������Ԫ�ظ���(���50)
	int                    queue_volumes[LEN_MAX_ORDER_QUEUE];          ///ί��������(���50�����п���С��50, ��Ч���ݳ���ȡ����queue_orders)
};

struct VarietyInfo
{
	char     variety_name[LEN_NAME];          //Ʒ�����ƣ�ȫ����д��������������ǰ׺��
	int      sec_type;                        //Ʒ������
	int      sec_type_ext;                    //Ʒ����չ����
	char     exchange[LEN_EXCHANGE];          //����������
	char     quote_unit[LEN_TYPE];            //���۵�λ
	double   price_tick;                      //��С�䶯��λ  
	double   multiplier;                      //"��Լ���� (����Ʊ��Ȩ�ĺ�Լ��λ����Ʒ��Ȩ�Ľ��׵�λ��ָ����Ȩ�ĺ�Լ������"
	int      trade_n;                         //�����ƶȣ�0��ʾT+0��1��ʾT+1��2��ʾT+2
	char     option_type;                     //��Ȩ��ʽ��E:ŷʽ��A:��ʽ 
};


struct Order
{
	char         strategy_id[LEN_ID];                     //����ID                                      
	char         account_id[LEN_ID];                      //�˺�ID                                      
	char         account_name[LEN_NAME];                  //�˻���¼��                                  

	char         cl_ord_id[LEN_ID];                       //ί�пͻ���ID                                
	char         order_id[LEN_ID];                        //ί�й�̨ID                     
	char         ex_ord_id[LEN_ID];                       //ί�н�����ID   
	char         algo_order_id[LEN_ID];                   //�㷨ĸ��ID
	int          order_business;                          //ҵ������

	char         symbol[LEN_SYMBOL];                      //symbol                                      
	int          side;                                    //��������ȡֵ�ο�enum OrderSide            
	int          position_effect;                         //��ƽ��־��ȡֵ�ο�enum PositionEffect       
	int          position_side;                           //�ֲַ���ȡֵ�ο�enum PositionSide         

	int          order_type;                              //ί�����ͣ�ȡֵ�ο�enum OrderType            
	int          order_duration;                          //ί��ʱ�����ԣ�ȡֵ�ο�enum OrderDuration    
	int          order_qualifier;                         //ί�гɽ����ԣ�ȡֵ�ο�enum OrderQualifier   
	int          order_src;                               //ί����Դ��ȡֵ�ο�enum OrderSrc   
	int          position_src;                            //ͷ����Դ��������������ȯ����ȡֵ�ο� enum PositionSrc
	char         debtsno[LEN_ID];                         //����ҵ��, ����/����ʱ��Ӧ�ĺ�Լ���
	int          repay_type;                              //����ҵ��, ����ʱ�Ƿ�ֻ����Ϣ 0-��1-��

	int          status;                                  //ί��״̬��ȡֵ�ο�enum OrderStatus          
	int          ord_rej_reason;                          //ί�оܾ�ԭ��ȡֵ�ο�enum OrderRejectReason
	char         ord_rej_reason_detail[LEN_INFO];         //ί�оܾ�ԭ������                            

	double       price;                                   //ί�м۸�                                    
	double       stop_price;                              //ί��ֹ��/ֹӯ�����۸�                       

	int          order_style;                             //ί�з��ȡֵ�ο� enum OrderStyle          
	long long    volume;                                  //ί����                                      
	double       value;                                   //ί�ж�                                      
	double       percent;                                 //ί�аٷֱ�                                  
	long long    target_volume;                           //ί��Ŀ����                                  
	double       target_value;                            //ί��Ŀ���                                  
	double       target_percent;                          //ί��Ŀ��ٷֱ�                              

	long long    filled_volume;                           //�ѳ���                                      
	double       filled_vwap;                             //�ѳɾ���                                    
	double       filled_amount;                           //�ѳɽ��                                    
	double       filled_commission;                       //�ѳ�������                                  

	long long    created_at;                              //ί�д���ʱ��                                
	long long    updated_at;                              //ί�и���ʱ��  
};

struct AlgoOrder
{
	char         strategy_id[LEN_ID];                     //����ID                                      
	char         account_id[LEN_ID];                      //�˺�ID                                      
	char         account_name[LEN_NAME];                  //�˻���¼��                                  

	char         cl_ord_id[LEN_ID];                       //ί�пͻ���ID                                
	char         order_id[LEN_ID];                        //ί�й�̨ID                                  
	char         ex_ord_id[LEN_ID];                       //ί�н�����ID
	int          order_business;                          //ҵ������

	char         symbol[LEN_SYMBOL];                      //symbol                                      
	int          side;                                    //��������ȡֵ�ο�enum OrderSide            
	int          position_effect;                         //��ƽ��־��ȡֵ�ο�enum PositionEffect       
	int          position_side;                           //�ֲַ���ȡֵ�ο�enum PositionSide         

	int          order_type;                              //ί�����ͣ�ȡֵ�ο�enum OrderType            
	int          order_duration;                          //ί��ʱ�����ԣ�ȡֵ�ο�enum OrderDuration    
	int          order_qualifier;                         //ί�гɽ����ԣ�ȡֵ�ο�enum OrderQualifier   
	int          order_src;                               //ί����Դ��ȡֵ�ο�enum OrderSrc    
	int          position_src;                            //ͷ����Դ��������������ȯ����ȡֵ�ο� enum PositionSrc

	int          status;                                  //ί��״̬��ȡֵ�ο�enum OrderStatus          
	int          ord_rej_reason;                          //ί�оܾ�ԭ��ȡֵ�ο�enum OrderRejectReason
	char         ord_rej_reason_detail[LEN_INFO];         //ί�оܾ�ԭ������                            

	double       price;                                   //ί�м۸�                                    
	double       stop_price;                              //ί��ֹ��/ֹӯ�����۸�                       

	int          order_style;                             //ί�з��ȡֵ�ο� enum OrderStyle          
	long long    volume;                                  //ί����                                      
	double       value;                                   //ί�ж�                                      
	double       percent;                                 //ί�аٷֱ�                                  
	long long    target_volume;                           //ί��Ŀ����                                  
	double       target_value;                            //ί��Ŀ���                                  
	double       target_percent;                          //ί��Ŀ��ٷֱ�                              

	long long    filled_volume;                           //�ѳ���                                      
	double       filled_vwap;                             //�ѳɾ���                                    
	double       filled_amount;                           //�ѳɽ��                                    
	double       filled_commission;                       //�ѳ�������     

	char         algo_provider[LEN_NAME];                 //�㷨AppID
	char         algo_name[LEN_NAME];                     //�㷨������
	char         algo_param[LEN_PARAM];                   //�㷨���Բ���
	int          algo_status;                             //�㷨����״̬,����ΪAlgoOrder Pause������Σ�ȡֵ�ο� enum AlgoOrderStatus
	char         algo_comment[LEN_COMMENT];               //�㷨����ע

	long long    created_at;                              //ί�д���ʱ��                                
	long long    updated_at;                              //ί�и���ʱ��  
};

struct ExecRpt
{
	char               strategy_id[LEN_ID];                  //����ID                                                                                                        
	char               account_id[LEN_ID];                   //�˺�ID                                                       
	char               account_name[LEN_NAME];               //�˻���¼��                                                                                                    

	char               cl_ord_id[LEN_ID];                    //ί�пͻ���ID                                                                                                  
	char               order_id[LEN_ID];                     //ί�й�̨ID                                                                                                    
	char               exec_id[LEN_ID];                      //ί�лر�ID                                                                                                    

	char               symbol[LEN_SYMBOL];                   //symbol                                                                                                        

	int                position_effect;                      //��ƽ��־��ȡֵ�ο�enum PositionEffect                                                                         
	int                side;                                 //��������ȡֵ�ο�enum OrderSide                                                                              
	int                ord_rej_reason;                       //ί�оܾ�ԭ��ȡֵ�ο�enum OrderRejectReason                                                                  
	char               ord_rej_reason_detail[LEN_INFO];      //ί�оܾ�ԭ������                                                                                              
	int                exec_type;                            //ִ�лر�����, ȡֵ�ο�enum ExecType                                                                           

	double             price;                                //ί�гɽ��۸�                                                                                                  
	long long          volume;                               //ί�гɽ���                                                                                                    
	double             amount;                               //ί�гɽ����                                                                                                  
	double             commission;                           //ί�гɽ�������                                                                                                
	double             cost;                                 //ί�гɽ��ɱ����  
	long long          created_at;                           //�ر�����ʱ��

};

struct Cash
{
	char          account_id[LEN_ID];                        //�˺�ID               
	char          account_name[LEN_NAME];                    //�˻���¼��                                                                   

	int           currency;                                  //����                                                                         

	double        nav;                                        //��ֵ(cum_inout + cum_pnl + fpnl - cum_commission)                            
	double        pnl;                                        //������(nav-cum_inout)                                                        
	double        fpnl;                                       //����ӯ��(sum(each position fpnl))                                            
	double        frozen;                                     //�ֲ�ռ���ʽ�                                                                 
	double        order_frozen;                               //�ҵ������ʽ�                                                                 
	double        available;                                  //�����ʽ�                                                                     
															  //no  leverage:  available=(cum_inout + cum_pnl - cum_commission - frozen - order_frozen)        
															  //has leverage:  fpnl     =(fpnl>0 ? fpnl : (frozen < |fpnl|) ? (frozen-|fpnl|) : 0)             
															  //               available=(cum_inout + cum_pnl - cum_commission - frozen - order_frozen + fpnl) 
	double        balance;                                    //�ʽ����                                                                     
	double        market_value;                               //�ֲ���ֵ
	double        market_value_long;                          //Ȩ������ֵ(��Ȩ)
	double        market_value_short;                         //�������ֵ(��Ȩ)
	double        used_bail;                                  //���ñ�֤��(��Ȩ)
	double        enable_bail;                                //���ñ�֤��(��Ȩ)
	double        cum_inout;                                  //�ۼƳ����                                                                   
	double        cum_trade;                                  //�ۼƽ��׶�                                                                   
	double        cum_pnl;                                    //�ۼ�ƽ������(û�۳�������)                                                   
	double        cum_commission;                             //�ۼ�������                                                                   

	double        last_trade;                                 //��һ�ν��׶�                                                                 
	double        last_pnl;                                   //��һ������                                                                   
	double        last_commission;                            //��һ��������                                                                 
	double        last_inout;                                 //��һ�γ����                                                                 
	int           change_reason;                              //�ʽ���ԭ��ȡֵ�ο�enum CashPositionChangeReason                          
	char          change_event_id[LEN_ID];                    //�����ʽ����¼���ID     

	long long     created_at;                                 //�ʽ��ʼʱ��
	long long     updated_at;                                 //�ʽ���ʱ��

};

struct Position
{
	char                 account_id[LEN_ID];              //�˺�ID                                       
	char                 account_name[LEN_NAME];          //�˻���¼��                                                                                       

	char                 symbol[LEN_SYMBOL];              //symbol                                       
	int                  side;                            //�ֲַ���ȡֵ�ο�enum PositionSide          
	long long            volume;                          //�ֲܳ���; ��ֲ���(volume-volume_today)                                                          
	long long            volume_today;                    //���ճֲ���                                                                                       
	double               vwap;                            //�ֲ־���(��ƱΪ���ڿ��ּ۵ĳֲ־��ۣ��ڻ�Ϊ���ڽ���۵ĳֲ־���)  
	double               vwap_diluted;                    //̯���ɱ���
	double               vwap_open;                       //���ڿ��ּ۵ĳֲ־���(�ڻ�)
	double               amount;                          //�ֲֶ�(volume*vwap*multiplier)                                                                   

	double               price;                           //��ǰ����۸�                                                                                     
	double               fpnl;                            //�ֲָ���ӯ��((price-vwap)*volume*multiplier) 
	double               fpnl_diluted;                    //�ֲָ���ӯ��,���ڳֲ־���((price-vwap_diluted)*volume*multiplier)
	double               fpnl_open;                       //�ֲָ���ӯ��,���ڿ��־��ۣ��������ڻ�((price-vwap_open)*volume*multiplier)
	double               cost;                            //�ֲֳɱ�(vwap*volume*multiplier*margin_ratio)                                                    
	long long            order_frozen;                    //�ҵ������λ                                                                                     
	long long            order_frozen_today;              //�ҵ������ֲ�λ                                                                                 
	long long            available;                       //�����ܲ�λ(volume-order_frozen); �������λ(available-available_today)                           
	long long            available_today;                 //���ý��λ(volume_today-order_frozen_today)
	long long            available_now;                   //��ǰ��ƽ��λ
	long long            credit_position_sellable_volume; //����Ʒ������λ(����)
	double               market_value;                    //�ֲ���ֵ

	double               last_price;                      //��һ�γɽ���                                                                                     
	long long            last_volume;                     //��һ�γɽ���                                                                                     
	long long            last_inout;                      //��һ�γ���ֲ���                                                                                 
	int                  change_reason;                   //��λ���ԭ��ȡֵ�ο�enum CashPositionChangeReason                                              
	char                 change_event_id[LEN_ID];         //�����ʽ����¼���ID                                                                             

	int                  has_dividend;                    //�ֲ������зֺ�����   
	long long            created_at;                      //����ʱ��
	long long            updated_at;                      //��λ���ʱ��

};

struct Account 
{
	char            account_id[LEN_ID];           //�˺�ID
	char            account_name[LEN_NAME];       //�˻���¼��
	char            title[LEN_NAME];              //�˺�����
	char            intro[LEN_INFO];              //�˺�����
	char            comment[LEN_INFO];            //�˺ű�ע
};

struct AccountStatus
{
	char            account_id[LEN_ID];           //�˺�ID
	char            account_name[LEN_NAME];       //�˻���¼��
	int             state;                        //�˻�״̬
	int             error_code;                   //������
	char            error_msg[LEN_INFO];          //������Ϣ
};

struct Parameter
{
	char   key[LEN_ID];          //������
	double value;                //����ֵ
	double min;                  //�����õ���Сֵ
	double max;                  //�����õ����ֵ
	char   name[LEN_NAME];       //������
	char   intro[LEN_INFO];      //����˵��
	char   group[LEN_NAME];      //����
	bool   readonly;             //�Ƿ�ֻ��
};

struct Indicator
{
	char      account_id[LEN_ID];             //�˺�ID
	double    pnl_ratio;                      //�ۼ�������(pnl/cum_inout)
	double    pnl_ratio_annual;               //�껯������
	double    sharp_ratio;                    //���ձ���
	double    max_drawdown;                   //���س�
	double    risk_ratio;                     //���ձ���
	int       open_count;                     //���ִ���
	int       close_count;                    //ƽ�ִ���
	int       win_count;                      //ӯ������
	int       lose_count;                     //�������
	double    win_ratio;                      //ʤ��

	long long  created_at;                    //ָ�괴��ʱ��
	long long  updated_at;                    //ָ����ʱ��
};

struct CollateralInstrument
{
	char                 symbol[LEN_SYMBOL];  //����֤ȯ���
	char                 name[LEN_NAME];      //����
	double               pledge_rate;         //������
};

struct BorrowableInstrument
{
	char                 symbol[LEN_SYMBOL];        //����֤ȯ���
	char                 name[LEN_NAME];            //����
	double               margin_rate_for_cash;      //���ʱ�֤�����
	double               margin_rate_for_security;  //��ȯ��֤�����
};

struct BorrowableInstrumentPosition
{
	char                 symbol[LEN_SYMBOL];        //����֤ȯ���
	char                 name[LEN_NAME];            //����
	double               balance;                   //֤ȯ���
	double               available;                 //֤ȯ����
};

struct CreditContract
{
	char symbol[LEN_SYMBOL];              //֤ȯ���� stkcode    
	char name[LEN_NAME];                  //����
	int orderdate;                        //ί������                   
	char ordersno[LEN_ID];                //ί �� ��                   
	char creditdirect;                    //������ȯ����               
	double orderqty;                      //ί������                   
	double matchqty;                      //�ɽ�����                   
	double orderamt;                      //ί�н��                   
	double orderfrzamt;                   //ί�ж�����               
	double matchamt;                      //�ɽ����                   
	double clearamt;                      //������                   
	char lifestatus;                      //��Լ״̬                   
	int enddate;                          //��ծ��ֹ����               
	int oldenddate;                       //ԭʼ�ĸ�ծ��ֹ����         
	double creditrepay;                   //T��֮ǰ�黹���            
	double creditrepayunfrz;              //T�չ黹���                
	double fundremain;                    //Ӧ�����                   
	double stkrepay;                      //T��֮ǰ�黹����            
	double stkrepayunfrz;                 //T�չ黹����                
	double stkremain;                     //Ӧ��֤ȯ����               
	double stkremainvalue;                //Ӧ��֤ȯ��ֵ               
	double fee;                           //������ȯϢ����             
	double overduefee;                    //����δ����Ϣ����           
	double fee_repay;                     //������Ϣ����               
	double punifee;                       //��Ϣ�����ķ�Ϣ             
	double punifee_repay;                 //��������Ϣ                 
	double rights;                        //δ����Ȩ����             
	double overduerights;                 //����δ����Ȩ��             
	double rights_repay;                  //������Ȩ��                 
	double lastprice;                     //���¼�                     
	double profitcost;                    //����ӯ��                   
	int sysdate;                          //ϵͳ����                   
	char sno[LEN_ID];                     //��Լ���                   
	int lastdate;                         //���һ�μ���Ϣ������       
	int closedate;                        //��Լȫ����������           
	double punidebts;                     //���ڱ���Ϣ               
	double punidebts_repay;               //����Ϣ����               
	double punidebtsunfrz;                //���ڱ���Ϣ               
	double punifeeunfrz;                  //����Ϣ�ѷ�Ϣ               
	double punirights;                    //����Ȩ�淣Ϣ               
	double punirights_repay;              //Ȩ�淣Ϣ����               
	double punirightsunfrz;               //����Ȩ�淣Ϣ               
	double feeunfrz;                      //ʵʱ������Ϣ               
	double overduefeeunfrz;               //ʵʱ����������Ϣ           
	double rightsqty;                     //δ����Ȩ������             
	double overduerightsqty;              //����δ����Ȩ������    
};


struct CreditCash
{
	double fundintrrate;                        //��������                 
	double stkintrrate;                         //��ȯ����                 
	double punishintrrate;                      //��Ϣ����                 
	char   creditstatus;                        //����״̬                 
	double marginrates;                         //ά�ֵ�������             
	double realrate;                            //ʵʱ��������             
	double asset;                               //���ʲ�                   
	double liability;                           //�ܸ�ծ                   
	double marginavl;                           //��֤�������             
	double fundbal;                             //�ʽ����                 
	double fundavl;                             //�ʽ������               
	double dsaleamtbal;                         //��ȯ���������ʽ�         
	double guaranteeout;                        //��ת�������ʲ�           
	double gagemktavl;                          //����֤ȯ��ֵ             
	double fdealavl;                            //���ʱ���                 
	double ffee;                                //����Ϣ��                 
	double ftotaldebts;                         //���ʸ�ծ�ϼ�             
	double dealfmktavl;                         //Ӧ����ȯ��ֵ             
	double dfee;                                //��ȯϢ��                 
	double dtotaldebts;                         //��ȯ��ծ�ϼ�             
	double fcreditbal;                          //�������Ŷ��             
	double fcreditavl;                          //���ʿ��ö��             
	double fcreditfrz;                          //���ʶ�ȶ���             
	double dcreditbal;                          //��ȯ���Ŷ��             
	double dcreditavl;                          //��ȯ���ö��             
	double dcreditfrz;                          //��ȯ��ȶ���             
	double rights;                              //����Ȩ��                 
	double serviceuncomerqrights;               //����Ȩ��(��;)           
	double rightsqty;                           //���Ȩ��                 
	double serviceuncomerqrightsqty;            //���Ȩ��(��;)           
	double acreditbal;                          //�ܶ��                   
	double acreditavl;                          //�ܿ��ö��               
	double acashcapital;                        //�����ֽ��ʲ��������ʲ���������ȯ������     
	double astkmktvalue;                        //����֤ȯ��ֵ�������������롢�ǵ���Ʒ��     
	double withdrawable;                        //��ȡ�ʽ�                                   
	double netcapital;                          //���ʲ�                                     
	double fcreditpnl;                          //����ӯ��                                   
	double dcreditpnl;                          //��ȯӯ��                                   
	double fcreditmarginoccupied;               //����ռ�ñ�֤��                             
	double dcreditmarginoccupied;               //��ȯռ�ñ�֤��                             
	double collateralbuyableamt;                //���򵣱�Ʒ�ʽ�                             
	double repayableamt;                        //�ɻ�����                                 
	double dcreditcashavl;                      //��ȯ�����ʽ�     
};

//�¹��깺���
struct IPOQI
{
	char exchange[LEN_TYPE];        //�г�����
	double quota;                   //�г����
	double sse_star_quota;          //�Ϻ��ƴ������
};

struct IPOInstruments
{
	char     symbol[LEN_SYMBOL];     //�깺�¹�symbol
	double   price;                  //�깺�۸�
	int      min_vol;                //�깺��С����
	int      max_vol;                //�깺�������
};

struct IPOMatchNumber
{
	char order_id[LEN_ID];           //ί�к�
	char symbol[LEN_SYMBOL];         //�¹�symbol
	int   volume;                    //�ɽ�����
	char match_number[LEN_ID];       //�깺���
	int  order_at;                   //ί������
	int  match_at;                    //�������
};

struct IPOLotInfo
{
	char symbol[LEN_SYMBOL];   //�¹�symbol   
	int order_at;              //ί������     
	int lot_at;                //��ǩ����     
	int lot_volume;            //��ǩ����     
	int give_up_volume;        //��������     
	double price;              //��ǩ�۸�     
	double amount;             //��ǩ���     
	double pay_volume;         //�ѽɿ�����   
	double pay_amount;         //�ѽɿ���   

};

struct SmartReorderParameter
{
	char      symbol[LEN_SYMBOL];
	double    price;
	int       volume;
	int       side;
	int       order_type;
	int       position_effect;
	int       repeat_n;
	int       max_price_offset;
	int       time_out;
	int       time_wait;
	char      account_id[LEN_ID];             //�˺�ID
	int       reorder_id;                     //����:���ر������ܵ���ID, �����ڳ������ܵ�
};

struct OptionPreorderValidVolumeRequest
{
	char      symbol[LEN_SYMBOL];
	double    price;
	int       side;
	int       covered_flag;
	char      account_id[LEN_ID];             //�˺�ID
};

struct OptionPreorderSellMarginRequest
{
	char      symbol[LEN_SYMBOL];
	char      account_id[LEN_ID];             //�˺�ID
};

struct TradingTimes
{
	char       variety_name[LEN_ID];                     //Ʒ��ID
	char       time_trading[LEN_TIME_LIST];              //��������ʱ��
	char       time_callauction[LEN_TIME_LIST];          //���Ͼ���ʱ��
};

struct BacktestConfig 
{
	BacktestConfig()
	{
		initial_cash = 1000000;
		transaction_ratio = 1;
		commission_ratio = 0;
		commission_unit = 0;
		slippage_ratio = 0;
		option_float_margin_ratio1 = 0.2;
		option_float_margin_ratio2 = 0.4;
		adjust = 0;
		match_mode = 0;
		check_cache = 1;
	}

	char   start_time[LEN_ISO_DATATIME];
	char   end_time[LEN_ISO_DATATIME];
	double initial_cash;
	double transaction_ratio;
	double commission_ratio;
	double commission_unit;
	double slippage_ratio;
	double option_float_margin_ratio1;
	double option_float_margin_ratio2;
	int    adjust;
	int    match_mode;
	int    check_cache;
};


// �������ݽṹ ////////////////////////////////////////////

struct SymbolInfo
{
	// ��Ĵ���
	char symbol[LEN_SYMBOL];
	// ֤ȯƷ�ִ���
	long long sec_type1;
	// ֤ȯƷ��ϸ��
	long long sec_type2;
	// ���
	long long board;
	// ����������
	char exchange[LEN_EXCHANGE];
	// ��������Ĵ���
	char sec_id[LEN_SYMBOL];
	// �������������
	char sec_name[LEN_NAME];
	// ��������ļ��
	char sec_abbr[LEN_NAME];
	// ��С�䶯��λ
	double price_tick;
	// �����ƶ� 0��ʾT+0 1��ʾT+1 2��ʾT+2
	long long trade_n;
	// �������� ֤ȯ/ָ���������� ����Ʒ��Լ�Ĺ�����
	char listed_date[LEN_ISO_DATATIME];
	// �������� ��Ʊ/����������� �ڻ�/��Ȩ�ĵ�����(�������) ��תծ����صǼ���
	char delisted_date[LEN_ISO_DATATIME];
	// ����ʲ�
	char underlying_symbol[LEN_SYMBOL];
	// ��Ȩ��ʽ
	char option_type[LEN_TYPE];
	// ��Ȩ��֤��������1
	double option_margin_ratio1;
	// ��Ȩ��֤��������2
	double option_margin_ratio2;
	// ��Լ����
	char call_or_put[LEN_TYPE];
	// ��תծ��ʼת������
	char conversion_start_date[LEN_ISO_DATATIME];
	// ����ʱ��
	char update_time[LEN_ISO_DATATIME];
	// ���֤ȯ����-����
	int sec_type;
};

struct SymbolContent
{
	// ��������
	char trade_date[LEN_ISO_DATATIME];
	// ��Լ����
	bool is_adjusted;
	// �Ƿ�ͣ��
	bool is_suspended;
	// �ֲ���
	long long position;
	// �����
	double settle_price;
	// ������
	double pre_settle;
	// �����̼�
	double pre_close;
	// ��ͣ��
	double upper_limit;
	// ��ͣ��
	double lower_limit;
	// ������
	double turn_rate;
	// ��Ȩ����
	double adj_factor;
	// ��֤�����
	double margin_ratio;
	// ת�ɼ�
	double conversion_price;
	// ��Ȩ��
	double exercise_price;
	// ��Լ����
	long long multiplier;
	// ���� info �е��ֶ�
	SymbolInfo info;
	// �Ƿ�ST
	bool is_st;
};

struct TradingDateContent
{
	//// ��Ȼ��
	char date[LEN_ISO_DATATIME];
	//// ������
	char trade_date[LEN_ISO_DATATIME];
	//// ��һ��������
	char next_trade_date[LEN_ISO_DATATIME];
	//// ��һ��������
	char pre_trade_date[LEN_ISO_DATATIME];
};

struct Session
{
	char start[LEN_ISO_DATATIME];
	char end[LEN_ISO_DATATIME];
};

struct TradingSession
{
	// ��Ĵ���
	char symbol[LEN_SYMBOL];
	// ����������
	char exchange[LEN_EXCHANGE];

	// ��������ʱ��
	Session time_trading[MAX_TRADING_SESSION_NUM];
	// time_trading����ʵ�ʳ���
	int time_trading_num;

	// ���Ͼ���ʱ��
	Session time_auction[MAX_TRADING_SESSION_NUM];
	// time_auctionʵ�ʳ���
	int time_auction_num;
};

struct ContractExpireRestDays
{
	// ���� �����ַ�������
	char date[LEN_ISO_DATATIME];
	// ��Ĵ���
	char symbol[LEN_SYMBOL];
	// ʣ������ ����������Ϊ���ַ���
	char days_to_expire[LEN_INFO];
};

struct StkIndustryCategory
{
	// ��ҵ����
	char industry_code[LEN_ID];
	// ��ҵ����
	char industry_name[LEN_NAME];
};

struct StkIndustryConstituent
{
	// ��ҵ����
	char industry_code[LEN_ID];
	// ��ҵ����
	char industry_name[LEN_NAME];
	// �ɷֹ�Ʊ����, ��ʽ: exchange.sec_id
	char symbol[LEN_SYMBOL];
	// �ɷֹ�Ʊ����
	char sec_name[LEN_NAME];
	// ��������, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char date_in[LEN_ISO_DATATIME];
	// �޳�����, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char date_out[LEN_ISO_DATATIME];
};

struct StkSymbolIndustry
{
	// ��Ʊ����, ��ʽ: exchange.sec_id
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// ��ҵ����
	char industry_code[LEN_ID];
	// ��ҵ����
	char industry_name[LEN_NAME];
};

struct StkIndexConstituent
{
	// ָ������
	char index[LEN_SYMBOL];
	// �ɷֹɴ���
	char symbol[LEN_SYMBOL];
	// �ɷֹ�Ȩ��
	double weight;
	// ����ֵ, ��λ: ��Ԫ
	double market_value_total;
	// ��ͨ��ֵ, ��λ: ��Ԫ
	double market_value_circ;
	// ��������, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char trade_date[LEN_ISO_DATATIME];
};

struct StkDividend
{
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ���䷽��, ���ֽ�ֺ�, �͹�, ת��
	char scheme_type[LEN_TYPE];
	// ������, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char pub_date[LEN_ISO_DATATIME];
	// ��Ȩ��Ϣ��, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char ex_date[LEN_ISO_DATATIME];
	// ��Ȩ�Ǽ���, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char equity_reg_date[LEN_ISO_DATATIME];
	// �ֽ����������(��Ϣ��), ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char cash_pay_date[LEN_ISO_DATATIME];
	// �ͣ�ת�����ɷݵ�����, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char share_acct_date[LEN_ISO_DATATIME];
	// ���������, �ͣ�ת�����ɷ����н�����, ����ʱ��, ��ʽΪ: YYYY-MM-DD
	char share_lst_date[LEN_ISO_DATATIME];
	// ˰�������Ԫ/10�ɣ�
	double cash_af_tax;
	// ˰ǰ������Ԫ/10�ɣ�
	double cash_bf_tax;
	// �͹ɱ���, 10:X
	double bonus_ratio;
	// ת������, 10:X
	double convert_ratio;
	// �ɱ���׼��
	char base_date[LEN_ISO_DATATIME];
	// �ɱ�����(��׼�ɱ�)
	double base_share;
};

struct StkAdjFactor
{
	// ��������
	char trade_date[LEN_ISO_DATATIME];
	// ���պ�Ȩ����, T�պ�Ȩ����=T-1�յ����̼�/T��ǰ�ռ�
	double adj_factor_bwd;
	// �����ۼƺ�Ȩ����, T���ۼƺ�Ȩ����=T�պ�Ȩ����*T-1���ۼƺ�Ȩ����
	// ����һ���ۼƺ�Ȩ����=��һ����Ȩ���ӣ�
	double adj_factor_bwd_acc;
	// ����ǰ��Ȩ����, T��ǰ��Ȩ����=T�պ�Ȩ����/��Ȩ��׼�պ�Ȩ����
	double adj_factor_fwd;
	// �����ۼ�ǰ��Ȩ����, T���ۼ�ǰ��Ȩ����=T�պ�Ȩ����
	// T-1���ۼ�ǰ��Ȩ����=T�պ�Ȩ����*T-1�պ�Ȩ����
	// ����һ���ۼ�ǰ��Ȩ����=�����ۼƺ�Ȩ���ӣ�
	double adj_factor_fwd_acc;
};

struct StkSectorCategory
{
	// ������
	char sector_code[LEN_SYMBOL];
	// �������
	char sector_name[LEN_NAME];
};

struct StkSectorConstituent
{
	// ������
	char sector_code[LEN_SYMBOL];
	// �������
	char sector_name[LEN_NAME];
	// ��Ʊ����, ��ʽ: exchange.sec_id
	char symbol[LEN_SYMBOL];
	// �ɷֹ�Ʊ����
	char sec_name[LEN_NAME];
};

struct StkSymbolSector
{
	// ��Ʊ����, ��ʽ: exchange.sec_id
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// ������
	char sector_code[LEN_SYMBOL];
	// �������
	char sector_name[LEN_NAME];
};

struct StkRation
{
	// ��Ĵ���
	char symbol[LEN_SYMBOL];
	// ������
	char pub_date[LEN_ISO_DATATIME];
	// ��Ȩ�Ǽ���
	char equity_reg_date[LEN_ISO_DATATIME];
	// ��Ȩ��Ϣ��
	char ex_date[LEN_ISO_DATATIME];
	// ��ɱ���
	double ration_ratio;
	// ��ɼ۸�
	double ration_price;
};

struct StkShareholderNum
{
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// ��������
	char pub_date[LEN_ISO_DATATIME];
	// ��ֹ����
	char expiry_date[LEN_ISO_DATATIME];
	// �ɶ�����
	long long total_share;
	// A�ɹɶ�����
	long long total_share_a;
	// ��ͨB�ɹɶ�����
	long long total_share_b;
	// ��ͨH�ɹɶ�����
	long long total_share_h;
	// �����ɶ�����
	long long other_share;
	// ���ȹɹɶ����������Ȩ�ָ���
	long long total_share_pfd;
	// �ɶ���������������ȯ��
	long long total_share_mgn;
	// �ɶ�����������������ȯ��
	long long total_share_no_mgn;
};

struct StkShareholder
{
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// ��������
	char pub_date[LEN_ISO_DATATIME];
	// ��ֹ����
	char expiry_date[LEN_ISO_DATATIME];
	// �ɶ�����
	char holder_name[LEN_COMMENT];
	// �ɶ���ţ����Σ�
	int holder_rank;
	// �ɶ�����
	char holder_type[LEN_INFO];
	// �ɶ�����
	char holder_attr[LEN_INFO];
	// �ɷ�����(�ɷ�����)
	char share_type[LEN_INFO];
	// �����������ɣ�
	double share_num;
	// �ֹɱ���1, �ֹ�ռ�ܹɱ�������%��
	double share_ratio1;
	// �ֹɱ���2, �ֹ�ռ��������ͨ�ɱ�����%��
	double share_ratio2;
	// ��Ѻ�ɷ�����, ��Ȩ��Ѻ�漰�������ɣ�
	double share_pledge;
	// ����ɷ�����, ��Ȩ�����漰�������ɣ�
	double share_freeze;
};

struct StkShareChange
{
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��˾����
	char company_name[LEN_COMMENT];
	// ��������
	char pub_date[LEN_ISO_DATATIME];
	// �ɱ��䶯����
	char chg_date[LEN_ISO_DATATIME];
	// �ɱ��䶯ԭ��
	char chg_reason[LEN_COMMENT];
	// �ɱ��䶯�¼�
	char chg_event[LEN_COMMENT];
	// �ܹɱ�, δ��ͨ�ɷ�+����ͨ�ɷ�, ��λ: ��
	double share_total;
	// δ��ͨ�ɷ�
	double share_total_nlf;
	// �����˹ɷ�: ���з����˹� + ������ᷨ�˹� + ���������˹ɷ�, ��λ: ��
	double share_prom;
	// ���з����˹�: ���ҳֹ�+���з��˹�, ��λ: ��
	double share_prom_state;
	// ���ҹ�
	double share_state;
	// ���з��˹�
	double share_state_lp;
	// ������ᷨ�˹�: ������ᷨ�˹�+���ⷨ�˹�, ��λ: ��
	double share_prom_soc;
	// ������ᷨ�˹�
	double share_dc_lp;
	// ���ⷨ�˹�
	double share_os_lp;
	// ���������˹ɷ�
	double share_prom_other;
	// ļ���˹ɷ�: ļ�����ҹ�+ļ�����ڷ��˹�+ļ�����ⷨ�˹�, ��λ: ��
	double share_rs;
	// ļ�����ҹ�
	double share_rs_state;
	// ļ�����ڷ��˹�: ļ�����ڹ��з��˹�+ļ��������ᷨ�˹�, ��λ: ��
	double share_rs_dc_lp;
	// ļ�����ڹ��з��˹�
	double share_rs_state_lp;
	// ļ��������ᷨ�˹�
	double share_rs_soc_lp;
	// ļ�����ⷨ�˹�
	double share_rs_os_lp;
	// �ڲ�ְ����
	double share_emp_nlf;
	// ���ȹ�
	double share_pfd_nlf;
	// ����δ��ͨ�ɷ�
	double share_oth_nlf;
	// ��ͨ�ɷ�
	double share_circ;
	// �����������ɷ�
	double share_ttl_unl;
	// �������ͨ�ɣ�A�ɣ�
	double share_a_unl;
	// �����������ʹɣ�B�ɣ�
	double share_b_unl;
	// �����������ʹɣ�H�ɣ�
	double share_h_unl;
	// ��������ͨ�ɷ�
	double share_oth_unl;
	// �����������ɷ�
	double share_ttl_ltd;
	// һ�������������ɷ�: ���۹��ҳֹ�+ ���۹��з��˳ֹ�+ �����������ʳֹ�+ �������ʳֹ�+ �����ɷ�+ �߹ֹܳ�, ��λ: ��
	double share_gen_ltd;
	// ���۹��ҳֹ�
	double share_state_ltd;
	// ���۹��з��˳ֹ�
	double share_state_lp_ltd;
	// �����������ʳֹ�: ���۾��ڷǹ��з��˳ֹ�+���۾�����Ȼ�˳ֹ�, ��λ: ��
	double share_oth_dc_ltd;
	// ���۾��ڷǹ��з��˳ֹ�
	double share_nst_dc_lp_ltd;
	// ���۾�����Ȼ�˳ֹ�
	double share_dc_np_ltd;
	// �������ʳֹ�: ���۾��ⷨ�˳ֹ�+���۾�����Ȼ�˳ֹ�, ��λ: ��
	double share_forn_ltd;
	// ���۾��ⷨ�˳ֹ�
	double share_os_lp_ltd;
	// ���۾�����Ȼ�˳ֹ�
	double share_os_np_ltd;
	// �����ɷ�
	double share_lk_ltd;
	// �߹ֹܳ�(ԭʼ��¶)
	double share_gm_ltd;
	// ���۷��˳ֹ�: ս��Ͷ�������۹ɷ�+һ�㷨��Ͷ��������+ ֤ȯͶ�ʻ������۹ɷ�, ��λ: ��
	double share_plc_lp_ltd;
	// ս��Ͷ�������۹ɷ�
	double share_plc_si_ltd;
	// һ�㷨��Ͷ�������۹ɷ�
	double share_plc_lp_gen_ltd;
	// ֤ȯͶ�ʻ������۹ɷ�
	double share_plc_fnd_ltd;
	// ������ͨA��
	double share_a_ltd;
	// ������ͨB��
	double share_b_ltd;
	// ������ͨH��
	double share_h_ltd;
	// �������۹ɷ�
	double share_oth_ltd;
	// �䶯�ɷ�������
	char share_list_date[LEN_ISO_DATATIME];

};



struct FutContinuousContractsInfo
{
	// ��Ĵ���
	char symbol[LEN_SYMBOL];
	// ��������
	char trade_date[LEN_ISO_DATATIME];
};

struct FutContractInfo
{
	// ����Ʒ��  --����Ʒ�����ƣ��磺����300ָ������
	char  product_name[LEN_NAME];
	// ���״���  --����Ʒ�ִ��룬�磺IF��AL
	char  product_code[LEN_TYPE];
	// ��Լ��� --�磺SHSE.000300�� AL
	char  underlying_symbol[LEN_SYMBOL];
	// ��Լ����  --�磺200��5
	int   multiplier;
	// ���׵�λ  --�磺ÿ�������200Ԫ��5��/��
	char  trade_unit[LEN_INFO];
	// ���۵�λ   --�磺ָ���㣬Ԫ������ң�/��
	char  price_unit[LEN_INFO];
	// �۸���С�䶯��λ  --�磺0.2�㣬5Ԫ/��
	char  price_tick[LEN_INFO];
	// ��Լ�·�  --�磺���¡����¼�����������£�1��12��
	char  delivery_month[LEN_INFO];
	// ����ʱ��  --�磺��9:30-11:30��13:00-15:00����������9:00��11:30 ������1:30��3:00�ͽ������涨����������ʱ�䡱
	char  trade_time[LEN_INFO];
	// �ǵ�ͣ�����  --ÿ�ռ۸���󲨶����ƣ��磺����һ�������ս���۵ġ�10%��������һ�����ս���ۡ�3%��
	char  price_range[LEN_INFO];
	// ��ͽ��ױ�֤��  --��������������ͱ�֤��������磺����Լ��ֵ��8%��������Լ��ֵ��5%��
	char  minimum_margin[LEN_INFO];
	// �������   -- �磺����Լ�����·ݵĵ����������壬�����ҷ�������˳�ӡ�������Լ�·ݵ�15�գ������ҷ����ڼ���˳�ӣ������·ݵ�������ս����������е�����֪ͨ����
	char  last_trade_date[LEN_COMMENT];
	// ��������  --�磺��ͬ������ա�����������պ��������������ա�
	char  delivery_date[LEN_INFO];
	// ���ʽ  --�磺�ֽ𽻸ʵ�ｻ��
	char  delivery_method[LEN_INFO];
	// ���������� --���н��������ƣ��磺�й������ڻ����������Ϻ��ڻ�������
	char  exchange_name[LEN_INFO];
	// ����������  --���н��������룬�磺CFFEX��SHFE
	char  exchange[LEN_SYMBOL];

};

struct FutTransactionRanking
{
	// �ڻ���Լ����  --���ʹ��ʱ�ο�symbol
	char symbol[LEN_SYMBOL];
	// ��������  --
	char trade_date[LEN_ISO_DATATIME];
	// �ڻ���˾��Ա���
	char member_name[LEN_NAME];
	// ����ָ����ֵ  --��λ���֡��Ӻ���ѡ������ָ��indicator���ֱ�Ϊ���ɽ�����indicatorΪ'volume'ʱ����������indicatorΪ'long'ʱ������������indicatorΪ��short��ʱ��
	int  indicator_number;
	// ����ָ����Ͻ���������  --��λ����
	int  indicator_change;
	// ��������
	int  ranking;
	// �������α��Ͻ���������
	int  ranking_change;
	// �ж� ranking_change ��ֵ�Ƿ�Ϊ��
	bool ranking_change_is_null;
};

struct FutWarehouseReceiptInfo
{
	// �������� --
	char trade_date[LEN_ISO_DATATIME];
	// �ڻ����������� --�ڻ�Ʒ�ֶ�Ӧ���������룬�磺CFFEX��SHFE
	char  exchange[LEN_SYMBOL];
	// �ڻ����������� --���н��������ƣ��磺�й������ڻ����������Ϻ��ڻ�������
	char exchange_name[LEN_NAME];
	// ���״��� --����Ʒ�ִ��룬�磺IF��AL
	char product_code[LEN_SYMBOL];
	// ����Ʒ�� --����Ʒ�����ƣ��磺����300ָ������
	char product_name[LEN_NAME];
	// ע��ֵ����� --
	int  on_warrant;
	// �ֵ���λ -- ��֧��֣����Ʒ��
	char warrant_unit[LEN_TYPE];
	// �ֿ����� --
	char warehouse[LEN_NAME];
	// �ڻ���� --
	int  future_inventory;
	// �ڻ�������� --
	int  future_inventory_change;
	// ���ÿ����� --
	int  warehouse_capacity;
	// ���ÿ��������� --
	int  warehouse_capacity_change;
	// ���С�� --
	int  inventory_subtotal;
	// ���С������ --
	int  inventory_subtotal_change;
	// ��ЧԤ�� --��֧��֣����Ʒ��
	int  effective_forecast;
	// ����ˮ --
	int  premium;
};

struct FndAdjFactorInfo
{
	// ��������    --���½����յ�����
	char trade_date[LEN_ISO_DATATIME];
	// ���պ�Ȩ���� --T�պ�Ȩ����=T-1�յ����̼�/T��ǰ�ռ�
	double adj_factor_bwd_acc;
	// ����ǰ��Ȩ����   --T��ǰ��Ȩ����=T�պ�Ȩ����/��Ȩ��׼�պ�Ȩ����
	double adj_factor_fwd;
};

struct FndDividendInfo
{
	// �������   --��ѯ�ֺ���Ϣ�Ļ������
	char fund[LEN_SYMBOL];
	// ������
	char pub_date[LEN_ISO_DATATIME];
	// ��������
	char event_progress[LEN_COMMENT];
	// ��Ϣ���� --10:X��ÿ10��˰ǰ�ֺ�
	double dvd_ratio;
	// ���������׼��
	char dvd_base_date[LEN_ISO_DATATIME];
	// Ȩ��Ǽ���
	char rt_reg_date[LEN_ISO_DATATIME];
	// ʵ�ʳ�Ϣ��
	char ex_act_date[LEN_ISO_DATATIME];
	// ���ڳ�Ϣ��
	char ex_dvd_date[LEN_ISO_DATATIME];
	// ���ں���������
	char pay_dvd_date[LEN_ISO_DATATIME];
	// ���ں������˻�������
	char trans_dvd_date[LEN_ISO_DATATIME];
	// ������Ͷ��ȷ����
	char reinvest_cfm_date[LEN_ISO_DATATIME];
	// ������Ͷ�ʷݶ����
	char ri_shr_arr_date[LEN_ISO_DATATIME];
	// ������Ͷ�������ʼ��
	char ri_shr_rdm_date[LEN_ISO_DATATIME];
	// �ɷ������� --��λ��Ԫ
	double earn_distr;
	// ����ʵ�ʺ������� --��λ��Ԫ
	double cash_pay;
	// ��׼�ջ���ݶֵ
	double base_unit_nv;
};

struct FndSplitInfo
{
	// �������
	char  fund[LEN_SYMBOL];
	// ������
	char pub_date[LEN_ISO_DATATIME];
	// �����������
	char split_type[LEN_INFO];
	// ����������
	double split_ratio;
	// ��������׼��
	char base_date[LEN_ISO_DATATIME];
	// ������㳡�ڳ�Ȩ��
	char ex_date[LEN_ISO_DATATIME];
	// ����ݶ����Ǽ���
	char share_change_reg_date[LEN_ISO_DATATIME];
	// ������¶��ֵ���������
	char nv_split_pub_date[LEN_ISO_DATATIME];
	// Ȩ��Ǽ���
	char rt_reg_date[LEN_ISO_DATATIME];
	// ���ڳ�Ȩ��(���̼�)
	char ex_date_close[LEN_ISO_DATATIME];
};

struct FndEtfConstituents
{
	// ETF����
	char etf[LEN_SYMBOL];
	// ETF����
	char etf_name[LEN_NAME];
	// ��������
	char trade_date[LEN_ISO_DATATIME];
	// �ɷֹɴ���
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	double amount;
	// �ֽ������־
	char cash_subs_type[LEN_TYPE];
	// �̶�������
	double cash_subs_sum;
	// �ֽ������۱��� --��λ��%
	double cash_premium_rate;
};

struct FndPortfolioStockInfo
{
	// �������  --��ѯ�ʲ���ϵĻ������
	char fund[LEN_SYMBOL];
	// ��������
	char fund_name[LEN_NAME];
	// ��������  --��ָ��ʱ���[��ʼʱ��,����ʱ��]�ڵĹ�������
	char pub_date[LEN_ISO_DATATIME];
	// ������ -- �ֲֽ�ֹ����
	char period_end[LEN_ISO_DATATIME];
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// �ֲֹ���
	double hold_share;
	// �ֲ���ֵ
	double hold_value;
	// ռ��ֵ����  --��λ��%
	double nv_rate;
	// ռ�ܹɱ����� --��λ��%
	double ttl_share_rate;
	// ռ��ͨ�ɱ��� --��λ��%s
	double circ_share_rate;
};

struct FndPortfolioBondInfo
{
	// �������  --��ѯ�ʲ���ϵĻ������
	char fund[LEN_SYMBOL];
	// ��������
	char fund_name[LEN_NAME];
	// ��������  --��ָ��ʱ���[��ʼʱ��,����ʱ��]�ڵĹ�������
	char pub_date[LEN_ISO_DATATIME];
	// ������ -- �ֲֽ�ֹ����
	char period_end[LEN_ISO_DATATIME];
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// �ֲֹ���
	double hold_share;
	// �ֲ���ֵ
	double hold_value;
	// ռ��ֵ���� --��λ��%
	double nv_rate;
};

struct FndPortfolioFundInfo
{
	// �������  --��ѯ�ʲ���ϵĻ������
	char fund[LEN_SYMBOL];
	// ��������
	char fund_name[LEN_NAME];
	// ��������  --��ָ��ʱ���[��ʼʱ��,����ʱ��]�ڵĹ�������
	char pub_date[LEN_ISO_DATATIME];
	// ������ -- �ֲֽ�ֹ����
	char period_end[LEN_ISO_DATATIME];
	// ��Ʊ����
	char symbol[LEN_SYMBOL];
	// ��Ʊ����
	char sec_name[LEN_NAME];
	// �ֲֹ���
	double hold_share;
	// �ֲ���ֵ
	double hold_value;
	// ռ��ֵ���� --��λ��%
	double nv_rate;
};

struct FndNetValueInfo
{
	// �������  --��ѯ��ֵ�Ļ������
	char fund[LEN_SYMBOL];
	// ��������
	char trade_date[LEN_ISO_DATATIME];
	// ��λ��ֵ  --T�յ�λ��ֵ��ÿ������ݶ����T�յľ�ֵ��Ҳ������ļ۸�
	double unit_nv;
	// �ۼƵ�λ��ֵ  --T���ۼƾ�ֵ��ָ���ڻ������֮��Ͷ�ʸû���1ԪǮ�����ֽ�ֺ췽ʽ�£�����T���˻��ľ�ֵ
	double unit_nv_accu;
	// ��Ȩ��λ��ֵ  --T�ո�Ȩ��ֵ��ָ���ڻ������֮��Ͷ�ʸû���1ԪǮ���ڷֺ���Ͷ�ʷ�ʽ�£�����T���˻��ľ�ֵ
	double unit_nv_adj;
};

struct BndConversionPrice
{
	// ��������
	char pub_date[LEN_ISO_DATATIME];
	// ת�ɼ۸���Ч����
	char effective_date[LEN_ISO_DATATIME];
	// ִ������
	char execution_date[LEN_ISO_DATATIME];
	// ת�ɼ۸� --��λ��Ԫ
	double conversion_price;
	// ת�ɱ��� --��λ��%
	double conversion_rate;
	// ��λ����
	double conversion_volume;
	// �ۼ�ת�ɽ�� --��λ����Ԫ���ۼ�תծ�Ѿ�תΪ��Ʊ�Ľ��ۼ�ÿ��ת�ɽ��
	double conversion_amount_total;
	// ծȯ��ͨ��� --��λ����Ԫ
	double bond_float_amount_remain;
	// �¼�����  --��ʼת�ɼۣ�����ת�ɼۣ�����ת�ɼ�
	char event_type[LEN_INFO];
	// ת�ɼ۱䶯ԭ��
	char change_reason[LEN_COMMENT];
};

struct BndCallInfo
{
	// ������ --��ع�����
	char pub_date[LEN_ISO_DATATIME];
	// ����� --��������Ȩ�գ�ʵ�ʣ�������������������ڼ��ջ�˳��Ϊ�ǽڼ���
	char call_date[LEN_ISO_DATATIME];
	// ��صǼ��� --���۵Ǽ��գ��ǽڼ���
	char record_date[LEN_ISO_DATATIME];
	// ����ʽ����� --Ͷ������ؿ����
	char cash_date[LEN_ISO_DATATIME];
	// ������� --������أ�ȫ�����
	char call_type[LEN_INFO];
	// ���ԭ�� --1:�������������2:ǿ����أ�3:�������
	char call_reason[LEN_INFO];
	// ��ؼ۸� --��λ��Ԫ/�ţ�ÿ��Ԫ��ֵ��ؼ۸�Ԫ����ծȯ��ֵ �ӵ���Ӧ����Ϣ����˰��
	double call_price;
	// ��ؽ��
	double call_amount;
	// �Ƿ������Ϣ  -- 0:��������1:����
	bool interest_included;
};

struct BndPutInfo
{
	// ������ --��ع�����
	char pub_date[LEN_ISO_DATATIME];
	// ������ʼ�� --Ͷ������Ȩ��ʼ��
	char put_start_date[LEN_ISO_DATATIME];
	// ���۽�ֹ�� --Ͷ������Ȩ��ֹ��
	char put_end_date[LEN_ISO_DATATIME];
	// �����ʽ����� --Ͷ���߻��ۿ����
	char cash_date[LEN_ISO_DATATIME];
	// ����ԭ�� --1:����������2:���㸽�ӻ�������
	char put_reason[LEN_INFO];
	// ���ۼ۸� --��λ��Ԫ/�ţ�ÿ��Ԫ��ֵ���ۼ۸�Ԫ����ծȯ��ֵ �ӵ���Ӧ����Ϣ����˰��
	double put_price;
	// ���۽��
	double put_amount;
	// �Ƿ������Ϣ  -- 0:��������1:����
	bool   interest_included;
};

struct BndAmountChange
{
	// ������
	char pub_date[LEN_ISO_DATATIME];
	// �䶯���� --�׷���������ת�� ����ء�����(ע��)������
	char change_type[LEN_INFO];
	// �䶯����
	char change_date[LEN_ISO_DATATIME];
	// ���α䶯��� --��λ����Ԫ
	double change_amount;
	// ʣ���� --�䶯�����λ����Ԫ
	double remain_amount;
};

#endif
