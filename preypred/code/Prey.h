
#include "objects\actor.h"
#include "pathfinding\Path.h"
#include "math\matrix2d.h"


class Prey : public Actor
{
	typedef Actor super;

private:

	void (Prey::*m_currentbehavior)(void) ;

	enum
	{
		e_idle,
		e_init,
		e_flee,

	};

	int					m_state;

	void Behavior_Idle();
	void Behavior_Flee();
	void Behavior_Hide();

	void DoBehavior();


public:
	bool		IsAnyPredatorHeard();
	Square		GetPosAwayFrom(Square prdtrPos);
	void		GetAWayAround(Square o, Square p, list<Situation> & l_path);
	vector2d	GetTheNearestVectorTo(vector2d theidealdestinationVector);
	void		Walk(bool toARefuge);
	vector2d	GetVectorAwayFromAll(list<Square> & l_prdt);
	Square		GetNearestObject(Square s1, Square s2);
	void		FindARefuge();
	void		FindARefuge(Square o, Path *ppath);
	bool		IsAValidRefuge(Square refuge);
	void		GetHeardPrdt(list<Square> & l_prdt);

private:
	void test();
	void test_FindARefuge();
	void test_FindARefuge(int);
	void test_GetVectorAwayFromAll();
	void test_GetAStraightPathInDirection();
	void test_detection();
	void test_SquareLine_GetALine();
	void test_getangle2();


private:
	int					m_timetowait;

	Square				m_PredatorHeardAt;
	Path				m_path;



	struct RefugeData
	{
		int		orientation;
		Square	s_front;
		Square	s_blocking;
		Square	s_start; //to avoid loop indefinitely over a wall.
							//initially s_start=s_front. when s_front is once again equal 
						//to s_start, this means that we searched over the whole wall and could find no refuge.
						//stop searching then, and continue fleeing.

		list<Square>	l_squares[2];
	}m_refugedata;


	Square		m_refuge;
	vector2d	m_fleeingvector;
	double		m_fleeingvectorangle;

	int			m_hidingtime;//the time amount we are hiding from predators. 
							//when this time == HidingTime from configuration, we're free.
	int			m_fleeingwithoutdangertime;//time amount we're running from predator without seeing or hearing 
											//any predator. we'll stop running when this time is high. 



	void GetAnyPath();



public:
	Prey(int _iX, int _iY, eDirection d, Board *b);

	void Update();

///// configuration
private:
	struct stConfiguration
	{
		int HearingRange;
		int NbrOfFramesBeforeUpdate;//MovementSpeed;
		int NbrOfFramesHiding;
		int NbrOfFramesInFleeWithoutDanger;
		
		struct
		{
			int		range;
			int		angle;

		}	Vision;

	} ;
	static  stConfiguration ms_Configuration;
public:
	static int & HearingRange () {return ms_Configuration.HearingRange;}
	static int & VisionRange () {return ms_Configuration.Vision.range;}
	static int & VisionAngle () {return ms_Configuration.Vision.angle;}
	static int & MovementSpeed () {return ms_Configuration.NbrOfFramesBeforeUpdate;}
	static int & HidingTime()	{return ms_Configuration.NbrOfFramesHiding;}
	static int & InFleeWithoutDangerTime() {return ms_Configuration.NbrOfFramesInFleeWithoutDanger;}

};