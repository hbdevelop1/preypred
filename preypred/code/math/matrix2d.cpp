#include "../../MFCApplication1/stdafx.h"
#include "matrix2d.h"
#include <math.h>



double one_over_sqrt_2 = 1.0 /sqrt(2.0);
double PI_Over_8 = atan(1)/2;


//todo : optimization : avoid multiplication to 0 members of matrices.
matrix2d matrix2d::rot_P_1PI_over_4=matrix2d(one_over_sqrt_2, -one_over_sqrt_2, one_over_sqrt_2, one_over_sqrt_2);
matrix2d matrix2d::rot_P_2PI_over_4=matrix2d(0, -1, 1, 0);
matrix2d matrix2d::rot_N_1PI_over_4=matrix2d(one_over_sqrt_2, one_over_sqrt_2, -one_over_sqrt_2, one_over_sqrt_2);
matrix2d matrix2d::rot_N_2PI_over_4=matrix2d(0, 1, -1, 0);



matrix2d::matrix2d(double a, double b, double c, double d) 
{
	e[0]=a;
	e[1]=b;
	e[2]=c;
	e[3]=d;

	validate();
}



vector2d operator*(matrix2d m, vector2d v)
{
	vector2d vout;
	vout[0]=m[0]*v[0] + m[1]*v[1];
	vout[1]=m[2]*v[0] + m[3]*v[1];
	return vout;
}



void matrix2d::validate()
{
  for(int i =0; i<4; i++)
	if (IsZero(e[i]))
	  e[i] = 0;
}
