#include "../MFCApplication1/stdafx.h"
#include "board.h"
#include "objects\actor.h"

Board::stConfiguration Board::ms_Configuration;

void Board::SetActorAt(Actor *a, int x, int y )
{
	ClearContentAt(a->GetPosition()[0],a->GetPosition()[1]) ;
	SetContentAt(a->GetType(),x,y) ;
}

eObjectType Board::GetContentAt(int x, int y) 
{
	int sizeY = Board::SizeY();

	assert(x>=0 && x<Board::SizeX() && 
		y>=0 && y<sizeY);

	return m_grid[x*sizeY+y];
}


void Board::SetContentAt(eObjectType o, int x, int y) 
{
	int sizeY = Board::SizeY();

	assert(x>=0 && x<Board::SizeX() && 
		y>=0 && y<sizeY );
	assert(o>=e_empty && o <=e_predator);

	m_grid[x*sizeY+y]=o;
}

void Board::ClearContentAt(int x, int y) 
{
	int sizeY = Board::SizeY();

	assert(x>=0 && x<Board::SizeX() && 
		y>=0 && y<sizeY );

	m_grid[x*sizeY+y]=e_empty;
}

//	int bx = Board::SizeX();	int sizeY = Board::SizeY();

Board::Board()
{
	int sizeX =Board::SizeX();
	int sizeY =Board::SizeY();
	
	assert(sizeX>=0 && sizeX<=20 && sizeY>=0 && sizeY<=20);

	m_grid=new eObjectType[sizeof(char)*sizeX*sizeY];
	memset(m_grid,0,sizeof(char)*sizeX*sizeY);
}

Board::~Board()
{
	delete [] m_grid;
}



vector2d Board::GetCenterInPx(Square pos)
{
	
	int CellWidth		= Board::CellWidthInPx();
	int CellHeight		= Board::CellHeightInPx();

	
	return vector2d( CellHeight*pos.x + (CellHeight>>1),
					 CellWidth*pos.y + (CellWidth>>1));
}

Square Board::GetSquareFromLogPx(vector2d p)
{
	return Square(p.x/Board::CellHeightInPx(), p.y/Board::CellWidthInPx());
}

void Board::GetSquareCornersInLogPx(Square o, vector2d  *l_corners)
{
int CellHeight=Board::CellHeightInPx();
int CellWidth=Board::CellWidthInPx();

	l_corners[0]=vector2d( o.x    * CellHeight,  o.y    * CellWidth);
	l_corners[1]=vector2d((o.x+1) * CellHeight,  o.y    * CellWidth);
	l_corners[2]=vector2d((o.x+1) * CellHeight, (o.y+1) * CellWidth);
	l_corners[3]=vector2d( o.x    * CellHeight, (o.y+1) * CellWidth);
}
