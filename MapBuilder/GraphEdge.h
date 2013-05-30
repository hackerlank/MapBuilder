#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

#include <ostream>
#include <fstream>
#include "GraphRelateEnums.h"

class GraphEdge
{
public:
	//构造函数;
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

	//根据输入流来构造边,有顺序;
	GraphEdge(std::ifstream& stream)
	{
		char buffer[50];
		stream >> buffer >> mFrom >> buffer >> mTo >> buffer >> mCost;
	}

	virtual ~GraphEdge(){}

	//获取和设置边的源节点;
	int		From() const{ return mFrom;}
	void	SetFrom(int newFrom){mFrom = newFrom;}

	//获取和设置边的目标节点;
	int		To() const{return mTo;}
	void	SetTo(int newTo){mTo = newTo;}

	//获取和设置代价（权值）;
	double	Cost() const{return mCost;}
	void	SetCost(double newCost){mCost = newCost;}

	//重载逻辑判断操作符;
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

	//读写到流中;
	friend std::ostream& operator<<(std::ostream& os, const GraphEdge& e)
	{
		os << "mFrom: "<< e.mFrom <<"mTo: "<<e.mTo
			<<"mCost: "<< e.mCost<< std::endl;
		return os;
	}
protected:

	//边链接的两个节点的索引值;
	int	mFrom;
	int mTo;
	//经过此边所需的开销(权值);
	double	mCost;
};

#endif