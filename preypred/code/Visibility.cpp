#include "../MFCApplication1/stdafx.h"

#include "Visibility.h"

#include "math\matrix2d.h"
#include "pathfinding\SquareLine.h"
#include "objects\actor.h"
#include "Debugger.h"

#define _debug_visibility_




Visibility::Visibility(Actor * parent,Board *board, int vr, int va):
	m_parent(parent),m_board(board),m_VisionRange(vr),m_VisionAngleOver2(va/2)
{
}





void Visibility::Update()
{

	int d = m_parent->GetDirection();

	vector2d v;
	int hpx = m_VisionRange*Board::CellHeightInPx();
	int wpx = m_VisionRange*Board::CellWidthInPx();

	if(d==e_right)
	{
		v=vector2d(0,1)*wpx;
	}
	else if(d==e_left)
	{
		v=vector2d(0,-1)*wpx;
	}
	else if(d==e_up)
	{
		v=vector2d(-1,0)*hpx;
	}
	else if(d==e_down)
	{
		v=vector2d(1,0)*hpx;
	}


	double a_rad = M_PI / 180 * m_VisionAngleOver2 ;

	//todo : use vector interpolation is case wpx != hpx
	matrix2d m1(cos(a_rad), -sin(a_rad), sin(a_rad), cos(a_rad));
	m_directionVector1=m1 * v;
	
	a_rad*=-1 ; matrix2d m2(cos(a_rad), -sin(a_rad), sin(a_rad), cos(a_rad));
	m_directionVector2=m2 * v;
	

			
	vector2d p_center_pdrt = m_board->GetCenterInPx(m_parent->GetPosition());
	//line 1
	vector2d p1=p_center_pdrt +m_directionVector1;
	m_line1=line2d(p_center_pdrt,p1);
	//line 2
	vector2d p2=p_center_pdrt +m_directionVector2;
	m_line2=line2d(p_center_pdrt,p2);
		

}



void Visibility::GetSquaresInViewingTriangle(list<Square> & l_squaresintriangle)
{
	int pdtr_dir = m_parent->GetDirection();
	assert( pdtr_dir>=e_left 
		&& pdtr_dir<= e_down);


	int hpx = m_VisionRange*Board::CellHeightInPx();
	int wpx = m_VisionRange*Board::CellWidthInPx();
	double range_fake;
	
	
	vector2d pdtr_dir_v;
	vector2d dir_v1, dir_v2;
	
	if(pdtr_dir==e_up || pdtr_dir==e_down)
	{
		range_fake  = hpx*hpx;
		
		dir_v1=vector2d(0,1);
		dir_v2=vector2d(0,-1);

		if(pdtr_dir==e_up)
		{
			pdtr_dir_v=vector2d(-1,0);
		}
		else
		{
			pdtr_dir_v=vector2d(1,0);
		}
		
	}
	else //if(pdtr_dir==DIR_LEFT || pdtr_dir==DIR_RIGHT)
	{
		range_fake  = wpx*wpx;
		
		dir_v1=vector2d(1,0);
		dir_v2=vector2d(-1,0);

		if(pdtr_dir==e_right)
		{
			pdtr_dir_v=vector2d(0,1);
		}
		else
		{
			pdtr_dir_v=vector2d(0,-1);
		}
	}

	Square pdtr_square = m_parent->GetPosition();
	Square s=pdtr_square;
	vector2d p_center_prdt=m_board->GetCenterInPx(pdtr_square);

	vector2d p_center_s,p_center_ss;
	Square ss;
	

	//for each considered square, see if its center is not out of range and is between lines;
	bool outofrange=false;
	do
	{
		s=s+pdtr_dir_v;

		//1-is the front square is in range. if it's not, no need to see squares on the right and the left.
		if(NotOutOfBoard(s)==false)
		{
			outofrange=true;
			break;
		}

		p_center_s = m_board->GetCenterInPx(s);
		if(distance_fake(p_center_s,p_center_prdt)>range_fake)
		{
			outofrange=true;
			break;
		}

		l_squaresintriangle.push_back(s);


		double dist1_ss,dist2_ss ,dist1_s,dist2_s;
		int i;
		bool outofrange = false;
		//2-see for the neighbooring squares on one side
		//2.1-get its distance (and the sign) from the line
		dist1_s =m_line1.distanceOf(p_center_s);
		dist2_s =m_line2.distanceOf(p_center_s);
		
		i=0;
		bool sideoutofrange=false;
		do
		{
			i++;
			assert(i<16);
			//3-for each square neighbooring square
			ss=s+dir_v1*i;
			if(NotOutOfBoard(ss)==false)
			{
				sideoutofrange = true;
				break; //gone past a border
			}
				
			p_center_ss = m_board->GetCenterInPx(ss);
			if(distance_fake(p_center_ss,p_center_prdt)<range_fake)
			{
				dist1_ss =m_line1.distanceOf(p_center_ss);
				dist2_ss =m_line2.distanceOf(p_center_ss);
				if(SAME_SIGNS(dist1_ss,dist1_s ) && SAME_SIGNS(dist2_ss,dist2_s ))
				{
				 	l_squaresintriangle.push_back(ss);
				}
				else
				{
					sideoutofrange = true;
					break; //gone past the line 
				}
			}
			else
			{
				sideoutofrange = true;
				break; //gone past the line 
			}
		}
		while (!sideoutofrange);

		//3-see for the neighbooring squares on the other side
		//3.1-get its distance (and the sign) from the line
		i=0;
		sideoutofrange=false;
		do
		{
			i++;
			assert(i<16);
			//3-for each square neighbooring square
			ss=s-static_cast<Square>(dir_v1*i);
			if(NotOutOfBoard(ss)==false)
			{
				sideoutofrange = true;
				break; //gone past a border
			}
				
			p_center_ss = m_board->GetCenterInPx(ss);
			if(distance_fake(p_center_ss,p_center_prdt)<range_fake)
			{
				dist1_ss =m_line1.distanceOf(p_center_ss);
				dist2_ss =m_line2.distanceOf(p_center_ss);
				if(SAME_SIGNS(dist1_ss,dist1_s ) && SAME_SIGNS(dist2_ss,dist2_s ))
				{
				 	l_squaresintriangle.push_back(ss);
				}
				else
				{
					sideoutofrange = true;
					break; //gone past the line 
				}
			}
			else
			{
				sideoutofrange = true;
				break; //gone past the line 
			}
		}
		while (!sideoutofrange);
	}while(!outofrange);
	
}



void Visibility::GetSeenObjects(eObjectType ot,list<Square> & l_seen)
{
	list<Square> l_seensquares;
	GetSeenSquares(l_seensquares);

#if defined(_debug_visibility_)
	char g_debug_GetSeenObjects=0;
	if(g_debug_GetSeenObjects)
	{
		Debugger::GetInstance().Output("-----\n");

		list<Square>::iterator it;
		for(it=l_seensquares.begin(); it!=l_seensquares.end(); it++)
		{
			Debugger::GetInstance().Output("(%d,%d)\n",(*it).x,(*it).y);
		}		
	}
#endif //defined(_debug_visibility_)

	list<Square>::iterator it;
	for(it=l_seensquares.begin(); it!=l_seensquares.end(); it++)
	{
		if(m_board->GetContentAt(*it) == ot)
		{
			l_seen.push_back(Square(it->x, it->y));
		}
	}

}

void Visibility::GetSeenSquares(list<Square> & l_squares)
{

	Square s_prdt=m_parent->GetPosition();

	list<Square> l_squaresintriangle;
	GetSquaresInViewingTriangle(l_squaresintriangle);

	for(list<Square>::iterator it=l_squaresintriangle.begin(); it!=l_squaresintriangle.end(); it++)
	{
		SquareLine sl;
		if(sl.CanSee(s_prdt, *it,m_board))
		{
			l_squares.push_back(*it);
		}
	}

}

