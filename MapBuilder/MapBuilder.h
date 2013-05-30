#ifndef MAG_BUILDER_H
#define MAG_BUILDER_H

#include "SparseGraph.h"
#include "NavGraphNode.h"
#include "NavGraphEdge.h"
#include "GraphRelateEnums.h"
#include "Algorithms.h"


class MapBuilder
{
public:
	MapBuilder();
	~MapBuilder();

	static  MapBuilder* Instance();   //返回一个唯一实例;

	void	BulidMap(int numCellX, int numCellY);		 //构建地图;
	void	ClearMap();		 //清空图，初始状态;
	void	Render();		 //绘制地图;

	void	SaveMap(char *filename);	 //储存地图到磁盘;
	void	LoadMap(char *filename);	 //从磁盘加载地图;

	void	SetBrush(BrushType _brush);  //设置当前画刷;
	void	SetCellBrush(int x, int y);  //根据鼠标的位置（x， y）以及所选的画刷来设置所在节点的地形;

	void	RenderCells();				 //根据当前的画刷来绘制地图的各单元;
	void	CreatePath();				 //根据当前选择的算法绘制从源到目的节点的路径;
	void	RenderPath();				 //绘制根据算法求出的路径;

	void	TestOut();  //测试输出删除掉的节点;

private:
	//稀疏图;
	typedef SparseGraph<NavGraphNode<void*>, NavGraphEdge> NavGraph;
	NavGraph	*mGraph;

	//稀疏图删除掉的节点，储存它而不储存其他的节点或者边，比较特殊;
	NavGraph::NodeVector  mNodesDeleted;

	//通过寻路获得的从源节点到目的节点之间的路径;
	std::vector<int>	mPath;

	//算法;
	algorithm_BFS<NavGraph> *mAlgorithmBFS;

	//当前使用的画刷;
	BrushType	mCurrentBrush;

	//起点的节点索引和终点的节点索引;
	int			mSourceIndex;
	int			mDestIndex;

	//绘制的单元的宽和高;
	float		mCellWidth;
	float		mCellHeight;

	//在X轴和Y轴上的单元的个数;
	int			mNumCellX;
	int			mNumCellY;
};

#endif 