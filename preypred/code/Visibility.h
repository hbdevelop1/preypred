
#if !defined(VISIBILITY__INCLUDED_)
#define VISIBILITY__INCLUDED_



#include <list>
#include "Square.h"
#include <math.h>
#include "math\vector2d.h"
#include "math\line2d.h"


using namespace std;

class Actor;
class Board;

class Visibility
{

	vector2d 	m_directionVector1;
	vector2d 	m_directionVector2;
	line2d		m_line1;
	line2d		m_line2;


	eDirection									m_dir;
	int											m_VisionRange;
	int											m_VisionAngleOver2;
	Actor										*m_parent;
	Board										*m_board;


private:

	void GetSeenSquares(list<Square> & l_squares);
	void GetSquaresInViewingTriangle(list<Square> & l_squaresintriangle);

public:
	Visibility(Actor *parent,Board *board, int vr, int va);
	void SetParent(Actor *parent) {m_parent=parent;}
	void GetSeenObjects(eObjectType ot,list<Square> & l_seenobjs);
	void Update();


public:
	void test();
private:
	void test_GetSquaresInViewingTriangle();
	
};



#endif //!define VISIBILITY__INCLUDED_
