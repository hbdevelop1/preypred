
#include "Board.h"
#include <list>
#include "Square.h"
using namespace std;

#include "common.h"

#include "objects\hobject.h"




class Game
{
	enum eState
	{
		e_stopgame=1,
		e_playgame,
	};

private:
	Game(const Game &);
	Game & operator=(const Game &);
private:

	Board		m_board;
	eState		m_state;

	list<Obstacle *> 	m_obstacles;
	list<Actor *> 		m_actors;


public:
	Game();
	~Game();

	static Game & GetInstance()
	{
		static Game inst;
		return inst;
	}
	void Stop() {m_state=e_stopgame;}
	eState GetState(){return m_state;}
	void	Loop ()	;

	static UINT GameLoop (LPVOID pParam); //A thread function is a callback function, so it must be either a static class member function or a global function declared outside a class. 

	void CreateObject(eObjectType type, int x, int y,eDirection d=e_left);
	void DestroyPreyAt(Square s) ;

	Board * GetBoard(){return  &m_board;}
	list<Obstacle *> 		* GetObstacles() {return &m_obstacles;}
	list<Actor *> 		* GetActors() {return &m_actors;}

};






Game *GetGame();


