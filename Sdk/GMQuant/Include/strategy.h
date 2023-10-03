#ifndef GMCXXSDK_STRATEGY_H
#define GMCXXSDK_STRATEGY_H

#include "gmapi.h"

#define GM_CLASS GM_CXX_API 

class GM_CLASS Strategy
{
public:
	Strategy(const char *token, const char *strategy_id, int mode);
	Strategy();
	virtual ~Strategy();

public: //��������

	//���в���
	int run();

	//ֹͣ����
	void stop();

	//���ò���ID
	void set_strategy_id(const char *strategy_id);

	//�����û�token
	void set_token(const char *token);

	//���ò�������ģʽ
	void set_mode(int mode);

	//��ʱ����
	int schedule(const char *data_rule, const char *time_rule);

	//������ʱ��
	int timer(int period, int start_delay);

	//ֹͣ��ʱ��
	void timer_stop(int timer_id);

	//��ǰʱ��
	double now();

	//���ûز����
	int set_backtest_config(
		const char  *start_time,
		const char  *end_time,
		double initial_cash = 1000000,
		double transaction_ratio = 1,
		double commission_ratio = 0,
		double slippage_ratio = 0,
		int    adjust = 0,
		int    check_cache = 1
	);

	//���ûز����
	int set_backtest_config(BacktestConfig &cfg);
	//��ȡ������Ϣ
	const char *get_last_error_detail();
		

public: //���ݺ���

		// ��������
	int subscribe(const char *symbols, const char * frequency, bool unsubscribe_previous = false);

	// �˶�����
	int unsubscribe(const char *symbols, const char * frequency);


public: //���׺���

	//��ѯ�����˺�
	DataArray<Account>* get_accounts();

	//��ѯָ�������˺�״̬
	int get_account_status(const char *account, AccountStatus &as);

	//��ѯ���н����˺�״̬
	DataArray<AccountStatus>* get_all_account_status();

	//��ָ����ί��
	Order order_volume(const char *symbol, int volume, int side, int order_type, int position_effect, double price = 0, const char *account = NULL);

	//��ָ����ֵί��
	Order order_value(const char *symbol, double value, int side, int order_type, int position_effect, double price = 0, const char *account = NULL);

	//�����ʲ�ָ������ί��
	Order order_percent(const char *symbol, double percent, int side, int order_type, int position_effect, double price = 0, const char *account = NULL);

	//���ֵ�Ŀ��ֲ���
	Order order_target_volume(const char *symbol, int volume, int position_side, int order_type, double price = 0, const char *account = NULL);

	//���ֵ�Ŀ��ֲֶ�
	Order order_target_value(const char *symbol, double value, int position_side, int order_type, double price = 0, const char *account = NULL);

	//���ֵ�Ŀ��ֱֲ��������ʲ��ı�����
	Order order_target_percent(const char *symbol, double percent, int position_side, int order_type, double price = 0, const char *account = NULL);

	//ƽ��ǰ���п�ƽ�ֲ�
	DataArray<Order>* order_close_all();

	//ί�г���
	int order_cancel(const char *cl_ord_id, const char *account = NULL);

	//��������ί��
	int order_cancel_all();

	//ί���µ�
	Order place_order(const char *symbol, int volume, int side, int order_type, int position_effect, double price = 0, int order_duration = 0, int order_qualifier = 0, double stop_price = 0, int order_business = 0, const char *account = NULL);

	//�̺󶨼۽���
	Order order_after_hour(const char *symbol, int volume, int side, double price, const char *account = NULL);

	//�Զ�׷��
	int smart_reorder(SmartReorderParameter &param);

	//�����Զ�׷��
	int smart_reorder_cancel(int reorder_id);

	//��ѯί��
	DataArray<Order>* get_orders(const char *account = NULL);

	//��ѯδ��ί��
	DataArray<Order>* get_unfinished_orders(const char *account = NULL);

	//��ѯ�ɽ�
	DataArray<ExecRpt>* get_execution_reports(const char *account = NULL);

	//��ѯ�ʽ�
	DataArray<Cash>* get_cash(const char *accounts = NULL);

	//��ѯ�ֲ�
	DataArray<Position>* get_position(const char *account = NULL);

	//ί���㷨��
	AlgoOrder order_algo(const char *symbol, int volume, int position_effect, int side, int order_type, double price, const char *provider, const char *algo_name, const char *algo_param, const char *account = NULL);

	//�����㷨ί��
	int algo_order_cancel(const char *provider, const char *cl_ord_id, const char *account = NULL);

	//��ͣ/�ָ��㷨��
	int algo_order_pause(const char *provider, const char *cl_ord_id, int status, const char *account = NULL);

	//��ѯ�㷨ί��
	DataArray<AlgoOrder>* get_algo_orders(const char *account = NULL);

	//��ѯ�㷨��ί��
	DataArray<Order>* get_algo_child_orders(const char *cl_ord_id, const char *account = NULL);

	//���ܺŵ���
	int raw_func(const char *account, const char *func_id, const char *func_args, char*&rsp);


	/* ����ҵ�� */

	//��������
	Order credit_buying_on_margin(int position_src, const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��ȯ����
	Order credit_short_selling(int position_src, const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��ȯ��ȯ
	Order credit_repay_share_by_buying_share(int position_src, const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��ȯ��ȯ(ָ����Լ)
	Order credit_repay_share_by_buying_share_on_debt(int position_src, const char *symbol, int volume, double price, const char *debtsno, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��ȯ����
	Order credit_repay_cash_by_selling_share(int position_src, const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��ȯ����(ָ����Լ����Ϣ����)
	Order credit_repay_cash_by_selling_share_on_debt(int position_src, const char *symbol, int volume, double price, const char *debtsno, int bond_fee_only, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//����Ʒ����
	Order credit_buying_on_collateral(const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//����Ʒ����
	Order credit_selling_on_collateral(const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//ֱ�ӻ�ȯ
	Order credit_repay_share_directly(int position_src, const char *symbol, int volume, const char *account = NULL);
	//ֱ�ӻ�ȯ(ָ����Լ)
	Order credit_repay_share_directly_on_debt(int position_src, const char *symbol, int volume, const char *debtsno, const char *account = NULL);
	//ֱ�ӻ���
	int credit_repay_cash_directly(int position_src, double amount, const char *account = NULL, double *actual_repay_amount = NULL, char *error_msg_buf = NULL, int buf_len = 0);
	//ֱ�ӻ���(ָ����Լ����Ϣ����)
	int credit_repay_cash_directly_ex(int position_src, double amount, const char *debtsno, int bond_fee_only, const char *account = NULL, double *actual_repay_amount = NULL, char *error_msg_buf = NULL, int buf_len = 0);
	//����Ʒת��
	Order credit_collateral_in(const char *symbol, int volume, const char *account = NULL);
	//����Ʒת��
	Order credit_collateral_out(const char *symbol, int volume, const char *account = NULL);
	//��ѯ����֤ȯ
	DataArray<CollateralInstrument>* credit_get_collateral_instruments(const char *account = NULL);
	//��ѯ���֤ȯ��������ȯ��Ĺ�Ʊ�б�
	DataArray<BorrowableInstrument>* credit_get_borrowable_instruments(int position_src, const char *account = NULL);
	//��ѯȯ����ȯ�˻�ͷ�磬������ȯ������
	DataArray<BorrowableInstrumentPosition>* credit_get_borrowable_instruments_positions(int position_src, const char *account = NULL);
	//��ѯ������ȯ��Լ
	DataArray<CreditContract>* credit_get_contracts(int position_src, const char *account = NULL);
	//��ѯ������ȯ�ʽ�
	int credit_get_cash(CreditCash &cash, const char *account = NULL);
	//��ѯ������ȯ��Լ���ʽ���Ϣ
	DataSet* credit_get_report(const char *func_id, const char *account = NULL);

	/* �¹�/��ծҵ�� */

	//�¹�/��ծ�깺
	Order ipo_buy(const char *symbol, int volume, double price, const char *account = NULL);
	//��ѯ�ͻ��¹�/��ծ�깺���
	DataArray<IPOQI>* ipo_get_quota(const char *account = NULL);
	//��ѯ�����¹�/��ծ�嵥
	DataArray<IPOInstruments>* ipo_get_instruments(int security_type, const char *account = NULL);
	//��Ų�ѯ
	DataArray<IPOMatchNumber>* ipo_get_match_number(const char* start_date, const char* end_date, const char *account = NULL);
	//��ǩ��ѯ
	DataArray<IPOLotInfo>* ipo_get_lot_info(const char* start_date, const char* end_date, const char *account = NULL);

	/* ����ҵ�� */

	//ETF�깺
	Order fund_etf_buy(const char *symbol, int volume, double price, const char *account = NULL);
	//ETF���
	Order fund_etf_redemption(const char *symbol, int volume, double price, const char *account = NULL);
	//�����Ϲ�
	Order fund_subscribing(const char *symbol, int volume, const char *account = NULL);
	//�����깺
	Order fund_buy(const char *symbol, int volume, const char *account = NULL);
	//�������
	Order fund_redemption(const char *symbol, int volume, const char *account = NULL);

	/* ծȯҵ�� */

	//��ծ��ع�
	Order bond_reverse_repurchase_agreement(const char *symbol, int volume, double price, int order_type = OrderType_Limit, int order_duration = OrderDuration_Unknown, int order_qualifier = OrderQualifier_Unknown, const char *account = NULL);
	//��תծת��
	Order bond_convertible_call(const char *symbol, int volume, double price, const char *account = NULL);
	//��תծ����
	Order bond_convertible_put(const char *symbol, int volume, double price, const char *account = NULL);
	//��תծ���۳���
	Order bond_convertible_put_cancel(const char *symbol, int volume, const char *account = NULL);

	/* ��Ȩҵ�� */

	//���ҿ���
	Order option_covered_open(const char *symbol, int volume, int order_type, double price = 0, const char *account = NULL);
	//����ƽ��
	Order option_covered_close(const char *symbol, int volume, int order_type, double price = 0, const char *account = NULL);
	//��Ȩ
	Order option_exercise(const char *symbol, int volume, const char *account = NULL);
	//��ȡ�ɿ�����
	int option_preorder_valid_volume(OptionPreorderValidVolumeRequest &request, int &volume);
	//������Ȩ�������ֱ�֤��
	int option_preorder_sell_margin(OptionPreorderSellMarginRequest &request, double &margin);

public: //���Բ����ຯ��

	//��Ӳ���
	int add_parameters(Parameter *params, int count);

	//ɾ������
	int del_parameters(const char *keys);

	//���ò���
	int set_parameters(Parameter *params, int count);

	//��ȡ����
	DataArray<Parameter>* get_parameters();

	//���ñ��
	int set_symbols(const char *symbols);

	//��ȡ���
	DataArray<Symbol>* get_symbols();


public: //�¼�����

		//��ʼ�����
		virtual void on_init();
		//�յ�Tick����
		virtual void on_tick(Tick *tick);
		//�յ�bar����
		virtual void on_bar(Bar *bar);
		//�յ���ʳɽ���L2����ʱ��Ч��
		virtual void on_l2transaction(L2Transaction *l2transaction);
		//�յ����ί�У����L2����ʱ��Ч��
		virtual void on_l2order(L2Order *l2order);
		//�յ�ί�ж��У��Ͻ���L2����ʱ��Ч��
		virtual void on_l2order_queue(L2OrderQueue *l2queue);
		//ί�б仯
		virtual void on_order_status(Order *order);
		//ִ�лر�
		virtual void on_execution_report(ExecRpt *rpt);
		//�㷨ί�б仯
		virtual void on_algo_order_status(AlgoOrder *order);
		//�ʽ�����
		virtual void on_cash(Cash *cash);
		//�ֲ�����
		virtual void on_position(Position *position);
		//�����仯
		virtual void on_parameter(Parameter *param);
		//��ʱ���񴥷�
		virtual void on_schedule(const char *data_rule, const char *time_rule);
		//��ʱ������
		virtual void on_timer(int timer_id);
		//�ز���ɺ��յ���Ч����
		virtual void on_backtest_finished(Indicator *indicator);
		//ʵ���˺�״̬�仯
		virtual void on_account_status(AccountStatus *account_status);
		//�������
		virtual void on_error(int error_code, const char *error_msg);
		//�յ�����ֹͣ�ź�
		virtual void on_stop();
		//�����Ѿ�������
		virtual void on_market_data_connected();
		//�����Ѿ�������
		virtual void on_trade_data_connected();
		//�������ӶϿ���
		virtual void on_market_data_disconnected();
		//�������ӶϿ���
		virtual void on_trade_data_disconnected();

};




#endif
