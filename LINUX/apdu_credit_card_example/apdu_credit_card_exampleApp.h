/***************************************************************
 * Name:      apdu_credit_card_exampleApp.h
 * Purpose:   Defines Application Class
 * Author:    Aleksandar Krstic (aleksandar.krstic@d-logic.rs)
 * Created:   2018-04-19
 * Copyright: Aleksandar Krstic (www.d-logic.net)
 * License:
 **************************************************************/

#ifndef APDU_CREDIT_CARD_EXAMPLEAPP_H
#define APDU_CREDIT_CARD_EXAMPLEAPP_H

#include <wx/app.h>

class apdu_credit_card_exampleApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // APDU_CREDIT_CARD_EXAMPLEAPP_H
