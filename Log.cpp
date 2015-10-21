#include "Log.h"
#include "direct.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <Windows.h>
extern Clog *GlobalLog;
void Clog::InsertOpenClose( TradeStruct *OpenTrade, TradeStruct *CloseTrade,char* strategyname/* =NULL */)
{
	//if(thisisgloble)
	//{
	//	HANDLE MUTEX=CreateMutex(NULL,FALSE,L"MYTESTMAIN");
	//	if(GetLastError()==ERROR_ALREADY_EXISTS)
	//		MUTEX=OpenMutex(MUTEX_ALL_ACCESS,NULL,L"MYTESTMAIN");
	//	WaitForSingleObject(MUTEX,INFINITE);
	//}
	OpenCloseStruct OPENCLOSE;
	memcpy(&OPENCLOSE.Open,OpenTrade,sizeof(TradeStruct));
	memcpy(&OPENCLOSE.Close,CloseTrade,sizeof(TradeStruct));
	if(strlen(strategyname)!=0)
	{strcpy(OPENCLOSE.Stretegy,strategyname);}
	if(strcmp(OpenTrade->buyorsell,"买")==0)
		OPENCLOSE.Currentprofit=(CloseTrade->price-OpenTrade->price)*CloseTrade->volume*this->m_Mutil-OpenTrade->tradefee-CloseTrade->tradefee;
	else
		OPENCLOSE.Currentprofit=(OpenTrade->price-CloseTrade->price)*CloseTrade->volume*this->m_Mutil-OpenTrade->tradefee-CloseTrade->tradefee;
	if(this->m_TradeList.size()!=0)
		OPENCLOSE.Totalprofit=this->m_TradeList.back().Totalprofit+OPENCLOSE.Currentprofit;
	else
		OPENCLOSE.Totalprofit=OPENCLOSE.Currentprofit+0;
	this->m_TradeList.push_back(OPENCLOSE);
}
Clog::Clog(double mutil,double money,bool flag)
{
	this->m_Mutil=mutil;
	this->m_InitMoney=money;
	thisisgloble=flag;
}
bool Clog::Initialize(double mutil,double money,bool flag)
{
	this->m_Mutil=mutil;
	this->m_InitMoney=money;
	thisisgloble=flag;
	return true;
}
void Clog::FlushTradeList(char *dirName)
{
	char *fullfilepath=new char[200],*buffer=new char[3056];
	sprintf(fullfilepath,".\\%s",dirName);
	if(this->m_TradeList.size()==0)
	{
		delete[] fullfilepath;
		fullfilepath=NULL;
		delete[] buffer;
		buffer=NULL;
		return ;
	}
	if(!thisisgloble)
		sprintf(fullfilepath,".\\%s\\%s%s.csv",dirName,"交易记录_策略",this->m_TradeList.begin()->Stretegy);
	else
		sprintf(fullfilepath,".\\%s\\%s.csv",dirName,"交易记录_策略组合");
	FILE *file=fopen(fullfilepath,"w");
	vector<OpenCloseStruct>::iterator it;
	if(file!=NULL)
	{
		for(it=this->m_TradeList.begin();it!=this->m_TradeList.end();it++)
		{
			fprintf(file,"%s,%s,%s,%d,%s,%s,%s,%lf,%d,%s,%s,%d,%s,%s,%s,%lf,%d,%lf,%lf\n",it->Stretegy,it->Open.trading_day,it->Open.trading_time,it->Open.microsecond,it->Open.instrument,it->Open.buyorsell,it->Open.openorcolse,it->Open.price,it->Open.volume
				,it->Close.trading_day,it->Close.trading_time,it->Close.microsecond,it->Close.instrument,it->Close.buyorsell,it->Close.openorcolse,it->Close.price,it->Open.volume,
				it->Currentprofit,it->Totalprofit);
		}
		fflush(file);
		fclose(file);
	}
	delete []fullfilepath;
	delete []buffer;
}
//输出汇总后的交易记录
void Clog::FlushYearList(char *filename)
{
	vector<OpenCloseStruct>::iterator it;
	for(it=this->m_TradeList.begin();it!=this->m_TradeList.end();it++)
	{
		;
	}
}
void Clog::FlushDayList(char *dirName)
{
	char CurrentDay[9];
	memset(CurrentDay,0,9);
	vector<OpenCloseStruct>::iterator it;
	if(this->m_TradeList.size()==0)
		return ;
	for(it=this->m_TradeList.begin();it!=this->m_TradeList.end();it++)
	{
		if(strlen(CurrentDay)==0)//第一天的结果
		{
			DayResult Month;
			memset(&Month,0,sizeof(MonthResult));
			Month.InitMoney=this->m_InitMoney+it->Totalprofit-it->Currentprofit;//设置当月初始资金
			this->m_DayList.push_back(Month);
			strcpy(CurrentDay,it->Close.trading_day);
		}
		else
		{
			if(strncmp(CurrentDay,it->Close.trading_day,8)!=0)//当前这天为新的一天的开头
			{
				DayResult Month;
				memset(&Month,0,sizeof(MonthResult));
				Month.InitMoney=this->m_InitMoney+it->Totalprofit-it->Currentprofit;//设置当月初始资金
				this->m_DayList.push_back(Month);
			}
			strcpy(CurrentDay,it->Close.trading_day);
		}
		DayResult &res=this->m_DayList.back();
		strncpy(res.TradingMonth,it->Close.trading_day,8);//设置交易月份
		res.NetProfit=res.NetProfit+it->Currentprofit;//设置净利润
		res.NetProfitRatio=res.NetProfit/(res.InitMoney);//设置收益率
		res.MaxProfit=max(res.MaxProfit,res.NetProfit);//设置最大盈利
		res.MaxLoss=min(res.MaxLoss,res.NetProfit);//设置最大亏损
		if(thisisgloble)
			strcpy(res.StregreyName,"GlobalStragery");
		else
			strcpy(res.StregreyName,it->Stretegy);
	}

	char *fullfilepath=new char[400],*buffer=new char[3056];
	sprintf(fullfilepath,".\\%s",dirName);
	//_mkdir(fullfilepath);
	if(!thisisgloble)
		sprintf(fullfilepath,".\\%s\\%s%s.csv",dirName,"单日报告_策略_",this->m_MonthList.begin()->StregreyName);
	else
		sprintf(fullfilepath,".\\%s\\%s.csv",dirName,"单日报告_策略组合");	
	FILE *file=fopen(fullfilepath,"w");
	if(file!=NULL)
	{
		fprintf(file,"策略名称,交易日期,当日初始资金,净利润,收益率,最大利润,最大亏损\n");
		vector<DayResult>::iterator monthit;
		for(monthit=this->m_DayList.begin();monthit!=this->m_DayList.end();monthit++)
		{
			fprintf(file,"%s,%s,%.3lf,%.3lf,%.3lf%%,%.3lf,%.3lf\n",monthit->StregreyName,monthit->TradingMonth,monthit->InitMoney,monthit->NetProfit,monthit->NetProfitRatio*100,monthit->MaxProfit,monthit->MaxLoss);
		}
		fflush(file);
		fclose(file);
	}
	delete[] fullfilepath;
	delete[] buffer;

}

//输出月度总结
void Clog::FlushMonthList(char *dirName)
{
	char CurrentMonth[9];
	memset(CurrentMonth,0,9);
	vector<OpenCloseStruct>::iterator it;
	if(this->m_TradeList.size()==0)
		return ;
	for(it=this->m_TradeList.begin();it!=this->m_TradeList.end();it++)
	{
		if(strlen(CurrentMonth)==0)
		{
			MonthResult Month;
			memset(&Month,0,sizeof(MonthResult));
			Month.InitMoney=this->m_InitMoney+it->Totalprofit-it->Currentprofit;//设置当月初始资金
			this->m_MonthList.push_back(Month);
			strcpy(CurrentMonth,it->Close.trading_day);
		}
		else
		{
			if(strncmp(CurrentMonth,it->Close.trading_day,6)!=0)//当前这天为新的一月的开头
			{
				MonthResult Month;
				memset(&Month,0,sizeof(MonthResult));
				Month.InitMoney=this->m_InitMoney+it->Totalprofit-it->Currentprofit;//设置当月初始资金
				this->m_MonthList.push_back(Month);
			}
			strcpy(CurrentMonth,it->Close.trading_day);
		}
		MonthResult &res=this->m_MonthList.back();
		strncpy(res.TradingMonth,it->Close.trading_day,6);//设置交易月份
		res.NetProfit=res.NetProfit+it->Currentprofit;//设置净利润
		res.NetProfitRatio=res.NetProfit/(res.InitMoney);//设置收益率
		res.MaxProfit=max(res.MaxProfit,res.NetProfit);//设置最大盈利
		res.MaxLoss=min(res.MaxLoss,res.NetProfit);//设置最大亏损
		if(thisisgloble)
			strcpy(res.StregreyName,"GlobalStragery");
		else
			strcpy(res.StregreyName,it->Stretegy);
	}

	char *fullfilepath=new char[400],*buffer=new char[3056];
	sprintf(fullfilepath,".\\%s",dirName);
	//_mkdir(fullfilepath);
	if(!thisisgloble)
		sprintf(fullfilepath,".\\%s\\%s%s.csv",dirName,"月度报告_策略_",this->m_MonthList.begin()->StregreyName);
	else
		sprintf(fullfilepath,".\\%s\\%s.csv",dirName,"月度报告_策略组合");	
	FILE *file=fopen(fullfilepath,"w");
	if(file!=NULL)
	{
		fprintf(file,"策略名称,交易月份,当月初始资金,净利润,收益率,最大利润,最大亏损\n");
		vector<MonthResult>::iterator monthit;
		for(monthit=this->m_MonthList.begin();monthit!=this->m_MonthList.end();monthit++)
		{
			fprintf(file,"%s,%s,%.3lf,%.3lf,%.3lf%%,%.3lf,%.3lf\n",monthit->StregreyName,monthit->TradingMonth,monthit->InitMoney,monthit->NetProfit,monthit->NetProfitRatio*100,monthit->MaxProfit,monthit->MaxLoss);
		}
		fflush(file);
		fclose(file);
	}
	delete[] fullfilepath;
	delete[] buffer;
	
}

//输出汇总总结---全局文件的路径名
void Clog::FlushTotalList(char *dirName)
{
	vector<OpenCloseStruct>::iterator it;
	memset(&this->m_totalresult,0,sizeof(TotalResult));
	TotalResult *result=&this->m_totalresult;
	result->InitMoney=this->m_InitMoney;
	char LastDay[20];//累计天数
	memset(LastDay,0,20);
	if(this->m_TradeList.size()==0)
		return;
	for(it=this->m_TradeList.begin();it!=this->m_TradeList.end();it++)
	{
		if(strlen(LastDay)==0)
		{
			result->TradeDayNum++;
			strcpy(LastDay,it->Close.trading_day);
			result->CurrentYestdayProfit=result->InitMoney+result->Netprofit;
		}
		else
		{
			if(strcmp(LastDay,it->Close.trading_day)!=0)
			{	
				result->TradeDayNum++;
				result->CurrentYestdayProfit=result->InitMoney+result->Netprofit;
			}
			strcpy(LastDay,it->Close.trading_day);
		}
		result->Netprofit=it->Totalprofit;//设置净利润
		result->ProfitRatio=result->Netprofit/result->InitMoney;//设置收益率
		result->NetprofitOneYears=result->Netprofit*250/result->TradeDayNum;//设置年净利润
		result->ProfitOneYearsRatio=result->NetprofitOneYears/result->InitMoney;//设置年收益率
		result->Tradetimes=result->Tradetimes+it->Close.volume;//设置总的交易次数
		if(it->Currentprofit>0)
		{
			result->WinTimes= result->WinTimes+it->Close.volume;//设置盈利次数
			result->TotalProfit=result->TotalProfit+it->Currentprofit;//设置总盈利
			result->AvgWinOneWinTimes=result->TotalProfit/result->WinTimes;//设置平均盈利
			result->MaxProfitOneTimes=max(result->MaxProfitOneTimes,it->Currentprofit);//设置最大盈利
			result->CurrentAlwaysWinTimes++;
			result->currentAlwaysLossTimes=0;
			result->AlwaysWinTimes=max(result->CurrentAlwaysWinTimes,result->AlwaysWinTimes);//是在最大连续盈利次数
		}
		else
		{
			result->LossTimes=result->LossTimes+it->Close.volume;//设置亏损次数
			result->TotalLoss=result->TotalLoss+it->Currentprofit;//设置总亏损
			result->AvgLossOneLossTimes=result->TotalLoss/result->LossTimes;//设置平均亏损
			result->MaxLossOneTimes=min(result->MaxLossOneTimes,it->Currentprofit);//设置最大亏损
			result->currentAlwaysLossTimes++;
			result->CurrentAlwaysWinTimes=0;
			result->AlwaysLossTimes=max(result->AlwaysLossTimes,result->currentAlwaysLossTimes);//设置最大连续亏损次数

		}
		result->WinLossRatio=abs(result->TotalProfit/result->TotalLoss);//设置盈亏比
		result->WintimesPercent=result->WinTimes*1.0/result->Tradetimes;//设置盈利比率
		result->AvgprofitOneTime=result->Netprofit/result->Tradetimes;//设置平均利润

		if(result->Netprofit>result->CurrentMaxPofit)
		{
			//修改为新的当前最高收益
			result->CurrentMaxPofit=result->Netprofit;
			//修改为新的当前最低收益
			result->CurrentMinProfit=result->Netprofit;
		}
		result->CurrentMinProfit=min(result->Netprofit,result->CurrentMinProfit);
		result->MaxLossAlways=min(result->MaxLossAlways,result->CurrentMinProfit-result->CurrentMaxPofit);//设置最大回撤
		result->MaxLossPercentNetprofit=min(result->MaxLossPercentNetprofit,(-1)*(result->CurrentMaxPofit-result->CurrentMinProfit)/(result->CurrentMaxPofit+result->InitMoney));//设置最大资产回撤比例
		result->MaxLossPercenOneDay=min(result->MaxLossPercenOneDay,(-1)*(result->CurrentYestdayProfit-result->Netprofit-result->InitMoney)/result->CurrentYestdayProfit);//设置单日最大回撤比例
		result->MaxLossOneDay=min(result->MaxLossOneDay,(-1)*(result->CurrentYestdayProfit-result->Netprofit-result->InitMoney));
	}

	if(thisisgloble)
	{
		strcpy(result->StrageryName,"策略组合");
	}
	else
	{
		strcpy(result->StrageryName,this->m_TradeList.begin()->Stretegy);
	}
	char *fullfilepath=new char[200],*buffer=new char[3056];

	//加快速度修改了文件夹判断
	sprintf(fullfilepath,".\\%s",dirName);
	//_mkdir(fullfilepath);

	sprintf(fullfilepath,".\\%s\\%s.csv",dirName,"策略总结报告");
	FILE *file;
	//file=fopen(fullfilepath,"r");
	//if(file!=NULL)//如果文件存在
	//{	
	//	fclose(file);	
	//	sprintf(buffer,"%s,%.3lf,%.3lf,%.3lf%%,%.3lf,%.3lf%%,%.3lf,%.3lf,%d,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf%%,%.3lf%%,%.3lf,%.3lf,%.3lf,%d,%d,%d,%d,%.3lf,%.3lf,%.3lf,%.3lf\n",
	//		result->StrageryName,//策略的名字和参数
	//		result->InitMoney,//初始资金-ok
	//		result->Netprofit,//净利润-ok
	//		result->ProfitRatio*100,//收益率-ok
	//		result->NetprofitOneYears,//年化净收益-ok
	//		result->ProfitOneYearsRatio*100,//年度收益率-ok
	//		result->WintimesPercent,//盈利比率-ok
	//		result->AvgprofitOneTime,//平均利润-ok
	//		result->Tradetimes,//交易次数-ok
	//		result->MaxLossAlways,//最大资产回测--ok
	//		result->IncRatio,//增长系数
	//		result->WinRisk,//收益风险
	//		result->Sharp,//夏普比
	//		result->MaxLossPercenOneDay*100,//单日最大回撤比例-ok
	//		result->MaxLossPercentNetprofit*100,//最大资产回撤比例-ok
	//		result->WinLossRatio,//盈亏比-ok
	//		result->TotalProfit,//总盈利-ok
	//		result->TotalLoss,//总亏损-ok
	//		result->WinTimes,//盈利次数-ok
	//		result->LossTimes,//亏损次数-ok
	//		result->AlwaysWinTimes,//最大连续盈利次数-ok
	//		result->AlwaysLossTimes,//最大连续亏损次数-ok
	//		result->MaxProfitOneTimes,//单次最大盈利-ok
	//		result->MaxLossOneTimes,//单次最大亏损-ok
	//		result->AvgWinOneWinTimes,//平均盈利-ok
	//		result->AvgLossOneLossTimes);//平均亏损-ok)

	//}
	//else//如果文件不存在
	{
		sprintf(buffer,"%s,%.3lf,%.3lf,%.3lf%%,%.3lf,%.3lf%%,%.3lf,%.3lf,%d,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf%%,%.3lf,%.3lf%%,%.3lf,%.3lf,%.3lf,%d,%d,%d,%d,%.3lf,%.3lf,%.3lf,%.3lf\n",
			result->StrageryName,//策略的名字和参数
			result->InitMoney,//初始资金-ok
			result->Netprofit,//净利润-ok
			result->ProfitRatio*100,//收益率-ok
			result->NetprofitOneYears,//年化净收益-ok
			result->ProfitOneYearsRatio*100,//年度收益率-ok
			result->WintimesPercent,//盈利比率-ok
			result->AvgprofitOneTime,//平均利润-ok
			result->Tradetimes,//交易次数-ok
			result->MaxLossAlways,//最大资产回测--ok
			result->IncRatio,//增长系数
			result->WinRisk,//收益风险
			result->Sharp,//夏普比
			result->MaxLossPercenOneDay*100,//单日最大回撤比例-ok
			result->MaxLossOneDay,//单日最大回撤值-ok
			result->MaxLossPercentNetprofit*100,//最大资产回撤比例-ok
			result->WinLossRatio,//盈亏比-ok
			result->TotalProfit,//总盈利-ok
			result->TotalLoss,//总亏损-ok
			result->WinTimes,//盈利次数-ok
			result->LossTimes,//亏损次数-ok
			result->AlwaysWinTimes,//最大连续盈利次数-ok
			result->AlwaysLossTimes,//最大连续亏损次数-ok
			result->MaxProfitOneTimes,//单次最大盈利-ok
			result->MaxLossOneTimes,//单次最大亏损-ok
			result->AvgWinOneWinTimes,//平均盈利-ok
			result->AvgLossOneLossTimes);//平均亏损-ok)

	}
	file=fopen(fullfilepath,"a+");
	if(file!=NULL)//写入数据到文件
	{
		fprintf(file,"%s",buffer);
		fflush(file);
		fclose(file);
	}
	//if(file!=NULL)//写入数据到文件
	//{
	//	fprintf(file,"%s",buffer);
	//}
	delete[] fullfilepath;
	delete[] buffer;
}
