#ifndef DATASTRUCT_H
#define DATASTRUCT_H
struct  IKBar //K线数据
{
	char	m_sInstrumentID[30]; //合约编号
	char	m_sTradingDay[16];	//本bar交易日期
	char	m_sBeginTime[12];	//本bar开始时间
	char	m_sEndTime[12];	//本bar结束时间
	int	    m_microsecond;
	double	m_fOpen;	//开盘价;
	double	m_fHighest;	//最高价
	double	m_fLowest;	//最低价
	double	m_fClose;	//收盘价
	double	m_fAverage;	//成交量加权均价
	double	m_fAverage2;	//算数均价
	long	m_nVolume;	//成交量
	long	m_nVolumeDiff;//成交量变化
	double	m_fAmmount; //成交金额,
	double	m_fAmmountDiff; //成交金额变化
	long	m_nPosition;	//持仓量
	long	m_nPositionDiff;//持仓量变化;
	long	m_nStatus;//Bar状态;0-未关闭,1关闭;
	double m_routatime;
	int	m_nIndex;
};
struct TradeStruct //记录每次开平交易的结果体-----用于Ctrade ,Log
{
	char trading_day[9];
	char trading_time[9];
	int microsecond;
	char instrument[30];
	char buyorsell[4];
	char openorcolse[4];
	double price;
	int volume;
	double tradefee;
};
struct OpenCloseStruct //记录开和平的两个报单的结构体，构成一个开平交易对，供Log使用
{
	char Stretegy[200];//策略名称
	TradeStruct Open;//开仓信息
	TradeStruct Close;//平仓信息
	double Currentprofit;//当前开平仓盈利
	double Totalprofit;//所有利润
};
struct MonthResult //月度报告
{
	char StregreyName[200];//策略名字
	char TradingMonth[9];//交易月份
	double InitMoney;//当月初始资金
	double NetProfit;//净利润
	double NetProfitRatio;//收益率
	double MaxProfit;//最大纯利润
	double MaxLoss;//最大亏损
	/************************************************************************/
	/* 内部变量，不输出                                                     */
	/************************************************************************/
	double CurrentMaxProfit;//当前最大纯利润
	double CurrentMaxLoss;//当前最大亏损
	char CurrentMonth[9];//当前月份
	double LastMonthProfit;//前一月的利润
};
struct DayResult //日报告
{
	char StregreyName[200];//策略名字
	char TradingMonth[9];//交易日期
	double InitMoney;//当月初始资金
	double NetProfit;//净利润
	double NetProfitRatio;//收益率
	double MaxProfit;//最大纯利润
	double MaxLoss;//最大亏损
	/************************************************************************/
	/* 内部变量，不输出                                                     */
	/************************************************************************/
	double CurrentMaxProfit;//当前最大纯利润
	double CurrentMaxLoss;//当前最大亏损
	char CurrentMonth[9];//当前月份
	double LastMonthProfit;//前一月的利润
};
struct YearResult //年度报告
{
	char StrageryName[200];//策略名字
	char TradingYear[9];//交易念佛
	double NetProfit;//净利润
	double NetProfitRatio;//收益率
	double MaxProfit;//最大纯利润
	double MaxLoss;//最大亏损
	/************************************************************************/
	/* 内部变量，不输出                                                     */
	/************************************************************************/
	double CurrentMaxProfit;//当前最大纯利润
	double CurrentMaxLoss;//当前最大亏损
};
struct TotalResult //总结报告
{
	char StrageryName[200];//策略的名字和参数
	double InitMoney;//初始资金-ok
	double Netprofit;//净利润-ok
	double ProfitRatio;//收益率-ok
	double NetprofitOneYears;//年化净收益-ok
	double ProfitOneYearsRatio;//年度收益率-ok
	double WintimesPercent;//盈利比率-ok
	double AvgprofitOneTime;//平均利润-ok
	int Tradetimes;//交易次数-ok
	double MaxLossAlways;//最大资产回测--ok
	double IncRatio;//增长系数
	double WinRisk;//收益风险
	double Sharp;//夏普比
	double MaxLossPercenOneDay;//单日最大回撤比例-ok
	double MaxLossOneDay;
	double MaxLossPercentNetprofit;//最大资产回撤比例-ok

	double WinLossRatio;//盈亏比-ok
	double TotalProfit;//总盈利-ok
	double TotalLoss;//总亏损-ok
	int WinTimes;//盈利次数-ok
	int LossTimes;//亏损次数-ok
	int AlwaysWinTimes;//最大连续盈利次数-ok
	int AlwaysLossTimes;//最大连续亏损次数-ok
	double MaxProfitOneTimes;//单次最大盈利-ok
	double MaxLossOneTimes;//单次最大亏损-ok
	double AvgWinOneWinTimes;//平均盈利-ok
	double AvgLossOneLossTimes;//平均亏损-ok
	/************************************************************************/
	/* 一些内部统计的辅助变量       不输出                                  */
	/************************************************************************/
	int TradeDayNum;//交易日数量
	double CurrentMaxPofit;//当前最高收益
	double CurrentMinProfit;//当前最低收益
	double CurrentMaxLoss;//当前最大资产回测
	double CurrentMaxLossPercenOneDay;//当前单日最大回撤比例
	double CurrentMaxLossPercentNetprofit;//当前最大资产回撤比例
	int CurrentAlwaysWinTimes;//当前最大连续盈利次数
	int currentAlwaysLossTimes;//当前最大连续亏损次数
	double CurrentYestdayProfit;//前一日总资金
	double CurrentMaxLossToday;//当日亏损
};
#endif
