#ifndef GMCPPSDK_H
#define GMCPPSDK_H

#include <iostream>
#include "gmdef.h"

#ifdef WIN32
#ifdef GMSDKC_EXPORTS  
#define GM_CXX_API __declspec(dllexport)
#else
#define GM_CXX_API __declspec(dllimport)
#endif
#else
#define GM_CXX_API
#endif

//�������
class DataSet
{
public:
	//��ȡapi���ý��, 0: �ɹ��� ��0: ������
	virtual int status() = 0;

	//��status() != 0 ʱ�����ش���������Ϣ, utf-8����
	virtual const char* errer_msg() = 0;

	//�ж��Ƿ��Ѿ��ǵ�������ĩβ
	virtual bool is_end() = 0;

	//�Ƶ���һ����¼
	virtual void next() = 0;

	//��ȡ����ֵ
	virtual int get_integer(const char *key) = 0;

	//��ȡ������ֵ
	virtual long long get_long_integer(const char *key) = 0;

	//��ȡ������ֵ
	virtual double get_real(const char *key) = 0;

	//��ȡ�ַ�����ֵ
	virtual const char* get_string(const char *key) = 0;

	//�ͷ����ݼ���
	virtual void release() = 0;

	//��ӡ����
	virtual const char* debug_string() = 0;
};

//�������
template <typename T>
class DataArray
{
public:
	//��ȡapi���ý��, 0: �ɹ��� ��0: ������
	virtual int status() = 0;

	//��status() != 0 ʱ�����ش���������Ϣ, utf-8����
	virtual const char* errer_msg() = 0;

	//���ؽṹ�����ָ��
	virtual T* data() = 0;

	//�������ݵĳ���
	virtual int count() = 0;

	//�����±�Ϊi�Ľṹ���ã���0��ʼ
	virtual T& at(int i) = 0;

	//�ͷ����ݼ���
	virtual void release() = 0;
};

// ��ȡsdk�汾��
GM_CXX_API const char* get_version();

// ����token
GM_CXX_API void set_token(const char *token);

// �Զ�������ַ
GM_CXX_API void set_serv_addr(const char *addr);

// �Զ�������ַv5�汾
GM_CXX_API void set_serv_addr_v5(const char *addr, const char *orgcode, const char *site_id);

// ���ûز��߳�[1-32]
GM_CXX_API void set_backtest_threadnum(int n);

//������ϵͳ����������Ϣ
GM_CXX_API void set_mfp(const char *mfp);

GM_CXX_API void set_simtrade_only();

GM_CXX_API void set_account_id(const char *account_ids);

// ��ѯ��ǰ�������
GM_CXX_API DataArray<Tick>* current(const char *symbols);

// ��ѯ��ʷTick����
GM_CXX_API DataArray<Tick>* history_ticks(const char *symbols, const char *start_time, const char *end_time, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

// ��ѯ��ʷBar����
GM_CXX_API DataArray<Bar>* history_bars(const char *symbols, const char *frequency, const char *start_time, const char *end_time, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

// ��ѯ����n��Tick����
GM_CXX_API DataArray<Tick>* history_ticks_n(const char *symbols, int n, const char *end_time = NULL, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

//��ѯ����n��Bar����
GM_CXX_API DataArray<Bar>* history_bars_n(const char *symbols, const char *frequency, int n, const char *end_time = NULL, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

// ��ѯ��ʷL2 Tick����
GM_CXX_API DataArray<Tick>* history_l2ticks(const char *symbols, const char *start_time, const char *end_time, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

// ��ѯ��ʷL2 Bar����
GM_CXX_API DataArray<Bar>* history_l2bars(const char *symbols, const char *frequency, const char *start_time, const char *end_time, int adjust = 0, const char *adjust_end_time = NULL, bool skip_suspended = true, const char *fill_missing = NULL);

// ��ѯ��ʷL2 ��ʳɽ�
GM_CXX_API DataArray<L2Transaction>* history_l2transactions(const char *symbols, const char *start_time, const char *end_time);

// ��ѯ��ʷL2 ���ί��
GM_CXX_API DataArray<L2Order>* history_l2orders(const char *symbols, const char *start_time, const char *end_time);

// ��ѯ��ʷL2 ί�ж���(���ż����50��ί����)
GM_CXX_API DataArray<L2OrderQueue>* history_l2orders_queue(const char *symbols, const char *start_time, const char *end_time);

// ��ѯ����������
GM_CXX_API DataSet* get_fundamentals(const char *table, const char *symbols, const char *start_date, const char *end_date, const char *fields, const char *filter = NULL, const char *order_by = NULL, int limit = 1000);

// ��ѯ��������������n��
GM_CXX_API DataSet* get_fundamentals_n(const char *table, const char *symbols, const char *end_date, const char *fields, int n = 1, const char *filter = NULL, const char * order_by = NULL);

// ��ѯ���½��ױ����Ϣ
GM_CXX_API DataSet* get_instruments(const char *exchanges = NULL, const char *sec_types = NULL, const char* fields = NULL);

// ��ѯ���ױ����ʷ����
GM_CXX_API DataSet* get_history_instruments(const char *symbols, const char *start_date, const char *end_date, const char *fields = NULL);

// ��ѯ���ױ�Ļ�����Ϣ
GM_CXX_API DataSet* get_instrumentinfos(const char *symbols = NULL, const char *exchanges = NULL, const char * sec_types = NULL, const char *names = NULL, const char *fields = NULL);

// ��ѯָ���ɷݹ�
GM_CXX_API DataSet* get_constituents(const char *index, const char *trade_date = NULL);

// ��ѯ��ҵ��Ʊ�б�
GM_CXX_API DataArray<Symbol>* get_industry(const char *code);

// ��ѯ�������Ʊ�б�
GM_CXX_API DataArray<Symbol>* get_concept(const char *code);

// ��ѯ�������б�
GM_CXX_API DataArray<TradingDate>* get_trading_dates(const char *exchange, const char *start_date, const char *end_date);

// ����ָ�����ڵ���һ��������
GM_CXX_API int get_previous_trading_date(const char *exchange, const char *date, char *output_date);

// ����ָ�����ڵ���һ��������
GM_CXX_API int get_next_trading_date(const char *exchange, const char *date, char *output_date);

// ��ѯ�ֺ�����
GM_CXX_API DataSet* get_dividend(const char *symbol, const char *start_date, const char *end_date);

// ��ȡ������Լ
GM_CXX_API DataSet* get_continuous_contracts(const char *csymbol, const char *start_date, const char *end_date);

// ��ѯ��ѯƷ����Ϣ
GM_CXX_API DataArray<VarietyInfo>* get_varietyinfos(const char *variety_names);

// ��ѯ����ʱ��
GM_CXX_API DataArray<TradingTimes>* get_trading_time(const char *variety_names);

// ��ѯ��Ȩ��Լ
GM_CXX_API DataArray<Symbol>* option_get_symbols_by_exchange(const char *exchange = NULL, const char *trade_date = NULL, const char *call_or_put = NULL);

// ��ѯʵƽ��ֵĳ����Լ
GM_CXX_API DataArray<Symbol>* option_get_symbols_by_in_at_out(const char *underlying_symbol = NULL, const char *trade_date = NULL, int excute_month = 0, const char *call_or_put = NULL, int in_at_out = IN_AT_OUT_NO_SPECIFIED, const char *price_type = NULL, double price = -1);

// ��ѯ��Ȩ�������б�
GM_CXX_API DataArray<TradingDate>* option_get_delisted_dates(const char *underlying_symbol = NULL, const char *trade_date = NULL, int excute_month = 0);

// ��ѯ��Ȩ��Ȩ���б�
GM_CXX_API DataArray<double>* option_get_exercise_prices(const char *underlying_symbol = NULL, const char *trade_date = NULL, int excute_month = 0, const char *adjust_flag = NULL);


/*************************************************************** �������ݺ���  **************************************************************************/

//ͨ�û�������

//��ѯ��Ļ�����Ϣ
GM_CXX_API DataArray<SymbolInfo>* get_symbol_infos(long long sec_type1, long long sec_type2 = 0, const char *exchanges = NULL, const char *symbols = NULL);
//��ѯָ�������ն��Ľ�����Ϣ
GM_CXX_API DataArray<SymbolContent>* get_symbols_by_date(long long sec_type1, long long sec_type2 = 0, const char *exchanges = NULL, const char *symbols = NULL, bool skip_suspended = true, bool skip_st = true, const char *trade_date = NULL);
//��ѯָ����Ķ��ս�����Ϣ
GM_CXX_API DataArray<SymbolContent>* get_history_symbol(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ��Ƚ�������
GM_CXX_API DataArray<TradingDateContent>* get_trading_dates_by_year(const char *exchange, int start_year, int end_year);
//��ѯ����ʱ��
GM_CXX_API DataArray<TradingSession>* get_trading_session(const char *symbols);
// - ��ѯ��Լ����ʣ������
GM_CXX_API DataArray<ContractExpireRestDays>* get_contract_expire_rest_days(const char *symbols, const char *start_date, const char *end_date, bool trade_flag = false);

//��Ʊ��������

//��ѯ��ҵ����
GM_CXX_API DataArray<StkIndustryCategory>* stk_get_industry_category(const char *source = NULL, int level = 0);
//��ѯ��ҵ�ɷֹ�
GM_CXX_API DataArray<StkIndustryConstituent>* stk_get_industry_constituents(const char *industry_code, const char *date = NULL);
//��ѯ��Ʊ��������ҵ
GM_CXX_API DataArray<StkSymbolIndustry>* stk_get_symbol_industry(const char *symbols, const char *source = NULL, int level = 0, const char *date = NULL);
// - ��ѯ������
GM_CXX_API DataArray<StkSectorCategory>* stk_get_sector_category(const char *sector_type);
// - ��ѯ���ɷֹ�
GM_CXX_API DataArray<StkSectorConstituent>* stk_get_sector_constituents(const char *sector_code);
// - ��ѯ��Ʊ���������
GM_CXX_API DataArray<StkSymbolSector>* stk_get_symbol_sector(const char *symbols, const char *sector_type);

//��ѯָ�����³ɷֹ� 
GM_CXX_API DataArray<StkIndexConstituent>* stk_get_index_constituents(const char *symbol, const char *trade_date = NULL);
//��ѯ��Ʊ�ֺ��͹���Ϣ
GM_CXX_API DataArray<StkDividend>* stk_get_dividend(const char *symbol, const char *start_date, const char *end_date);
// - ��ѯ��Ʊ�����Ϣ
GM_CXX_API DataArray<StkRation>* stk_get_ration(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);

//��ѯ��Ʊ�ĸ�Ȩ����
GM_CXX_API DataArray<StkAdjFactor>* stk_get_adj_factor(const char *symbol, const char *start_date = NULL, const char *end_date = NULL, const char *base_date = NULL);
// - ��ѯ�ɶ�����
GM_CXX_API DataArray<StkShareholderNum>* stk_get_shareholder_num(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯʮ��ɶ�
GM_CXX_API DataArray<StkShareholder>* stk_get_top_shareholder(const char *symbol, const char *start_date = NULL, const char *end_date = NULL, bool tradable_holder = false);
// - ��ѯ�ɱ��䶯
GM_CXX_API DataArray<StkShareChange>* stk_get_share_change(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);

// - ��ѯ�ʲ���ծ������
GM_CXX_API DataSet* stk_get_fundamentals_balance(const char *symbol, const char *fields, int rpt_type = 0, int data_type = 0, const char *start_date = NULL, const char *end_date = NULL);
//  - ��ѯ�ֽ�����������
GM_CXX_API DataSet* stk_get_fundamentals_cashflow(const char *symbol, const char *fields, int rpt_type = 0, int data_type = 0, const char *start_date = NULL, const char *end_date = NULL);
//  - ��ѯ���������
GM_CXX_API DataSet* stk_get_fundamentals_income(const char *symbol, const char *fields, int rpt_type = 0, int data_type = 0, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯ�ʲ���ծ���������
GM_CXX_API DataSet* stk_get_fundamentals_balance_pt(const char *symbols, const char *fields, int rpt_type = 0, int data_type = 0, const char *date = NULL);
// - ��ѯ�ֽ��������������
GM_CXX_API DataSet* stk_get_fundamentals_cashflow_pt(const char *symbols, const char *fields, int rpt_type = 0, int data_type = 0, const char *date = NULL);
// - ��ѯ������������
GM_CXX_API DataSet* stk_get_fundamentals_income_pt(const char *symbols, const char *fields, int rpt_type = 0, int data_type = 0, const char *date = NULL);

// - ��ѯ������Ҫָ������
GM_CXX_API DataSet* stk_get_finance_prime(const char *symbol, const char *fields, int rpt_type = 0, int data_type = 0, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯ��������ָ������
GM_CXX_API DataSet* stk_get_finance_deriv(const char *symbol, const char *fields, int rpt_type = 0, int data_type = 0, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯ��ֵָ��ÿ������
GM_CXX_API DataSet* stk_get_daily_valuation(const char *symbol, const char *fields, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯ��ֵָ��ÿ������
GM_CXX_API DataSet* stk_get_daily_mktvalue(const char *symbol, const char *fields, const char *start_date = NULL, const char *end_date = NULL);
// - ��ѯ����ָ��ÿ������
GM_CXX_API DataSet* stk_get_daily_basic(const char *symbol, const char *fields, const char *start_date = NULL, const char *end_date = NULL);

// - ��ѯ������Ҫָ���������
GM_CXX_API DataSet* stk_get_finance_prime_pt(const char *symbols, const char *fields, int rpt_type = 0, int data_type = 0, const char *date = NULL);
// - ��ѯ��������ָ���������
GM_CXX_API DataSet* stk_get_finance_deriv_pt(const char *symbols, const char *fields, int rpt_type = 0, int data_type = 0, const char *date = NULL);
// - ��ѯ��ֵָ�굥�ս�������
GM_CXX_API DataSet* stk_get_daily_valuation_pt(const char *symbols, const char *fields, const char *trade_date = NULL);
// - ��ѯ��ֵָ�굥�ս�������
GM_CXX_API DataSet* stk_get_daily_mktvalue_pt(const char *symbols, const char *fields, const char *trade_date = NULL);
// - ��ѯ����ָ�굥�ս�������
GM_CXX_API DataSet* stk_get_daily_basic_pt(const char *symbols, const char *fields, const char *trade_date = NULL);


//�ڻ���������

//��ѯ������Լ��Ӧ����ʵ��Լ 
GM_CXX_API DataArray<FutContinuousContractsInfo>* fut_get_continuous_contracts(const char *csymbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ�ڻ���׼Ʒ����Ϣ
GM_CXX_API DataArray<FutContractInfo>* fut_get_contract_info(const char *product_codes);
//��ѯ�ڻ�ÿ�ճɽ��ֲ�����
GM_CXX_API DataArray<FutTransactionRanking>* fut_get_transaction_ranking(const char *symbol, const char *trade_date = NULL, const char *indicator = NULL);
//��ѯ�ڻ��ֵ�����
GM_CXX_API DataArray<FutWarehouseReceiptInfo>* fut_get_warehouse_receipt(const char *product_code, const char *start_date = NULL, const char *end_date = NULL);


//�����������

//��ѯETF���³ɷֹ�
GM_CXX_API DataArray<FndEtfConstituents>* fnd_get_etf_constituents(const char *symbol);
//��ѯ�����ʲ����(��ƱͶ�����)
GM_CXX_API DataArray<FndPortfolioStockInfo>* fnd_get_stock_portfolio(const char *symbol, int report_type, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ�����ʲ����(ծȯͶ�����)
GM_CXX_API DataArray<FndPortfolioBondInfo>* fnd_get_bond_portfolio(const char *symbol, int report_type, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ�����ʲ����(����Ͷ�����)
GM_CXX_API DataArray<FndPortfolioFundInfo>* fnd_get_fund_portfolio(const char *symbol, int report_type, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ����ֵ����
GM_CXX_API DataArray<FndNetValueInfo>* fnd_get_net_value(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ����Ȩ����
GM_CXX_API DataArray<FndAdjFactorInfo>* fnd_get_adj_factor(const char *symbol, const char *start_date = NULL, const char *end_date = NULL, const char *base_date = NULL);
//��ѯ����ֺ���Ϣ
GM_CXX_API DataArray<FndDividendInfo>* fnd_get_dividend(const char *symbol, const char *start_date, const char *end_date);
//��ѯ������������Ϣ
GM_CXX_API DataArray<FndSplitInfo>* fnd_get_split(const char *symbol, const char *start_date, const char *end_date);


//ծȯ��������

//��ѯ��תծת�ɼ۱䶯��Ϣ
GM_CXX_API DataArray<BndConversionPrice>* bnd_get_conversion_price(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ��תծ�����Ϣ
GM_CXX_API DataArray<BndCallInfo>* bnd_get_call_info(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ��תծ������Ϣ
GM_CXX_API DataArray<BndPutInfo>* bnd_get_put_info(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);
//��ѯ��תծʣ���ģ�䶯
GM_CXX_API DataArray<BndAmountChange>* bnd_get_amount_change(const char *symbol, const char *start_date = NULL, const char *end_date = NULL);

#endif
