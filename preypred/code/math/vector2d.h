
#if !defined(VECTOR__INCLUDED_)
#define VECTOR__INCLUDED_

#include "..\Square.h"
#include <assert.h>


#include "..\common.h"


class vector2d
{
public:
	double x;
	double y;

	static vector2d Invalid;

/*
	static vector2d vector2d_00_p1;
	static vector2d vector2d_00_n1;

	static vector2d vector2d_n1_n1;
	static vector2d vector2d_n1_00;
	static vector2d vector2d_n1_p1;
		
	static vector2d vector2d_p1_n1;
	static vector2d vector2d_p1_00;
	static vector2d vector2d_p1_p1;
*/
	vector2d(Square s):x(s.x),y(s.y)
	{
	}
	vector2d():x(0),y(0)
	{
	}
	vector2d(double _x, double _y):x(_x),y(_y)
	{
	}

	vector2d normalize();
	void setint()
	{
		x=(int)x;
		y=(int)y;
	}
	operator Square();
	friend double distance(vector2d,vector2d);
	friend double distance_fake(vector2d);
	friend double distance_fake(vector2d,vector2d);
	friend double dot(vector2d v1, vector2d v2);
//	friend double getangle(vector2d v1, vector2d v2);
	friend double getangle2(vector2d v1, vector2d v2);
	friend double getangle2(vector2d v1);
	friend bool operator==(const vector2d v1, const vector2d v2);
	friend bool operator!=(const vector2d v1, const vector2d v2);
//	friend Square operator+(vector2d v, Square s); 	
//	friend Square operator+(Square s , vector2d v);
	friend vector2d operator+(vector2d v1 , vector2d v2);
	friend vector2d operator*(vector2d v,double f);
	friend vector2d operator-(vector2d v1 , vector2d v2);
	
	

	double & operator[](unsigned int i);
};



#endif //!defined(VECTOR__INCLUDED_)
