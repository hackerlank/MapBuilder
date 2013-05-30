#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <queue>
#include <algorithm>
#include "SparseGraph.h"

//-------------------------------------广度优先算法-------------------------------------------
//
//-------------------------------------------------------------------------------------------
template<class graph_type>
class algorithm_BFS   
{
public:
	algorithm_BFS(graph_type& graph,
		int iSource,
		int iDest);
	/*~algorithm_BFS();*/

	//是否找到目标;
	bool Found() const{return mFound;}
	
	//返回从源到目的地的所有路径的所有节点索引;
	std::vector<int> GetPath() const{return mPath;}

private:
	//图的节点类型和边类型;
	typedef typename graph_type::EdgeType   Edge;
	typedef typename graph_type::NodeType   Node;

private:
	//算法所要应用的图;
	graph_type &mGraph;

	//从源节点到目的节点所在路径的节点索引序列;
	std::vector<int>  mPath;

	//存放访问过的所有边，在获取mPath时用到;
	std::vector<Edge*> mEdgesVisited;

	//是否能找到目的地;
	bool		mFound;

	//源节点和目的节点索引;
	int			mISource;
	int			mIDest;

	//执行查找;
	bool		serach();
};

template<class graph_type>
algorithm_BFS<graph_type>::algorithm_BFS(graph_type& graph,
							 int iSource,
							 int iDest) : mGraph(graph),
										  mISource(iSource),
										  mIDest(iDest)
{
	mFound = serach();
}

//----------------------------serarch----------------------------
template<class graph_type>
bool algorithm_BFS<graph_type>::serach()
{
	//声明两个队列，分别用于储存已经访问过的和未访问过的节点的索引值;
	std::queue<int> unvisited;
	std::vector<int> visited;

	//当前访问的节点的索引值;
	int current;

	unvisited.push(mISource);  //先把源节点放到unvisited队列中;
	
	while( !unvisited.empty() )
	{
		current = unvisited.front();	//当前访问的节点;
		//如果当前节点是源节点;
		if (current == mIDest)
		{
			mFound = true;

			//计算出mPath;
			while(current != mISource)
			{
				//遍历已访问过的所有边;
				std::vector<Edge*>::iterator edgeItr;
				for (edgeItr = mEdgesVisited.begin();
					edgeItr != mEdgesVisited.end();
					++edgeItr)
				{
					//找到那条指向current的边;
					if (current == (*edgeItr)->To())
					{
						//把current放入mPath中;
						mPath.push_back(current);
						//重置current为指向当前current的节点;
						current = (*edgeItr)->From();
						//跳出for循环;
						break;
					}
				}
			}
			//最后，别忘了把源节点加入到mPath;
			mPath.push_back(mISource);

			//while循环之后，current == mIsource，mPath填满了从源到目的节点之间的节点索引;
			//退出最外层while循环;
			break;
		}

		//遍历当前节点的所有邻节点，检测是否已访问过;
		graph_type::EdgeIterator edgeItr(mGraph, current);
		for (Edge *curedge = edgeItr.begin();
			!edgeItr.end();
			curedge = edgeItr.next())
		{
			std::vector<int>::iterator ite = find(visited.begin(), visited.end(), curedge->To());
			if (ite == visited.end())  //未访问过该节点;
			{
				unvisited.push(curedge->To());
			}

			//把访问过的边放入容器中;
			mEdgesVisited.push_back(curedge);
		}

		visited.push_back(current);  //把当前节点作为已访问节点入队列;

		unvisited.pop(); //弹出当前的节点（已访问过）;
	}

	return mFound;

}

#endif
