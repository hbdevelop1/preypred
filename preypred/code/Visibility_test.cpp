

#include "Visibility.h"

#include "math\matrix2d.h"
#include "pathfinding\SquareLine.h"
#include "objects\actor.h"





/* test cases :
1-test normal situation
2-test extreem situations:
2.1-place actor paralelle at the borders.
2.2-place actor at the border facing outward and inward
2.3-place actor at the corner facing outward and inward
  */
void Visibility::test_GetSquaresInViewingTriangle()
{
	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_left);
	m_parent->SetPosition(7,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);
	
	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	
	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_left);
	m_parent->SetPosition(0,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	//i discovered that the centeral line was not inserted in l_squaresintriangle.
	//i discovered that i had to do an "else outofrange;" for "if(distance_fake(p_center_ss,p_center_prdt)<range_fake)"
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_left);
	m_parent->SetPosition(0,0);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_up);
	m_parent->SetPosition(0,0);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_up);
	m_parent->SetPosition(7,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}


	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_up);
	m_parent->SetPosition(7,15);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}


	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_up);
	m_parent->SetPosition(7,0);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_down);
	m_parent->SetPosition(7,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_down);
	m_parent->SetPosition(15,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_down);
	m_parent->SetPosition(7,15);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_down);
	m_parent->SetPosition(7,0);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}



	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_right);
	m_parent->SetPosition(7,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_right);
	m_parent->SetPosition(7,15);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_right);
	m_parent->SetPosition(0,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

	{
	list<Square> l_squaresintriangle;

	m_parent->SetDirection(e_right);
	m_parent->SetPosition(15,7);
	GetSquaresInViewingTriangle(l_squaresintriangle);

	list<Square>::iterator it;
	Square g;
	for(it=l_squaresintriangle.begin(); it != l_squaresintriangle.end(); it++)
	{
		g=(*it);
	}
	}

}


void Visibility::test()
{
	test_GetSquaresInViewingTriangle();
}