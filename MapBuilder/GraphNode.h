#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H


#include <fstream>
#include <ostream>
#include "GraphRelateEnums.h"

class GraphNode
{
public:
	//构造函数;
	GraphNode():mIndex(invalid_node_index){}

	GraphNode(int index):mIndex(index){}

	GraphNode(std::ifstream& stream)
	{
		char buffer[50]; 
		stream >> buffer >> mIndex;
	}

	virtual ~GraphNode(){}
	//获取节点的索引值;
	int		Index() const{return mIndex;}
	//设置节点索引;
	void	SetIndex(int newIndex){mIndex = newIndex;}

	//读写流;
	friend std::ostream& operator<<(std::ostream& os, const GraphNode n)
	{
		os << "Index: "<< n.mIndex << std::endl;
		return os;
	}

protected:
	//每个节点都有一个索引，一个有效索引>=0;
	int mIndex;
};

#endif