#include <stdlib.h>
#include <glut.h>
#include "xml/tinyxml.h"
#include "xml/tinystr.h"
#include "MapBuilder.h"
#include "GraphHelper.h"
#include "Constants.h"


MapBuilder::MapBuilder()
	: mCurrentBrush(bWATER),
	  mGraph(NULL),
	  mCellWidth(0),
	  mCellHeight(0),
	  mSourceIndex(souceNodeIndex),
	  mDestIndex(destNodeIndex)
{
}

MapBuilder::~MapBuilder()
{
	delete mGraph;
	delete mAlgorithmBFS;
}

//----------------------------------------------Instance-------------------------
//
//考虑builder在整个程序中只有一个实例，故设计为单体;
//------------------------------------------------------------------------------------
MapBuilder* MapBuilder::Instance()
{
	static MapBuilder builder;

	return &builder;
}


//----------------------------------------------BuildMap-------------------------
//
//numCellX : 在X方向的单元的个数;
//numCellY : 在Y方向的单元的个数;
//该函数通过给定的这两个参数来创建一个 numCellX × numCellY的地图;
//该图中的每个节点默认地形类型为normal;
//------------------------------------------------------------------------------------
void MapBuilder::BulidMap(int numCellX, int numCellY)
{
	//删除先前的图;
	if (mGraph != NULL)
	{
		delete mGraph;
		mGraph = NULL;
	}

	//创建一个新的稀疏图;
	mGraph = new NavGraph(false);

	//设置源和目的节点的索引;
	mSourceIndex = souceNodeIndex;
	mDestIndex = destNodeIndex;

	//根据稀疏图来构建地图网格;
	GraphHelper_CreateGrid((*mGraph), windowHeight, windowWith, numCellY, numCellX);

	//赋值绘制单元的大小（width, height）;
	mCellWidth	= (float)windowWith/(float)numCellX;
	mCellHeight	= (float)windowHeight/(float)numCellY;

	//赋值在X轴和Y轴上的单元的个数;
	mNumCellX = numCellX;
	mNumCellY = numCellY;
}


//----------------------------------------------Render-------------------------
//所有的绘制工作;
//------------------------------------------------------------------------------------
void MapBuilder::Render()
{
	RenderCells();

	RenderPath();
}


//----------------------------------------------ClearMap----------------------------
//清空图，初始状态;;
//------------------------------------------------------------------------------------
void MapBuilder::ClearMap()
{
	assert(mGraph != NULL &&
		"<MapBuilder::SaveMap>: mGraph is null");

	//GraphHelper_ClearMap((*mGraph));

	//遍历图的所有节点;
	BulidMap(mNumCellX, mNumCellY);

	mNodesDeleted.clear();

// 	NavGraph::NodeIterator nodeItr(*mGraph);
// 	for (NavGraph::NodeType *curnode = nodeItr.begin();
// 		!nodeItr.end();
// 		curnode = nodeItr.next())
// 	{
// 		//不清除源和目的节点;
// 		if (curnode->Index() == mSourceIndex ||
// 			curnode->Index() == mDestIndex)
// 		{
// 			continue;
// 		}
// 		curnode->SetTerrain(tNORMAL);
// 	}

	//还要考虑修复边;
}


//----------------------------------------------SaveMap----------------------------
//把地图信息保存到指定的xml文件中;
//保存的信息包含： 
//  节点：index， positionX，positionY, terrain;
//  边 ：form, to, cost;
//------------------------------------------------------------------------------------
void MapBuilder::SaveMap(char *filename)
{
	assert(mGraph != NULL && 
		"<MapBuilder::SaveMap>: mGraph is null");

	//创建xml文件;
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//创建根节点Nodes;
	TiXmlElement *nodes = new TiXmlElement("Nodes");
	doc.LinkEndChild(nodes);
	//创建根节点NodesDeleted;
	TiXmlElement *nodesDeleted = new TiXmlElement("NodesDeleted");
	doc.LinkEndChild(nodesDeleted);
	//创建根节点Edges;
	TiXmlElement *edges = new TiXmlElement("Edges");
	doc.LinkEndChild(edges);

	//声明一个元素node，在后面作为nodes的子元素;
	TiXmlElement *node;
	//声明一个元素nodeDel，在后面作为nodesdeleted的子元素;
	TiXmlElement *nodeDel;
	//声明一个元素edge，在后面作为edges的子元素;
	TiXmlElement *edge;

	//遍历图的所有节点;
	NavGraph::NodeIterator nodeItr((*mGraph));
	for (NavGraph::NodeType *curnode = nodeItr.begin();
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
		NavGraph::EdgeIterator edgeItr(*mGraph, curnode->Index());
		for (NavGraph::EdgeType *curedge = edgeItr.begin();
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

	//遍历被删除的所有节点;
	NavGraph::NodeVector::iterator nodeDelItr;
	for(nodeDelItr = mNodesDeleted.begin();
		nodeDelItr != mNodesDeleted.end();
		++nodeDelItr)
	{
		//根据图中的节点来创建xml的nodeDel子元素;
		nodeDel = new TiXmlElement("nodeDel");
		if (nodeDel != NULL)  //节点创建成功;
		{
			nodeDel->SetAttribute("index", nodeDelItr->Index());
			nodeDel->SetDoubleAttribute("posX", nodeDelItr->Pos().x);
			nodeDel->SetDoubleAttribute("posY", nodeDelItr->Pos().y);
			nodeDel->SetAttribute("terrain", nodeDelItr->Terrain());
			nodesDeleted->LinkEndChild(nodeDel);
		}
	}

	//保存文件;
	doc.SaveFile(filename);
}


//----------------------------------------------LoadMap-------------------------------
//把地图信息从指定的xml文件中恢复;
//恢复的信息包含： 
//  节点：index， positionX，positionY, 【terrain】;
//  边 ：form, to, cost;
//------------------------------------------------------------------------------------
void MapBuilder::LoadMap(char *filename)
{
	assert(mGraph != NULL &&
		"<MapBuilder::LoadMap>: mGraph is null");

	//清空图;
	ClearMap();

	//根据文件名创建xml文档;
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) return;

	//变量声明;
	TiXmlHandle hDoc(&doc);
	//TiXmlElement*   pBaseElement;
	
	TiXmlHandle	hRoot(hDoc); //在后面使用;

	//访问<Nodes>元素;
	TiXmlElement*	pNodesElem;
	pNodesElem = hRoot.FirstChild("Nodes").FirstChild().Element();
	if(!pNodesElem) return;
	//遍历所有<node>元素;
	for(pNodesElem; pNodesElem; pNodesElem = pNodesElem->NextSiblingElement())
	{
		//获取节点的索引值;
		const char *cIndex = pNodesElem->Attribute("index");
		int nodeIndex = atoi(cIndex);

		//获取节点的地形;
		const char *cTerrain = pNodesElem->Attribute("terrain");
		int terrain = atoi(cTerrain);

		//如果是源节点或目的节点;
		if ((TerrainType)terrain == tSOURCE)
		{
			mSourceIndex = nodeIndex;
		}
		else if ((TerrainType)terrain == tDEST)
		{
			mDestIndex = nodeIndex;
		}

		//设置节点的地形;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)terrain);

		//打印节点的信息;
		printf("%s\t, %s\t, %s\t, %s\t\n",pNodesElem->Attribute("index"), pNodesElem->Attribute("positionX"),
			pNodesElem->Attribute("positionY"), pNodesElem->Attribute("terrain"));
	}


	//访问<NodesDeleted>元素;
	TiXmlElement*  pNodesDelElem;
	pNodesDelElem = hRoot.FirstChild("NodesDeleted").FirstChild().Element();
	if(!pNodesDelElem) return;
	//遍历所有<NodeDel>元素;
	for (pNodesDelElem; pNodesDelElem; pNodesDelElem = pNodesDelElem->NextSiblingElement())
	{
		//获取节点的索引值;
		const char* cIndex = pNodesDelElem->Attribute("index");
		int nodeIndex = atoi(cIndex);
		 
		//获取节点的地形;
		const char *cTerrain = pNodesDelElem->Attribute("terrain");
		int terrain = atoi(cTerrain);
		 
		//因为原来的图是清空了的，所以所有节点都必存在;
		mNodesDeleted.push_back(mGraph->GetNode(nodeIndex));
		mGraph->RemoveNode(nodeIndex);
	}

}


//----------------------------------------------SetBrush-------------------------
//
//根据给定的画刷类型类设定当前的画刷;
//----------------------------------------------------------------------------------------
void MapBuilder::SetBrush(BrushType brush)
{
	mCurrentBrush = brush;
}


//----------------------------------------------SetCellBrush------------------------------
//
//根据鼠标的位置（x， y）以及所选的画刷来设置所在单元的颜色;
//----------------------------------------------------------------------------------------
void MapBuilder::SetCellBrush(int x, int y)
{
	//根据鼠标位置求出节点的索引值;
	int xNodeIndex = x/mCellWidth;
	int yNodeIndex = y/mCellHeight;
	int nodeIndex = yNodeIndex * mNumCellX + xNodeIndex;

	//------------当前要操作的节点存在---------------------;
	if ( mGraph->IsNodePresent(nodeIndex))
	{
		//如果该节点原来的地形为源或目的类型，则不能更改;
		TerrainType oldTerrain = mGraph->GetNode(nodeIndex).Terrain();
		switch(oldTerrain)
		{
		case tSOURCE:
		case tDEST:
			return;
		}

		//如果选择的画刷类型是WALL，则删除该节点;
		if (mCurrentBrush == bWALL)
		{
			NavGraph::NodeType tempNode = mGraph->GetNode(nodeIndex);

			mNodesDeleted.push_back(tempNode);  

			mGraph->RemoveNode(nodeIndex);
		}
	}
	//---------------当前要操作的节点不存在----------------;
	else
	{
		//如果画刷是rWALL，则不更改节点地形;
		if (mCurrentBrush == bWALL)
		{
			return;
		}
		//新增节点的位置;
		int xPos = xNodeIndex*mCellWidth + mCellWidth/2;
		int yPos = yNodeIndex*mCellHeight + mCellHeight/2;

		//把该索引值的节点从“被删除节点”--mNodeDeleted--列表中去除;
		NavGraph::NodeVector::iterator nodeDelItr;
		for (nodeDelItr = mNodesDeleted.begin();
			nodeDelItr != mNodesDeleted.end();
			++nodeDelItr)
		{
			if (nodeDelItr->Index() == nodeIndex) 
			{
				mNodesDeleted.erase(nodeDelItr);
			}
		}
		
		//在稀疏图中添加节点;
		mGraph->AddNode(NavGraph::NodeType(nodeIndex, Vector2D(xPos, yPos)));

		//添加此节点的所有邻边;
		GraphHelper_AddAllNeighborsToNode(*mGraph, yNodeIndex, xNodeIndex, mNumCellX, mNumCellY);
	}


	//------------------------------------------------
	//要操作的节点的地形不是源或目的，则根据当前画刷进行修改;
	switch(mCurrentBrush)
	{
	case bSOURCE:
		//首先把旧的源节点的地形设置为normal;
		mGraph->GetNode(mSourceIndex).SetTerrain((TerrainType)bNORMAL);
		//设置新的源和目的点节点的索引;
		mSourceIndex = nodeIndex;
		//根据当前选择的画刷设置节点的地形;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	case bDEST:
		//首先把旧的目的地节点的地形设置为normal;
		mGraph->GetNode(mDestIndex).SetTerrain((TerrainType)bNORMAL);
		//设置新的源和目的点节点的索引;
		mDestIndex = nodeIndex;
		//根据当前选择的画刷设置节点的地形;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	default:
		//根据当前选择的画刷设置节点的地形;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	}

}


//----------------------------------------------RenderCells-------------------------
//
//把图的节点信息绘制出来，该函数被glut的绘制函数调用;
//----------------------------------------------------------------------------------------
void MapBuilder::RenderCells()
{
	GLfloat delta = 5;

	NavGraph::NodeIterator nodeItr((*mGraph));
	for (NavGraph::NodeType *pNode = nodeItr.begin();
		!nodeItr.end();
		pNode = nodeItr.next())
	{
		//根据节点的地形类型来设置画刷的类型;
		BrushType nodeTerrain = (BrushType)pNode->Terrain(); //强制类型转换;
		switch(nodeTerrain) //
		{
		case bNORMAL:
			glColor3f(1, 1, 1);
			break;
		case bWALL:
			glColor3f(0, 1, 0);
			break;
		case bWATER:
			glColor3f(0, 0, 1);
			break;
		case bSOURCE:
			glColor3f(1, 1, 0);
			break;
		case bDEST:
			glColor3f(0, 1, 1);
			break;
		default:
			break;
		}


		//获取节点的位置信息;
		GLfloat posX = pNode->Pos().x;
		GLfloat posY = pNode->Pos().y;

		//利用一个点进而绘制一个单元，顾需要x，y方向上的偏移量;
		float xCellDelta = mCellWidth/2-0.5;
		float yCellDelta = mCellHeight/2-0.5;

		glBegin(GL_QUADS);
		//设置绘制矩形的四个点;
		glVertex2d(posX+xCellDelta, posY+yCellDelta);
		glVertex2d(posX+xCellDelta, posY-yCellDelta);
		glVertex2d(posX-xCellDelta, posY-yCellDelta);
		glVertex2d(posX-xCellDelta, posY+yCellDelta);
		glEnd();

		//glRasterPos2f(posX, posY);
	}
}


//----------------------------------------------RenderPath-------------------------------
//
//绘制根据算法求出的路径;
//----------------------------------------------------------------------------------------
void MapBuilder::RenderPath()
{
	//如果路径为空;
	if (mPath.empty())
	{
		return;
	}

	//路径不为空，则根据位置信息绘制;
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);

	//遍历mPath中所有节点;
	std::vector<int>::iterator nodeItr;
	for (nodeItr = mPath.begin();
		 nodeItr != mPath.end();
		 ++nodeItr)
	{
		//获取节点的位置信息;
		GLfloat posX = mGraph->GetNode(*nodeItr).Pos().x;
		GLfloat posY = mGraph->GetNode(*nodeItr).Pos().y;

		glVertex2d(posX, posY);
	}
	glEnd();
}


//----------------------------------------------RenderPath----------------------------
//
//根据当前选择的算法绘制从源到目的节点的路径;
//------------------------------------------------------------------------------------
void MapBuilder::CreatePath()
{
	if (mAlgorithmBFS != NULL)
	{
		delete mAlgorithmBFS;
		mAlgorithmBFS = NULL;
	}
	mAlgorithmBFS = new algorithm_BFS<NavGraph>(*mGraph, mSourceIndex, mDestIndex);

	//存在从源到目的之间的路径;
	if (mAlgorithmBFS->Found())
	{
		mPath = mAlgorithmBFS->GetPath();

		printf("find the dest!\n");
	}
}



void MapBuilder::TestOut()
{
	NavGraph::NodeVector::iterator nodeItr;
	for (nodeItr = mNodesDeleted.begin();
		nodeItr != mNodesDeleted.end();
		++nodeItr)
	{
		printf("index:%d\t posX:%f\t posY:%f\t terrain:%d\n", nodeItr->Index(), nodeItr->Pos().x, nodeItr->Pos().y, nodeItr->Terrain());
	}
}