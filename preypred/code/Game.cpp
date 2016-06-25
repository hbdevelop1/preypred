
#include "../MFCApplication1/stdafx.h"
#include <afxwin.h>
#include "Game.h"
#include "objects\Actor.h"

#include "predator.h"
#include "prey.h"


Game::Game()
{
	m_state=e_playgame;
/*
	CreateActor(e_predator,1,1,Getboard());
	CreateActor(e_predator,2,2,Getboard());
	*/
}


/*
UINT Game::GameLoop (LPVOID pParam)
{
	g_game.CreateActor(e_predator,1,1,g_game.Getboard());

	while(g_game.GetState() != Game::e_stopgame)
	{
		char a;
		a=2;
	}

//	AfxMessageBox(" game ended");
	return 0;

}

*/

void Game::Loop ()
{
	for(list<Actor*>::iterator it=m_actors.begin(); it != m_actors.end(); it++)
	{
		(*it)->Update();
	}
}


void Game::CreateObject(eObjectType type, int x, int y,eDirection d)
{
	assert(type >= e_obstacle && type <=  e_predator);

//verify that no collision between obstacles and actors

	for(list<Obstacle*>::iterator ito=m_obstacles.begin(); ito != m_obstacles.end(); ito++)
	{
		Square s=(*ito)->GetPosition();
		if( s.x==x && s.y==y)
			return;
	}

	for(list<Actor*>::iterator it=m_actors.begin(); it != m_actors.end(); it++)
	{
		Square s=(*it)->GetPosition();
		if( s.x==x && s.y==y)
			return;
	}

	if(type == e_obstacle)
	{
		m_obstacles.push_back(new Obstacle(x,y));
	}
	else if(type == e_prey)
	{
		 m_actors.push_back(new Prey(x,y, d,&m_board));
	}
	else if(type == e_predator)
	{
		 m_actors.push_back(new Predator(x,y, d,&m_board));
	}

	m_board.SetContentAt(type, x,y);

}


void Game::DestroyPreyAt(Square s) 
{
	int x=s.x;
	int y=s.y;
	
	if (	x >= Board::SizeX()
		||	y >= Board::SizeY()
		|| x < 0 || y < 0 )
	{
		// invalid index
		return;
	}

	m_board.ClearContentAt(x,y);

	
	Square so=Square::Invalid;
	
	list<Actor*>::iterator it;
	//find the actor located at this square and destroy it.
	for(it=m_actors.begin(); it != m_actors.end(); it++)
	{
		so=(*it)->GetPosition();
		if( s==so)
		{
			break;
		}
	}

	if( s==so)
	{
		Actor *p =*it;
		assert(p->GetType() == e_prey);
		
		m_actors.erase(it);
		
		delete p;
	}
	else
		assert(0);//prey not found. better verify that is doesn't exist in eat behavior, and react appropriately.

}


Game::~Game()
{
	for(list<Obstacle*>::iterator ito=m_obstacles.begin(); ito != m_obstacles.end(); ito++)
	{
		Obstacle *a=*ito;
		delete a;
	}

	for(list<Actor*>::iterator it=m_actors.begin(); it != m_actors.end(); it++)
	{
		Actor *a=*it;
		delete a;

	}

	m_obstacles.clear();
	m_actors.clear();
}

