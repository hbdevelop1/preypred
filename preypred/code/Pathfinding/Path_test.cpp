


#include "Path.h"
#include <assert.h>

#include <algorithm>


#include "..\Debugger.h"

#include "..\math\vector2d.h"
#include "..\math\matrix2d.h"







void Path::test_GetAStraightPathFromTo()
{
	Actor::SituationA s;
	Square g;

	s=Actor::SituationA(7,7,e_left);
	g=Square(11,11);
	GetAStraightPathFromTo(s,g);
	Output();

	g=Square(4,4);
	GetAStraightPathFromTo(s,g);
	Output();

	g=Square(4,11);
	GetAStraightPathFromTo(s,g);
	Output();

	g=Square(11,4);
	GetAStraightPathFromTo(s,g);
	Output();

	g=Square(7,11);
	GetAStraightPathFromTo(s,g);
	Output();
	
	g=Square(7,1);
	GetAStraightPathFromTo(s,g);
	Output();
	
	g=Square(3,7);
	GetAStraightPathFromTo(s,g);
	Output();
	
	g=Square(11,7);
	GetAStraightPathFromTo(s,g);
	Output();

	g=Square(6,3);
	GetAStraightPathFromTo(s,g);
	Output();



}

void Path::test_ConstructThePathFromSquares()
{
}

void Path::test()
{

	test_GetAStraightPathFromTo();
}