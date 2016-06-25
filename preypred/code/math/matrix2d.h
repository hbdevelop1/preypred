#if !defined(MATRIX__INCLUDED_)
#define MATRIX__INCLUDED_


#include "..\common.h"

#include "vector2d.h"



class matrix2d
{
public:
	double e[4];

	static matrix2d rot_P_1PI_over_4;
	static matrix2d rot_P_2PI_over_4;
	static matrix2d rot_N_1PI_over_4;
	static matrix2d rot_N_2PI_over_4;
	

	
	matrix2d(){}

	matrix2d(double a, double b, double c, double d) ;
	
	friend vector2d operator*(matrix2d m, vector2d v);

	void validate();
	double operator[](unsigned int i)
	{
		assert(i>=0 && i<4);
		return e[i];
	}

};


#endif //MATRIX__INCLUDED_
