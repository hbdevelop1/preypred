

#pragma warning(disable:4786)

#include "../MFCApplication1/stdafx.h"

#include <afxwin.h>
#include "Loader.h"

#include "Board.h"

#include "game.h"

#include "xml\xml.h"

#include <assert.h>
#include <time.h>

#include "Predator.h"
#include "Prey.h"


bool Loader::LoadGame(char *directory)
{
	LoadPredatorConfiguration(directory);
	LoadPreyConfiguration(directory);

	if(!LoadMap(directory))
		return false;

	return true;
}

bool Loader::SetSize(char *directory)
{
	 
	
	String s(directory);
	s+="\\map.xml";

	XmlParser	xmlParser;
	XmlNodeRef rootNode= xmlParser.parse( s);

	
	if (!rootNode) 
	{
		MessageBox(NULL,"Data problem" ,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		assert(0);
	}

	int i;

	//for of all, get the size of the board
	for (i = 0; i < rootNode->getChildCount(); i++) 
	{
		XmlNodeRef child = rootNode->getChild(i);


		if (child->isTag("size"))
		{
			int x,y;

			String size = child->getAttribute("value");
			sscanf(size.c_str(),"%d %d",&x,&y);

			Board::ms_Configuration.sizeX=x;
			Board::ms_Configuration.sizeY=y;
			
			break;
		}
	}

	if(i >= rootNode->getChildCount())
	{
		//size of the board unknown.
		//cannot continue

		MessageBox(NULL,"size of board unknown. cannot continue" ,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		assert(0);


		return false;
	}
	 
	return true;
}

bool Loader::LoadMap(char *directory)
{
	
	String s(directory);
	s+="\\map.xml";

	XmlParser	xmlParser;
	XmlNodeRef rootNode= xmlParser.parse( s);

	
	if (!rootNode) 
	{
		MessageBox(NULL,"Data problem" ,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		assert(0);
	}

	int i;


	for (i = 0; i < rootNode->getChildCount(); i++) 
	{
		XmlNodeRef child = rootNode->getChild(i);

		if (child->isTag("object"))
		{
			String type = child->getAttribute("type");
			String position = child->getAttribute("position");
			String direction = child->getAttribute("direction");

			int x,y,d;
			sscanf(position.c_str(),"%d %d",&x,&y);
			
			if(type == "Predator")
			{
//todo : verify the correctness of direction and position
				sscanf(direction.c_str(),"%d",&d);
				assert(d>=e_left && d<=e_down);
				GetGame()->CreateObject(e_predator,x,y,static_cast<eDirection>(d));
			}
			else if(type == "Prey")
			{
//todo : verify the correctness of direction and position
				sscanf(direction.c_str(),"%d",&d);
				assert(d>=e_left && d<=e_down);
				GetGame()->CreateObject(e_prey,x,y,static_cast<eDirection>(d));
			}
			else if (type == "Obstacle")
			{
				GetGame()->CreateObject(e_obstacle,x,y);
			}
			else
				assert(0);
		}
		else if(child->isTag("CellSizeInPixels"))
		{
			int x,y;

			String size = child->getAttribute("value");
			sscanf(size.c_str(),"%d %d",&x,&y);

			Board::ms_Configuration.cellHeightInPx=x;
			Board::ms_Configuration.cellWidthInPx=y;
		}
		else if (child->isTag("size"))
		{
		//already done.
		}
		else
			assert(0); 
	}
	 
		return true;

}


void Loader::LoadPredatorConfiguration(char *directory)
{

String s(directory);
	s+="\\predator.xml";

	XmlParser	xmlParser;
	XmlNodeRef rootNode= xmlParser.parse( s);

	
	if (!rootNode) 
	{
		MessageBox(NULL,"Data problem" ,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		assert(0);
	}




	for (int i = 0; i < rootNode->getChildCount(); i++) 
	{
		XmlNodeRef child = rootNode->getChild(i);


		if (child->isTag("Vision")) 
		{
			int h;
			String v = child->getAttribute("range");
			sscanf(v.c_str(),"%d",&h);
			Predator::VisionRange() = h;

			String w = child->getAttribute("angle");
			sscanf(w.c_str(),"%d",&h);
			Predator::VisionAngle() = h;
		}
		else if (child->isTag("NbrOfFramesBeforeUpdate")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Predator::MovementSpeed() = h;
		}
		else if (child->isTag("MemoryTime")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Predator::MemoryTime() = h;
		}
	}


}

void Loader::LoadPreyConfiguration(char *directory )
{
	
	String s(directory);
	s+="\\prey.xml";

	XmlParser	xmlParser;
	XmlNodeRef rootNode= xmlParser.parse( s);


	if (!rootNode) 
	{
		MessageBox(NULL,"Data problem.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		assert(0);
	}

	for (int i = 0; i < rootNode->getChildCount(); i++) 
	{
		XmlNodeRef child = rootNode->getChild(i);


		if (child->isTag("HearingRange")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Prey::HearingRange() = h;
		}
		else if (child->isTag("Vision")) 
		{
			int h;
			String v = child->getAttribute("range");
			sscanf(v.c_str(),"%d",&h);
			Prey::VisionRange() = h;

			String w = child->getAttribute("angle");
			sscanf(w.c_str(),"%d",&h);
			Prey::VisionAngle() = h;
		}
		else if (child->isTag("NbrOfFramesBeforeUpdate")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Prey::MovementSpeed() = h;
		}
		else if (child->isTag("NbrOfFramesHiding")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Prey::HidingTime() = h;
		}
		else if (child->isTag("NbrOfFramesInFleeWithoutDanger")) 
		{
			int h;
			String v = child->getAttribute("value");
			sscanf(v.c_str(),"%d",&h);
			Prey::InFleeWithoutDangerTime() = h;
		}
	}
	
}