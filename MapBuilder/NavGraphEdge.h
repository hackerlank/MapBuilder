#ifndef NAV_GRAPH_EDGE
#define NAV_GRAPH_EDGE

#include "GraphEdge.h"

class NavGraphEdge : public GraphEdge
{
public:
// 	enum 
// 	{
// 		normal
// 	};

protected:
	//储存一些需要的信息;
	int		mFlags;
	//如果这条边穿过一个物体（比如一颗石头），
	//那么这个ID就是这颗石头的ID;
	int mIDoIntersectingEntity;  

public:
	NavGraphEdge(int	from,
				int		to,
				double	cost,
				int		flags = 0,
				int		id = -1):GraphEdge(from, to, cost),
								mFlags(flags),
								mIDoIntersectingEntity(id)
	{}

	NavGraphEdge(std::ifstream& stream)
	{
		char buffer[50];
		stream >> buffer >> mFrom >> buffer >> mTo >> buffer >> mCost;
		stream >> buffer >> mFlags>> buffer >> mIDoIntersectingEntity;
	}

	//获取和设置标签;
	int		Flags() const{ return mFlags;}
	void	SetFlags(int flags){mFlags = flags;}

	//获取和设置穿透物体的ID;
	int		IDofIntersectingEntity() const{return mIDoIntersectingEntity;}
	void	SetIDofIntersectingEntity(int id){mIDoIntersectingEntity = id;}

	//输入边的信息;
	friend	std::ostream& operator<<(std::ostream& os, const NavGraphEdge& ne)
	{
		os << " mFrom:"<< ne.mFrom << " mTo:"<< ne.mTo
			<<" mCost:"<< ne.mCost << " mFlags:" << ne.mFlags
			<<" ID:"<<ne.mIDoIntersectingEntity << std::endl;

		return os;
	}
};

#endif