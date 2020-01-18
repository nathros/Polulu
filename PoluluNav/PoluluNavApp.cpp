/***************************************************************
 * Name:      PoluluNavApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Copyright:  ()
 * License:
 **************************************************************/

#include "PoluluNavApp.h"

//(*AppHeaders
#include "PoluluNavMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PoluluNavApp);

bool PoluluNavApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PoluluNavFrame* Frame = new PoluluNavFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
