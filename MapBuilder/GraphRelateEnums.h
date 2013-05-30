#ifndef GRAPH_RALATE_ENUMS
#define GRAPH_RALATE_ENUMS

//与图（Graph）相关的一些枚举值;
enum
{
	invalid_node_index = -1
};

//节点地形;
enum TerrainType  
{
	tNORMAL	= 0,    //普通地形;
	tWALL	= 1,    //墙，起阻挡作用;
	tWATER	= 2,    //水;
	tSOURCE	= 3,    //寻路的起点;
	tDEST	= 4		//寻路的终点;
};

//画刷类型;
enum  BrushType
{
	bNORMAL	= 0,
	bWALL	= 1,
	bWATER	= 2,
	bSOURCE	= 3,  //起点;
	bDEST	= 4   //终点;
};

//菜单的各种事件类型;
enum MenuEvent
{
	NEW		= 0,
	LOAD	= 1,
	SAVE	= 2,
	QUIT	= 3
};
#endif