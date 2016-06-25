#include "../../MFCApplication1/stdafx.h"


#include "vector2d.h"
#include <math.h>

#include "matrix2d.h"


vector2d vector2d::Invalid=vector2d(0xFFFFFFFF,0xFFFFFFFF);


#define	    IsZero(x)	(fabs(x) <= 1.e-06)  

extern double one_over_sqrt_2;

double getangle2(vector2d v1)
{
	return	atan2(v1.y,v1.x);
}

double getangle2(vector2d v1, vector2d v2)
{
	assert(v1==vector2d(1,0)||v1==vector2d(-1,0)
		|| v1==vector2d(0,-1)||v1==vector2d(0,1));

//v(1,0) is the base for atan2 to calculate the angle. so transform all reference vectors to be mapped to v(1,0)
	double r=0;
	if(v1==vector2d(1,0))
	{
		r=atan2(v2.y,v2.x);
//		double a_deg = r/M_PI * 180   ;
		return r;
	}
	else if(v1==vector2d(-1,0))
	{
		r=atan2(-v2.y,-v2.x); //transform v2 the same way v1 was to be mapped to v(1,0). 
//		double a_deg = r/M_PI * 180   ;
		return r;
	}
	else if(v1==vector2d(0,1))
	{
		v2 = matrix2d::rot_N_2PI_over_4*v2; //transform v2 the same way v1 was to be mapped to v(1,0). 
		r=atan2(v2.y,v2.x);
//		double a_deg = r/M_PI * 180   ;
		return r;
	}
	else if(v1==vector2d(0,-1))
	{
		v2= matrix2d::rot_P_2PI_over_4*v2; //transform v2 the same way v1 was to be mapped to v(1,0). 
		r=atan2(v2.y,v2.x);
//		double a_deg = r/M_PI * 180   ;
		return r;
	}
	else
		assert(0);//need to find the correct matix for any arbitrary main angle.
		return r;

}

double getangle(vector2d v1, vector2d v2)
{
	v1.normalize();
	v2.normalize();
	double d = dot(v1,v2);
	double angle = acos(d);
	return angle;
}

double dot(vector2d v1, vector2d v2)
{
	return v1.x*v2.x+v1.y*v2.y;
}
vector2d vector2d::normalize()
{
	double dist=sqrt(x*x+y*y);
	x /= dist;
	y /= dist;
	return *this;
}


double & vector2d::operator[](unsigned int i)
{
	if(i==0)
		return x;
	if(i==1)
		return y;

	assert(0);
	return x;
}

bool operator==(const vector2d v1, const vector2d v2)
{
	return (IsZero(v1.x-v2.x) && IsZero(v1.y-v2.y));
}
bool operator!=(const vector2d v1, const vector2d v2)
{
	return !(v1==v2);
}
/*
Square operator+(vector2d v, Square s)
{
	if(IsZero(v.x-one_over_sqrt_2) || IsZero(v.x-1)) 
		s.x+=1;
	else if(IsZero(v.x+one_over_sqrt_2) || IsZero(v.x+1)) 
		s.x-=1;
	else if(!IsZero(v.x-0))
		assert(0);

	if(IsZero(v.y-one_over_sqrt_2) || IsZero(v.y-1)) 
		s.y+=1;
	else if(IsZero(v.y+one_over_sqrt_2) || IsZero(v.y+1)) 
		s.y-=1;
	else if(!IsZero(v.y-0))
		assert(0);

	return s;
}

Square operator+(Square s , vector2d v)
{
	if(IsZero(v.x-one_over_sqrt_2) || IsZero(v.x-1)) 
		s.x+=1;
	else if(IsZero(v.x+one_over_sqrt_2) || IsZero(v.x+1)) 
		s.x-=1;
	else if(!IsZero(v.x-0))
		assert(0);

	if(IsZero(v.y-one_over_sqrt_2) || IsZero(v.y-1)) 
		s.y+=1;
	else if(IsZero(v.y+one_over_sqrt_2) || IsZero(v.y+1)) 
		s.y-=1;
	else if(!IsZero(v.y-0))
		assert(0);

	return s;
}
*/

#if 0
Square operator+(vector2d v, Square s)
{
	if(IsZero(v.x-one_over_sqrt_2) || IsZero(v.x-1)) 
		s.x+=1;
	else if(IsZero(v.x+one_over_sqrt_2) || IsZero(v.x+1)) 
		s.x-=1;
	else if(!IsZero(v.x-0))
		assert(0);

	if(IsZero(v.y-one_over_sqrt_2) || IsZero(v.y-1)) 
		s.y+=1;
	else if(IsZero(v.y+one_over_sqrt_2) || IsZero(v.y+1)) 
		s.y-=1;
	else if(!IsZero(v.y-0))
		assert(0);

	return s;
}

Square operator+(Square s , vector2d v)
{
	s.x += v.x;
	s.y += v.y;

	return s;
}
#endif 
vector2d operator+(vector2d v1 , vector2d v2)
{
	return vector2d(v1.x+v2.x,v1.y+v2.y);
}

vector2d operator-(vector2d v1 , vector2d v2)
{
	return vector2d(v1.x-v2.x,v1.y-v2.y);
}

vector2d operator*(vector2d v,double f)
{
	return vector2d(v.x*f, v.y*f);
}

double distance_fake(vector2d p1,vector2d p2)
{
	vector2d v(p1.x-p2.x,p1.y-p2.y);
	return (v.x*v.x+v.y*v.y);
}

double distance(vector2d p1,vector2d p2)
{
	vector2d v(p1.x-p2.x,p1.y-p2.y);
	return sqrt(v.x*v.x+v.y*v.y);
}

double distance_fake(vector2d v)
{
	return (v.x*v.x+v.y*v.y);
}



vector2d::operator Square()
{
	int xx=(int)x;
	int yy=(int)y;
/*
	assert(xx>=0 && xx<=15 //todo:read the board configuration for the size
		&& yy>=0 && yy<=15);
*/	
	return Square(xx,yy);
}
