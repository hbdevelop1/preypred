
#if !defined(HOBJECT__INCLUDED_)
#define HOBJECT__INCLUDED_



//Situation  is not generally defined, a benefit is to avoid confusion of Situation : 
//	always specify the scope of Situation :HObject or Actor


class HObject //to have a list of obstacles and actors
{
protected:
	eObjectType			m_type;

	
	
	//protected://no need for it to be public. but microsot compiler refuses protected
public:
	class Situation //to be able to derive from it
	{
	protected:
	private:
		Square				m_pos;
	public:
		Situation(int x, int y)	:m_pos(x,y){}
		Situation(Square pos)	:m_pos(pos){}
		Situation(const Situation & s)
		{
			m_pos=s.m_pos;
		}
		void operator=(const Situation & s)
		{
			m_pos=s.m_pos;
		}

		Square 		GetPosition() {return m_pos;}
		void		SetPosition(Square s){m_pos=s;}
		void		SetPosition14(Square s){m_pos=s;}

	};
	
public:
	HObject(eObjectType t):m_type(t){}

	virtual Square 		GetPosition()=0;
	virtual	~HObject(){}
	eObjectType 	GetType()	{return m_type;}

};

class Obstacle : public HObject
{
	HObject::Situation	m_sit;
public:
	Obstacle(Square pos):HObject(e_obstacle),m_sit(pos)
	{
	}
	Obstacle(int x, int y):HObject(e_obstacle),m_sit(x,y){};

	Square 		GetPosition(){return m_sit.GetPosition();}
};


#endif //!defined(HOBJECT__INCLUDED_)