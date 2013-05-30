#ifndef GRAPHFUNCS_H
#define GRAPHFUNCS_H

// #include "xml/tinyxml.h"
// #include "xml/tinystr.h"
#include "SparseGraph.h"
#include "Constants.h"


//----------------------------------------------ValidNeighbour-------------------------
//
//����ڸ�λ���µĽڵ��Ƿ�Ϸ�;
//--------------------------------------------------------------------------------------
bool ValidNeighbour(int x, int y, int numCellsX, int numCellsY)
{
	return !((x < 0) || (x >= numCellsX) || (y < 0) || (y >= numCellsY));
}


//----------------------------------------------AddAllNeighboursToNode--------------------
//
//��Ӹ����ڵ�������ڱ�;
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
			//�ڵ��ڽӽڵ��λ��;
			int nodeX = col + j;
			int nodeY = row + i;

			//��� i = j = 0,���Ǳ��ڵ�;
			if ((i == 0) && (j == 0))
			{
				continue;
			}

			if (ValidNeighbour(nodeX, nodeY, numCellsX, numCellsY))
			{
				//�ڵ������ֵ;
				int indexNode = row*numCellsX + col;
				//�ڽӽڵ������ֵ;
				int indexNeighborNode = nodeY*numCellsX + nodeX;

				//��������ֵ��ȡ���ڵ��λ��;
				Vector2D Node = graph.GetNode(indexNode).Pos();
				//��������ֵ��ȡ�ڽӽڵ��λ��;
				Vector2D nabNode = graph.GetNode(indexNeighborNode).Pos();

				//�ڵ�������ڽӽڵ�֮��Ļ���;
				int dis = Node.Distance(nabNode);

				NavGraphEdge newEdge(indexNode, indexNeighborNode, dis);

				graph.AddEdge(newEdge);

				//���������ͼ�����������һ����;
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
//���ݸ����Ĵ��ڳߴ�͵�Ԫ���������������;
//------------------------------------------------------------------------------------
template<class graph_type>
void GraphHelper_CreateGrid(graph_type& graph,
				int cySize,
				int	cxSize,
				int numCellsY,
				int numCellsX)
{
	//�ñ����������Խڵ��������;
	/*bool iswall = false;*/

	double CellWidth = (double)cxSize / (double)numCellsX;
	double CellHeight = (double)cySize / (double)numCellsY;

	double midX = CellWidth/2;
	double midY = CellHeight/2;

	//�����ڵ�;
	for (int row=0; row<numCellsY; ++row)
	{
		for (int col=0; col<numCellsX; ++col)
		{
			
			graph.AddNode(NavGraphNode<> (graph.GetNextFreeNodeIndex(),
				Vector2D(midX + (col*CellWidth),
				midY + (row*CellHeight))));
		}
	}

	//���ó�ʼ��Ĭ�ϵ�Դ�ڵ��Ŀ�Ľڵ㣬�������ǵĵ������ͼ���;
	graph.GetNode(souceNodeIndex).SetTerrain(tSOURCE);
	graph.GetNode(destNodeIndex).SetTerrain(tDEST);

	//Ϊ������ÿ������������ڱߣ�ÿ���ڵ���8���ڱ�;
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
//��ͼ�Ľڵ���Ϣ�ͱߵ���Ϣ���浽ָ����xml�ļ���;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_SaveMap(graph_type& graph, char* filename)
{
	//����xml�ļ�;
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//�������ڵ�Nodes;
	TiXmlElement *nodes = new TiXmlElement("Nodes");
	doc.LinkEndChild(nodes);
	//�������ڵ�Edges;
	TiXmlElement *edges = new TiXmlElement("Edges");
	doc.LinkEndChild(edges);

	//����һ��Ԫ��node���ں�����Ϊnodes����Ԫ��;
	TiXmlElement *node;
	//����һ��Ԫ��edge���ں�����Ϊedges����Ԫ��;
	TiXmlElement *edge;

	//����ͼ�����нڵ�;
	graph_type::NodeIterator nodeItr(graph);
	for (graph_type::NodeType *curnode = nodeItr.begin();
		!nodeItr.end();
		curnode = nodeItr.next())
	{
		//����ͼ�еĽڵ�������xml��node��Ԫ��;
		node = new TiXmlElement("Node");
		if (node != NULL)//�ڵ㴴���ɹ�;
		{
			//����Ԫ�ص�����;
			node->SetAttribute("index", curnode->Index());
			node->SetDoubleAttribute("positionX", curnode->Pos().x);
			node->SetDoubleAttribute("positionY", curnode->Pos().y);
			node->SetAttribute("terrain", curnode->Terrain());
			nodes->LinkEndChild(node);
		}

		//����ͼ�е����бߣ�����ÿ���ڵ㶼��һ���ߵ��б�;
		//�ʱ���ÿ���ڵ�ı��б�;
		graph_type::EdgeIterator edgeItr(graph, curnode->Index());
		for (graph_type::EdgeType *curedge = edgeItr.begin();
			!edgeItr.end();
			curedge = edgeItr.next())
		{
			//����ͼ�еı�������xml��edge��Ԫ��;
			edge = new TiXmlElement("edge");
			if (edge != NULL)  //Ԫ�ش����ɹ�;
			{
				//���ñ���Ԫ�ص�����;
				edge->SetAttribute("from", curedge->From());
				edge->SetAttribute("to", curedge->To());
				edge->SetDoubleAttribute("cost", curedge->Cost());
				edges->LinkEndChild(edge);
			}
		}
			
	}

	//�����ļ�;
	doc.SaveFile(filename);
}*/


//----------------------------------------------LoadMap-------------------------------
//
//��ͼ�Ľڵ���Ϣ�ͱߵ���Ϣ��xml�ļ��лָ�����;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_LoadMap(graph_type& graph, char* filename,
							int &sorceIndex, int &destIndex)
{
	//�����ļ�������xml�ĵ�;
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement*	pElem;
	TiXmlHandle	hRoot(0); //�ں���ʹ��;

	//����<Nodes>Ԫ��;
	pElem = hDoc.FirstChildElement().Element();
	if(!pElem) return;
	hRoot = TiXmlHandle(pElem);

	
	graph_type::NodeIterator nodeItr(graph);
	graph_type::NodeType  *curNode = nodeItr.begin();  //ͼ�еĵ�һ���ڵ�;
	//��������<node>Ԫ��;
	pElem = hRoot.FirstChildElement().Element();
	for(pElem; pElem; pElem = pElem->NextSiblingElement())
	{
		//char to int
		const char *cTerrain = pElem->Attribute("terrain");
		int terrain = atoi(cTerrain);
		
		//�����Դ�ڵ��Ŀ�Ľڵ�;
		if ((TerrainType)terrain == tSOURCE)
		{
			sorceIndex = curNode->Index();
		}
		else if ((TerrainType)terrain == tDEST)
		{
			destIndex = curNode->Index();
		}

		//���ýڵ�ĵ���;
		curNode->SetTerrain((TerrainType)terrain);
		curNode = nodeItr.next();

		//��ӡ�ڵ����Ϣ;
		printf("%s\t, %s\t, %s\t, %s\t\n",pElem->Attribute("index"), pElem->Attribute("positionX"),
			pElem->Attribute("positionY"), pElem->Attribute("terrain"));
	}
}*/


//----------------------------------------------ClearMap-------------------------------
//
//���ͼ����ͼ�Ľڵ���Ϣ�ͱߵ���Ϣ��ʼ�����հ�ͼ״̬;
//------------------------------------------------------------------------------------
/*template<class graph_type>
void	GraphHelper_ClearMap(graph_type& graph)
{
	//����ͼ�����нڵ�;
	graph_type::NodeIterator nodeItr(graph);
	for (graph_type::NodeType *curnode = nodeItr.begin();
		!nodeItr.end();
		curnode = nodeItr.next())
	{
		curnode->SetTerrain(tNORMAL);
	}

	//��Ҫ�����޸���;
}*/

#endif