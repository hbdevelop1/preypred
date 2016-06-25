
#include "Predator.h"
#include "Debugger.h"
#include "FSM.h"
#include <assert.h>

#include "math\vector2d.h"
#include "math\matrix2d.h"

#include "math\xlines.h"
#include "math\line2d.h"

#include "pathfinding\SquareLine.h"






/* test cases :
1-make sure the generated line is correct.
2-make sure the distance from a line is correct
2.1-when a point belongs to it
2.2-when it 2 units away from it on one side
2.3-when it 2 units away from it on the other side
*/
void Predator::test_line2d()
{
	line2d line1(vector2d(1,3),vector2d(-1,1));
	line2d line2(vector2d(-1,1),vector2d(1,3));

	vector2d v0(1,2);
	vector2d v1(-1,2);
	vector2d v2(2,4);

	double dv0l2 = line2.distanceOf(v0);
	double dv0l1 = line1.distanceOf(v0);
	double dv1l2 = line2.distanceOf(v1);
	double dv1l1 = line1.distanceOf(v1);
	double dv2l2 = line2.distanceOf(v2);
	double dv2l1 = line1.distanceOf(v2);
}




/* test cases :
1-no prey on map
2-one prey
3-one prey, then one appears suddenly 
3.1-farther than the first
3.2-nearer than the first
4-memory:
4.1-prey is lost long before we arrive to it.
4.2-prey is lost just before we arrive to it.
3.3-nearer than the first
*/
void Predator::test_SearchAndChase()
{
	switch(m_testvalue)
	{
	case 1:
/*
<root>
<periode value="0.5"/>
<size value="16 16"/>
<actor type="Predator" position="5 1" direction="1"/>
</root>
	*/
	SearchAndChase();

		break;
	case 2:
	{
		//prey doesn't move
	/*
<root>
<periode value="0.5"/>
<size value="16 16"/>
<actor type="Predator" position="5 1" direction="1"/>
<actor type="Prey" position="5 8" direction="1"/>
</root>
	*/
	SearchAndChase();
	}
		break;
	case 3:
	{
		//prey doesn't move
	/*
<root>
<periode value="0.5"/>
<size value="16 16"/>
<actor type="Predator" position="5 1" direction="1"/>
<actor type="Prey" position="5 8" direction="1"/>
</root>
	*/
		//manually modify the board to add a new prey
	SearchAndChase();
	}
		break;
	case 4:
	{
		//prey doesn't move
	/*
<root>
<periode value="0.5"/>
<size value="16 16"/>
<actor type="Predator" position="5 1" direction="1"/>
<actor type="Prey" position="5 8" direction="1"/>
</root>

<root>
<HearingRange 	value="2"/>
<Vision  angle="70" range="6"/>
<NbrOfFramesBeforeUpdate 	value="2"/>
<MemoryTime 	value="2"/>
</root>
		*/
		//manually modify the board to remove the prey
	SearchAndChase();
	}
		break;

	case 5:
	{
		//prey doesn't move
	/*
<root>
<periode value="0.5"/>
<size value="16 16"/>
<actor type="Predator" position="5 1" direction="1"/>
<actor type="Prey" position="5 8" direction="1"/>
</root>

<root>
<HearingRange 	value="2"/>
<Vision  angle="70" range="6"/>
<NbrOfFramesBeforeUpdate 	value="2"/>
<MemoryTime 	value="2"/>
</root>
		*/
		//manually modify the board to move the prey one square
	SearchAndChase();
	}
		break;
	default:
		break;
	}
}



void Predator::test()
{
//	test_GetSquaresInViewingTriangle2();
//	test_line2d();
//	test_SquareLine();
//	m_path.test	();
//	m_visibility.test();


	m_testvalue=2;
	test_SearchAndChase();



}





void matrix_test()
{
	/*
	Move *m;
	for(list<Node*>::iterator itl = m_l.begin(); itl != m_l.end(); itl++)
	{
		Node * n=*itl;
		m = dynamic_cast<Move*>(n);
		if(m != NULL)
		{
			Square s=m->s;
		}
		else
		{
			Orientation *d1 = dynamic_cast<Orientation*>(*itl);
			int s=d1->d;
		}
	}
	*/

	extern double one_over_sqrt_2;

//	vector2d v0(0,0);
	vector2d v1(0,1);
	vector2d v2(0,-1);
	vector2d v3(1,0);
	vector2d v4(one_over_sqrt_2,one_over_sqrt_2);
	vector2d v5(one_over_sqrt_2,-one_over_sqrt_2);
	vector2d v6(-1,0);
	vector2d v7(-one_over_sqrt_2,one_over_sqrt_2);
	vector2d v8(-one_over_sqrt_2,-one_over_sqrt_2);

	vector2d v1p4(matrix2d::rot_P_1PI_over_4*v1);
	vector2d v1p2(matrix2d::rot_P_2PI_over_4*v1);
	vector2d v1n4(matrix2d::rot_N_1PI_over_4*v1);
	vector2d v1n2(matrix2d::rot_N_2PI_over_4*v1);

	vector2d v2p4(matrix2d::rot_P_1PI_over_4*v2);
	vector2d v2p2(matrix2d::rot_P_2PI_over_4*v2);
	vector2d v2n4(matrix2d::rot_N_1PI_over_4*v2);
	vector2d v2n2(matrix2d::rot_N_2PI_over_4*v2);

	vector2d v3p4(matrix2d::rot_P_1PI_over_4*v3);
	assert(vector2d(one_over_sqrt_2,one_over_sqrt_2)==v3p4);
	
	vector2d v3p2(matrix2d::rot_P_2PI_over_4*v3);
	assert(vector2d(0,1)==v3p2);
	
	vector2d v3n4(matrix2d::rot_N_1PI_over_4*v3);
	assert(vector2d(one_over_sqrt_2,-one_over_sqrt_2)==v3n4);

	vector2d v3n2(matrix2d::rot_N_2PI_over_4*v3);
	assert(vector2d(0,-1)==v3n2);


	vector2d v4p4(matrix2d::rot_P_1PI_over_4*v4);
	assert(vector2d(0,1)==v4p4);

	vector2d v4p2(matrix2d::rot_P_2PI_over_4*v4);
	assert(vector2d(-one_over_sqrt_2,one_over_sqrt_2)==v4p2);

	vector2d v4n4(matrix2d::rot_N_1PI_over_4*v4);
	assert(vector2d(1,0)==v4n4);

	vector2d v4n2(matrix2d::rot_N_2PI_over_4*v4);
	assert(vector2d(one_over_sqrt_2,-one_over_sqrt_2)==v4n2);


	vector2d v5p4(matrix2d::rot_P_1PI_over_4*v5);
	assert(vector2d(1,0)==v5p4);

	vector2d v5p2(matrix2d::rot_P_2PI_over_4*v5);
	assert(vector2d(one_over_sqrt_2,one_over_sqrt_2)==v5p2);

	vector2d v5n4(matrix2d::rot_N_1PI_over_4*v5);
	assert(vector2d(0,-1)==v5n4);

	vector2d v5n2(matrix2d::rot_N_2PI_over_4*v5);
	assert(vector2d(-one_over_sqrt_2,-one_over_sqrt_2)==v5n2);


	vector2d v6p4(matrix2d::rot_P_1PI_over_4*v6);
	assert(vector2d(-one_over_sqrt_2,-one_over_sqrt_2)==v6p4);

	vector2d v6p2(matrix2d::rot_P_2PI_over_4*v6);
	assert(vector2d(0,-1)==v6p2);

	vector2d v6n4(matrix2d::rot_N_1PI_over_4*v6);
	assert(vector2d(-one_over_sqrt_2,one_over_sqrt_2)==v6n4);

	vector2d v6n2(matrix2d::rot_N_2PI_over_4*v6);
	assert(vector2d(0,1)==v6n2);


	vector2d v7p4(matrix2d::rot_P_1PI_over_4*v7);
	assert(vector2d(-1,0)==v7p4);

	vector2d v7p2(matrix2d::rot_P_2PI_over_4*v7);
	assert(vector2d(-one_over_sqrt_2,-one_over_sqrt_2)==v7p2);

	vector2d v7n4(matrix2d::rot_N_1PI_over_4*v7);
	assert(vector2d(0,1)==v7n4);

	vector2d v7n2(matrix2d::rot_N_2PI_over_4*v7);
	assert(vector2d(one_over_sqrt_2,one_over_sqrt_2)==v7n2);


	vector2d v8p4(matrix2d::rot_P_1PI_over_4*v8);
	assert(vector2d(0,-1)==v8p4);

	vector2d v8p2(matrix2d::rot_P_2PI_over_4*v8);
	assert(vector2d(one_over_sqrt_2,-one_over_sqrt_2)==v8p2);

	vector2d v8n4(matrix2d::rot_N_1PI_over_4*v8);
	assert(vector2d(-1,0)==v8n4);

	vector2d v8n2(matrix2d::rot_N_2PI_over_4*v8);
	assert(vector2d(-one_over_sqrt_2,one_over_sqrt_2)==v8n2);









}
