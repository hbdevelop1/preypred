
#include "../../MFCApplication1/stdafx.h"

#pragma warning(disable:4786)


#include "Path.h"
#include <assert.h>

#include <algorithm>


#include "..\Debugger.h"

#include "..\math\vector2d.h"
#include "..\math\matrix2d.h"
#include "SquareLine.h"


#if defined(_showdebugger_)
#define _debugging_path_
//#define _debugging_path2_
#endif //defined(_showdebugger_)


void Move::Alter(Actor *a)
{
	int dx,dy;
	int x,y;
	int d = a->GetDirection();

	if( d== e_right)
	{
		dx=0;
		dy=1;
	}
	else if(d== e_up)
	{
		dx=-1;
		dy=0;
	}
	else if(d== e_left)
	{
		dx=0;
		dy=-1;
	}
	else if(d== e_down)
	{
		dx=1;
		dy=0;
	}


	a->GetPosition(x,y);
	x+=dx;
	y+=dy;
	a->SetPosition(x,y);

}


void Move::SetSquare(Square & s ,eDirection d)
{
	int dx,dy;

	if( d== e_right)
	{
		dx=0;
		dy=1;
	}
	else if(d== e_up)
	{
		dx=-1;
		dy=0;
	}
	else if(d== e_left)
	{
		dx=0;
		dy=-1;
	}
	else if(d== e_down)
	{
		dx=1;
		dy=0;
	}


	s.x+=dx;
	s.y+=dy;

}




Path::~Path()
{
	Clear();
}

bool Path::StartWalk()
{
	if(m_l.empty() == false)
	{
		m_currentPos = m_l.begin();
		return true;
	}
	
	return false;
}

bool Path::Completed()
{
	assert(m_l.empty() == false);

	if(m_currentPos == m_l.end())
		return true;
	
	return false;
}

bool Path::NearlyCompleted()
{
	assert(m_l.empty() == false);

	list<Instruction*>::iterator			it =m_currentPos;
	list<Instruction*>::reverse_iterator	itr=m_l.rbegin();

	Move *m0=dynamic_cast<Move*>(*it);
	Move *m1=dynamic_cast<Move*>(*itr);

//	if(itr.base()==m_currentPos) 		return true;

	if(m0==m1)
		return true;

	return false;
}

bool Path::Walk(Actor *a)
{
if(this==reinterpret_cast<Path*>(0x00aa2700) && m_goal==Square(2,15))
{
char a;
a=1;
}
	assert(m_l.empty() == false);

	if(m_currentPos == m_l.end())
	{
		return false;
	}



	(*m_currentPos)->Alter(a);



	m_currentPos++;

	return true;
}

void Path::Clear()
{
  for(list<Instruction*>::iterator it=m_l.begin(); it!=m_l.end(); it++)
  {
	Instruction * n = *it;
	delete n;
  }
  m_l.clear();

  m_goal=Square::Invalid;
}




void	Path::Append(Path & p)
{
	Move   *m;
	Orientation  *o;


	list<Instruction*>::iterator it;
	for(it = p.m_l.begin(); it != p.m_l.end(); it++)
	{

		
		m = dynamic_cast<Move*>(*it);
		if(m != NULL)
		{
			PushBack(new Move(m->s));
		}
		else
		{
			o = dynamic_cast<Orientation*>(*it);
			PushBack(new Orientation(o->d));

		}

	}


	m_goal=p.m_goal;
}

Actor::SituationA Path::GetLastSituation()
{
	Actor::SituationA sit;
	
	list<Instruction*>::reverse_iterator it= m_l.rbegin();

	Move* m1 = dynamic_cast<Move*>(*it);
	assert(m1 != NULL); //we should never end a path with an orientation
	sit.SetPosition(m1->s.x, m1->s.y);

	if(m_l.size()==1) //in case, the path contains only one instruction of type Move
	{
		sit.GetDirection()=e_same;
		return sit;
	}

	it++;

	Move* m2 = dynamic_cast<Move*>(*it);
	if(m2 != NULL)
	{
		vector2d v=m1->s - m2->s;

		if(v==vector2d(-1,0))
			sit.GetDirection()=e_up;
		else if(v==vector2d(1,0))
			sit.GetDirection()=e_down;
		else if(v==vector2d(0,1))
			sit.GetDirection()=e_right;
		else if(v==vector2d(0,-1))
			sit.GetDirection()=e_left;
	}
	else
	{
		Orientation* o = dynamic_cast<Orientation*>(*it);
		assert(o);
		sit.GetDirection()=o->d;
	}

	return sit;
}


#define RANGEOFFLEE	5

void Path::GetAStraightPathInDirection(Actor::SituationA sit ,vector2d fleeingvector)
{
	Square pos = sit.GetPosition();

	vector2d p1 = m_board->GetCenterInPx(pos);

	int hpx = RANGEOFFLEE	*Board::CellHeightInPx();

	vector2d w;
	w=fleeingvector*hpx;

	vector2d p2 = w+p1;
	Square g = m_board->GetSquareFromLogPx(p2);
	
	//clamping
	if(g.x>15)
		g.x=15;
	if(g.y>15)
		g.y=15;
	if(g.x<0)
		g.x=0;
	if(g.y<0)
		g.y=0;
	
	
	GetAStraightPathFromTo(sit,g);//we may not arrive to g, but we'll have a path towards it, which is sufficent, in this version, for prey running out of predator.
}



//s and g are supposed to be visible to each other. 
void Path::GetAStraightPathFromTo(Actor::SituationA sit,Square g)
{
if(this==reinterpret_cast<Path*>(0x00aa2700) && g==Square(2,15))
{
char a;
a=1;
}

	Square pos = sit.GetPosition();

	vector2d p1 = m_board->GetCenterInPx(pos);
	vector2d p2 = m_board->GetCenterInPx(g);


	SquareLine sl;
	list<Square> l_squares;
	sl.GetALine(p1,p2,pos,m_board,l_squares);//get a list of squares that lead to p2. 
	//no guarantee that we really can arrive to p2. we may have been stoped by a border or an obstacle. 
	//if called from predator to find a path to a prey, there should be no border or an obstacle, as it has see nthe prey.

	//todo:improve SquareLine::GetALine to dodge obstacles

	Clear();

	if(l_squares.size())
		m_goal=ConstructThePathFromSquares(sit,l_squares);

	//m_goal is different from the wanted g when we're stoped by an obstacle


}


//make a path out of the list of squares
Square Path::ConstructThePathFromSquares(Actor::SituationA sit, list<Square> & l_squares)
{
/*
	watch out : we don't care here, if squares are obstacles ! it's supposed that this verification has been done elsewhere.
	*/

	Orientation	*o;
	Move		*m;

	Square		s_curpos=sit.GetPosition();

	
	
	/*
	todo:fix the following problem: there is not a direct path linking two successive 
	squares, diagonal to each other when neighbooring squares are obstacles.
	*/
	eDirection	d, d_lastdirectionrecorded =sit.GetDirection();
	int			n;
	for(list<Square>::iterator it =l_squares.begin();it!=l_squares.end();it++)
	{
		vector2d g=(*it)-s_curpos;
		
		assert(
			g==vector2d( 0, 1) ||
			g==vector2d( 0,-1) ||
			g==vector2d( 1, 0) ||
			g==vector2d(-1, 0) 
			/*||
			g==vector2d( 1, 1) ||
			g==vector2d( 1,-1) ||
			g==vector2d(-1, 1) ||
			g==vector2d(-1,-1) */
			);

		if(g.x == 0 || g.y == 0)
		{
			n=1;
		}
		else
		{
			n=2;
		}

		int n_cpy;
		//to each displacement, an orientation is deduced depending on the current position.
		//then, if the actor is already in that direction, it's not recorded in the path.
		while(n>0)
		{
			n_cpy=n;
			
			if(g.x!=0)
			{
				if(g.x>0)
					d=e_down;
				else// if(g.x<0)
					d=e_up;
					
				if(d_lastdirectionrecorded != d)
				{
					o= new 	Orientation(d);
					PushBack(o);
					d_lastdirectionrecorded = d;
				}
				
				s_curpos=Square(g.x,0)+s_curpos;
				m=new Move(s_curpos);
				PushBack(m);
				
				n--;
			}
			if(g.y!=0)
			{
				if(g.y>0)
					d=e_right;
				else// if(g.y<0)
					d=e_left;
					
				if(d_lastdirectionrecorded != d)
				{
					o= new 	Orientation(d);
					PushBack(o);
					d_lastdirectionrecorded = d;
				}
				
				s_curpos=Square(0,g.y)+s_curpos;
				m=new Move(s_curpos);
				PushBack(m);
			
				n--;
			}
			
			assert(n_cpy>n);//each time we arrive here (max 2 times), we should have recorded a move either on y-axis or x-axis.
		}
		
		assert(s_curpos==(*it));
	}

	return s_curpos;
}


void Path::Output()
{
#if defined (_debugging_path_)
	list<Instruction*>::iterator it;
	for(it = m_l.begin(); it != m_l.end(); it++)
	{

		Move* m = dynamic_cast<Move*>(*it);

		if(m != NULL)
		{
			Debugger::GetInstance().Output("moving to : %d %d\n",m->s.x, m->s.y);
		}
		else
		{
			Orientation* o = dynamic_cast<Orientation*>(*it);
			assert(o);

			Debugger::GetInstance().Output("changing direction to : %d\n",o->d);

		}

	}
#endif //defined (_debugging_path_)
}


void Path::operator=(const Path & p)
{
	Clear();
//	m_l=p.m_l;

	list<Instruction*>::const_iterator it = p.m_l.begin();

	Move   *m;
	Orientation  *o;

	for(; it != p.m_l.end(); it++)
	{
		m = dynamic_cast<Move*>(*it);
		if(m != NULL)
		{
			PushBack(new Move(m->s));
		}
		else
		{
			o = dynamic_cast<Orientation*>(*it);
			PushBack(new Orientation(o->d));

		}

	}

	assert(p.m_goal!=Square::Invalid);
	m_goal=p.m_goal; 
}
