
#include "objects\actor.h"

#include "pathfinding\Path.h"
#include "math\vector2d.h"
#include "math\line2d.h"

#include "Visibility.h"




class Predator : public Actor
{
	typedef Actor super;

private:

	void (Predator::*m_currentbehavior)(void) ;

	enum
	{
		e_search,
		e_hunt,
		e_eat,

	};

	int					m_state;


	void Behavior_Hunt();
	void Behavior_Eat();
	void Behavior_SearchAndChase();

	void	SearchAndChase();
	void	GetSeenPreys(list<Square> & l_seenpreys);
	void	Walk(bool =true);

	void DoBehavior();

private:
	int					m_timetowait;
	Path				m_path;
	
	void GetAnyPath();
	void GetSeenSquares(list<Square> & l_squaresintriangle);


private:
	void test();
	void test_GetSquaresInViewingTriangle2();
	void test_line2d();
	void test_SquareLine();
	void test_SearchAndChase();
	int		m_testvalue;


private:
	struct stMemory
	{
		int		delay;

		void Reset()
		{
			delay=Predator::MemoryTime();
		}
		void Update()
		{	
			delay--;
		}
		void LooseIt()
		{
			delay=0;
		}
		
		bool IsLost()
		{
			return (delay<=0);
		}
	}	m_memory;


public:
	Predator(int _iX, int _iY, eDirection d, Board *b);

	void Update();




///// configuration
private:
	struct stConfiguration
	{
		int NbrOfFramesBeforeUpdate; //MovementSpeed
		int MemoryTime;
		struct
		{
			int		range;
			int		angle;

		}	Vision;
	} ;
	static  stConfiguration ms_Configuration;
public:
	static int & VisionRange () {return ms_Configuration.Vision.range;}
	static int & VisionAngle () {return ms_Configuration.Vision.angle;}
	static int & MovementSpeed () {return ms_Configuration.NbrOfFramesBeforeUpdate;}
	static int & MemoryTime () {return ms_Configuration.MemoryTime;}
};




