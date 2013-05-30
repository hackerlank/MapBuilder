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

	static  MapBuilder* Instance();   //����һ��Ψһʵ��;

	void	BulidMap(int numCellX, int numCellY);		 //������ͼ;
	void	ClearMap();		 //���ͼ����ʼ״̬;
	void	Render();		 //���Ƶ�ͼ;

	void	SaveMap(char *filename);	 //�����ͼ������;
	void	LoadMap(char *filename);	 //�Ӵ��̼��ص�ͼ;

	void	SetBrush(BrushType _brush);  //���õ�ǰ��ˢ;
	void	SetCellBrush(int x, int y);  //��������λ�ã�x�� y���Լ���ѡ�Ļ�ˢ���������ڽڵ�ĵ���;

	void	RenderCells();				 //���ݵ�ǰ�Ļ�ˢ�����Ƶ�ͼ�ĸ���Ԫ;
	void	CreatePath();				 //���ݵ�ǰѡ����㷨���ƴ�Դ��Ŀ�Ľڵ��·��;
	void	RenderPath();				 //���Ƹ����㷨�����·��;

	void	TestOut();  //�������ɾ�����Ľڵ�;

private:
	//ϡ��ͼ;
	typedef SparseGraph<NavGraphNode<void*>, NavGraphEdge> NavGraph;
	NavGraph	*mGraph;

	//ϡ��ͼɾ�����Ľڵ㣬�������������������Ľڵ���߱ߣ��Ƚ�����;
	NavGraph::NodeVector  mNodesDeleted;

	//ͨ��Ѱ·��õĴ�Դ�ڵ㵽Ŀ�Ľڵ�֮���·��;
	std::vector<int>	mPath;

	//�㷨;
	algorithm_BFS<NavGraph> *mAlgorithmBFS;

	//��ǰʹ�õĻ�ˢ;
	BrushType	mCurrentBrush;

	//���Ľڵ��������յ�Ľڵ�����;
	int			mSourceIndex;
	int			mDestIndex;

	//���Ƶĵ�Ԫ�Ŀ�͸�;
	float		mCellWidth;
	float		mCellHeight;

	//��X���Y���ϵĵ�Ԫ�ĸ���;
	int			mNumCellX;
	int			mNumCellY;
};

#endif 