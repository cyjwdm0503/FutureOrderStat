#include "Ctrade.h"
Clog *GlobalLog = NULL;
Ctrade::Ctrade(double mutiltick,double percent,int type,int twoorsingle,double money,char* name)
{
	this->m_mutiltick=mutiltick;
	this->fee=percent;
	this->type=type;
	m_position=0;
	m_holdprice=0;
	twosingle=twoorsingle;
	strcpy(info,name);
	log=new Clog(mutiltick,money,false);
}
bool Ctrade::Initialize(double mutiltick,double percent,int type,int twoorsingle,double money,char* name)
{
	this->m_mutiltick=mutiltick;
	this->fee=percent;
	this->type=type;
	m_position=0;
	m_holdprice=0;
	twosingle=twoorsingle;
	strcpy(info,name);
	log=new Clog(mutiltick,money,false);
	return true;
}
void Ctrade::Buy(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume/* =1 */)
{
	m_holdprice=m_holdprice*abs(m_position)+price*volume;
	this->m_position=m_position+volume;
	m_holdprice=m_holdprice/abs(this->m_position);
	TradeStruct opentrade;
	strcpy(opentrade.buyorsell,"买");
	strcpy(opentrade.openorcolse,"开");
	strcpy(opentrade.instrument,instrument);
	opentrade.microsecond=update_second;
	strcpy(opentrade.trading_day,trading_day);
	strcpy(opentrade.trading_time,update_time);
	opentrade.tradefee=GetTradefee(0,price,volume);
	opentrade.price=price;
	opentrade.volume=volume;
	this->m_openlist.push_back(opentrade);
}
void Ctrade::Sell(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume/* =1 */)
{
	m_holdprice=m_holdprice*abs(m_position)-m_holdprice*volume;
	this->m_position=m_position-volume;
	m_holdprice=this->m_position!=0?m_holdprice/abs(this->m_position):0;
	TradeStruct opentrade;

	strcpy(opentrade.buyorsell,"卖");
	strcpy(opentrade.openorcolse,"平");
	strcpy(opentrade.instrument,instrument);
	opentrade.microsecond=update_second;
	strcpy(opentrade.trading_day,trading_day);
	strcpy(opentrade.trading_time,update_time);
	opentrade.tradefee=GetTradefee(1,price,volume);
	opentrade.price=price;
	opentrade.volume=volume;
	//this->m_closelist.push_back(opentrade);

	/************************************************************************/
	/* 平仓后把开仓后持仓信息传入Log                                        */
	/************************************************************************/
	
	//如果当前平仓数量为前面开仓数量的的总和这样，就遍历开仓，直到平仓数量和持仓数量相同
		int closevol=opentrade.volume;
		while(closevol>0)
		{
			if(closevol>this->m_openlist.front().volume)
			{
				TradeStruct open;
				memcpy(&open,&(*this->m_openlist.begin()),sizeof(TradeStruct));
				closevol=closevol-this->m_openlist.front().volume;
				this->m_openlist.erase(this->m_openlist.begin());
				opentrade.volume=opentrade.volume-closevol;
				opentrade.tradefee=GetTradefee(1,price,opentrade.volume);
				log->InsertOpenClose(&open,&opentrade,info);
				if(GlobalLog!=NULL)
					GlobalLog->InsertOpenClose(&open,&opentrade,info);
				opentrade.volume=closevol;
				opentrade.tradefee=GetTradefee(1,price,opentrade.volume);
			}
			else if(closevol==this->m_openlist.front().volume)
			{
				opentrade.tradefee=GetTradefee(1,opentrade.price,opentrade.volume);
				this->m_openlist.front().tradefee=GetTradefee(0,this->m_openlist.front().price,this->m_openlist.front().volume);
				log->InsertOpenClose(&(*this->m_openlist.begin()),&opentrade,info);
				if(GlobalLog!=NULL)
				GlobalLog->InsertOpenClose(&(*this->m_openlist.begin()),&opentrade,info);
				closevol=closevol-this->m_openlist.front().volume;
				this->m_openlist.erase(this->m_openlist.begin());

			}
			else if(closevol<this->m_openlist.front().volume)
			{
				TradeStruct open;
				TradeStruct &temopen=this->m_openlist.front();
				memcpy(&open,&(*this->m_openlist.begin()),sizeof(TradeStruct));
				open.volume=closevol;
				open.tradefee=GetTradefee(0,open.price,open.volume);
				temopen.volume=temopen.volume-closevol;
				temopen.tradefee=GetTradefee(0,temopen.price,temopen.volume);
				opentrade.tradefee=GetTradefee(1,opentrade.price,opentrade.volume);
				log->InsertOpenClose(&open,&opentrade,info);
				if(GlobalLog!=NULL)
				GlobalLog->InsertOpenClose(&open,&opentrade,info);
				closevol=closevol-opentrade.volume;

			}
		}
}
void Ctrade::Sellshort(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume/* =1 */)
{
	m_holdprice=m_holdprice*abs(m_position)+price*volume;
	this->m_position=m_position-volume;
	m_holdprice=m_holdprice/abs(this->m_position);
	TradeStruct opentrade;
	strcpy(opentrade.buyorsell,"卖");
	strcpy(opentrade.openorcolse,"开");
	strcpy(opentrade.instrument,instrument);
	opentrade.microsecond=update_second;
	strcpy(opentrade.trading_day,trading_day);
	strcpy(opentrade.trading_time,update_time);
	opentrade.tradefee=GetTradefee(0,price,volume);
	opentrade.price=price;
	opentrade.volume=volume;
	this->m_openlist.push_back(opentrade);
}
void Ctrade::BuyToCover(char* trading_day,char* update_time,int update_second,char* instrument,double price,int volume/* =1 */)
{
	m_holdprice=m_holdprice*abs(m_position)-m_holdprice*volume;
	this->m_position=m_position+volume;
	m_holdprice=this->m_position!=0?m_holdprice/abs(this->m_position):0;
	TradeStruct opentrade;
	strcpy(opentrade.buyorsell,"买");
	strcpy(opentrade.openorcolse,"平");
	strcpy(opentrade.instrument,instrument);
	opentrade.microsecond=update_second;
	strcpy(opentrade.trading_day,trading_day);
	strcpy(opentrade.trading_time,update_time);
	opentrade.tradefee=GetTradefee(1,price,volume);
	opentrade.price=price;
	opentrade.volume=volume;

	
	int closevol=opentrade.volume;
	while(closevol>0)
	{
		if(closevol>this->m_openlist.front().volume)
		{
			TradeStruct open;
			memcpy(&open,&(*this->m_openlist.begin()),sizeof(TradeStruct));
			closevol=closevol-this->m_openlist.front().volume;
			this->m_openlist.erase(this->m_openlist.begin());
			opentrade.volume=opentrade.volume-closevol;
			opentrade.tradefee=GetTradefee(1,price,opentrade.volume);
			log->InsertOpenClose(&open,&opentrade,info);
			if(GlobalLog!=NULL)
				GlobalLog->InsertOpenClose(&open,&opentrade,info);
			opentrade.volume=closevol;
			opentrade.tradefee=GetTradefee(1,price,opentrade.volume);
		}
		else if(closevol==this->m_openlist.front().volume)
		{
			opentrade.tradefee=GetTradefee(1,opentrade.price,opentrade.volume);
			this->m_openlist.front().tradefee=GetTradefee(0,this->m_openlist.front().price,this->m_openlist.front().volume);
			log->InsertOpenClose(&(*this->m_openlist.begin()),&opentrade,info);
			if(GlobalLog!=NULL)
				GlobalLog->InsertOpenClose(&(*this->m_openlist.begin()),&opentrade,info);
			closevol=closevol-this->m_openlist.front().volume;
			this->m_openlist.erase(this->m_openlist.begin());

		}
		else if(closevol<this->m_openlist.front().volume)
		{
			TradeStruct open;
			TradeStruct &temopen=this->m_openlist.front();
			memcpy(&open,&(*this->m_openlist.begin()),sizeof(TradeStruct));
			open.volume=closevol;
			open.tradefee=GetTradefee(0,open.price,open.volume);
			temopen.volume=temopen.volume-closevol;
			temopen.tradefee=GetTradefee(0,temopen.price,temopen.volume);
			opentrade.tradefee=GetTradefee(1,opentrade.price,opentrade.volume);
			log->InsertOpenClose(&open,&opentrade,info);
			if(GlobalLog!=NULL)
				GlobalLog->InsertOpenClose(&open,&opentrade,info);
			closevol=closevol-opentrade.volume;

		}
	}

}
double Ctrade::GetTradefee(int open_close,double price,int vol)
{
	if(type==0)//按手收取
	{

		if(twosingle==-1)//两边收
		{
			return vol*this->fee;
		}
		else if(twosingle==0)
		{
			if(open_close==0)
				return vol*this->fee;
			else
				return 0;
		}
		else if(twosingle==1)
		{
			if(open_close==1)
				return vol*this->fee;
			else
				return 0;
		}
	}
	else
	{
		if(twosingle==-1)//两边收
		{
			return vol*this->fee*m_mutiltick;
		}
		else if(twosingle==0)
		{
			if(open_close==0)
				return vol*this->fee*m_mutiltick;
			else
				return 0;
		}
		else if(twosingle==1)
		{
			if(open_close==1)
				return vol*this->fee*m_mutiltick;
			else
				return 0;
		}
	}
	return -1;
}
bool Ctrade::FlushTradeList(char *filename)
{
	log->FlushTradeList(filename);
	return true;
}
bool Ctrade::FlushMonthList(char *dirName)
{
	log->FlushMonthList(dirName);
	return true;
}
bool Ctrade::FlushTotalList(char *dirName)
{
	log->FlushTotalList(dirName);
	return true;
}
TradeStruct* Ctrade::GetLastOpen()
{
	if(m_openlist.size()!=0)
		return &m_openlist.back();
	return NULL;
}
