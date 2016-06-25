#include "../MFCApplication1/stdafx.h"
#include <afxwin.h>
#include "WinPreyPred.h"
#include <mmsystem.h>

#include "objects\Actor.h"

#include "Loader.h"
#include "Debugger.h"


CMyApp myApp;




Game *GetGame()
{
	return dynamic_cast<CMyApp*>(AfxGetApp())->GetGame();
}



/////////////////////////////////////////////////////////////////////////
// CMyApp member functions
CMyApp::CMyApp()
{
	m_lCurTime = 0;             
	m_dwTimeCount = 8;   
	m_lPerfCounter = 0;       
	m_bPerFlag = FALSE;    
	m_lNextTime = 0;    
	//m_lLastTime = 0;	 
	//m_dTimeElapsed = 0.0;	 
	m_dTimeScale = 0.0;		
	m_lFramesPerSecond = 1;

}

BOOL CMyApp::InitInstance ()
{
	if( !Loader::GetInstance().SetSize("data"))
		return 0;

	m_pgame = new Game();

	if( !Loader::GetInstance().LoadGame("data"))
		return 0;


    m_pMainWnd = new CMainWindow;
    m_pMainWnd->ShowWindow (m_nCmdShow);
    m_pMainWnd->UpdateWindow ();


	//m_GameThread = AfxBeginThread (Game::GameLoop, NULL);

   Debugger::GetInstance().Init();


    return TRUE;
}


int CMyApp::ExitInstance() 
{
//AfxMessageBox("ending application");	
	m_pgame->Stop();
	delete m_pgame;

//	::WaitForSingleObject (m_GameThread->m_hThread, INFINITE);

//	AfxMessageBox("ending CWinApp");



   Debugger::GetInstance().Deinit();

   return CWinApp::ExitInstance();
}


Game * CMyApp::GetGame()
{
	return m_pgame;
}

int CMyApp::Run()
{
	if (!m_pMainWnd)
		AfxPostQuitMessage(0);

	MSG msg;
	CMainWindow* pFrame = dynamic_cast<CMainWindow*>(m_pMainWnd );

	assert(pFrame!=NULL);

	if ( QueryPerformanceFrequency ( ( LARGE_INTEGER *) &m_lPerfCounter ) ) { 

		// yes, set m_dwTimeCount and timer choice flag 
		m_bPerFlag = TRUE;
		m_dwTimeCount = unsigned long(m_lPerfCounter / m_lFramesPerSecond);
		QueryPerformanceCounter ( ( LARGE_INTEGER * ) &m_lNextTime ); 
		m_dTimeScale = 1.0 / m_lPerfCounter;
	} 
	else  
	{ 
		// no performance counter, read in using timeGetTime 

		m_lNextTime = timeGetTime (); 
		m_dTimeScale = 0.001;
	} 

	while ( TRUE ) 
	{
		//see if there is a message waiting

		if ( ::PeekMessage ( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{ 
			do //if there is pump all waiting
			{
				if ( !PumpMessage () )
					return ExitInstance ();
			} while ( ::PeekMessage ( &msg, NULL, 0, 0, PM_NOREMOVE ) );
		} 
		else 
		{
			if ( m_bPerFlag ) 
			{
				QueryPerformanceCounter ( ( LARGE_INTEGER * ) &m_lCurTime );	
			} 
			else 
			{
				m_lCurTime=timeGetTime (); 
			}

			// is it time to render the frame? 

			if ( m_lCurTime > m_lNextTime ) 
			{ 

				pFrame->Invalidate();
				m_pgame->Loop();

				// set time for next frame 

				m_lNextTime = m_lCurTime + m_dwTimeCount; 

			} // end if

		} // end if else

	} // end while
	return msg.wParam;

} 



/////////////////////////////////////////////////////////////////////////
// CMainWindow message map and member functions

BEGIN_MESSAGE_MAP (CMainWindow, CWnd)
    ON_WM_PAINT ()
	ON_WM_ACTIVATEAPP()

END_MESSAGE_MAP ()


#define RRRRR	2

CMainWindow::CMainWindow ():
	m_offsettop(20),m_offsetbottom(20),m_offsetleft(20),m_offsetright(20)
{


	//
	// Register a WNDCLASS.
	//
    CString strWndClass = AfxRegisterWndClass (
        CS_DBLCLKS,										// Class style
        AfxGetApp ()->LoadStandardCursor (IDC_ARROW),   // Class cursor
        (HBRUSH) (COLOR_3DFACE + 1),					// Background brush
        AfxGetApp ()->LoadStandardIcon (IDI_WINLOGO)	// Class icon
    );

	//
	// Create a window.
	//
    CreateEx (0, strWndClass, _T ("Prey vs Predator"),
        WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL);

	//
	// Size the window.
	//

    m_BoardRectInPx = CRect(m_offsetleft, m_offsettop,
		(Board::CellWidthInPx())*Board::SizeY()+m_offsetright, 
		(Board::CellHeightInPx())*Board::SizeX()+m_offsetbottom);

	CRect rect(
		0, 0, 
		m_offsetleft+(+Board::CellWidthInPx())*Board::SizeY()+m_offsetright, 
		m_offsettop+(+Board::CellHeightInPx())*Board::SizeX()+m_offsetbottom
		);
    CalcWindowRect (&rect);

    SetWindowPos (NULL, 0, 0, rect.Width (), rect.Height (),
        SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);



	m_bAppIsActive = FALSE;


m_font.CreateFont(
   12,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_NORMAL,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "Arial");                 // lpszFacename

}

void CMainWindow::PostNcDestroy ()
{
    delete this;
}

void CMainWindow::OnPaint ()
{
    CPaintDC dc (this);
    DrawBoard (&dc);    
}


void CMainWindow::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CWnd::OnActivateApp(bActive, dwThreadID);
	m_bAppIsActive = bActive;
	
	if(m_bAppIsActive==TRUE)
		OutputDebugString("active window\n");
	else
		OutputDebugString("de active window\n");

}



void CMainWindow::DrawBoard (CDC* pDC)
{
	CPen pen (PS_SOLID, RRRRR, RGB (0, 0, 0));
    CPen* pOldPen = pDC->SelectObject (&pen);


	int x,y;
	x=m_BoardRectInPx.top;
	for(int j=0; j<Board::SizeY()+1;j++)
	{
		y=m_BoardRectInPx.left + (Board::CellWidthInPx()) * j;

		pDC->MoveTo (y, x);
		pDC->LineTo (y, x+(Board::CellHeightInPx()) *Board::SizeX());

	}

	y=m_BoardRectInPx.left;
	for(int i=0; i<Board::SizeX()+1;i++)
	{
		x=m_BoardRectInPx.top + (Board::CellHeightInPx()) * i;

		pDC->MoveTo (y, x);
		pDC->LineTo (y+(Board::CellWidthInPx()) *Board::SizeY(), x);

	}



	list<Obstacle *> *plo = GetGame()->GetObstacles();
	for(list<Obstacle*>::iterator ito=plo->begin(); ito != plo->end(); ito++)
	{
		DrawObj(pDC,*ito);
	}

	DrawActors(pDC);
	
	
    pDC->SelectObject (pOldPen);
}


void CMainWindow::DrawActors (CDC* pDC)
{

	list<Actor *> *pla = GetGame()->GetActors();
	CFont *f = pDC->SelectObject(&m_font);

	for(list<Actor*>::iterator it=pla->begin(); it != pla->end(); it++)
	{
		DrawObj(pDC,*it);
	}

	pDC->SelectObject(f);

}



void CMainWindow::DrawObj(CDC* pDC,HObject *p)
{
//s.GetPosition()[0],s.GetPosition()[1],(*it)->GetType(),(*it)->GetDirection()
	
	if(p->GetType()==e_obstacle)
	{
		CBrush brush(RGB( 32,32,32 ) );
		CBrush * pobrush = pDC->SelectObject (&brush);

		CRect rect0 = GetSquareInPx(Square(0,0));

		CRect rect(GetSquareInPx(p->GetPosition()));

		//rect.DeflateRect (2, 2);

		pDC->Rectangle (rect);

		pDC->SelectObject (pobrush);

	}
	else
	{

		COLORREF color;
		if(p->GetType()==e_prey)
			color= RGB (0, 0, 255);
		else //if(p->GetType()==e_predator)
			color= RGB (255, 0, 0);

		CPen pen (PS_SOLID, RRRRR, color);
		CPen* pOldPen = pDC->SelectObject (&pen);
		pDC->SelectStockObject (NULL_BRUSH);

		Actor *a = dynamic_cast<Actor *>(p);
		
		CRect rect(GetSquareInPx(a->GetPosition()));


		int middleY=(rect.left+rect.right)/2;
		int middleX=(rect.top+rect.bottom)/2;
		pDC->MoveTo (middleY, middleX);

		eDirection d = a->GetDirection(); 

		if(d==e_right)
			pDC->LineTo (rect.right, middleX);
		else if(d==e_left)
			pDC->LineTo (rect.left, middleX);
		else if(d==e_up)
			pDC->LineTo (middleY, rect.top);
		else// if(d==e_down)
			pDC->LineTo (middleY, rect.bottom);
		
		
		
		rect.DeflateRect (2, 2);
		pDC->Ellipse (rect);

		string s = a->GetInfoText();
		if ( !s.empty() )
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor( RGB( 0,128,0 ) );
			pDC->TextOut( middleY-4, middleX-2, 
				s.c_str(), 
				1 );
		}
		



		pDC->SelectObject (pOldPen);
	}
}

CRect CMainWindow::GetSquareInPx(Square s)
{
	int x=s.x;
	int y=s.y;
	return CRect (
		m_BoardRectInPx.left + y*(+Board::CellWidthInPx()),
		m_BoardRectInPx.top  + x*(+Board::CellHeightInPx()),
		m_BoardRectInPx.left + y*(+Board::CellWidthInPx())+Board::CellWidthInPx(),
		m_BoardRectInPx.top  + x*(+Board::CellHeightInPx())+Board::CellHeightInPx()
		);
}