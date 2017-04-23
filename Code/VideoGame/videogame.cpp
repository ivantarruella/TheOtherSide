#include <Windows.h>
#include "Engine.h"
#include "GameProcess.h"
#include "FontManager.h"		
#include "LanguageManager.h"	
#include "ActionToInput.h"	
#include "Logger.h"
#include "Exception.h"
#include "Base.h"				


#define APPLICATION_NAME	"ThE oThEr SiDe v1.04"


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
	/*
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
	*/
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

void ShowErrorMessage (const std::string& message)
{
	bool logSaved = false;
	logSaved = LOGGER->SaveLogsInFile();
	std::string end_message = "";
	if (logSaved)	{end_message += "Sorry, Application failed. Logs saved\n";}
	else{	end_message += "Sorry, Application failed. Logs could not be saved\n";}
	end_message += message;
	MessageBox(0, end_message.c_str(), "The Other Side Project Report", MB_OK | MB_ICONERROR);
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
#if defined( _DEBUG )
	MemLeaks::MemoryBegin();
#endif //defined(_DEBUG)

	// Creamos LOGGER
	CLogger* logger = new CLogger();

	// Creamos Engine
	CEngine* l_Engine = new CEngine();

	// Creamos CGameProcess
	CGameProcess l_proc;
	
	// Leemos XML de configuración para el Engine
	l_Engine->LoadConfig("data/XML/settings.xml");

	// Register the window class
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };
	RegisterClassEx( &wc );

	try {
		// Create the application's window
		HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 
									l_Engine->m_InitParams.x, l_Engine->m_InitParams.y, l_Engine->m_InitParams.w, l_Engine->m_InitParams.h, NULL, NULL, wc.hInstance, NULL );

		// Inicialización del Engine y de nuestra aplicación (game) 
		l_Engine->SetProcess(&l_proc);
		l_Engine->Init(hWnd);

		// Añadir aquí el Init de la applicación
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		// Añadir en el while la condición de salida del programa de la aplicación
		while( msg.message != WM_QUIT && !l_Engine->GetProcess()->GetEndGame())
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Main loop: Update y Render de la aplicación principal
				l_Engine->Update();
				l_Engine->Render();
			}
		}
		UnregisterClass( APPLICATION_NAME, wc.hInstance );
	}	
	catch(CException& e){
		ShowErrorMessage(e.GetDescription());
	}
	catch (...)	{
		ShowErrorMessage("Exception Occured");
	}	

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	l_proc.Release();
	CHECKED_DELETE(l_Engine);
	CHECKED_DELETE(logger);

	return 0;
}

