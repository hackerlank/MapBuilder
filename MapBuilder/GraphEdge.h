#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

#include <ostream>
#include <fstream>
#include "GraphRelateEnums.h"

class GraphEdge
{
public:
	//���캯��;
	GraphEdge()
		:mCost(1.0),
		mFrom(invalid_node_index),
		mTo(invalid_node_index)
	{}

	GraphEdge(int from, int to, double cost)
		:mCost(cost),
		mFrom(from),
		mTo(to)
	{}

	GraphEdge(int from, int to)
		:mCost(1.0),
		mFrom(from),
		mTo(to)
	{}

	//�����������������,��˳��;
	GraphEdge(std::ifstream& stream)
	{
		char buffer[50];
		stream >> buffer >> mFrom >> buffer >> mTo >> buffer >> mCost;
	}

	virtual ~GraphEdge(){}

	//��ȡ�����ñߵ�Դ�ڵ�;
	int		From() const{ return mFrom;}
	void	SetFrom(int newFrom){mFrom = newFrom;}

	//��ȡ�����ñߵ�Ŀ��ڵ�;
	int		To() const{return mTo;}
	void	SetTo(int newTo){mTo = newTo;}

	//��ȡ�����ô��ۣ�Ȩֵ��;
	double	Cost() const{return mCost;}
	void	SetCost(double newCost){mCost = newCost;}

	//�����߼��жϲ�����;
	bool operator==(const GraphEdge* rhs)
	{
		return 
			rhs->mFrom	== this->mFrom	&&
			rhs->mTo		== this->mTo		&&
			rhs->mCost	== this->mCost;
	}

	bool operator!=(const GraphEdge& rhs)
	{
		//return !(*this == rhs);
	}

	//��д������;
	friend std::ostream& operator<<(std::ostream& os, const GraphEdge& e)
	{
		os << "mFrom: "<< e.mFrom <<"mTo: "<<e.mTo
			<<"mCost: "<< e.mCost<< std::endl;
		return os;
	}
protected:

	//�����ӵ������ڵ������ֵ;
	int	mFrom;
	int mTo;
	//�����˱�����Ŀ���(Ȩֵ);
	double	mCost;
};

#endif