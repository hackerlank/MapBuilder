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
//����builder������������ֻ��һ��ʵ���������Ϊ����;
//------------------------------------------------------------------------------------
MapBuilder* MapBuilder::Instance()
{
	static MapBuilder builder;

	return &builder;
}


//----------------------------------------------BuildMap-------------------------
//
//numCellX : ��X����ĵ�Ԫ�ĸ���;
//numCellY : ��Y����ĵ�Ԫ�ĸ���;
//�ú���ͨ������������������������һ�� numCellX �� numCellY�ĵ�ͼ;
//��ͼ�е�ÿ���ڵ�Ĭ�ϵ�������Ϊnormal;
//------------------------------------------------------------------------------------
void MapBuilder::BulidMap(int numCellX, int numCellY)
{
	//ɾ����ǰ��ͼ;
	if (mGraph != NULL)
	{
		delete mGraph;
		mGraph = NULL;
	}

	//����һ���µ�ϡ��ͼ;
	mGraph = new NavGraph(false);

	//����Դ��Ŀ�Ľڵ������;
	mSourceIndex = souceNodeIndex;
	mDestIndex = destNodeIndex;

	//����ϡ��ͼ��������ͼ����;
	GraphHelper_CreateGrid((*mGraph), windowHeight, windowWith, numCellY, numCellX);

	//��ֵ���Ƶ�Ԫ�Ĵ�С��width, height��;
	mCellWidth	= (float)windowWith/(float)numCellX;
	mCellHeight	= (float)windowHeight/(float)numCellY;

	//��ֵ��X���Y���ϵĵ�Ԫ�ĸ���;
	mNumCellX = numCellX;
	mNumCellY = numCellY;
}


//----------------------------------------------Render-------------------------
//���еĻ��ƹ���;
//------------------------------------------------------------------------------------
void MapBuilder::Render()
{
	RenderCells();

	RenderPath();
}


//----------------------------------------------ClearMap----------------------------
//���ͼ����ʼ״̬;;
//------------------------------------------------------------------------------------
void MapBuilder::ClearMap()
{
	assert(mGraph != NULL &&
		"<MapBuilder::SaveMap>: mGraph is null");

	//GraphHelper_ClearMap((*mGraph));

	//����ͼ�����нڵ�;
	BulidMap(mNumCellX, mNumCellY);

	mNodesDeleted.clear();

// 	NavGraph::NodeIterator nodeItr(*mGraph);
// 	for (NavGraph::NodeType *curnode = nodeItr.begin();
// 		!nodeItr.end();
// 		curnode = nodeItr.next())
// 	{
// 		//�����Դ��Ŀ�Ľڵ�;
// 		if (curnode->Index() == mSourceIndex ||
// 			curnode->Index() == mDestIndex)
// 		{
// 			continue;
// 		}
// 		curnode->SetTerrain(tNORMAL);
// 	}

	//��Ҫ�����޸���;
}


//----------------------------------------------SaveMap----------------------------
//�ѵ�ͼ��Ϣ���浽ָ����xml�ļ���;
//�������Ϣ������ 
//  �ڵ㣺index�� positionX��positionY, terrain;
//  �� ��form, to, cost;
//------------------------------------------------------------------------------------
void MapBuilder::SaveMap(char *filename)
{
	assert(mGraph != NULL && 
		"<MapBuilder::SaveMap>: mGraph is null");

	//����xml�ļ�;
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//�������ڵ�Nodes;
	TiXmlElement *nodes = new TiXmlElement("Nodes");
	doc.LinkEndChild(nodes);
	//�������ڵ�NodesDeleted;
	TiXmlElement *nodesDeleted = new TiXmlElement("NodesDeleted");
	doc.LinkEndChild(nodesDeleted);
	//�������ڵ�Edges;
	TiXmlElement *edges = new TiXmlElement("Edges");
	doc.LinkEndChild(edges);

	//����һ��Ԫ��node���ں�����Ϊnodes����Ԫ��;
	TiXmlElement *node;
	//����һ��Ԫ��nodeDel���ں�����Ϊnodesdeleted����Ԫ��;
	TiXmlElement *nodeDel;
	//����һ��Ԫ��edge���ں�����Ϊedges����Ԫ��;
	TiXmlElement *edge;

	//����ͼ�����нڵ�;
	NavGraph::NodeIterator nodeItr((*mGraph));
	for (NavGraph::NodeType *curnode = nodeItr.begin();
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
		NavGraph::EdgeIterator edgeItr(*mGraph, curnode->Index());
		for (NavGraph::EdgeType *curedge = edgeItr.begin();
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

	//������ɾ�������нڵ�;
	NavGraph::NodeVector::iterator nodeDelItr;
	for(nodeDelItr = mNodesDeleted.begin();
		nodeDelItr != mNodesDeleted.end();
		++nodeDelItr)
	{
		//����ͼ�еĽڵ�������xml��nodeDel��Ԫ��;
		nodeDel = new TiXmlElement("nodeDel");
		if (nodeDel != NULL)  //�ڵ㴴���ɹ�;
		{
			nodeDel->SetAttribute("index", nodeDelItr->Index());
			nodeDel->SetDoubleAttribute("posX", nodeDelItr->Pos().x);
			nodeDel->SetDoubleAttribute("posY", nodeDelItr->Pos().y);
			nodeDel->SetAttribute("terrain", nodeDelItr->Terrain());
			nodesDeleted->LinkEndChild(nodeDel);
		}
	}

	//�����ļ�;
	doc.SaveFile(filename);
}


//----------------------------------------------LoadMap-------------------------------
//�ѵ�ͼ��Ϣ��ָ����xml�ļ��лָ�;
//�ָ�����Ϣ������ 
//  �ڵ㣺index�� positionX��positionY, ��terrain��;
//  �� ��form, to, cost;
//------------------------------------------------------------------------------------
void MapBuilder::LoadMap(char *filename)
{
	assert(mGraph != NULL &&
		"<MapBuilder::LoadMap>: mGraph is null");

	//���ͼ;
	ClearMap();

	//�����ļ�������xml�ĵ�;
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) return;

	//��������;
	TiXmlHandle hDoc(&doc);
	//TiXmlElement*   pBaseElement;
	
	TiXmlHandle	hRoot(hDoc); //�ں���ʹ��;

	//����<Nodes>Ԫ��;
	TiXmlElement*	pNodesElem;
	pNodesElem = hRoot.FirstChild("Nodes").FirstChild().Element();
	if(!pNodesElem) return;
	//��������<node>Ԫ��;
	for(pNodesElem; pNodesElem; pNodesElem = pNodesElem->NextSiblingElement())
	{
		//��ȡ�ڵ������ֵ;
		const char *cIndex = pNodesElem->Attribute("index");
		int nodeIndex = atoi(cIndex);

		//��ȡ�ڵ�ĵ���;
		const char *cTerrain = pNodesElem->Attribute("terrain");
		int terrain = atoi(cTerrain);

		//�����Դ�ڵ��Ŀ�Ľڵ�;
		if ((TerrainType)terrain == tSOURCE)
		{
			mSourceIndex = nodeIndex;
		}
		else if ((TerrainType)terrain == tDEST)
		{
			mDestIndex = nodeIndex;
		}

		//���ýڵ�ĵ���;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)terrain);

		//��ӡ�ڵ����Ϣ;
		printf("%s\t, %s\t, %s\t, %s\t\n",pNodesElem->Attribute("index"), pNodesElem->Attribute("positionX"),
			pNodesElem->Attribute("positionY"), pNodesElem->Attribute("terrain"));
	}


	//����<NodesDeleted>Ԫ��;
	TiXmlElement*  pNodesDelElem;
	pNodesDelElem = hRoot.FirstChild("NodesDeleted").FirstChild().Element();
	if(!pNodesDelElem) return;
	//��������<NodeDel>Ԫ��;
	for (pNodesDelElem; pNodesDelElem; pNodesDelElem = pNodesDelElem->NextSiblingElement())
	{
		//��ȡ�ڵ������ֵ;
		const char* cIndex = pNodesDelElem->Attribute("index");
		int nodeIndex = atoi(cIndex);
		 
		//��ȡ�ڵ�ĵ���;
		const char *cTerrain = pNodesDelElem->Attribute("terrain");
		int terrain = atoi(cTerrain);
		 
		//��Ϊԭ����ͼ������˵ģ��������нڵ㶼�ش���;
		mNodesDeleted.push_back(mGraph->GetNode(nodeIndex));
		mGraph->RemoveNode(nodeIndex);
	}

}


//----------------------------------------------SetBrush-------------------------
//
//���ݸ����Ļ�ˢ�������趨��ǰ�Ļ�ˢ;
//----------------------------------------------------------------------------------------
void MapBuilder::SetBrush(BrushType brush)
{
	mCurrentBrush = brush;
}


//----------------------------------------------SetCellBrush------------------------------
//
//��������λ�ã�x�� y���Լ���ѡ�Ļ�ˢ���������ڵ�Ԫ����ɫ;
//----------------------------------------------------------------------------------------
void MapBuilder::SetCellBrush(int x, int y)
{
	//�������λ������ڵ������ֵ;
	int xNodeIndex = x/mCellWidth;
	int yNodeIndex = y/mCellHeight;
	int nodeIndex = yNodeIndex * mNumCellX + xNodeIndex;

	//------------��ǰҪ�����Ľڵ����---------------------;
	if ( mGraph->IsNodePresent(nodeIndex))
	{
		//����ýڵ�ԭ���ĵ���ΪԴ��Ŀ�����ͣ����ܸ���;
		TerrainType oldTerrain = mGraph->GetNode(nodeIndex).Terrain();
		switch(oldTerrain)
		{
		case tSOURCE:
		case tDEST:
			return;
		}

		//���ѡ��Ļ�ˢ������WALL����ɾ���ýڵ�;
		if (mCurrentBrush == bWALL)
		{
			NavGraph::NodeType tempNode = mGraph->GetNode(nodeIndex);

			mNodesDeleted.push_back(tempNode);  

			mGraph->RemoveNode(nodeIndex);
		}
	}
	//---------------��ǰҪ�����Ľڵ㲻����----------------;
	else
	{
		//�����ˢ��rWALL���򲻸��Ľڵ����;
		if (mCurrentBrush == bWALL)
		{
			return;
		}
		//�����ڵ��λ��;
		int xPos = xNodeIndex*mCellWidth + mCellWidth/2;
		int yPos = yNodeIndex*mCellHeight + mCellHeight/2;

		//�Ѹ�����ֵ�Ľڵ�ӡ���ɾ���ڵ㡱--mNodeDeleted--�б���ȥ��;
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
		
		//��ϡ��ͼ����ӽڵ�;
		mGraph->AddNode(NavGraph::NodeType(nodeIndex, Vector2D(xPos, yPos)));

		//��Ӵ˽ڵ�������ڱ�;
		GraphHelper_AddAllNeighborsToNode(*mGraph, yNodeIndex, xNodeIndex, mNumCellX, mNumCellY);
	}


	//------------------------------------------------
	//Ҫ�����Ľڵ�ĵ��β���Դ��Ŀ�ģ�����ݵ�ǰ��ˢ�����޸�;
	switch(mCurrentBrush)
	{
	case bSOURCE:
		//���ȰѾɵ�Դ�ڵ�ĵ�������Ϊnormal;
		mGraph->GetNode(mSourceIndex).SetTerrain((TerrainType)bNORMAL);
		//�����µ�Դ��Ŀ�ĵ�ڵ������;
		mSourceIndex = nodeIndex;
		//���ݵ�ǰѡ��Ļ�ˢ���ýڵ�ĵ���;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	case bDEST:
		//���ȰѾɵ�Ŀ�ĵؽڵ�ĵ�������Ϊnormal;
		mGraph->GetNode(mDestIndex).SetTerrain((TerrainType)bNORMAL);
		//�����µ�Դ��Ŀ�ĵ�ڵ������;
		mDestIndex = nodeIndex;
		//���ݵ�ǰѡ��Ļ�ˢ���ýڵ�ĵ���;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	default:
		//���ݵ�ǰѡ��Ļ�ˢ���ýڵ�ĵ���;
		mGraph->GetNode(nodeIndex).SetTerrain((TerrainType)mCurrentBrush);
		break;
	}

}


//----------------------------------------------RenderCells-------------------------
//
//��ͼ�Ľڵ���Ϣ���Ƴ������ú�����glut�Ļ��ƺ�������;
//----------------------------------------------------------------------------------------
void MapBuilder::RenderCells()
{
	GLfloat delta = 5;

	NavGraph::NodeIterator nodeItr((*mGraph));
	for (NavGraph::NodeType *pNode = nodeItr.begin();
		!nodeItr.end();
		pNode = nodeItr.next())
	{
		//���ݽڵ�ĵ������������û�ˢ������;
		BrushType nodeTerrain = (BrushType)pNode->Terrain(); //ǿ������ת��;
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


		//��ȡ�ڵ��λ����Ϣ;
		GLfloat posX = pNode->Pos().x;
		GLfloat posY = pNode->Pos().y;

		//����һ�����������һ����Ԫ������Ҫx��y�����ϵ�ƫ����;
		float xCellDelta = mCellWidth/2-0.5;
		float yCellDelta = mCellHeight/2-0.5;

		glBegin(GL_QUADS);
		//���û��ƾ��ε��ĸ���;
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
//���Ƹ����㷨�����·��;
//----------------------------------------------------------------------------------------
void MapBuilder::RenderPath()
{
	//���·��Ϊ��;
	if (mPath.empty())
	{
		return;
	}

	//·����Ϊ�գ������λ����Ϣ����;
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);

	//����mPath�����нڵ�;
	std::vector<int>::iterator nodeItr;
	for (nodeItr = mPath.begin();
		 nodeItr != mPath.end();
		 ++nodeItr)
	{
		//��ȡ�ڵ��λ����Ϣ;
		GLfloat posX = mGraph->GetNode(*nodeItr).Pos().x;
		GLfloat posY = mGraph->GetNode(*nodeItr).Pos().y;

		glVertex2d(posX, posY);
	}
	glEnd();
}


//----------------------------------------------RenderPath----------------------------
//
//���ݵ�ǰѡ����㷨���ƴ�Դ��Ŀ�Ľڵ��·��;
//------------------------------------------------------------------------------------
void MapBuilder::CreatePath()
{
	if (mAlgorithmBFS != NULL)
	{
		delete mAlgorithmBFS;
		mAlgorithmBFS = NULL;
	}
	mAlgorithmBFS = new algorithm_BFS<NavGraph>(*mGraph, mSourceIndex, mDestIndex);

	//���ڴ�Դ��Ŀ��֮���·��;
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