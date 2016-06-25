
#include "../MFCApplication1/stdafx.h"

#pragma warning(disable:4786)

#include "Predator.h"
#include "Debugger.h"
#include "FSM.h"
#include <assert.h>

#include "math\vector2d.h"
#include "math\matrix2d.h"

#include "math\xlines.h"

#include "pathfinding\SquareLine.h"

#include "Game.h"

//#define _debug_hunting_
//#define _debug_path_

struct Predator::stConfiguration Predator::ms_Configuration;








Predator::Predator(int _iX, int _iY, eDirection d, Board *b):
super(e_predator, _iX,_iY, d, b, Predator::VisionRange(), Predator::VisionAngle())
,m_path(b)
{

	START_BEHAVIOR(Predator,SearchAndChase);

	m_timetowait = 0;
	m_memory.LooseIt();
}



void Predator::Update()
{

//	test(); 	return;	

	m_timetowait--;
	if(m_timetowait <= 0)
	{
		DoBehavior();
		m_timetowait = Predator::ms_Configuration.NbrOfFramesBeforeUpdate;
	}
}


void Predator::DoBehavior()
{
	(this->*m_currentbehavior)();
}





void Predator::Behavior_SearchAndChase()
{

	CONSTRUCT_BEHAVIOR_BEGIN

				m_path.Clear();

	CONSTRUCT_BEHAVIOR_END



	TICK_BEHAVIOR_BEGIN
		
		SearchAndChase();

	TICK_BEHAVIOR_END


	DESTRUCT_BEHAVIOR_BEGIN	

	DESTRUCT_BEHAVIOR_END

}

/*
during test, make sure all conditions blocks were hit.
*/
void Predator::SearchAndChase()
{
	bool findanypath=false;
	list<Square> l_seen;
	
	m_visibility.GetSeenObjects(e_prey,l_seen);

	if(l_seen.size())
	{
		Square pos =GetPosition();
		Actor::SituationA sit =GetSituation() ;

		Square nearestpreypos = GetNearestObject(l_seen);

		if(m_memory.IsLost())
		{//we're chasing no other prey. so chase this one.
			SetInfoText("Chasing");

			m_memory.Reset();

			m_path.GetAStraightPathFromTo(sit,nearestpreypos );
			assert(!m_path.IsEmpty());//we need a path to the square
			m_path.StartWalk();
			Walk();
		}
		else
		{
			//we already have a goal in memory (prey). is it the same ?
			Square goalpos = m_path.GetGoal();
			if(goalpos==nearestpreypos)
			{//the prey is not moving (or has been replaced) :
			//keep the same path and continue following it.
				m_memory.Reset();
				Walk();
			}
			else
			{
				if(m_board->GetContentAt(goalpos)==e_prey)
				{
					//nearestpreypos is another prey;
					//drop the one we remember in goalpos, and follow the nearest
					m_memory.Reset();

					m_path.GetAStraightPathFromTo(sit,nearestpreypos );
					assert(!m_path.IsEmpty());//we need a path to the square
					m_path.StartWalk();
					Walk();
				}
				else
				{ 
					//1-either our prey has disappeared. two cases:
					//1.1-either it's the same prey we've found in a previous frame, it has just moved.
					//1.2-or it has disappeared behind a corner (or has been eaten by another predator, but we have no way to detect this case)
					//todo:improvement:we can see if a corner is around, take a look around where the prey was for a wall.
					//2-or our prey hasn't disappeared. it is just us that can't see it while we are changing direction following our way.
					
					//first, see if it's the same
					Square diff=goalpos-nearestpreypos;
					if(diff.x>=-2 && diff.x<=2 && diff.y>=-2 && diff.y<=2)
					{
						//it's the same prey we're chasing, it has just moved.
						//but adjust the path to it.
						m_memory.Reset();

						m_path.GetAStraightPathFromTo(sit,nearestpreypos );
						assert(!m_path.IsEmpty());//we need a path to the square
						m_path.StartWalk();
						Walk();
					}
					else 
					{
						//the prey we're chasing has really disappeared. so pick the nearest.
						Square pos = GetPosition();
						double dist_to_prey = distance_fake(pos,nearestpreypos);
						double dist_to_goal = distance_fake(pos,goalpos );
						if(dist_to_goal <dist_to_prey && m_memory.IsLost()==false)
						{//the place where the prey has disappeared is nearer than the seen prey
						//so keep the same path and continue following it, but memory is starting to be lost

							m_memory.Update();
							Walk();
						}
						else
						{//the place where the prey has disappeared is farther than the seen prey
						//so chase the new one
							m_memory.Reset();

							m_path.GetAStraightPathFromTo(sit,nearestpreypos );
							assert(!m_path.IsEmpty());//we need a path to the square
							m_path.StartWalk();
							Walk();
						}
					}
				}
			}
		}
	}
	else
	{ //if we've seen no prey, we'll use our memory
	
		bool takeanypath = false;

		if(m_memory.IsLost())
		{
			takeanypath=true;
		}
		else
		{
			//do we have a path ?
			if(m_path.IsEmpty())
			{
				assert(0); //we never had a path ?!!!!!
			}
			else
			{
				SetInfoText("Memory");
				m_memory.Update();
			
				if(m_path.Completed() )
				{
					//we've arrived to where the prey had last been seen
					//we're either near a corner or the prey has been eaten by another predator 
					//todo: improvement : try to search for it around us
					//in this version, just find another way
					takeanypath=true;
				}
				else
				{
					Walk(false);
				}
			}
		}
		
		if(takeanypath==true)
		{
			SetInfoText("");

			if(m_path.IsEmpty() || m_path.Completed())
			{
				GetAnyPath();
				assert(!m_path.IsEmpty()); //GetAnyPath is not supposed to return with no path found
				m_path.StartWalk();
				m_path.Walk(this);
			}
			else
			{
				m_path.Walk(this);
			}
		}
	}

	
}




void Predator::Behavior_Eat()
{

	CONSTRUCT_BEHAVIOR_BEGIN

		Debugger::GetInstance().Output("Predator-Eating\n");

		Square goalpos = m_path.GetGoal();
		if(m_board->GetContentAt(goalpos)==e_prey) //this test is needed in case more than one predator is chasing the same prey.
		{
			GetGame()->DestroyPreyAt(goalpos);
			//SetPosition(goalpos.x,goalpos.y);
			m_path.Walk(this);
		}
		else
		{
			//make sure another predator has eaten the prey.
			assert(0);
		}

	CONSTRUCT_BEHAVIOR_END



	TICK_BEHAVIOR_BEGIN

		CHANGE_BEHAVIOR(Predator,SearchAndChase)

	TICK_BEHAVIOR_END


	DESTRUCT_BEHAVIOR_BEGIN	

		SetInfoText("");

	DESTRUCT_BEHAVIOR_END


	
}





	
void Predator::Walk(bool seen)
{

	if(m_path.NearlyCompleted() )
	{
		if(m_board->GetContentAt(m_path.GetGoal())==e_prey)
		{
			m_memory.LooseIt();
			CHANGE_BEHAVIOR(Predator,Eat);
		}
		else
		{
			if(seen)
				assert(0); //in case we've seen the prey, this shouldn't happen.
			else
			{
				//in case, we're here because we remember it, prey must have disappeared behind a corner or has been eaten.
				m_memory.LooseIt(); //to find a new way next time.
				m_path.Walk(this);//and move to the last position.
			}

		}
	}
	else if(m_path.Completed() )
	{
		assert(0); 
			//in case it's the first time we see a prey, it means we're colliding with it (15). todo:improve
			//in case the found prey is not moving and is already in memory : this shouldn't happen
			//other case : this shouldn't happen. we're supposed to be at eat behavior when Completed() happens
	}
	else
		m_path.Walk(this);

		
}



//generate any target square, generate a path to it
//todo:improvement:review all directions and select a square, until a path could be found
void Predator::GetAnyPath()
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
