#ifndef NAVGRAPH_NODE_H
#define NAVGRAPH_NODE_H

#include "2D/Vector2D.h"
#include "GraphNode.h"
#include "GraphRelateEnums.h"


template <class extra_info = void*>
class NavGraphNode : public GraphNode
{
public:
	NavGraphNode():mExtraInfo(extra_info()){}

	//���ݽڵ��������λ����Ϣ�������ڵ�;
	NavGraphNode(int _nodidx,Vector2D _pos, TerrainType _terrain=tNORMAL)
		:GraphNode(_nodidx),
		mPosition(_pos),
		mTerrainType(_terrain),
		mExtraInfo(extra_info())
	{}

	//����������������ڵ�;
	NavGraphNode(std::ifstream& stream):mExtraInfo(extra_info())
	{
		char buffer[50];
		stream >> buffer >> mIndex >> buffer >> mPosition.x >> buffer >> mPosition.y >> buffer >> mTerrainType;
	}

	virtual ~NavGraphNode(){};

	//��ȡ�ڵ��λ����Ϣ;
	Vector2D	Pos() const{ return mPosition;}
	//����λ��;
	void		SetPos(Vector2D newPosition){ mPosition = newPosition;}

	//��ȡ�ڵ�ĵ�������;
	TerrainType Terrain() const{ return mTerrainType; }
	//���ýڵ�ĵ���;
	void		SetTerrain(TerrainType _terrain){ mTerrainType = _terrain;}

	//��ȡ�ڵ������;
	extra_info	ExtraInfo() const{ return mExtraInfo;}

	void		SetExtraInfo(extra_info info){ mExtraInfo = info;}

	friend std::ostream& operator << (std::ostream& os, const NavGraphNode& n)
	{
		os << "Index:"<<n.Index() <<" "<< "PosX:"<< n.Pos().x <<" "<< "PosY:" <<n.Pos().y << std::endl;
		return os;
	}

protected:
	//�ڵ�λ��;
	Vector2D	mPosition;
	//�ڵ������;
	TerrainType	mTerrainType;
	//������Ϣ��Ȩֵ��;
	extra_info	mExtraInfo;
};

#endif