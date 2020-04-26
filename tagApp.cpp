// tagApp.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "tagApp.h"
#include "EuroScopePlugIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(tagApp, CWinApp)
END_MESSAGE_MAP()

tagApp::tagApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
tagApp::~tagApp()
{

}

// Das einzige Anwendungsobjekt

//CWinApp theApp;
tagApp theApp;

using namespace std;

BOOL tagApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

tagPlugin    * gpMyPlugin = NULL;

//---EuroScopePlugInInit-----------------------------------------------

void    __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		

		// create the instance
		* ppPlugInInstance = gpMyPlugin = new tagPlugin();
}


//---EuroScopePlugInExit-----------------------------------------------

void    __declspec (dllexport) EuroScopePlugInExit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

		// delete the instance
		delete gpMyPlugin;
}
