#ifndef GRAPHFUNCS_H
#define GRAPHFUNCS_H

// #include "xml/tinyxml.h"
// #include "xml/tinystr.h"
#include "SparseGraph.h"
#include "Constants.h"


//----------------------------------------------ValidNeighbour-------------------------
//
//检查在该位置下的节点是否合法;
//--------------------------------------------------------------------------------------
bool ValidNeighbour(int x, int y, int numCellsX, int numCellsY)
{
	return !((x < 0) || (x >= numCellsX) || (y < 0) || (y >= numCellsY));
}


//----------------------------------------------AddAllNeighboursToNode--------------------
//
//添加给定节点的所有邻边;
//------------------------------------------------------------------------------------
template<class graph_type>
void	GraphHelper_AddAllNeighborsToNode(graph_type& graph,
							int	row, int col,
							int	numCellsX, int	numCellsY)
{
	for (int i=-1; i<2; ++i)
	{
		for (int j=-1; j<2; ++j)
		{
			//节点邻接节点的位置;
			int nodeX = col + j;
			int nodeY = row + i;

			//如果 i = j = 0,则是本节点;
			if ((i == 0) && (j == 0))
			{
				continue;
			}

			if (ValidNeighbour(nodeX, nodeY, numCellsX, numCellsY))
			{
				//节点的索引值;
				int indexNode = row*numCellsX + col;
				//邻接节点的索引值;
				int indexNeighborNode = nodeY*numCellsX + nodeX;

				//根据索引值获取主节点的位置;
				Vector2D Node = graph.GetNode(indexNode).Pos();
				//根据索引值获取邻接节点的位置;
				Vector2D nabNode = graph.GetNode(indexNeighborNode).Pos();

				//节点和他的邻接节点之间的花销;
				int dis = Node.Distance(nabNode);

				NavGraphEdge newEdge(indexNode, indexNeighborNode, dis);

				graph.AddEdge(newEdge);

				//如果是无向图，则添加另外一条边;
				if (!graph.IsDigraph())
				{
					NavGraphEdge newOppEdge(indexNeighborNode, indexNode, dis);

					graph.AddEdge(newOppEdge);
				}
			}

		}
	}
}


//----------------------------------------------CreateGrid-------------------------
//
//根据给定的窗口尺寸和单元格个数来创建网格;
//------------------------------------------------------------------------------------
template<class graph_type>
void GraphHelper_CreateGrid(graph_type& graph,
				int cySize,
				int	cxSize,
				int numCellsY,
				int numCellsX)
{
	//该变量用来测试节点的类型用;
	/*bool iswall = false;*/

	double CellWidth = (double)cxSize / (double)numCellsX;
	double CellHeight = (double)cySize / (double)numCellsY;

	double midX = CellWidth/2;
	double midY = CellHeight/2;

	//创建节点;
	for (int row=0; row<numCellsY; ++row)
	{
		for (int col=0; col<numCellsX; ++col)
		{
			
			graph.AddNode(NavGraphNode<> (graph.GetNextFreeNodeIndex(),
				Vector2D(midX + (col*CellWidth),
				midY + (row*CellHeight))));
		}
	}

	//设置初始的默认的源节点和目的节点，设置他们的地形类型即可;
	graph.GetNode(souceNodeIndex).SetTerrain(tSOURCE);
	graph.GetNode(destNodeIndex).SetTerrain(tDEST);

	//为创建的每个点添加所有邻边，每个节点有8条邻边;
	for (int row=0; row<numCellsY; ++row)
	{
		for (int col=0; col<numCellsX; ++col)
		{
			GraphHelper_AddAllNeighborsToNode(graph, row, col, numCellsX, numCellsY);
		}
	}
}


//----------------------------------------------SaveMap-------------------------------
//
//把图的节点信息和边的信息保存到指定的xml文件中;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_SaveMap(graph_type& graph, char* filename)
{
	//创建xml文件;
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//创建根节点Nodes;
	TiXmlElement *nodes = new TiXmlElement("Nodes");
	doc.LinkEndChild(nodes);
	//创建根节点Edges;
	TiXmlElement *edges = new TiXmlElement("Edges");
	doc.LinkEndChild(edges);

	//声明一个元素node，在后面作为nodes的子元素;
	TiXmlElement *node;
	//声明一个元素edge，在后面作为edges的子元素;
	TiXmlElement *edge;

	//遍历图的所有节点;
	graph_type::NodeIterator nodeItr(graph);
	for (graph_type::NodeType *curnode = nodeItr.begin();
		!nodeItr.end();
		curnode = nodeItr.next())
	{
		//根据图中的节点来创建xml的node子元素;
		node = new TiXmlElement("Node");
		if (node != NULL)//节点创建成功;
		{
			//设置元素的属性;
			node->SetAttribute("index", curnode->Index());
			node->SetDoubleAttribute("positionX", curnode->Pos().x);
			node->SetDoubleAttribute("positionY", curnode->Pos().y);
			node->SetAttribute("terrain", curnode->Terrain());
			nodes->LinkEndChild(node);
		}

		//遍历图中的所有边，由于每个节点都有一个边的列表;
		//故遍历每个节点的边列表;
		graph_type::EdgeIterator edgeItr(graph, curnode->Index());
		for (graph_type::EdgeType *curedge = edgeItr.begin();
			!edgeItr.end();
			curedge = edgeItr.next())
		{
			//根据图中的边来创建xml的edge子元素;
			edge = new TiXmlElement("edge");
			if (edge != NULL)  //元素创建成功;
			{
				//设置边子元素的属性;
				edge->SetAttribute("from", curedge->From());
				edge->SetAttribute("to", curedge->To());
				edge->SetDoubleAttribute("cost", curedge->Cost());
				edges->LinkEndChild(edge);
			}
		}
			
	}

	//保存文件;
	doc.SaveFile(filename);
}*/


//----------------------------------------------LoadMap-------------------------------
//
//把图的节点信息和边的信息从xml文件中恢复过来;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_LoadMap(graph_type& graph, char* filename,
							int &sorceIndex, int &destIndex)
{
	//根据文件名创建xml文档;
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement*	pElem;
	TiXmlHandle	hRoot(0); //在后面使用;

	//访问<Nodes>元素;
	pElem = hDoc.FirstChildElement().Element();
	if(!pElem) return;
	hRoot = TiXmlHandle(pElem);

	
	graph_type::NodeIterator nodeItr(graph);
	graph_type::NodeType  *curNode = nodeItr.begin();  //图中的第一个节点;
	//遍历所有<node>元素;
	pElem = hRoot.FirstChildElement().Element();
	for(pElem; pElem; pElem = pElem->NextSiblingElement())
	{
		//char to int
		const char *cTerrain = pElem->Attribute("terrain");
		int terrain = atoi(cTerrain);
		
		//如果是源节点或目的节点;
		if ((TerrainType)terrain == tSOURCE)
		{
			sorceIndex = curNode->Index();
		}
		else if ((TerrainType)terrain == tDEST)
		{
			destIndex = curNode->Index();
		}

		//设置节点的地形;
		curNode->SetTerrain((TerrainType)terrain);
		curNode = nodeItr.next();

		//打印节点的信息;
		printf("%s\t, %s\t, %s\t, %s\t\n",pElem->Attribute("index"), pElem->Attribute("positionX"),
			pElem->Attribute("positionY"), pElem->Attribute("terrain"));
	}
}*/


//----------------------------------------------ClearMap-------------------------------
//
//清空图，把图的节点信息和边的信息初始化到空白图状态;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_ClearMap(graph_type& graph)
{
	//遍历图的所有节点;
	graph_type::NodeIterator nodeItr(graph);
	for (graph_type::NodeType *curnode = nodeItr.begin();
		!nodeItr.end();
		curnode = nodeItr.next())
	{
		curnode->SetTerrain(tNORMAL);
	}

	//还要考虑修复边;
}*/

#endif