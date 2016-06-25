



#include "..\Square.h"
#include "..\math\vector2d.h"
#include "..\math\Segment.h"
#include <list>

class Board;


class SquareLine
{
private:
	Segment				sg;
	Square				s_cur;
	Square				s_goal;
	vector2d			p_lastipoint;
	Board				*board;
	std::list<Square>	*pl_squares;



public:
	SquareLine() {}

public:
	bool CanSee(Square b, Square e, Board *);
private:
	bool CanSee();


public:
	Square GetAdjacentSquareTowards(Square s_from, Square s_to,Board *);
public:
	void GetALine(vector2d p1, vector2d p2, Square startings,Board	*,std::list<Square> &);
private:
	void GetALine();
};