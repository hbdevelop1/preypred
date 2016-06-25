
#include "../../MFCApplication1/stdafx.h"
#include "line2d.h"

line2d::line2d(vector2d p1, vector2d p2)
{
    m_a = p2.y - p1.y;
    m_b = p1.x - p2.x;
    m_c = p2.x * p1.y - p1.x * p2.y;
}
double line2d::distanceOf(vector2d p)
{
	return (m_a * p.x + m_b * p.y + m_c);
}