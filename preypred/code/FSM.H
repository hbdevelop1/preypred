
enum
{
	e_starting=1,
		e_ticking,
		e_ending
};

#define CONSTRUCT_BEHAVIOR_BEGIN	\
	if(m_state == e_starting) \
	{\

#define CONSTRUCT_BEHAVIOR_END		\
		m_state = e_ticking; \
	}\
	else

#define TICK_BEHAVIOR_BEGIN	\
	if(m_state == e_ticking) \
	{\

#define TICK_BEHAVIOR_END		\
	}\
	else

#define DESTRUCT_BEHAVIOR_BEGIN		\
	if(m_state == e_ending) \
	{\

#define DESTRUCT_BEHAVIOR_END		\
		m_state = e_starting; \
	}



#define START_BEHAVIOR(Class,newbehavior)  \
{ \
	m_state = e_starting;	\
	m_currentbehavior = & Class::Behavior_##newbehavior;	\
}

#define CHANGE_BEHAVIOR(Class,newbehavior)  \
{ \
	m_state = e_ending;	\
	(this->*m_currentbehavior)();\
	m_currentbehavior = & Class::Behavior_##newbehavior;	\
	(this->*m_currentbehavior)(); \
}



/*
CONSTRUCT_BEHAVIOR_BEGIN

CONSTRUCT_BEHAVIOR_END


TICK_BEHAVIOR_BEGIN

TICK_BEHAVIOR_END


DESTRUCT_BEHAVIOR_BEGIN	

DESTRUCT_BEHAVIOR_END

*/