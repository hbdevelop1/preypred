
#include "../../MFCApplication1/stdafx.h"
#include "actor.h"


Actor::Actor(eObjectType type, int _iX, int _iY, eDirection d, Board *b,int vr, int va):
	HObject(type), m_sit(_iX,_iY,d),m_board(b)
	,m_visibility(this,b, vr, va)
{
//	m_visibility.SetParent(this);
	UpdateVision();

}


void Actor::SetPosition( int _iX, int _iY )
{
	m_board->SetActorAt(this,_iX, _iY);

	m_sit.SetPosition(_iX,_iY);


	UpdateVision();


}


//compile this SetPosition
void Actor::SituationA::SetPosition(int x,int y)  
{
	Situation::SetPosition(Square(x,y));

}


void Actor::GetPosition( int& _iX, int& _iY ) //const
{
	Square s(m_sit.GetPosition());
	_iX = s[0];
	_iY = s[1];
}


void Actor::SetDirection( eDirection _dir )
{
	m_sit.GetDirection() = _dir;
	UpdateVision();
}

void Actor::SetInfoText(string s)
{
	m_infotext=s;

}

Square Actor::GetNearestObject(list<Square> & l)
{
	Square pos = GetPosition();
	list<Square>::iterator it=l.begin();

	Square nearest=*it;
	
	double mindist = distance_fake((*it),pos);
	double dist;

	for(; it!=l.end(); it++)
	{
		dist =distance_fake((*it),pos);
		if(dist<mindist)
		{
			nearest=*it;
			mindist	=dist;
		}
	}
	
	return nearest;
}


Square Actor::GetPosition()
{
	return m_sit.GetPosition();
}
void Actor::SetSituation(int _iX, int _iY, eDirection _dir) 
{ 
	m_sit.GetDirection() = _dir;
	m_sit.SetPosition(_iX,_iY);
}

void Actor::SetSituation(SituationA sit) 
{
	m_sit=sit;
}

Actor::SituationA Actor::GetSituation() 
{
	return m_sit;
}
