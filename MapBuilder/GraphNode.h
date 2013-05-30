#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H


#include <fstream>
#include <ostream>
#include "GraphRelateEnums.h"

class GraphNode
{
public:
	//���캯��;
	GraphNode():mIndex(invalid_node_index){}

	GraphNode(int index):mIndex(index){}

	GraphNode(std::ifstream& stream)
	{
		char buffer[50]; 
		stream >> buffer >> mIndex;
	}

	virtual ~GraphNode(){}
	//��ȡ�ڵ������ֵ;
	int		Index() const{return mIndex;}
	//���ýڵ�����;
	void	SetIndex(int newIndex){mIndex = newIndex;}

	//��д��;
	friend std::ostream& operator<<(std::ostream& os, const GraphNode n)
	{
		os << "Index: "<< n.mIndex << std::endl;
		return os;
	}

protected:
	//ÿ���ڵ㶼��һ��������һ����Ч����>=0;
	int mIndex;
};

#endif