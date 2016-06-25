
#if !defined(COMMON__INCLUDED_)
#define COMMON__INCLUDED_


#include <math.h>

#define	    IsZero(x)	(fabs(x) <= 1.e-06)  

#define M_PI          3.14159265358979323846



extern double one_over_sqrt_2;
extern double PI_Over_8;


#define SAME_SIGNS( a, b )	\
		(((long) ((unsigned long) a ^ (unsigned long) b)) >= 0 )

//////////////////////////////

enum eObjectType
{
	e_empty,
	e_obstacle,
	e_prey,
	e_predator,
};

enum eDirection
{
	e_left,
	e_right,
	e_up,
	e_down,
	e_same,
};


#define NotOutOfBoard(ss) (ss.x>=0 && ss.x<=15 && ss.y>=0 && ss.y<=15)


/*
F : Fleeing
R : Going to refuge
H : Hiding

C : Chasing
L : Lost prey, can't see it.
M : using memory

  */

#endif //!defined(COMMON__INCLUDED_)

