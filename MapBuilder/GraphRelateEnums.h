#ifndef GRAPH_RALATE_ENUMS
#define GRAPH_RALATE_ENUMS

//��ͼ��Graph����ص�һЩö��ֵ;
enum
{
	invalid_node_index = -1
};

//�ڵ����;
enum TerrainType  
{
	tNORMAL	= 0,    //��ͨ����;
	tWALL	= 1,    //ǽ�����赲����;
	tWATER	= 2,    //ˮ;
	tSOURCE	= 3,    //Ѱ·�����;
	tDEST	= 4		//Ѱ·���յ�;
};

//��ˢ����;
enum  BrushType
{
	bNORMAL	= 0,
	bWALL	= 1,
	bWATER	= 2,
	bSOURCE	= 3,  //���;
	bDEST	= 4   //�յ�;
};

//�˵��ĸ����¼�����;
enum MenuEvent
{
	NEW		= 0,
	LOAD	= 1,
	SAVE	= 2,
	QUIT	= 3
};
#endif