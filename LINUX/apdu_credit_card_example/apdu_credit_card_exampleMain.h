/***************************************************************
 * Name:      apdu_credit_card_exampleMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2018-04-02
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef APDU_CREDIT_CARD_EXAMPLEMAIN_H
#define APDU_CREDIT_CARD_EXAMPLEMAIN_H

//(*Headers(apdu_credit_card_exampleFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
//*)

class apdu_credit_card_exampleFrame: public wxFrame
{
    public:

        apdu_credit_card_exampleFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~apdu_credit_card_exampleFrame();

    private:

        //(*Handlers(apdu_credit_card_exampleFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnCheckPseText(wxCommandEvent& event);
        void OnREAD_CHECK_PSEClick(wxCommandEvent& event);
        void OnCheckBox1Click(wxCommandEvent& event);
        void OnCheckBox2Click(wxCommandEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnButton4Click1(wxCommandEvent& event);
        void OnButton5Click1(wxCommandEvent& event);
        void OnButton6Click(wxCommandEvent& event);
        void OnButton7Click(wxCommandEvent& event);
        void OnButton8Click(wxCommandEvent& event);
        void OnButton9Click(wxCommandEvent& event);
        void OnButton10Click(wxCommandEvent& event);
        void OnButton11Click(wxCommandEvent& event);
        void OnButton10Click1(wxCommandEvent& event);
        void OnButton11Click1(wxCommandEvent& event);
        void tryEmvPseLog(const char *df_name, const char *szTitlePse);
        void OnButton10Click2(wxCommandEvent& event);
        void OnButton11Click2(wxCommandEvent& event);
        //*)

        //(*Identifiers(apdu_credit_card_exampleFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT12;
        static const long ID_TEXTCTRL9;
        static const long ID_RADIOBUTTON1;
        static const long ID_RADIOBUTTON2;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL4;
        static const long ID_TEXTCTRL5;
        static const long ID_TEXTCTRL6;
        static const long ID_TEXTCTRL7;
        static const long ID_BUTTON10;
        static const long ID_BUTTON11;
        static const long ID_GRID1;
        static const long ID_PANEL2;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_PANEL3;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_PANEL4;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL3;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_PANEL5;
        static const long ID_NOTEBOOK2;
        static const long ID_PANEL1;
        static const long ID_NOTEBOOK1;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(apdu_credit_card_exampleFrame)
        wxButton* Button10;
        wxButton* Button11;
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxButton* Button4;
        wxButton* Button5;
        wxButton* Button6;
        wxButton* Button7;
        wxButton* Button8;
        wxButton* Button9;
        wxGrid* Transactions_Grid;
        wxNotebook* Notebook1;
        wxNotebook* Notebook2;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
        wxPanel* Panel5;
        wxRadioButton* RadioButton1;
        wxRadioButton* RadioButton2;
        wxStaticText* PortLabel;
        wxStaticText* StaticText11;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStatusBar* StatusBar1;
        wxTextCtrl* CC_n1;
        wxTextCtrl* CC_n2;
        wxTextCtrl* CC_n3;
        wxTextCtrl* CC_n4;
        wxTextCtrl* Check_pse;
        wxTextCtrl* DLL;
        wxTextCtrl* EMV_CARD;
        wxTextCtrl* EMV_LOG;
        //*)

        DECLARE_EVENT_TABLE()
};

extern apdu_credit_card_exampleFrame *Frame;
#endif // APDU_CREDIT_CARD_EXAMPLEMAIN_H
