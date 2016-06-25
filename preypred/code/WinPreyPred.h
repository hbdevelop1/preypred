#define EX 1
#define OH 2

#include "Game.h"

class CMyApp : public CWinApp
{
	LONGLONG	m_lCurTime;			// current time
	DWORD		m_dwTimeCount;		// ms per frame, default if no performance counter
	LONGLONG	m_lPerfCounter;		// performance timer frequency
	BOOL		m_bPerFlag;			// flag determining which timer to use
	LONGLONG	m_lNextTime;		// time to render next frame
	//LONGLONG	m_lLastTime;		// time of previous frame
	//double		m_dTimeElapsed;		// time since previous frame
	double		m_dTimeScale;		// scaling factor for time
	LONGLONG	m_lFramesPerSecond;	// How many frames per second to run App


	Game		* m_pgame;
public:
	CMyApp();
    virtual BOOL InitInstance ();
	virtual int ExitInstance() ;
	virtual int Run();
	Game		*GetGame();
};

class CMainWindow : public CWnd
{
public:
		BOOL	m_bAppIsActive;

private:

	int	m_cellWidth;
	int m_cellHeight;
	CRect m_BoardRectInPx;
	int m_offsettop,m_offsetbottom,m_offsetleft,m_offsetright;
	CFont m_font;



    void DrawBoard (CDC* pDC);
	void DrawObj(CDC* pDC,HObject *);
	CRect GetSquareInPx(Square );

	void DrawActors (CDC* pDC);

public:
    CMainWindow ();

protected:
    virtual void PostNcDestroy ();

    afx_msg void OnPaint ();
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);

    DECLARE_MESSAGE_MAP ()
};
