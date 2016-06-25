#include <afxwin.h>


class Debugger
{

	Debugger(){};
	Debugger(const Debugger &);
	Debugger & operator=(const Debugger &);

private:
	HANDLE	m_hMap;
	char * m_pMsg;

private:
	void CreateLogger();

public:
	enum
	{
		e_MaxLengthOfDebugMsg=511,
	};

	static Debugger & GetInstance()
	{
		static Debugger instance;
		return instance;
	}

	void Init();
	void Deinit();

//	void Output(char *);
	void Output( char * format, ... );
};

