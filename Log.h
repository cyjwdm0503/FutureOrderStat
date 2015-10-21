#ifndef LOG_H
#define LOG_H
#include <list>
#include <vector>
#include "DataStruct.h"
using namespace std;

class Clog
{
private:
	double m_Mutil;//合约乘数
	double m_InitMoney;//初始资金
	vector<OpenCloseStruct> m_TradeList;//记录开平仓信息
	TotalResult m_totalresult;
	vector<MonthResult> m_MonthList;
	vector<YearResult> m_YearList;
	vector<DayResult> m_DayList;
	bool thisisgloble;
public:
	Clog(double mutil,double money,bool flag=true);
	Clog(){};
	bool Initialize(double mutil,double money,bool flag=true);
	//输入开平仓点位和时间
	void InsertOpenClose( TradeStruct *OpenTrade, TradeStruct *CloseTrade,char* strategyname=NULL);

	//输出交易记录到文件
	void FlushTradeList(char *dirName);

	//输出汇总后的交易记录
	void FlushYearList(char *dirName);

	//输出汇总后的每日记录
	void FlushDayList(char *dirName);

	//输出月度总结
	void FlushMonthList(char *dirName);

	//输出汇总总结
	void FlushTotalList(char *dirName);
};

#endif

