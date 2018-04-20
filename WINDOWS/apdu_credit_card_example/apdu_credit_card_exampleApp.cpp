/***************************************************************
 * Name:      apdu_credit_card_exampleApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Aleksandar Krstic (aleksandar.krstic@d-logic.rs)
 * Created:   2018-04-18
 * Copyright: Aleksandar Krstic (www.d-logic.net)
 * License:
 **************************************************************/

#include "apdu_credit_card_exampleApp.h"

//(*AppHeaders
#include "apdu_credit_card_exampleMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(apdu_credit_card_exampleApp);

bool apdu_credit_card_exampleApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	apdu_credit_card_exampleFrame* Frame = new apdu_credit_card_exampleFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
