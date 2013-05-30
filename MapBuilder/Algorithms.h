#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <queue>
#include <algorithm>
#include "SparseGraph.h"

//-------------------------------------��������㷨-------------------------------------------
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

	//�Ƿ��ҵ�Ŀ��;
	bool Found() const{return mFound;}
	
	//���ش�Դ��Ŀ�ĵص�����·�������нڵ�����;
	std::vector<int> GetPath() const{return mPath;}

private:
	//ͼ�Ľڵ����ͺͱ�����;
	typedef typename graph_type::EdgeType   Edge;
	typedef typename graph_type::NodeType   Node;

private:
	//�㷨��ҪӦ�õ�ͼ;
	graph_type &mGraph;

	//��Դ�ڵ㵽Ŀ�Ľڵ�����·���Ľڵ���������;
	std::vector<int>  mPath;

	//��ŷ��ʹ������бߣ��ڻ�ȡmPathʱ�õ�;
	std::vector<Edge*> mEdgesVisited;

	//�Ƿ����ҵ�Ŀ�ĵ�;
	bool		mFound;

	//Դ�ڵ��Ŀ�Ľڵ�����;
	int			mISource;
	int			mIDest;

	//ִ�в���;
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
	//�����������У��ֱ����ڴ����Ѿ����ʹ��ĺ�δ���ʹ��Ľڵ������ֵ;
	std::queue<int> unvisited;
	std::vector<int> visited;

	//��ǰ���ʵĽڵ������ֵ;
	int current;

	unvisited.push(mISource);  //�Ȱ�Դ�ڵ�ŵ�unvisited������;
	
	while( !unvisited.empty() )
	{
		current = unvisited.front();	//��ǰ���ʵĽڵ�;
		//�����ǰ�ڵ���Դ�ڵ�;
		if (current == mIDest)
		{
			mFound = true;

			//�����mPath;
			while(current != mISource)
			{
				//�����ѷ��ʹ������б�;
				std::vector<Edge*>::iterator edgeItr;
				for (edgeItr = mEdgesVisited.begin();
					edgeItr != mEdgesVisited.end();
					++edgeItr)
				{
					//�ҵ�����ָ��current�ı�;
					if (current == (*edgeItr)->To())
					{
						//��current����mPath��;
						mPath.push_back(current);
						//����currentΪָ��ǰcurrent�Ľڵ�;
						current = (*edgeItr)->From();
						//����forѭ��;
						break;
					}
				}
			}
			//��󣬱����˰�Դ�ڵ���뵽mPath;
			mPath.push_back(mISource);

			//whileѭ��֮��current == mIsource��mPath�����˴�Դ��Ŀ�Ľڵ�֮��Ľڵ�����;
			//�˳������whileѭ��;
			break;
		}

		//������ǰ�ڵ�������ڽڵ㣬����Ƿ��ѷ��ʹ�;
		graph_type::EdgeIterator edgeItr(mGraph, current);
		for (Edge *curedge = edgeItr.begin();
			!edgeItr.end();
			curedge = edgeItr.next())
		{
			std::vector<int>::iterator ite = find(visited.begin(), visited.end(), curedge->To());
			if (ite == visited.end())  //δ���ʹ��ýڵ�;
			{
				unvisited.push(curedge->To());
			}

			//�ѷ��ʹ��ı߷���������;
			mEdgesVisited.push_back(curedge);
		}

		visited.push_back(current);  //�ѵ�ǰ�ڵ���Ϊ�ѷ��ʽڵ������;

		unvisited.pop(); //������ǰ�Ľڵ㣨�ѷ��ʹ���;
	}

	return mFound;

}

#endif
