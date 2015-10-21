/***
	时间太久远，已经不太记得。交流方式QQ：359444326 回到过去
***/
#ifndef CTRADE_H
#define CTRADE_H
#include "DataStruct.h"
#include "Log.h"
#include <list>
#include <vector>
class Ctrade
{
public:
	int m_position;
	double m_holdprice;
	double m_mutiltick;
private:
	double fee;//
	int type;
	int twosingle;
	vector<TradeStruct> m_openlist,m_closelist;
	Clog *log;
	char info[200];
public:

	/************************************************************************/
	/* 合约乘数，交易成本百分百，类型（0：手，1：金额）,单边双边（-1：收双边0：开仓收，1平仓手），初始资金*/
	/************************************************************************/
	Ctrade(double mutiltick,double percent,int type,int twoorsingle,double money,char* name);
	Ctrade(){};
	~Ctrade(){ delete log;log=NULL;};
	/************************************************************************/
	/* 合约乘数，交易成本百分百，类型（0：手，1：金额）,单边双边（-1：收双边0：开仓收，1平仓手）,初始资金*/
	/************************************************************************/
	bool Initialize(double mutiltick,double percent,int type,int twoorsingle,double money,char *name);
	void	Buy(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume);
	void	Sellshort(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume);
	void	BuyToCover(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume);
	void	Sell(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume);
	bool	FlushTradeList(char* dirName);
	bool   FlushTotalList(char *dirName);
	bool FlushMonthList(char *dirName);
	TradeStruct* GetLastOpen();
private:
	double GetTradefee(int open_close,double price,int vol);
};
#endif
