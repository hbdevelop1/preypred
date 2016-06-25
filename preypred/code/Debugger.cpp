#include "../MFCApplication1/stdafx.h"
#include "Debugger.h"


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>





void Debugger::Init()
{
#if defined(_showdebugger_)

  // Try to create file mapping object (assume that this is the server)
  m_hMap = ::CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,
                               (e_MaxLengthOfDebugMsg+1) * sizeof(char),
                               "ApplicationSpecificSharedMem");
  // Check if file mapping object already exists. If it does, then this is a 
  // client and in this case open existing file mapping object. Client also
  // needs to create a mutex object to synchronize access to the server
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    m_hMap = ::OpenFileMapping(FILE_MAP_WRITE,FALSE,"ApplicationSpecificSharedMem");
  }
  // Obtain a pointer from the handle to file mapping object
  m_pMsg = (char*)::MapViewOfFile(m_hMap,FILE_MAP_WRITE,0,0,(e_MaxLengthOfDebugMsg+1) * sizeof(char));


  CreateLogger();
#endif //defined(_showdebugger_)

}

void Debugger::Deinit()
{
#if defined(_showdebugger_)

  ::UnmapViewOfFile(m_pMsg);
  ::CloseHandle(m_hMap);

#endif //defined(_showdebugger_)
}



void Debugger::CreateLogger()
{
	PROCESS_INFORMATION ProcInfo;

	STARTUPINFO StartUpInfo={
					sizeof(STARTUPINFO),//size, in bytes, of the structure
					NULL,//reserved
					NULL,//desktop - the new process will inherit the desktop and window station of its parent process.
					NULL,//no special title. this fits GUI processes.
					0,0,0,0,//start position and size - i don't care.
					0,0,//size in chars of the new console window - i don't care. this will fit GUI processes.
					0,//no special attributes.
					STARTF_USESHOWWINDOW
					/*|STARTF_USESTDHANDLES*/,//use input, output and error standards. i think this shouldn't be set for GUI processes.
					SW_SHOWDEFAULT,
					0,NULL,//reserved
					NULL,NULL,NULL //use standard ones.
					};

	char AppName[100] = "logger.exe";


	BOOL CreationSucceded = CreateProcess(AppName//app name
							,NULL//arguments
							,NULL//ProcessAttributes
							,NULL//ThreadAttributes
							,TRUE//TRUE for Handles to be inherited. this is needed for hStdInput, hStdOutput, and hStdError in StartUpInfo.
							,/*CREATE_NEW_CONSOLE*/
							//CREATE_NO_WINDOW
							0
							,NULL//Environment
							,NULL//CurrentDirectory will be the same as parent process
							,&StartUpInfo//StartupInfo
							,&ProcInfo//ProcessInformation
							);
	if (!CreationSucceded)
	{
//		::MessageBox(("Process couldn't be created !",);
		
		DWORD err = GetLastError();
		//printf("error number %d occured.\n",err);
		
		return ;
	}
	else
	{
		//wait some time, before continuing loading the game.
		Sleep(1000);
	}

}



void Debugger::Output( char * format, ... )
{
#if defined(_showdebugger_)
   va_list args;

   va_start( args, format );
   
   char buff[255];

   int nSize = _vsnprintf( buff, sizeof(buff), format, args);
   
   if(nSize == -1)
   {
		//todo : issue a warning 
		strncpy(m_pMsg, buff, e_MaxLengthOfDebugMsg);
	   m_pMsg[e_MaxLengthOfDebugMsg+1]=0;
   }
   else
   {
	   /*
   		strncpy(m_pMsg, buff, nSize);
	   m_pMsg[nSize+1]=0;
	   */
   		strcpy(m_pMsg, buff);
		char r;
		r=1;
   }



	va_end(args);

#endif //defined(_showdebugger_)
}

