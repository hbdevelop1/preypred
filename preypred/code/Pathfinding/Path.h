
#if !defined(PATH__INCLUDED_)
#define PATH__INCLUDED_




#include <list>
#include "..\objects\actor.h"
#include <math.h>
#include "..\Square.h"





class Instruction
{
public:
	virtual ~Instruction(){}

	virtual void Alter(Actor *a)=0;
};

/*
when playing back a path, we only need the info that a Move object exists.
we then move one square in the current direction.
's' and 'SetSquare' are needed mainly to avoid looping.
*/
class Move : public Instruction
{
public:
	Square s;

	
	Move(Square _s):s(_s){}

	void Alter(Actor *a);
	void SetSquare(Square & s ,eDirection d);
};

class Orientation : public Instruction
{
public:
	eDirection d;


	Orientation(eDirection _d):d(_d){}

	
	void Alter(Actor *a)
	{
		a->SetDirection(static_cast<eDirection>(d));
	}
};



/*
Path has a list of nodes.
it knows the goal it's reaching.
*/

class Path
{
private:
	list<Instruction*>					m_l;
	list<Instruction*>::iterator		m_currentPos;//current position in the path. call Walk to advance (and change m_currentPos)

	Board*			m_board;
	Square						m_goal;


public:
	void FindPathAwayFrom(Square preyPos, eDirection d, Square prdtrPos);
	void FindPathToALevel();
	Square GetGoal(){return m_goal;}

	void GetAStraightPathInDirection(Actor::SituationA sit ,vector2d  w);
	void GetAStraightPathFromTo(Actor::SituationA sit,Square g);
	Square ConstructThePathFromSquares(Actor::SituationA sit,list<Square> & l_squares);
	Actor::SituationA GetLastSituation();
	void	Append(Path &);
	void	SetGoal(Square s) {m_goal=s;}


	void Output();


public:
	void test();
private:
	void test_GetAStraightPathFromTo();
	void test_ConstructThePathFromSquares();



public:
	Path(Board* w):m_board(w),m_goal(Square::Invalid) {}

	~Path();

	void PushBack(Instruction *n)
	{
		m_l.push_back(n);
	}
	void PopBack()
	{
		m_l.pop_back();
	}

	bool StartWalk();
	bool Completed();
	bool NearlyCompleted();
	bool Walk(Actor *a);


	bool IsEmpty()
	{
		return (m_l.size() == 0);
	}

	void Clear();
	
	void operator=(const Path & p);
};








#endif // !defined(PATH__INCLUDED_)