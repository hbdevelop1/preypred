
#if !defined(_BOARD_INCLUDED_)
#define _BOARD_INCLUDED_


#include <assert.h>
#include <string.h>
#include "common.h"
#include "loader.h"
#include "math\vector2d.h"



class Actor;

class Board
{

private:
	eObjectType	*m_grid;

private:
	Board(const Board &);
	Board& operator=(const Board &);


public:
	Board();
	~Board();

	eObjectType		GetContentAt(int x, int y) ;
	eObjectType		GetContentAt(Square s)
	{
		return GetContentAt(s.x,s.y);
	}

	void			SetContentAt(eObjectType o, int x, int y) ;
	void			SetActorAt(Actor *a, int x, int y );
	void			ClearContentAt(int x, int y) ;
	vector2d		GetCenterInPx(Square pos);

	eObjectType operator[](int k)
	{
		assert(k>=0 && k<ms_Configuration.sizeX*ms_Configuration.sizeY);
		return m_grid[k];
	}

	Square GetSquareFromLogPx(vector2d p);
	void GetSquareCornersInLogPx(Square o, vector2d *l_corners);




private:
	struct stConfiguration
	{
		int sizeX;
		int sizeY;
		int cellWidthInPx;
		int cellHeightInPx;
	} ;
	static  stConfiguration ms_Configuration;

public:
	static int  SizeX ()			{return ms_Configuration.sizeX;}
	static int  SizeY ()			{return ms_Configuration.sizeY;}
	static int	CellWidthInPx()		{return ms_Configuration.cellWidthInPx;}
	static int	CellHeightInPx()	{return ms_Configuration.cellHeightInPx;}

	friend bool Loader::LoadMap(char *);
	friend bool Loader::SetSize(char *directory);
};




#endif //!defined(_BOARD_INCLUDED_)