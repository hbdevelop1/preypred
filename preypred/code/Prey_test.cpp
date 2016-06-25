
#pragma warning(disable:4786)


#include "Prey.h"

#include "Debugger.h"


#include "FSM.h"

#include "math\vector2d.h"
#include "pathfinding\SquareLine.h"




void Prey::test()
{
//	test_FindARefuge(1);
//test_GetVectorAwayFromAll();
//test_GetAStraightPathInDirection();

//test_detection();

//	test_SquareLine_GetALine();
	
	test_getangle2();
}


void Prey::test_FindARefuge(int i)
{
/*
<obstacle  position="7 6"/>
<obstacle  position="7 7"/>
<obstacle  position="6 7"/>
<obstacle  position="5 7"/>
<obstacle  position="5 8"/>

<actor type="Prey" position="8 10" direction="1"/>
*/
	Path path(m_board);
	Square nearestobstacle = Square(7,7);
	m_fleeingvector=vector2d(-1,-1);


	FindARefuge(nearestobstacle, &path);

}

void Prey::test_FindARefuge()
{
/*
<obstacle  position="7 6"/>
<obstacle  position="7 7"/>
<obstacle  position="6 7"/>
<obstacle  position="5 7"/>
<obstacle  position="5 8"/>

<actor type="Prey" position="8 10" direction="1"/>
*/
	m_refugedata.s_blocking=Square(7,7);
	m_refugedata.s_front=Square(7,8);
	m_refugedata.orientation=0;
	FindARefuge();



}


void Prey::test_GetVectorAwayFromAll()
{
	list<Square> l;
	vector2d directionToFleeTo;


	l.push_back(Square(4,9));
	directionToFleeTo=GetVectorAwayFromAll(l);

	l.push_back(Square(4,11));
	directionToFleeTo=GetVectorAwayFromAll(l);

	l.push_back(Square(8,12));
	directionToFleeTo=GetVectorAwayFromAll(l);

}

void Prey::test_GetAStraightPathInDirection()
{
	list<Square> l;
	vector2d directionToFleeTo;

	l.push_back(Square(4,9));
	directionToFleeTo=GetVectorAwayFromAll(l);

	directionToFleeTo.normalize();
	m_path.GetAStraightPathInDirection(GetSituation(),directionToFleeTo); 
}


void Prey::test_detection()
{
	list<Square> l_detectedprdt;
	m_visibility.GetSeenObjects(e_predator,l_detectedprdt);
	GetHeardPrdt(l_detectedprdt);
}

void Prey::test_SquareLine_GetALine()
{
	/*
<obstacle  position="7 7"/>
<obstacle  position="6 6"/>
<actor type="Prey" position="6 8" direction="1"/>
*/
	SquareLine sl;
	list<Square> l_squares;


	Square s;
	Square g;

	s=Square (5,9);
	g=Square (7,5);

	m_path.GetAStraightPathFromTo(GetSituation(),g);




	g=Square (4,8);
	m_path.GetAStraightPathFromTo(GetSituation(),g);
}

void Prey::test_getangle2()
{
	double a1_deg,a2_deg;
	double a1, a2;
	vector2d v1,v2;
	
	v1=vector2d(1,1);
	v2=vector2d(1,0);
	a1=getangle2(v1);
	a2=getangle2(v2);
	a1_deg = a1/M_PI * 180;
	a2_deg = a2/M_PI * 180;

	v1=vector2d(1,1);
	v2=vector2d(4,0);
	a1=getangle2(v1);
	a2=getangle2(v2);
	a1_deg = a1/M_PI * 180;
	a2_deg = a2/M_PI * 180;

	v1=vector2d(4,1);
	v2=vector2d(1,1);
	a1=getangle2(v1);
	a2=getangle2(v2);
	a1_deg = a1/M_PI * 180;
	a2_deg = a2/M_PI * 180;
}