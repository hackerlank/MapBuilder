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

	//根据节点的索引和位置信息来创建节点;
	NavGraphNode(int _nodidx,Vector2D _pos, TerrainType _terrain=tNORMAL)
		:GraphNode(_nodidx),
		mPosition(_pos),
		mTerrainType(_terrain),
		mExtraInfo(extra_info())
	{}

	//根据输入流来构造节点;
	NavGraphNode(std::ifstream& stream):mExtraInfo(extra_info())
	{
		char buffer[50];
		stream >> buffer >> mIndex >> buffer >> mPosition.x >> buffer >> mPosition.y >> buffer >> mTerrainType;
	}

	virtual ~NavGraphNode(){};

	//获取节点的位置信息;
	Vector2D	Pos() const{ return mPosition;}
	//设置位置;
	void		SetPos(Vector2D newPosition){ mPosition = newPosition;}

	//获取节点的地形类型;
	TerrainType Terrain() const{ return mTerrainType; }
	//设置节点的地形;
	void		SetTerrain(TerrainType _terrain){ mTerrainType = _terrain;}

	//获取节点的类型;
	extra_info	ExtraInfo() const{ return mExtraInfo;}

	void		SetExtraInfo(extra_info info){ mExtraInfo = info;}

	friend std::ostream& operator << (std::ostream& os, const NavGraphNode& n)
	{
		os << "Index:"<<n.Index() <<" "<< "PosX:"<< n.Pos().x <<" "<< "PosY:" <<n.Pos().y << std::endl;
		return os;
	}

protected:
	//节点位置;
	Vector2D	mPosition;
	//节点的类型;
	TerrainType	mTerrainType;
	//额外信息，权值等;
	extra_info	mExtraInfo;
};

#endif