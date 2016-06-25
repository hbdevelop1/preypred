#include "../MFCApplication1/stdafx.h"

#pragma warning(disable:4786)


#include "Prey.h"

#include "Debugger.h"


#include "FSM.h"

#include "math\vector2d.h"
#include "pathfinding\SquareLine.h"



struct Prey::stConfiguration Prey::ms_Configuration;


Prey::Prey(int _iX, int _iY, eDirection d, Board *b):
super(e_prey, _iX,_iY, d, b, Prey::VisionRange (),Prey::VisionAngle ())

 ,m_path(b)
,m_fleeingvector(vector2d::Invalid)
,m_refuge(Square::Invalid)
{
	START_BEHAVIOR(Prey,Idle);

	m_timetowait = 0;
}

void Prey::Update()
{
//	test();	return;


	m_timetowait--;
	if(m_timetowait <= 0)
	{
		DoBehavior();
		m_timetowait = Prey::ms_Configuration.NbrOfFramesBeforeUpdate;
	}
}


void Prey::DoBehavior()
{
	(this->*m_currentbehavior)();
}


/*
m_refuge is m_path_flee.GetGoal();
m_refuge is adjacent to an obstacle so it covers a big angle
*/
bool Prey::IsAValidRefuge(Square refuge)
{

	
	if(refuge== Square::Invalid)
		return false;

	vector2d p2= m_board->GetCenterInPx(refuge);
	vector2d p1 = p2 - m_fleeingvector*m_board->CellHeightInPx()*5;/*to obtain a point p1 on a square a little far from refuge*/

	Square s1 = m_board->GetSquareFromLogPx(p1);

	SquareLine sl;

	return !sl.CanSee(refuge,s1,m_board); 

	//i expect that vision is blocked on the square adjacent to m_refuge.


	

}

/*
an prey found a nearby obstacle while in fleeing behavior
starting from the obstacle, the first parapm., it tries to find a square where i can't be seen by predator 
*/
void Prey::FindARefuge(Square o,Path *ppath)
{

	
	SquareLine sl;

	//find an adjacent non obstacle square, from which the path to the refuge (when found) is constructed.
	Square s = sl.GetAdjacentSquareTowards(o,GetPosition(),m_board);
	
	assert(m_board->GetContentAt(s) != e_obstacle);

	m_refugedata.l_squares[0].clear();
	m_refugedata.l_squares[1].clear();
	
	

	m_refugedata.s_blocking=o;
	m_refugedata.s_front=s;
	m_refugedata.s_start=s;
	m_refugedata.orientation=0;
	FindARefuge();

	m_refugedata.s_blocking=o;
	m_refugedata.s_front=s;
	m_refugedata.s_start=s;
	m_refugedata.orientation=1;
	FindARefuge();



	if(m_refugedata.l_squares[0].size()>0 || m_refugedata.l_squares[1].size()>0)
	{//we've found a way to a square refuge
	
		Actor::SituationA sit =GetSituation();
		if(s!=sit.GetPosition())
		{//we're not on the square s, so find a path to it.
			ppath->GetAStraightPathFromTo(GetSituation(),s);
			assert(!ppath->IsEmpty()); //we've seen 's', we should find a path to it.

			sit = ppath->GetLastSituation();
			if(sit.GetDirection() == e_same)
			{//we are in this case, when the path contains only one entry of type Move.
				sit.GetDirection()=GetSituation().GetDirection();
			}
		}

		Path path1(m_board);
		Square goal;

		if(m_refugedata.l_squares[0].size()>0 && m_refugedata.l_squares[1].size()>0)
		{
			if(m_refugedata.l_squares[0].size()<m_refugedata.l_squares[1].size())
				goal=path1.ConstructThePathFromSquares(sit,m_refugedata.l_squares[0]);
			else
				goal=path1.ConstructThePathFromSquares(sit,m_refugedata.l_squares[1]);
		}
		else if(m_refugedata.l_squares[0].size()>0)
		{
			goal=path1.ConstructThePathFromSquares(sit,m_refugedata.l_squares[0]);
		}
		else if(m_refugedata.l_squares[1].size()>0)
		{
			goal=path1.ConstructThePathFromSquares(sit,m_refugedata.l_squares[1]);
		}
		else
		{
			ppath->Clear();
			return;
		}


		ppath->Append(path1);
		ppath->SetGoal(goal);
		path1.Clear();

		m_refugedata.l_squares[0].clear();
		m_refugedata.l_squares[1].clear();
	}

	
}


void Prey::FindARefuge() 
{
	Square s_blocking=m_refugedata.s_blocking;
	Square s_front=m_refugedata.s_front;
	int	orientation=m_refugedata.orientation;


	vector2d v1=s_blocking-s_front;
	vector2d v2,v3;

	
	if(orientation==0)
		v2=Square(v1.y,-v1.x);
	else //if(orientation==1)
		v2=Square(-v1.y,v1.x);
		
	Square s2 = s_front+v2;
	if(NotOutOfBoard(s2) && s2 != m_refugedata.s_start)
	{
		if(m_board->GetContentAt(s2) != e_obstacle)
		{
			m_refugedata.l_squares[orientation].push_back(s2);

			if(IsAValidRefuge(s2)) //todo:it can be seen, but can the predator really reach us ?
				return;
			else
			{
				v3=v1+v2;
				Square s3 = s_front+v3;
				if(m_board->GetContentAt(s3) != e_obstacle)
				{
					if( s3 == m_refugedata.s_start)
					{ //we've looped around the wall, and no location was a refuge.
					
						m_refugedata.l_squares[orientation].clear();
						return;
					}
					else
					{
						m_refugedata.l_squares[orientation].push_back(s3);
						m_refugedata.s_front=s3;
						m_refugedata.s_blocking=s_blocking;
						FindARefuge();
					}
				}
				else
				{
					m_refugedata.s_front=s2;
					m_refugedata.s_blocking=s3;
					FindARefuge();
				}
			}

		}
		else
		{
			//l_squares.push_back(s_front);
			m_refugedata.s_front=s_front;
			m_refugedata.s_blocking=s2;

			FindARefuge();
		}
	}
	else
	{//todo : to test this bloc
	//we've looped around the wall, and no location was a refuge.
		m_refugedata.l_squares[orientation].clear();
		return;
	}
}

/*
each time, the prey hears all predators around it, and construct a fleeing vector accordingly.
*/

void Prey::Behavior_Flee()
{

	
	CONSTRUCT_BEHAVIOR_BEGIN
		m_fleeingvector=vector2d::Invalid;
		m_refuge = Square::Invalid;
		m_fleeingwithoutdangertime=0;

		Debugger::GetInstance().Output("Prey - Flee\n");

		SetInfoText("Fleeing");

	CONSTRUCT_BEHAVIOR_END



	TICK_BEHAVIOR_BEGIN
	{
		list<Square> l_detectedprdt;
		m_visibility.GetSeenObjects(e_predator,l_detectedprdt);
		GetHeardPrdt(l_detectedprdt);

		if(l_detectedprdt.size() || m_fleeingvector!=vector2d::Invalid)
		{
			bool findnewpath=false;

			if(l_detectedprdt.size())
			{
				vector2d directionToFleeTo=GetVectorAwayFromAll(l_detectedprdt);//todo:GetVectorAwayFromAll

				if(m_fleeingvector ==vector2d::Invalid)
				{
					m_fleeingvector=directionToFleeTo;
					m_fleeingvectorangle = getangle2(m_fleeingvector);
					findnewpath = true;
				}
				else
				{
					double a_new = getangle2(directionToFleeTo);
					if(fabs(m_fleeingvectorangle-a_new) > PI_Over_8)
					{
						findnewpath = true;
						m_fleeingvector=directionToFleeTo;
						m_fleeingvectorangle = a_new;
					}
					/*
					m_fleeingvector.normalize();
					directionToFleeTo.normalize();

					if(m_fleeingvector!=directionToFleeTo)
					{
						findnewpath = true;
						m_fleeingvector=directionToFleeTo;
					}
					*/
				}

				
				//if we are near border and the m_fleeingvector is directing us out of the board
				Square pos=GetPosition();
				if(pos.x==15 && m_fleeingvector.x>0)
					m_fleeingvector.x=0;
				if(pos.x==0 && m_fleeingvector.x<0)
					m_fleeingvector.x=0;
				if(pos.y==15 && m_fleeingvector.y>0)
					m_fleeingvector.y=0;
				if(pos.y==0 && m_fleeingvector.y<0)
					m_fleeingvector.y=0;

				if(m_fleeingvector.x==0 && m_fleeingvector.y==0)
				{
					m_fleeingvector.x=1,m_fleeingvector.y=1; //just any vector, for this version
					m_fleeingvectorangle=getangle2(m_fleeingvector);
				}

				m_fleeingvector.normalize();
				assert(m_fleeingvector!=vector2d(0,0));


				m_fleeingwithoutdangertime=0;

			}
			else
			{
				//we neither see nor hear any predator. may be we can stop running.
				m_fleeingwithoutdangertime++;
				if(m_fleeingwithoutdangertime>Prey::InFleeWithoutDangerTime())
				{
					CHANGE_BEHAVIOR(Prey,Idle);
				}
			}

			if(IsAValidRefuge(m_refuge))
			{
				SetInfoText("Refuge");
				//keep going to the refuge;
				Walk(true);
			}
			else
			{
				list<Square> l_seenobstacles;
				Path path(m_board);

				m_visibility.GetSeenObjects(e_obstacle,l_seenobstacles);
				if(l_seenobstacles.size())
				{
					Square nearestobstacle = super::GetNearestObject(l_seenobstacles);
					FindARefuge(nearestobstacle, &path);
				}

				if(path.IsEmpty())
				{
					m_refuge = Square::Invalid;

					if(findnewpath)
					{
						m_path.GetAStraightPathInDirection(GetSituation(),m_fleeingvector); 

						if(m_path.IsEmpty())
						{
							//if we can't find a path, get any path, for this version.
							GetAnyPath();
							m_fleeingvector=vector2d::Invalid;
						}
						assert(!m_path.IsEmpty()); //we need a path. if we can't get any here, see why this happens.
						m_path.StartWalk();
						Walk(false);
					}
					else
					{
						Walk(false);
					}
				}
				else
				{

					m_path=path;
					m_refuge = m_path.GetGoal();

					m_path.StartWalk();
					Walk(true);

					SetInfoText("Refuge");

				}
			}
		}
		else
		{
			CHANGE_BEHAVIOR(Prey,Idle);
		}
	}
	TICK_BEHAVIOR_END


	DESTRUCT_BEHAVIOR_BEGIN	

	DESTRUCT_BEHAVIOR_END
		
}


void Prey::Behavior_Hide()
{
	

	CONSTRUCT_BEHAVIOR_BEGIN

		m_hidingtime = 0;

		Debugger::GetInstance().Output("Prey - Hide\n");

		SetInfoText("Hiding");

	CONSTRUCT_BEHAVIOR_END



	TICK_BEHAVIOR_BEGIN
	{

		list<Square> l_detectedprdt;
		m_visibility.GetSeenObjects(e_predator,l_detectedprdt);
		GetHeardPrdt(l_detectedprdt);//if always here predators, consider stopping the latter 
									//from continuing it's way when it's memory is lost.

		if(l_detectedprdt.size())
		{
			CHANGE_BEHAVIOR(Prey,Flee);
		}
		else
		{
			m_hidingtime++;
			
			if(m_hidingtime > Prey::HidingTime())
			{
				CHANGE_BEHAVIOR(Prey,Idle);
			}
		}
	}

	TICK_BEHAVIOR_END


	DESTRUCT_BEHAVIOR_BEGIN	

	DESTRUCT_BEHAVIOR_END
		
}

void Prey::Behavior_Idle()
{
	

	CONSTRUCT_BEHAVIOR_BEGIN
		GetAnyPath();
		m_path.StartWalk();

		Debugger::GetInstance().Output("Prey - Idle\n");

		SetInfoText("");

	CONSTRUCT_BEHAVIOR_END



	TICK_BEHAVIOR_BEGIN

		list<Square> l_detectedprdt;
		m_visibility.GetSeenObjects(e_predator,l_detectedprdt);
		GetHeardPrdt(l_detectedprdt);
									

		if(l_detectedprdt.size())
		{
			CHANGE_BEHAVIOR(Prey,Flee);
		}
		else if(m_path.Completed())
		{
			GetAnyPath();
			m_path.StartWalk();
		}
		else
		{
			m_path.Walk(this);
		}
	TICK_BEHAVIOR_END


	DESTRUCT_BEHAVIOR_BEGIN	
	DESTRUCT_BEHAVIOR_END
		

}


void Prey::Walk(bool toARefuge)
{
	

	if(toARefuge==false)
	{
		if(m_path.Completed())
		{
			m_path.GetAStraightPathInDirection(GetSituation(),m_fleeingvector); 
			
			if(m_path.IsEmpty())
			{
			//if we can't find a path, get any path, for this version.
				GetAnyPath();
				m_fleeingvector=vector2d::Invalid;
			}

			assert(!m_path.IsEmpty()); //we need a path. if we can't get any here, see why this happens.
			m_path.StartWalk();
			m_path.Walk(this);
		}
		else
		{
			m_path.Walk(this);
		}
	}
	else
	{
		if(m_path.Completed())
		{
			m_refuge=Square::Invalid;
			CHANGE_BEHAVIOR(Prey,Hide);
		}
		else
			m_path.Walk(this);
	}
	
}

Square Prey::GetNearestObject(Square s1, Square s2)
{
	Square pos = GetPosition();

	if(distance_fake(s1,pos)<distance_fake(s1,pos))
	{
		return s1;
	}
	else
	{
		return s2;
	}

}


vector2d Prey::GetVectorAwayFromAll(list<Square> & l_prdt)
{
	//todo:two (nearly) opposite ones, should yield a (nearly) perpendicular vector and its opposite.

	Square pos=GetPosition();
	
	list<vector2d>	l_v;
	vector2d		v,w;
	double dist;

	for(list<Square>::iterator it=l_prdt.begin();it!=l_prdt.end();it++)
	{
		dist=distance_fake(pos,*it);
		v=vector2d(pos)-vector2d(*it);
		w=v*(1/dist);
		l_v.push_back(w);
	}


	vector2d directionToFleeTo=vector2d(0,0);

	for(list<vector2d>::iterator itv=l_v.begin();itv!=l_v.end();itv++)
	{
		directionToFleeTo=directionToFleeTo+(*itv);
	}

	if(directionToFleeTo.x==0 && directionToFleeTo.y==0)
		directionToFleeTo.x=1,directionToFleeTo.y=1; //just any vector, for this version.
	
	return directionToFleeTo;
}

void Prey::GetHeardPrdt(list<Square> & l_prdt)
{

	
	list<Square>  l_squaresheard;
	
	Square pos = GetPosition();
	
	vector2d w,v;
	
	int x_max = pos.x+ HearingRange ();
	int x_min = pos.x- HearingRange ();
	int y_max = pos.y+ HearingRange ();
	int y_min = pos.y- HearingRange ();

	x_max = min(x_max,Board::SizeX()-1);
	x_min = max(x_min,0);

	y_max = min(y_max,Board::SizeY()-1);
	y_min = max(y_min,0);

	for(int i=x_min; i<=x_max; i++)
	{
		for(int j=y_min; j<=y_max; j++)
		{
			if(m_board->GetContentAt(i,j) == e_predator)
			{
				l_squaresheard.push_back(Square(i,j));
			}
		}
	}
	
	for(list<Square>::iterator it=l_squaresheard.begin(); it!=l_squaresheard.end(); it++)
	{
		SquareLine sl;
		if(sl.CanSee(pos, *it,m_board))
		{
			l_prdt.push_back(*it);
		}
	}
	

	
}







void Prey::GetAnyPath()
{
	
	int x,y;

	int goalx, goaly;

	int xr= rand();
	int yr= rand();

	GetPosition(x,y);
	eDirection d=GetDirection();

	do
	{
		xr= rand();
		yr= rand();
		
		goalx=15*xr/RAND_MAX;
		goaly=15*yr/RAND_MAX;

		Square g(goalx,goaly);

		//m_path.FindPath(x, y, d, goalx, goaly);
		m_path.GetAStraightPathFromTo(GetSituation(),g);


	}while(m_path.IsEmpty());


//	m_path.StartWalk();

}
