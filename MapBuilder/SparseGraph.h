#ifndef SPARSE_GRAPH_H
#define SPARSE_GRAPH_H
//稀疏图类;

#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <iostream>


template<class node_type, class edge_type>
class	SparseGraph
{
public:
	//使得图中各种边和节点类型能够被使用;
	typedef	edge_type	EdgeType;
	typedef	node_type	NodeType;
	//一些类型定义;
	typedef	std::vector<node_type>	NodeVector;
	typedef	std::list<edge_type>	EdgeList;
	typedef	std::vector<EdgeList>	EdgeListVector;

private:
	//组成图的节点;
	NodeVector		mNodes;

	//边的动态数组，在边中，通过节点的索引来与节点联系;
	EdgeListVector	mEdges;
	
	//将要被添加到下一个节点的索引号;
	int		mNextNodeIndex;

	//是否是有向图;
	bool	mDigraph;

	//用于确保这条边以前不存在，在添加边的时候使用;
	bool	UniqueEdge(int from, int to) const;

	//去除掉那些与“不合法”的节点相连接的边;
	void	CullInvalidEdges();

public:
	//构造函数;
	SparseGraph(bool digraph) : mNextNodeIndex(0), mDigraph(digraph){}

	//返回给定索引的节点;
	const NodeType&	GetNode(int _index) const;
	//返回值非常量的版本;
	NodeType&		GetNode(int _index);

	//根据给定的两个点返回一条边;
	const EdgeType&	GetEdge(int _from, int _to) const;
	//返回非常量版本;
	EdgeType&		GetEdge(int _from, int _to);

	//取得下一个可用的节点的索引，添加节点时用;
	int				GetNextFreeNodeIndex() const{return mNextNodeIndex;}

	//添加一个节点到图中，并返回索引值;
	int				AddNode(NodeType _node);

	//通过设置一个节点的索引值为invalid_node_index来删除一个节点;
	void			RemoveNode(int _index);

	//添加一条边，确保添加的边合法，如果是有向图，那么自动添加另外一条边;
	void			AddEdge(EdgeType _edge);

	//如果这条边存在，则删除它；如果是有向图，则相应的删除另外一条边;
	void			RemoveEdge(int _from, int _to);

	//设置边的代价;
	void			SetEdgeCost(int from, int to, double cost);

	//返回当前图中的节点数;
	int				NumNodes() const { return mNodes.size(); }

	//返回当前图中活动的节点数;
	int				NumActiveNodes() const
	{
		int	count = 0;
		for (unsigned int n=0; n<mNodes.size(); n++)
		{
			if (mNodes[n].Index() != invalid_node_index)
			{
				count++;
			}
		}
		return count;
	}

	//返回当前图中的边数;
	int				NumEdges() const
	{
		int ecount = 0;
		for (EdgeListVector::const_iterator ite = mEdges.begin();
			ite != mEdges.end(); ite++)
		{
			ecount += ite->size();
		}

		return ecount;
	}

	//判断是否是有向图;
	bool	IsDigraph() const{return mDigraph;}

	//判断图是否为空（是否有节点）;
	bool	IsEmpty() const{ return mNodes.empty();}

	//如果节点在图中存在，则返回true;
	bool	IsNodePresent(int _nodeindex) const;

	//如果边在图中存在，则返回true;
	bool	IsEdgePresent(int from, int to) const;

	//保存图或者装载图;
	bool	Save(const char* fileName) const;
	bool	Save(std::ofstream&	stream) const;

	bool	Load(const char* fileName);
	bool	Load(std::ifstream& stream);

	//清除图,包括节点和边;
	void	Clear() 
	{
		mNextNodeIndex = 0; 
		mNodes.clear();
		mEdges.clear();
	}

	//移出所有边;
	void	RemoveEdges()
	{
		for (EdgeListVector::iterator ite = mEdges.begin();
			ite != mEdges.end(); ite++)
		{
			ite->clear();
		}
	}
// ----------------------------------------迭代器类-----------------------------------------------------

	//边迭代器;
	class	EdgeIterator
	{
	public:

		EdgeIterator(SparseGraph<node_type, edge_type>& graph,
			int node) : G(graph),
			NodeIndex(node)
		{
			curEdge = G.mEdges[NodeIndex].begin();
		}

		EdgeType*	begin()
		{
			curEdge = G.mEdges[NodeIndex].begin();

			return &(*curEdge);
		}

		EdgeType*	next()
		{
			++curEdge;

			if (end())
			{
				return NULL;
			}
			else
				return &(*curEdge);
		}

		bool end()
		{
			return (curEdge == G.mEdges[NodeIndex].end());
		}
	private:

		typename EdgeList::iterator		curEdge;

		SparseGraph<node_type, edge_type>& G;

		const	int		NodeIndex;
	};

	friend class EdgeIterator;  

	//常量的边迭代器，为了代表一个固定节点的迭代器;
	class	ConstEdgeIterator
	{

	public:

		ConstEdgeIterator(const SparseGraph<node_type, edge_type>& graph,
							int		nodeidx): G(graph),
												NodeIndex(nodeidx)
		{
			//不需要检查节点是否无效，如果节点无效，那么也没有边，curEdge = null;
			curEdge = G.NumEdges[NodeIndex].begin();
		}

		const	EdgeType* begin()
		{
			curEdge = G.NumEdges[NodeIndex].begin();

			return &(*curEdge);  //直接返回curEdge？
		}

		const	EdgeType*	next()
		{
			++curEdge;

			if (end())
			{
				return NULL;
			}
			else
			{
				return &(*curEdge);
			}
		}

		bool	end()
		{
			return (curEdge == G.NumEdges[NodeIndex].end());
		}
	private:

		typename EdgeList::const_iterator	curEdge;

		const	SparseGraph<node_type, edge_type>& G;

		const	int		NodeIndex;
	};

	friend class ConstEdgeIterator;

	//节点迭代器;
	class	NodeIterator
	{
	public:

		NodeIterator(SparseGraph<node_type, edge_type> &graph) : G(graph)
		{
			curNode = G.mNodes.begin();

			//GetNextValidNode(curNode);
		}

		node_type*	begin()
		{
			curNode = G.mNodes.begin();

			GetNextValidNode(curNode);

			return &(*curNode);  //不能直接返回curNode,不是想要的类型;
		}

		node_type*	next()
		{
			curNode++;

			GetNextValidNode(curNode);

			if (end())
			{
				return NULL;
			}
			else
				return &(*curNode);
		}

		bool		end()
		{
			return	(curNode == G.mNodes.end());
		}

	private:

		typename	NodeVector::iterator	curNode;

		SparseGraph<node_type, edge_type>&	G;

		//当删除一个节点时，vector中节点的索引值并没有实时的更新;也就是说迭代器指向的下一个节点不能是invalid_node_index;
		void	GetNextValidNode(typename NodeVector::iterator& it)
		{
			//如果迭代器指向vector的最后，或者它指向的的节点不是invalid_node_index;
			if (curNode == G.mNodes.end() || it->Index()!= invalid_node_index)
			{
				return;
			}

			//迭代器指向的节点索引是invalid_node_index，则不断跳过;
			while((it->Index() == invalid_node_index))
			{
				++it;

				if (curNode == G.mNodes.end())
				{
					break;
				}
			}
		}
	};

	friend	class NodeIterator;

	//常量版的节点迭代器;
	class ConstNodeIterator
	{
	public:

		ConstNodeIterator(const SparseGraph<node_type, edge_type> &graph)
		{
			curNode = G.mNodes.begin();
		}

		const	node_type*	begin()
		{
			curNode = G.mNodes.begin();

			GetNextValidNode(curNode);

			return &(*curNode);
		}

		const	node_type*	next()
		{
			++curNode;

			if (end())
			{
				return NULL;
			}
			else
			{
				GetNextValidNode(curNode);

				return &(*curNode);
			}
		}

		bool	end()
		{
			return (curNode == G.mNodes.end());
		}

	private:

		typename	NodeVector::const_iterator		curNode;

		const	SparseGraph<node_type, edge_type>&		G;

		void	GetNextValidNode(typename NodeVector::const_iterator& it)
		{
			if (curNode == G.mNodes.end() || it->Index() != invalid_node_index)
			{
				++it;

				if (curNode == G.mNodes.end())
				{
					break;
				}
			}
		}
	};

	friend class ConstNodeIterator;
};

//----------------------------------------------isNodePresent-------------------------
//
//如果给定索引值的节点存在，则返回true;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::IsNodePresent(int _nodeindex) const
{
	//确保_nodeindex没有越界;
	assert(_nodeindex >= 0 && "vector subindex out of range");

	if ((_nodeindex >= (int)mNodes.size()) ||
		(mNodes[_nodeindex].Index() == invalid_node_index))
	{
		return false;
	}
	else
		return true;
}

//----------------------------------------------isEdgePresent-------------------------
//
//如果给定起始点和终点的边存在，则返回true(不用判断是否是有向图);
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::IsEdgePresent(int from, int to) const
{
	bool result = false;
	//首先要这两个节点存在；
	if (IsNodePresent(from) && IsNodePresent(to))
	{
		for (EdgeList::const_iterator curEdge = mEdges[from].begin();
			curEdge!= mEdges[from].end();
			++curEdge)
		{
			if (curEdge->To() == to)
				result = true;
		}
	}
	
	return result;
}

//----------------------------------------------GetNode-------------------------
//
//根据索引值来获取节点;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
const node_type&	SparseGraph<node_type, edge_type>::GetNode(int _index) const
{
	assert( (_index < (int)mNodes.size()) &&
			(_index >= 0)	&&
			"<SparseGraph::GetNode>: invalid index");

	return mNodes[_index];
}

//----------------------------------------------GetNode------------------------------
//
//非常量版本;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
node_type& SparseGraph<node_type, edge_type>::GetNode(int _index)
{
	assert( (_index < (int)mNodes.size()) &&
		(_index >=0)             &&
		"<SparseGraph::GetNode>: invalid index");

	return mNodes[_index];
}

//----------------------------------------------GetEdge-------------------------
//
//根据起始点和终点的索引值来获取他们之间的边;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
const edge_type&	SparseGraph<node_type, edge_type>::GetEdge(int from, int to) const
{
	//两个节点必须有效;
	//边必须存在;
// 	if (isEdgePresent(from, to))
// 	{
// 		//
// 	}

	assert( (from < mNodes.size()) &&
			(from >= 0)			   &&
			mNodes[from].Index() != invalid_node_index &&
			"<SparseGraph::GetEdge>: invalid 'from' index");

	assert( (to < mNodes.size())	&&
			(to >= 0)				&&
			mNodes[to].Index() != invalid_node_index &&
			"<SparseGraph::GetEdge>: invalid 'to' index");

	for (EdgeList::const_iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if (curEdge->To == to)
		{
			return *curEdge;
		}
	}

	assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//----------------------------------------------GetEdge-------------------------
//
//非常量版本;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
edge_type& SparseGraph<node_type, edge_type>::GetEdge(int from, int to)
{
	assert( (from < mNodes.size()) &&
			(from >= 0)			   &&
			mNodes[from].Index() != invalid_node_index &&
		"<SparseGraph::GetEdge>: invalid 'from' index");

	assert( (to < mNodes.size())	&&
			(to >= 0)				&&
			mNodes[to].Index() != invalid_node_index &&
		"<SparseGraph::GetEdge>: invalid 'to' index");

	for (EdgeList::iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if (curEdge->To() == to)
		{
			return *curEdge;
		}
	}

	assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//----------------------------------------------AddEdge-------------------------
//
//添加一条边到图中，在添加之前，要保证边的合法性，如果边是有向图，在添加边时;
//自动添加另外一条边;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::AddEdge(EdgeType _edge)
{
	//首先要保证边合法;
	assert( (_edge.From() < mNextNodeIndex) && (_edge.To() < mNextNodeIndex) &&
		"<SparseGraph::AddEdge>: invalid node index");

	if( (mNodes[_edge.From()].Index() != invalid_node_index) &&
		(mNodes[_edge.To()].Index() != invalid_node_index) )
	{
		//保证边是唯一的;
		if(UniqueEdge(_edge.From(), _edge.To()))
		{
			mEdges[_edge.From()].push_back(_edge);
		}

		//如果是无向图，则把边添加到另外一个节点;
		if (!mDigraph)
		{
			//是否已存在;
			if (UniqueEdge(_edge.To(), _edge.From()))
			{
				edge_type  newEdge = _edge;

				newEdge.SetTo(_edge.From());
				newEdge.SetFrom(_edge.To());

				mEdges[_edge.To()].push_back(newEdge);
			}
		}
	}
}

//----------------------------------------------RemoveEdge-------------------------
//
//根据给定的两个点的索引值来移除他们之间的边;
//------------------------------------------------------------------------------------
template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveEdge(int _from, int _to)
{
	assert( (_from < (int)mNodes.size()) && (_to < (int)mNodes.size())
		&& "<SparseGraph::RemoveEdge>: invalid node index");

	EdgeList::iterator curEdge;

	//不是有向图，则要删掉从To节点指向From节点的这条边;
	if (!mDigraph)
	{
		for (curEdge = mEdges[_to].begin();
			 curEdge!= mEdges[_to].end();
			 ++curEdge)
		{
			if(curEdge->To() == _from)
			{
				curEdge = mEdges[_to].erase(curEdge);
				break;
			}
		}
	}
	//删掉从From节点指向To节点的这条边;
	for (curEdge = mEdges[_from].begin();
		 curEdge!= mEdges[_from].end();
		 ++curEdge)
	{
		if (curEdge->To() == _to)
		{
			curEdge = mEdges[_from].erase(curEdge);
			break;
		}
	}
}

//----------------------------------------------AddNode-------------------------
//
//首先检查这个点曾经是否已添加;
//如果没有，检查它的索引值是否满足下一个节点索引, 插入一个节点之后，就会一直存在于mNodes容器中;
//即使你remove它，也仅仅是把它的index设置invalid_node_index，它实际存在，只有下一个与他相同位;
//置的节点来替换它时，才把他覆盖了;
//------------------------------------------------------------------------------------
template<class node_type, class edge_type>
int	SparseGraph<node_type, edge_type>::AddNode(node_type node)
{
	//索引值与容器的元素个数不一致;
	if (node.Index() < (int)mNodes.size())
	{
		assert( mNodes[node.Index()].Index() == invalid_node_index &&
			"<SparseGraph::AddNode>: Attempting to add a node with a duplicate(重复的) ID");

		mNodes[node.Index()] = node;           //如果node.Index() < 0该如何？

		return mNextNodeIndex;
	}
	else
	{
		//确保新节点的索引值正确;
		assert(node.Index() == mNextNodeIndex &&
			"<SparseGraph::AddNode>:invalid index");

		mNodes.push_back(node);
		mEdges.push_back(EdgeList());

		return mNextNodeIndex++;
	}
}

//----------------------------------------------RemoveNode-------------------------
//
//删除掉一个节点，同时删除与其他节点之间的联系;
//删除该节点，即是把该节点的index设为invalid_node_index = -1，实际上该节点还存在于mNodes中
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveNode(int node)
{//有向图判断;
	assert( node < (int)mNodes.size() && 
		"<SparseGraph::RemoveNode>: invalid node index");

	//把节点设为invalid;
	mNodes[node].SetIndex(invalid_node_index);

	//如果是无向图，有向图就不删？;
	if(!mDigraph)
	{
		//对于所有node的邻居节点，删除它们的边中指向node的边;
		for (EdgeList::iterator curEdge = mEdges[node].begin();
			curEdge != mEdges[node].end();
			++curEdge)
		{
			//对node指向的节点操作;
			for (EdgeList::iterator curE = mEdges[curEdge->To()].begin();
				curE != mEdges[curEdge->To()].end();
				++curE)
			{
				if (curE->To() == node) //指向node的那条边;
				{
					curE = mEdges[curEdge->To()].erase(curE);
					break;
				}
			}
		}
		//最后清空这个节点的边;
		mEdges[node].clear();
	}
	//如果是有向图;
	else
	{
		CullInvalidEdges();
	}
	
}

//----------------------------------------------CullInvalidEdges-------------------------
//
//遍历图的所有边，然后删除掉那些指向索引值为invalid_node_index的节点的边;
//------------------------------------------------------------------------------------
template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::CullInvalidEdges()
{
	//
	for (EdgeListVector::iterator curEdgeList = mEdges.begin(); curEdgeList != mEdges.end(); ++curEdgeList)
	{
		for (EdgeList::iterator curEdge = (*curEdgeList).begin(); curEdge != (*curEdgeList).end(); ++curEdge)
		{
			if(mNodes[curEdge->To()].Index() == invalid_node_index ||
				mNodes[curEdge->From()].Index() == invalid_node_index)
			{
				curEdge = (*curEdgeList).erase(curEdge);
			}
		}
	}
}

//----------------------------------------------SetEdgeCost-------------------------
//
//设置边的代价;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::SetEdgeCost(int from, int to, double NewCost)
{
	//确保两个节点合法;
	assert( (from < mNodes.size()) && (to < mNodes.size())
			&& "<SparseGraph::SetEdgeCost>: invalid index");

	//获取from指向to的这条边，然后设置它的权值;
	for(EdgeList::iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if ( curEdge->To() == to )  //找到这条边;
		{
			curEdge->SetCost(NewCost);
			break;
		}
	}
}

//-------------------------------- UniqueEdge ----------------------------
//
//  当增加一条边时，如果这条边以前没有被添加（唯一），那么返回true;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::UniqueEdge(int from, int to)const
{
	for ( EdgeList::const_iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if (curEdge->To() == to)  //这条边存在;
		{
			return false;
		}
	}

	return true;
}

//-------------------------------- Save ---------------------------------------
//
//  保存图的信息到一个文件中;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(const char* FileName)const;

//-------------------------------- Save ---------------------------------------
//
//  保存图的信息到流中;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(std::ofstream& stream)const;

//------------------------------- Load ----------------------------------------
//
//  从文件中加载图;
//-----------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(const char* FileName);

//------------------------------- Load ----------------------------------------
//
//  从输入流中加载图;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(std::ifstream& stream);


#endif