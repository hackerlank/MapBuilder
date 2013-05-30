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
	//����һЩ��Ҫ����Ϣ;
	int		mFlags;
	//��������ߴ���һ�����壨����һ��ʯͷ����
	//��ô���ID�������ʯͷ��ID;
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

	//��ȡ�����ñ�ǩ;
	int		Flags() const{ return mFlags;}
	void	SetFlags(int flags){mFlags = flags;}

	//��ȡ�����ô�͸�����ID;
	int		IDofIntersectingEntity() const{return mIDoIntersectingEntity;}
	void	SetIDofIntersectingEntity(int id){mIDoIntersectingEntity = id;}

	//����ߵ���Ϣ;
	friend	std::ostream& operator<<(std::ostream& os, const NavGraphEdge& ne)
	{
		os << " mFrom:"<< ne.mFrom << " mTo:"<< ne.mTo
			<<" mCost:"<< ne.mCost << " mFlags:" << ne.mFlags
			<<" ID:"<<ne.mIDoIntersectingEntity << std::endl;

		return os;
	}
};

#endif