
#if !defined(SEGMENT__INCLUDED_)
#define SEGMENT__INCLUDED_


#include "vector2d.h"

class Segment
{
public:
	Segment(){}
	Segment(vector2d _p0, vector2d _p1):p0(_p0),p1(_p1){}

	vector2d p0;
	vector2d p1;
};


#endif //!defined(SEGMENT__INCLUDED_)
