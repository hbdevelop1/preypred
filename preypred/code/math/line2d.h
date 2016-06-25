
#if !defined(LINE__INCLUDED_)
#define LINE__INCLUDED_


#include "vector2d.h"

class line2d
{
private:
	double m_a;
	double m_b;
	double m_c;
public:
	line2d(){m_a=m_b=m_c=0;}
	line2d(vector2d p1, vector2d p2);
	double distanceOf(vector2d p);
};



#endif //!defined(LINE__INCLUDED_)
