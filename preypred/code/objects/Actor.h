
#if !defined(ACTOR__INCLUDED_)
#define ACTOR__INCLUDED_

#include "..\square.h"
#include "..\board.h"
#include "..\Visibility.h"

#include <list>
using namespace std;


#include "hobject.h"
/*
class Situation
{
private:
	eDirection		dir;
	int				position[2];

public:

	Situation()
	{
		dir=e_left;
		position[0]=0,position[1]=0;
	}

	Situation(eDirection d):dir(d)
	{
		position[0]=0,position[1]=0;
	}
	Situation(int x, int y, eDirection d=e_left):dir(d)
	{
		position[0]=x,position[1]=y;
	}
	Situation(int *p)
	{
		position[0]=p[0],position[1]=p[1];
	}
	Situation(Square s)
	{
		position[0]=s.x,position[1]=s.y;
	}
	Situation(const Situation & s):dir(s.dir)
	{
		position[0]=s.position[0];
		position[1]=s.position[1];
	}
	void operator=(int *p)
	{
		position[0]=p[0],position[1]=p[1];
	}
	Square GetPosition()
	{
		return Square(position[0],position[1]);
	}
	eDirection & GetDirection()
	{
		return dir;
	}
	void SetPosition(int x, int y)
	{
		position[0]=x;
		position[1]=y;
	}
};
*/
class Actor : public HObject
{
public:

	class SituationA : public HObject::Situation
	{
		eDirection	m_dir;
	public:
		SituationA(Square pos=Square::Invalid, eDirection d=e_left):HObject::Situation(pos),m_dir(d){}
		SituationA(int x, int y, eDirection d=e_left):HObject::Situation(x,y),m_dir(d){}

		void SetPosition(int x,int y);
		eDirection	& GetDirection(){return m_dir;}

	};
private:
	SituationA 			m_sit;
	string				m_infotext;
	
public:
	Actor(eObjectType type, int _iX, int _iY, eDirection d, Board *b, int vr, int va);
	virtual ~Actor(){}
	
	eDirection	GetDirection(){return m_sit.GetDirection();}
	void SetPosition( int _iX, int _iY );
	void GetPosition( int& _iX, int& _iY ) ;//const;
	void SetDirection( eDirection _dir );
	Square GetNearestObject(list<Square> & l);

	
	Square GetPosition();
	void SetSituation(int _iX, int _iY, eDirection _dir);
	void SetSituation(SituationA sit) ;
	SituationA GetSituation() ;

	virtual void Update()=0;

	string		GetInfoText(){return m_infotext;}

protected:
	Board							*m_board;

	Visibility						m_visibility;
	void UpdateVision() {m_visibility.Update();}
	


	void					SetInfoText(string s);

};



#endif //!defined(ACTOR__INCLUDED_)