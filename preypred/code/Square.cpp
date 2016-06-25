#include "../MFCApplication1/stdafx.h"




#include "Square.h"
#include <assert.h>



Square Square::Zero=Square(0,0);
Square Square::Invalid=Square(-1,-1);



int Square::operator[](unsigned int i)
{
	if(i==0)
		return x;
	if(i==1)
		return y;

	assert(0);
	return x;
}
