
#if !defined(SQUARE__INCLUDED_)
#define SQUARE__INCLUDED_

#include <assert.h>

class Square
{
public:
	int x;
	int y;
	double criteria;

	static Square Zero;
	static Square Invalid;

	Square(){};
	Square(int _x, int _y):x(_x), y(_y){}
	Square(int *pos):x(pos[0]), y(pos[1]){}

	void GetPosition(int & _x, int & _y) 
	{
		_x=x;
		_y=y;
	}

	bool operator==(const Square  s2) const
	{
		return (x == s2.x && y == s2.y);
	}
	friend bool operator!=(const Square  s1,const Square  s2) 
	{
		return !(s1== s2);
	}

/*
	friend bool operator==(const Square  s1, const Square s2) 
	{
		return (s1.x == s2.x && s1.y == s2.y);
	}
	*/
	friend Square operator-(Square s1, Square s2) 
	{//assert(0); //this has no meaning. shouldn't it return a vector?
		return Square(s1.x - s2.x, s1.y - s2.y);
	}

	int operator[](unsigned int i);

};



#endif //!defined(SQUARE__INCLUDED_)
