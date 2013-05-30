#ifndef SPARSE_GRAPH_H
#define SPARSE_GRAPH_H
//ϡ��ͼ��;

#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <iostream>


template<class node_type, class edge_type>
class	SparseGraph
{
public:
	//ʹ��ͼ�и��ֱߺͽڵ������ܹ���ʹ��;
	typedef	edge_type	EdgeType;
	typedef	node_type	NodeType;
	//һЩ���Ͷ���;
	typedef	std::vector<node_type>	NodeVector;
	typedef	std::list<edge_type>	EdgeList;
	typedef	std::vector<EdgeList>	EdgeListVector;

private:
	//���ͼ�Ľڵ�;
	NodeVector		mNodes;

	//�ߵĶ�̬���飬�ڱ��У�ͨ���ڵ����������ڵ���ϵ;
	EdgeListVector	mEdges;
	
	//��Ҫ����ӵ���һ���ڵ��������;
	int		mNextNodeIndex;

	//�Ƿ�������ͼ;
	bool	mDigraph;

	//����ȷ����������ǰ�����ڣ�����ӱߵ�ʱ��ʹ��;
	bool	UniqueEdge(int from, int to) const;

	//ȥ������Щ�롰���Ϸ����Ľڵ������ӵı�;
	void	CullInvalidEdges();

public:
	//���캯��;
	SparseGraph(bool digraph) : mNextNodeIndex(0), mDigraph(digraph){}

	//���ظ��������Ľڵ�;
	const NodeType&	GetNode(int _index) const;
	//����ֵ�ǳ����İ汾;
	NodeType&		GetNode(int _index);

	//���ݸ����������㷵��һ����;
	const EdgeType&	GetEdge(int _from, int _to) const;
	//���طǳ����汾;
	EdgeType&		GetEdge(int _from, int _to);

	//ȡ����һ�����õĽڵ����������ӽڵ�ʱ��;
	int				GetNextFreeNodeIndex() const{return mNextNodeIndex;}

	//���һ���ڵ㵽ͼ�У�����������ֵ;
	int				AddNode(NodeType _node);

	//ͨ������һ���ڵ������ֵΪinvalid_node_index��ɾ��һ���ڵ�;
	void			RemoveNode(int _index);

	//���һ���ߣ�ȷ����ӵıߺϷ������������ͼ����ô�Զ��������һ����;
	void			AddEdge(EdgeType _edge);

	//��������ߴ��ڣ���ɾ���������������ͼ������Ӧ��ɾ������һ����;
	void			RemoveEdge(int _from, int _to);

	//���ñߵĴ���;
	void			SetEdgeCost(int from, int to, double cost);

	//���ص�ǰͼ�еĽڵ���;
	int				NumNodes() const { return mNodes.size(); }

	//���ص�ǰͼ�л�Ľڵ���;
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

	//���ص�ǰͼ�еı���;
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

	//�ж��Ƿ�������ͼ;
	bool	IsDigraph() const{return mDigraph;}

	//�ж�ͼ�Ƿ�Ϊ�գ��Ƿ��нڵ㣩;
	bool	IsEmpty() const{ return mNodes.empty();}

	//����ڵ���ͼ�д��ڣ��򷵻�true;
	bool	IsNodePresent(int _nodeindex) const;

	//�������ͼ�д��ڣ��򷵻�true;
	bool	IsEdgePresent(int from, int to) const;

	//����ͼ����װ��ͼ;
	bool	Save(const char* fileName) const;
	bool	Save(std::ofstream&	stream) const;

	bool	Load(const char* fileName);
	bool	Load(std::ifstream& stream);

	//���ͼ,�����ڵ�ͱ�;
	void	Clear() 
	{
		mNextNodeIndex = 0; 
		mNodes.clear();
		mEdges.clear();
	}

	//�Ƴ����б�;
	void	RemoveEdges()
	{
		for (EdgeListVector::iterator ite = mEdges.begin();
			ite != mEdges.end(); ite++)
		{
			ite->clear();
		}
	}
// ----------------------------------------��������-----------------------------------------------------

	//�ߵ�����;
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

	//�����ıߵ�������Ϊ�˴���һ���̶��ڵ�ĵ�����;
	class	ConstEdgeIterator
	{

	public:

		ConstEdgeIterator(const SparseGraph<node_type, edge_type>& graph,
							int		nodeidx): G(graph),
												NodeIndex(nodeidx)
		{
			//����Ҫ���ڵ��Ƿ���Ч������ڵ���Ч����ôҲû�бߣ�curEdge = null;
			curEdge = G.NumEdges[NodeIndex].begin();
		}

		const	EdgeType* begin()
		{
			curEdge = G.NumEdges[NodeIndex].begin();

			return &(*curEdge);  //ֱ�ӷ���curEdge��
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

	//�ڵ������;
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

			return &(*curNode);  //����ֱ�ӷ���curNode,������Ҫ������;
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

		//��ɾ��һ���ڵ�ʱ��vector�нڵ������ֵ��û��ʵʱ�ĸ���;Ҳ����˵������ָ�����һ���ڵ㲻����invalid_node_index;
		void	GetNextValidNode(typename NodeVector::iterator& it)
		{
			//���������ָ��vector����󣬻�����ָ��ĵĽڵ㲻��invalid_node_index;
			if (curNode == G.mNodes.end() || it->Index()!= invalid_node_index)
			{
				return;
			}

			//������ָ��Ľڵ�������invalid_node_index���򲻶�����;
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

	//������Ľڵ������;
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
//�����������ֵ�Ľڵ���ڣ��򷵻�true;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::IsNodePresent(int _nodeindex) const
{
	//ȷ��_nodeindexû��Խ��;
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
//���������ʼ����յ�ıߴ��ڣ��򷵻�true(�����ж��Ƿ�������ͼ);
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::IsEdgePresent(int from, int to) const
{
	bool result = false;
	//����Ҫ�������ڵ���ڣ�
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
//��������ֵ����ȡ�ڵ�;
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
//�ǳ����汾;
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
//������ʼ����յ������ֵ����ȡ����֮��ı�;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
const edge_type&	SparseGraph<node_type, edge_type>::GetEdge(int from, int to) const
{
	//�����ڵ������Ч;
	//�߱������;
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
//�ǳ����汾;
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
//���һ���ߵ�ͼ�У������֮ǰ��Ҫ��֤�ߵĺϷ��ԣ������������ͼ������ӱ�ʱ;
//�Զ��������һ����;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::AddEdge(EdgeType _edge)
{
	//����Ҫ��֤�ߺϷ�;
	assert( (_edge.From() < mNextNodeIndex) && (_edge.To() < mNextNodeIndex) &&
		"<SparseGraph::AddEdge>: invalid node index");

	if( (mNodes[_edge.From()].Index() != invalid_node_index) &&
		(mNodes[_edge.To()].Index() != invalid_node_index) )
	{
		//��֤����Ψһ��;
		if(UniqueEdge(_edge.From(), _edge.To()))
		{
			mEdges[_edge.From()].push_back(_edge);
		}

		//���������ͼ����ѱ���ӵ�����һ���ڵ�;
		if (!mDigraph)
		{
			//�Ƿ��Ѵ���;
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
//���ݸ����������������ֵ���Ƴ�����֮��ı�;
//------------------------------------------------------------------------------------
template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveEdge(int _from, int _to)
{
	assert( (_from < (int)mNodes.size()) && (_to < (int)mNodes.size())
		&& "<SparseGraph::RemoveEdge>: invalid node index");

	EdgeList::iterator curEdge;

	//��������ͼ����Ҫɾ����To�ڵ�ָ��From�ڵ��������;
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
	//ɾ����From�ڵ�ָ��To�ڵ��������;
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
//���ȼ������������Ƿ������;
//���û�У������������ֵ�Ƿ�������һ���ڵ�����, ����һ���ڵ�֮�󣬾ͻ�һֱ������mNodes������;
//��ʹ��remove����Ҳ�����ǰ�����index����invalid_node_index����ʵ�ʴ��ڣ�ֻ����һ��������ͬλ;
//�õĽڵ����滻��ʱ���Ű���������;
//------------------------------------------------------------------------------------
template<class node_type, class edge_type>
int	SparseGraph<node_type, edge_type>::AddNode(node_type node)
{
	//����ֵ��������Ԫ�ظ�����һ��;
	if (node.Index() < (int)mNodes.size())
	{
		assert( mNodes[node.Index()].Index() == invalid_node_index &&
			"<SparseGraph::AddNode>: Attempting to add a node with a duplicate(�ظ���) ID");

		mNodes[node.Index()] = node;           //���node.Index() < 0����Σ�

		return mNextNodeIndex;
	}
	else
	{
		//ȷ���½ڵ������ֵ��ȷ;
		assert(node.Index() == mNextNodeIndex &&
			"<SparseGraph::AddNode>:invalid index");

		mNodes.push_back(node);
		mEdges.push_back(EdgeList());

		return mNextNodeIndex++;
	}
}

//----------------------------------------------RemoveNode-------------------------
//
//ɾ����һ���ڵ㣬ͬʱɾ���������ڵ�֮�����ϵ;
//ɾ���ýڵ㣬���ǰѸýڵ��index��Ϊinvalid_node_index = -1��ʵ���ϸýڵ㻹������mNodes��
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveNode(int node)
{//����ͼ�ж�;
	assert( node < (int)mNodes.size() && 
		"<SparseGraph::RemoveNode>: invalid node index");

	//�ѽڵ���Ϊinvalid;
	mNodes[node].SetIndex(invalid_node_index);

	//���������ͼ������ͼ�Ͳ�ɾ��;
	if(!mDigraph)
	{
		//��������node���ھӽڵ㣬ɾ�����ǵı���ָ��node�ı�;
		for (EdgeList::iterator curEdge = mEdges[node].begin();
			curEdge != mEdges[node].end();
			++curEdge)
		{
			//��nodeָ��Ľڵ����;
			for (EdgeList::iterator curE = mEdges[curEdge->To()].begin();
				curE != mEdges[curEdge->To()].end();
				++curE)
			{
				if (curE->To() == node) //ָ��node��������;
				{
					curE = mEdges[curEdge->To()].erase(curE);
					break;
				}
			}
		}
		//����������ڵ�ı�;
		mEdges[node].clear();
	}
	//���������ͼ;
	else
	{
		CullInvalidEdges();
	}
	
}

//----------------------------------------------CullInvalidEdges-------------------------
//
//����ͼ�����бߣ�Ȼ��ɾ������Щָ������ֵΪinvalid_node_index�Ľڵ�ı�;
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
//���ñߵĴ���;
//------------------------------------------------------------------------------------
template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::SetEdgeCost(int from, int to, double NewCost)
{
	//ȷ�������ڵ�Ϸ�;
	assert( (from < mNodes.size()) && (to < mNodes.size())
			&& "<SparseGraph::SetEdgeCost>: invalid index");

	//��ȡfromָ��to�������ߣ�Ȼ����������Ȩֵ;
	for(EdgeList::iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if ( curEdge->To() == to )  //�ҵ�������;
		{
			curEdge->SetCost(NewCost);
			break;
		}
	}
}

//-------------------------------- UniqueEdge ----------------------------
//
//  ������һ����ʱ�������������ǰû�б���ӣ�Ψһ������ô����true;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::UniqueEdge(int from, int to)const
{
	for ( EdgeList::const_iterator curEdge = mEdges[from].begin();
		curEdge != mEdges[from].end();
		++curEdge)
	{
		if (curEdge->To() == to)  //�����ߴ���;
		{
			return false;
		}
	}

	return true;
}

//-------------------------------- Save ---------------------------------------
//
//  ����ͼ����Ϣ��һ���ļ���;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(const char* FileName)const;

//-------------------------------- Save ---------------------------------------
//
//  ����ͼ����Ϣ������;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Save(std::ofstream& stream)const;

//------------------------------- Load ----------------------------------------
//
//  ���ļ��м���ͼ;
//-----------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(const char* FileName);

//------------------------------- Load ----------------------------------------
//
//  ���������м���ͼ;
//------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::Load(std::ifstream& stream);


#endif