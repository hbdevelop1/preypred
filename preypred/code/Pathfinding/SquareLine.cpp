#include "../../MFCApplication1/stdafx.h"

#include "SquareLine.h"
#include "..\Board.h"
#include "..\math\xlines.h"
#include "..\Debugger.h"
#include <list>


/*
Can we see s2 from s1 ?
*/
bool SquareLine::CanSee(Square s1, Square s2, Board *w)
{    
	s_cur=s1;
	s_goal=s2;
	p_lastipoint=vector2d::Invalid;
	board=w;
	sg.p0=board->GetCenterInPx(s1);
	sg.p1=board->GetCenterInPx(s2);

#if defined(_showdebugger_)
//	Debugger::GetInstance().Output("from (%d,%d) to (%d,%d)\n",s_cur.x,s_cur.y,s_goal.x,s_goal.y);
#endif //defined(_showdebugger_)

	bool t = CanSee();

#if defined(_showdebugger_)
//	Debugger::GetInstance().Output("\n");
#endif //defined(_showdebugger_)

	return t;
}



bool SquareLine::CanSee()
{
#if defined(_showdebugger_)
//	Debugger::GetInstance().Output("(%d,%d)  ",s_cur.x,s_cur.y);
#endif //defined(_showdebugger_)

	if(s_cur == s_goal)
		return true;
	if(!NotOutOfBoard(s_cur))//IsAValidRefuge can call CanSee with a square out of board. 
		return true;		//if no obstacle can hide the refuge from the borders of the map, the refuge is exposed.
	else if(board->GetContentAt(s_cur) == e_obstacle)
		return false;

	vector2d l_cornerspos[4]; //corners of a square
	board->GetSquareCornersInLogPx(s_cur,l_cornerspos);
	Segment sg2;
	int i,intersection_value ;
	vector2d ipoint;

	//foreach segment in the square, see intersection;
	for(i=0; i<4; i++)
	{
		if(i+1 == 4)
			sg2=Segment(l_cornerspos[i],l_cornerspos[0]);
		else
			sg2=Segment(l_cornerspos[i],l_cornerspos[i+1]);
		
		intersection_value = lines_intersect(sg,sg2, ipoint);

		ipoint.setint();

		if(intersection_value == 1 && ! (ipoint== p_lastipoint) )
		{
			if( ipoint == l_cornerspos[i] || ipoint == l_cornerspos[i+1])
			{
				vector2d g = sg.p1-ipoint;

				if(g.x>0)
				{
					if(g.y>0)
					{
						s_cur=s_cur+vector2d(1,1);
					}
					else if(g.y<0)
					{
						s_cur=s_cur+vector2d(1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else if(g.x<0)
				{
					if(g.y>0)
					{
						s_cur=s_cur+vector2d(-1,1);
					}
					else if(g.y<0)
					{
						s_cur=s_cur+vector2d(-1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else
					assert(0); //this means that the line coincides with the y-axis. and this is not possible since the destination point is a square center.
			}
			else if(i==0)
			{
				s_cur=s_cur+vector2d(0,-1);
			}
			else if(i==1)
			{
				s_cur=s_cur+vector2d(1,0);
			}
			else if(i==2)
			{
				s_cur=s_cur+vector2d(0,1);
			}
			else
			{
				s_cur=s_cur+vector2d(-1,0);
			}

			p_lastipoint=ipoint;

			break;
		}
	}

	return CanSee();
}














/*
the first square should be hit only one time by the line.
so it's important to set p1 as the center of the first square.
*/
void SquareLine::GetALine(vector2d p1, vector2d p2, Square startings,Board	*w, std::list<Square> & l_squares)
{
	if(!NotOutOfBoard(startings) || w->GetContentAt(startings) ==  e_obstacle)
		return;


	sg=Segment(p1, p2);
	s_cur=startings;
	p_lastipoint=vector2d::Invalid;
	pl_squares = &l_squares;
	board = w;

	GetALine();

}
/*GetALine follow squares intersecting the segment sg, until it reaches : 
-the border of the board, 
-an obstacle or 
-a square that insects the segment only one time (the segment can't go beyond it to neigbooring squares) 
*/
void SquareLine::GetALine()
{
	vector2d l_cornerspos[4]; //corners of a square
	board->GetSquareCornersInLogPx(s_cur,l_cornerspos);
	Segment sg2;
	int i,intersection_value ;
	vector2d ipoint;

	//foreach segment in the square, see intersection;
	for(i=0; i<4; i++)
	{
		if(i+1 == 4)
			sg2=Segment(l_cornerspos[i],l_cornerspos[0]);
		else
			sg2=Segment(l_cornerspos[i],l_cornerspos[i+1]);
		
		intersection_value = lines_intersect(sg,sg2, ipoint);

		ipoint.setint();

		if(intersection_value == 1 && ! (ipoint== p_lastipoint) )
		{
			if( ipoint == l_cornerspos[i] || ipoint == l_cornerspos[i+1])
			{
				vector2d g = sg.p1-ipoint;
				Square s;

				if(g.x>0)
				{
					if(g.y>0)
					{
						s=s_cur+vector2d(1,0);
						if(board->GetContentAt(s) ==  e_obstacle)
						{
							s=s_cur+vector2d(0,1);
							if(board->GetContentAt(s) ==  e_obstacle)
							{
								return;
							}
							else
							{
								pl_squares->push_back(s);
							}
						}
						else
						{
							pl_squares->push_back(s);
						}

						s_cur=s_cur+vector2d(1,1);
					}
					else if(g.y<0)
					{
						s=s_cur+vector2d(1,0);
						if(board->GetContentAt(s) ==  e_obstacle)
						{
							s=s_cur+vector2d(0,-1);
							if(board->GetContentAt(s) ==  e_obstacle)
							{
								return;
							}
							else
							{
								pl_squares->push_back(s);
							}
						}
						else
						{
							pl_squares->push_back(s);
						}

						s_cur=s_cur+vector2d(1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else if(g.x<0)
				{
					if(g.y>0)
					{
						s=s_cur+vector2d(-1,0);
						if(board->GetContentAt(s) ==  e_obstacle)
						{
							s=s_cur+vector2d(0,1);
							if(board->GetContentAt(s) ==  e_obstacle)
							{
								return;
							}
							else
							{
								pl_squares->push_back(s);
							}
						}
						else
						{
							pl_squares->push_back(s);
						}
						s_cur=s_cur+vector2d(-1,1);
					}
					else if(g.y<0)
					{
						s=s_cur+vector2d(-1,0);
						if(board->GetContentAt(s) ==  e_obstacle)
						{
							s=s_cur+vector2d(0,-1);
							if(board->GetContentAt(s) ==  e_obstacle)
							{
								return;
							}
							else
							{
								pl_squares->push_back(s);
							}
						}
						else
						{
							pl_squares->push_back(s);
						}
						s_cur=s_cur+vector2d(-1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else
					assert(0); //this means that the line coincides with the y-axis. and this is not possible since the destination point is a square center.
			}
			else if(i==0)
			{
				s_cur=s_cur+vector2d(0,-1);
			}
			else if(i==1)
			{
				s_cur=s_cur+vector2d(1,0);
			}
			else if(i==2)
			{
				s_cur=s_cur+vector2d(0,1);
			}
			else
			{
				s_cur=s_cur+vector2d(-1,0);
			}

			p_lastipoint=ipoint;

			break;
		}
	}

	if(!NotOutOfBoard(s_cur) || board->GetContentAt(s_cur) ==  e_obstacle)
	{
		return;
	}

	if(i==4) 
	{//when i == 4, it's the last square touched by the segment sg.
		return;
	}

	pl_squares->push_back(s_cur);
	
	GetALine();

}

//gets the square next to s_from, in the direction of s_to.
//used by FindARefuge to have a square next to an obstacle.
Square SquareLine::GetAdjacentSquareTowards(Square s_from, Square s_to, Board * board)
{
	vector2d l_cornerspos[4]; //corners of a square
	Square	s_ret=s_from;
	Segment sg2;

	int i,intersection_value ;
	vector2d ipoint;


	board->GetSquareCornersInLogPx(s_from,l_cornerspos);

	
	Segment sg(board->GetCenterInPx(s_from),board->GetCenterInPx(s_to));

	//foreach segment in the square, see intersection;
	for(i=0; i<4; i++)
	{
		if(i+1 == 4)
			sg2=Segment(l_cornerspos[i],l_cornerspos[0]);
		else
			sg2=Segment(l_cornerspos[i],l_cornerspos[i+1]);
		
		intersection_value = lines_intersect(sg,sg2, ipoint);

		ipoint.setint();

		if(intersection_value == 1)
		{
			if( ipoint == l_cornerspos[i] || ipoint == l_cornerspos[i+1])
			{
				vector2d g = sg.p1-ipoint;

				if(g.x>0)
				{
					if(g.y>0)
					{
						s_ret=s_ret+vector2d(1,1);
					}
					else if(g.y<0)
					{
						s_ret=s_ret+vector2d(1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else if(g.x<0)
				{
					if(g.y>0)
					{
						s_ret=s_ret+vector2d(-1,1);
					}
					else if(g.y<0)
					{
						s_ret=s_ret+vector2d(-1,-1);
					}
					else
						assert(0); //this means that the line coincides with the x-axis. and this is not possible since the destination point is a square center.
				}
				else
					assert(0); //this means that the line coincides with the y-axis. and this is not possible since the destination point is a square center.
			}
			else if(i==0)
			{
				s_ret=s_ret+vector2d(0,-1);
			}
			else if(i==1)
			{
				s_ret=s_ret+vector2d(1,0);
			}
			else if(i==2)
			{
				s_ret=s_ret+vector2d(0,1);
			}
			else
			{
				s_ret=s_ret+vector2d(-1,0);
			}


			break;
		}
	}

	return s_ret;
}