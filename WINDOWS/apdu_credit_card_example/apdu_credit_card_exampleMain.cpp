/***************************************************************
 * Name:      apdu_credit_card_exampleMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Aleksandar Krstic (aleksandar.krstic@d-logic.rs)
 * Created:   2018-04-18
 * Copyright: Aleksandar Krstic (www.d-logic.net)
 * License:
 **************************************************************/

#include "apdu_credit_card_exampleMain.h"
#include <wx/msgdlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#if _WIN32 || _WIN64
#include <conio.h>
#include <windows.h>
#elif linux || linux || APPLE
#define __USE_MISC 1
#include <unistd.h>
#include <termios.h>
#undef __USE_MISC
#include "conio_gnu.h"
#else
#error "Unknown build platform."
#endif
#include "lib/include/uFCoder.h"
#include "ini.h"
#include "iso3166.h"
#include "iso4217.h"
#include "emv.h"
#include "uFR.h"
#include "utils.h"
#include <iostream>

#define RAW_RES_MAX_LEN 258
#define MAX_AID_LEN 16
//(*InternalHeaders(apdu_credit_card_exampleFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)
wxString CHECK_PSE = ("");
wxString Status = ("");
void checkEmvPse(const char *df_name, const char *szTitlePse);
void tryEmvPseCardRead(const char *df_name, const char *szTitlePse);
void GetCreditCardNumber(const char *df_name, const char *szTitlePse);
void dec_to_hex(uint32_t dec, char HEXA[1000]);
void print_hex(const uint8_t *data, uint32_t len, const char *delimiter);

uint32_t hex2int(char *hex);

wxString LOG_VIEW = "";
wxString TRYEMV = ("");
uint32_t sw_int[2];
char sw_string1[4];
char sw_string2[4];
char EMV_STATUS_error[255] = "";
wxString EMVLOG = ("");
wxString CCN1 = "";
wxString CCN2 = "";
wxString CCN3 = "";
wxString CCN4 = "";
uint32_t cnt_integer, record_integer, sfi_integer, sfi_hex;
char SFI_HEX[255];
uint32_t response_integer[RAW_RES_MAX_LEN];
char RESPONSE[RAW_RES_MAX_LEN] = "";
wxString Transactions = "";
int parse(uint8_t *input, uint8_t size);
int FUNCTION_STATUS = 1;

//helper functions
enum wxbuildinfoformat
{
    short_f,
    long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(apdu_credit_card_exampleFrame)
const long apdu_credit_card_exampleFrame::ID_BUTTON1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON3 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT12 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL9 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_RADIOBUTTON1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_RADIOBUTTON2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT5 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL4 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL5 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL6 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL7 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON10 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON11 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_GRID1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_PANEL2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON4 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON5 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_PANEL3 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT3 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON6 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON7 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_PANEL4 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATICTEXT4 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_TEXTCTRL3 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON8 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_BUTTON9 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_PANEL5 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_NOTEBOOK2 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_PANEL1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_NOTEBOOK1 = wxNewId();
const long apdu_credit_card_exampleFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(apdu_credit_card_exampleFrame, wxFrame)
//(*EventTable(apdu_credit_card_exampleFrame)
//*)
END_EVENT_TABLE()

apdu_credit_card_exampleFrame::apdu_credit_card_exampleFrame(wxWindow *parent, wxWindowID id)
{
    //(*Initialize(apdu_credit_card_exampleFrame)
    wxBoxSizer *BoxSizer1;
    wxBoxSizer *BoxSizer2;
    wxBoxSizer *BoxSizer3;
    wxBoxSizer *BoxSizer4;
    wxGridSizer *GridSizer1;
    wxGridSizer *GridSizer2;
    wxGridSizer *GridSizer3;
    wxGridSizer *GridSizer4;
    wxGridSizer *GridSizer5;
    wxStaticBoxSizer *StaticBoxSizer1;
    wxStaticBoxSizer *StaticBoxSizer2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1217, 592));
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxPoint(192, 224), wxSize(1200, 600), 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    GridSizer1 = new wxGridSizer(5, 2, -150, wxDLG_UNIT(Panel1, wxSize(-75, 0)).GetWidth());
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Commands"));
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Reader Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticBoxSizer1->Add(Button1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("ReaderReset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticBoxSizer1->Add(Button2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("Reader Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticBoxSizer1->Add(Button3, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    PortLabel = new wxStaticText(Panel1, ID_STATICTEXT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticBoxSizer1->Add(PortLabel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT12, _("DLL version :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    StaticBoxSizer1->Add(StaticText11, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    DLL = new wxTextCtrl(Panel1, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    StaticBoxSizer1->Add(DLL, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(StaticBoxSizer1, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Select Payment System Enviroment"));
    RadioButton1 = new wxRadioButton(Panel1, ID_RADIOBUTTON1, _("PSE1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    StaticBoxSizer2->Add(RadioButton1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    RadioButton2 = new wxRadioButton(Panel1, ID_RADIOBUTTON2, _("PSE2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    StaticBoxSizer2->Add(RadioButton2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(StaticBoxSizer2, 1, wxALL | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 5);
    Notebook2 = new wxNotebook(Panel1, ID_NOTEBOOK2, wxDefaultPosition, wxSize(1200, 700), 0, _T("ID_NOTEBOOK2"));
    Panel2 = new wxPanel(Notebook2, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    GridSizer5 = new wxGridSizer(2, 1, -585, 0);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Credit card number : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    BoxSizer4->Add(StaticText4, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    CC_n1 = new wxTextCtrl(Panel2, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    BoxSizer4->Add(CC_n1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    CC_n2 = new wxTextCtrl(Panel2, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    BoxSizer4->Add(CC_n2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    CC_n3 = new wxTextCtrl(Panel2, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    BoxSizer4->Add(CC_n3, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    CC_n4 = new wxTextCtrl(Panel2, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    BoxSizer4->Add(CC_n4, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button10 = new wxButton(Panel2, ID_BUTTON10, _("READ"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON10"));
    BoxSizer4->Add(Button10, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button11 = new wxButton(Panel2, ID_BUTTON11, _("CLEAR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
    BoxSizer4->Add(Button11, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer5->Add(BoxSizer4, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    Transactions_Grid = new wxGrid(Panel2, ID_GRID1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID1"));
    Transactions_Grid->CreateGrid(10, 5);
    Transactions_Grid->EnableEditing(false);
    Transactions_Grid->EnableGridLines(true);
    Transactions_Grid->SetDefaultColSize(200, true);
    Transactions_Grid->SetColLabelValue(0, _("ATCounter"));
    Transactions_Grid->SetColLabelValue(1, _("Date"));
    Transactions_Grid->SetColLabelValue(2, _("Time"));
    Transactions_Grid->SetColLabelValue(3, _("Amount"));
    Transactions_Grid->SetColLabelValue(4, _("Currency"));
    Transactions_Grid->SetRowLabelValue(0, _("1"));
    Transactions_Grid->SetRowLabelValue(1, _("2"));
    Transactions_Grid->SetRowLabelValue(2, _("3"));
    Transactions_Grid->SetRowLabelValue(3, _("4"));
    Transactions_Grid->SetRowLabelValue(4, _("5"));
    Transactions_Grid->SetRowLabelValue(5, _("6"));
    Transactions_Grid->SetRowLabelValue(6, _("7"));
    Transactions_Grid->SetRowLabelValue(7, _("8"));
    Transactions_Grid->SetRowLabelValue(8, _("9"));
    Transactions_Grid->SetRowLabelValue(9, _("10"));
    Transactions_Grid->SetDefaultCellFont(Transactions_Grid->GetFont());
    Transactions_Grid->SetDefaultCellTextColour(Transactions_Grid->GetForegroundColour());
    GridSizer5->Add(Transactions_Grid, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    Panel2->SetSizer(GridSizer5);
    GridSizer5->Fit(Panel2);
    GridSizer5->SetSizeHints(Panel2);
    Panel3 = new wxPanel(Notebook2, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    GridSizer2 = new wxGridSizer(3, 1, -585, 0);
    StaticText1 = new wxStaticText(Panel3, ID_STATICTEXT2, _("Checking if card support Payment System Enviroment "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    GridSizer2->Add(StaticText1, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    Check_pse = new wxTextCtrl(Panel3, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(1185, 300), wxTE_MULTILINE | wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    GridSizer2->Add(Check_pse, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Button4 = new wxButton(Panel3, ID_BUTTON4, _("READ"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer1->Add(Button4, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button5 = new wxButton(Panel3, ID_BUTTON5, _("CLEAR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer1->Add(Button5, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(BoxSizer1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Panel3->SetSizer(GridSizer2);
    GridSizer2->Fit(Panel3);
    GridSizer2->SetSizeHints(Panel3);
    Panel4 = new wxPanel(Notebook2, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    GridSizer3 = new wxGridSizer(3, 1, -585, 0);
    StaticText2 = new wxStaticText(Panel4, ID_STATICTEXT3, _("Read and parse EMV card supporting PSE1 / PSE2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer3->Add(StaticText2, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    EMV_CARD = new wxTextCtrl(Panel4, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxSize(1185, 300), wxTE_MULTILINE | wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    GridSizer3->Add(EMV_CARD, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    Button6 = new wxButton(Panel4, ID_BUTTON6, _("READ"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer2->Add(Button6, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button7 = new wxButton(Panel4, ID_BUTTON7, _("CLEAR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer2->Add(Button7, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3->Add(BoxSizer2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Panel4->SetSizer(GridSizer3);
    GridSizer3->Fit(Panel4);
    GridSizer3->SetSizeHints(Panel4);
    Panel5 = new wxPanel(Notebook2, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    GridSizer4 = new wxGridSizer(3, 1, -585, 0);
    StaticText3 = new wxStaticText(Panel5, ID_STATICTEXT4, _("Read and parse EMV log on card supporting PSE1 / PSE 2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridSizer4->Add(StaticText3, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    EMV_LOG = new wxTextCtrl(Panel5, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxSize(1185, 300), wxTE_MULTILINE | wxTE_READONLY | wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    GridSizer4->Add(EMV_LOG, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    Button8 = new wxButton(Panel5, ID_BUTTON8, _("READ"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    BoxSizer3->Add(Button8, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Button9 = new wxButton(Panel5, ID_BUTTON9, _("CLEAR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    BoxSizer3->Add(Button9, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4->Add(BoxSizer3, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Panel5->SetSizer(GridSizer4);
    GridSizer4->Fit(Panel5);
    GridSizer4->SetSizeHints(Panel5);
    Notebook2->AddPage(Panel2, _("Transactions"), false);
    Notebook2->AddPage(Panel3, _("Check if card support PSE"), false);
    Notebook2->AddPage(Panel4, _("Read and parse EMV on card"), false);
    Notebook2->AddPage(Panel5, _("Read and parse EMV log on card"), false);
    GridSizer1->Add(Notebook2, 1, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
    Panel1->SetSizer(GridSizer1);
    GridSizer1->Fit(Panel1);
    GridSizer1->SetSizeHints(Panel1);
    Notebook1->AddPage(Panel1, _("MENU"), false);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = {-1};
    int __wxStatusBarStyles_1[1] = {wxSB_NORMAL};
    StatusBar1->SetFieldsCount(1, __wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1, __wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton1Click);
    Connect(ID_BUTTON2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton2Click);
    Connect(ID_BUTTON3, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton3Click);
    Connect(ID_BUTTON10, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton10Click2);
    Connect(ID_BUTTON11, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton11Click2);
    Connect(ID_BUTTON4, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton4Click1);
    Connect(ID_BUTTON5, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton5Click1);
    Connect(ID_BUTTON6, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton6Click);
    Connect(ID_BUTTON7, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton7Click);
    Connect(ID_BUTTON8, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton8Click);
    Connect(ID_BUTTON9, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&apdu_credit_card_exampleFrame::OnButton9Click);
    //*)
}

apdu_credit_card_exampleFrame::~apdu_credit_card_exampleFrame()
{
    //(*Destroy(apdu_credit_card_exampleFrame)
    //*)
}

void apdu_credit_card_exampleFrame::OnQuit(wxCommandEvent &event)
{
    Close();
}

void apdu_credit_card_exampleFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void apdu_credit_card_exampleFrame::OnButton1Click(wxCommandEvent &event)
{
    UFR_STATUS status;

    DLL->SetValue(GetDllVersionStr());

    status = ReaderOpen();

    if (status == UFR_OK)
    {

        ReaderUISignal(1, 1);

        PortLabel->SetLabel("Port opened");
    }
    else if (status != UFR_OK)
    {

        PortLabel->SetLabel("Port not opened!");
    }
}

void apdu_credit_card_exampleFrame::OnButton2Click(wxCommandEvent &event)
{

    UFR_STATUS status;

    status = ReaderReset();

    if (status == UFR_OK)
    {

        ReaderUISignal(1, 1);
        PortLabel->SetLabel("Reader reset");
    }
    else if (status != UFR_OK)
    {

        PortLabel->SetLabel("Reader not reset");
    }
}

void apdu_credit_card_exampleFrame::OnButton3Click(wxCommandEvent &event)
{

    UFR_STATUS status;

    status = ReaderClose();

    if (status == UFR_OK)
    {

        PortLabel->SetLabel("Reader closed");
    }
    else if (status != UFR_OK)
    {

        PortLabel->SetLabel("Reader not closed");
    }
}

void apdu_credit_card_exampleFrame::OnButton4Click(wxCommandEvent &event)
{
}

void checkEmvPse(const char *df_name, const char *szTitlePse)
{

    emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
    uint8_t r_apdu[RAW_RES_MAX_LEN];
    uint32_t Ne; // without SW
    uint8_t sw[2];
    uint8_t sfi, record, cnt;
    uint16_t *sw16_ptr = (uint16_t *)&sw;
    EMV_STATUS emv_status;
    UFR_STATUS status;
    CHECK_PSE = "";

    CHECK_PSE << "=====================================================================\n";
    CHECK_PSE << "  Checking if card support Payment System Environment " << szTitlePse << "\n";
    CHECK_PSE << " --------------------------------------------------------------------------------------------------------------------\n";

    do
    {
        status = SetISO14443_4_Mode();

        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {

            CHECK_PSE << "Error while switching into ISO 14443-4 mode, uFR status is: " << Status;
            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }
        else
        {

            CHECK_PSE << "1. Issuing Select PSE command " << df_name;
            CHECK_PSE << "\n[C] 00 A4 04 00 32 50 41 59 2E 53 59 53 2E 44 44 46 30 31 00 ";

            Ne = 256;

            status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (const uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);

            Status = UFR_Status2String(status);

            if (status != UFR_OK)
            {

                CHECK_PSE << " Error while executing APDU command, uFR status is: " << Status;

                wxString msg = "";
                msg << "Reading error ";
                wxMessageBox(msg, _("ERROR"));
                break;
            }
            else
            {

                if (*sw16_ptr != 0x90)
                {

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%X", sw_int[0]);
                    sprintf(sw_string2, "%X", sw_int[1]);

                    CHECK_PSE << "\nCould not continue execution due to an APDU error"
                              << "\n[SW] : " << sw_string1 << " " << sw_string2;

                    wxString msg = "";
                    msg << "Card does not support " << szTitlePse;
                    wxMessageBox(msg, _("ERROR"));
                    break;
                }

                CHECK_PSE << "\nAPDU command executed: response data length = " << (int)Ne;

                uint32_t response_integer[RAW_RES_MAX_LEN];

                for (uint32_t i = 0; i < Ne; i++)
                {

                    response_integer[i] = r_apdu[i];
                }

                char RESPONSE[RAW_RES_MAX_LEN] = "";

                for (uint32_t j = 0; j < Ne; j++)
                {

                    dec_to_hex(response_integer[j], RESPONSE);
                }

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%02X", sw_int[0]);
                sprintf(sw_string2, "%02X", sw_int[1]);

                CHECK_PSE << "\n[R] : " << RESPONSE << "\n [SW] : " << sw_string1 << " " << sw_string2;
            }

            emv_status = newEmvTag(&head, r_apdu, Ne, false);

            sprintf(EMV_STATUS_error, "%08X", emv_status);

            if (emv_status)
            {

                CHECK_PSE << "\nEMV parsing error code: " << EMV_STATUS_error;
                CHECK_PSE << "\n--------------------------------------------------------------------------------------------------------------------\n";

                wxString msg = "";
                msg << "Reading error ";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            emv_status = getSfi(head, &sfi);

            if (emv_status == EMV_OK)
            {

                cnt = 2;
                record = 1;

                cnt_integer = cnt;
                record_integer = record;
                sfi_integer = sfi;
                sfi_hex = (sfi << 3) | 4;

                sprintf(SFI_HEX, "%02X", sfi_hex);

                do
                {

                    CHECK_PSE << cnt_integer << ". Issuing Read Record command (record = " << record_integer << ", sfi = " << sfi_integer << " )";
                    CHECK_PSE << "\n[C] 00 B2 " << SFI_HEX;

                    emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);

                    if (emv_status == EMV_OK)
                    {

                        emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                        if (record == 1)
                            head->next = tail = temp;
                        else
                        {
                            tail->next = temp;
                            tail = tail->next;
                        }

                        if (Ne)
                        {

                            CHECK_PSE << "\nAPDU command executed: response data length = " << (int)Ne;

                            for (uint32_t i = 0; i < Ne; i++)
                            {

                                response_integer[i] = r_apdu[i];
                            }

                            char RESPONSE[RAW_RES_MAX_LEN] = "";

                            for (uint32_t j = 0; j < Ne; j++)
                            {

                                dec_to_hex(response_integer[j], RESPONSE);
                            }

                            CHECK_PSE << "\n[R] : " << RESPONSE;
                        }

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%02X", sw_int[0]);
                        sprintf(sw_string2, "%02X", sw_int[1]);

                        CHECK_PSE << "\n [SW] : " << sw_string1 << " " << sw_string2;
                    }
                    else
                    {

                        if (*sw16_ptr != 0x90)
                        {

                            sw_int[0] = sw[0];
                            sw_int[1] = sw[1];

                            sprintf(sw_string1, "%X", sw_int[0]);
                            sprintf(sw_string2, "%X", sw_int[1]);

                            CHECK_PSE << "\nThere is no records.\n"
                                      << "\n[SW] : " << sw_string1 << " " << sw_string2;
                        }
                    }

                    record++;
                    cnt++;

                } while (emv_status == EMV_OK);
            }
            CHECK_PSE << "\n--------------------------------------------------------------------------------------------------------------------\n";
            CHECK_PSE << "Card support Payment System Environment " << szTitlePse;
            CHECK_PSE << "\n=====================================================================\n";
        }

    } while (0);

    emvTreeCleanup(head);

    s_block_deselect(100);
}

void dec_to_hex(uint32_t dec, char HEXA[1000])
{

    char hex[1000] = "";
    char hex1[1000] = "";

    switch (dec / 16)
    {

    case 0:

        strcpy(hex, "0");

        break;
    case 1:

        strcpy(hex, "1");

        break;

    case 2:

        strcpy(hex, "2");

        break;

    case 3:

        strcpy(hex, "3");

        break;

    case 4:

        strcpy(hex, "4");

        break;

    case 5:

        strcpy(hex, "5");

        break;

    case 6:

        strcpy(hex, "6");

        break;

    case 7:

        strcpy(hex, "7");

        break;

    case 8:

        strcpy(hex, "8");

        break;

    case 9:

        strcpy(hex, "9");

        break;

    case 10:

        strcpy(hex, "A");

        break;

    case 11:

        strcpy(hex, "B");

        break;

    case 12:

        strcpy(hex, "C");

        break;

    case 13:

        strcpy(hex, "D");
        break;

    case 14:

        strcpy(hex, "E");

        break;

    case 15:

        strcpy(hex, "F");

        break;
    }

    switch (dec % 16)
    {

    case 0:

        strcpy(hex1, "0");

        break;
    case 1:

        strcpy(hex1, "1");

        break;

    case 2:

        strcpy(hex1, "2");

        break;

    case 3:

        strcpy(hex1, "3");

        break;

    case 4:

        strcpy(hex1, "4");

        break;

    case 5:

        strcpy(hex1, "5");

        break;

    case 6:

        strcpy(hex1, "6");

        break;

    case 7:

        strcpy(hex1, "7");

        break;

    case 8:

        strcpy(hex1, "8");

        break;

    case 9:

        strcpy(hex1, "9");

        break;

    case 10:

        strcpy(hex1, "A");

        break;

    case 11:

        strcpy(hex1, "B");

        break;

    case 12:

        strcpy(hex1, "C");

        break;

    case 13:

        strcpy(hex1, "D");
        break;

    case 14:

        strcpy(hex1, "E");

        break;

    case 15:

        strcpy(hex1, "F");

        break;
    }

    strcat(HEXA, hex);
    strcat(HEXA, hex1);
}

void apdu_credit_card_exampleFrame::OnButton4Click1(wxCommandEvent &event)
{

    if (RadioButton1->GetValue())
    {
        checkEmvPse("1PAY.SYS.DDF01", "PSE1");

        Check_pse->SetValue(CHECK_PSE);
    }
    else if (RadioButton2->GetValue())
    {

        checkEmvPse("2PAY.SYS.DDF01", "PSE2");
        Check_pse->SetValue(CHECK_PSE);
    }
    else
    {

        wxString msg = "";
        msg << "You must select Payment System Enviroment ";
        wxMessageBox(msg, _("ERROR"));
    }
}

void apdu_credit_card_exampleFrame::OnButton5Click1(wxCommandEvent &event)
{

    Check_pse->SetValue("");
    CHECK_PSE = "";
}

void tryEmvPseCardRead(const char *df_name, const char *szTitlePse)
{

    EMV_STATUS emv_status;
    UFR_STATUS status;
    bool head_attached = false;
    emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
    afl_list_item_t *afl_list = NULL, *afl_list_item = NULL;
    uint8_t afl_list_count;
    uint8_t r_apdu[RAW_RES_MAX_LEN];
    uint32_t Ne; // without SW
    uint8_t sw[2], aid[MAX_AID_LEN];
    uint8_t sfi, record, cnt = 1, aid_len;
    uint16_t *sw16_ptr = (uint16_t *)&sw;
    uint8_t *gpo_data_field = NULL;
    uint16_t gpo_data_field_size;
    char empty_string[RAW_RES_MAX_LEN] = "";
    TRYEMV = "";

    cnt = 1;
    TRYEMV << "=====================================================================\n";
    TRYEMV << "  Read and parse EMV card supporting " << szTitlePse << "\n";
    TRYEMV << " --------------------------------------------------------------------------------------------------------------------\n";

    do
    {

        status = SetISO14443_4_Mode();

        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {

            TRYEMV << " Error while switching into ISO 14443-4 mode, uFR status is: " << Status << "\n";
            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        TRYEMV << (int)cnt++ << ". Issuing Select PSE command: " << df_name << "\n";
        TRYEMV << "  [C] 00 A4 04 00 \n";

        Ne = 256;
        status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);
        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {

            TRYEMV << " Error while executing APDU command, uFR status is: " << Status;

            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }
        else
        {

            if (*sw16_ptr != 0x90)
            {

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%X", sw_int[0]);
                sprintf(sw_string2, "%X", sw_int[1]);

                TRYEMV << "\nCould not continue execution due to an APDU error"
                       << "\n[SW] : " << sw_string1 << " " << sw_string2;

                wxString msg = "";
                msg << "Reading error ";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            if (Ne)
            {
                TRYEMV << "\nAPDU command executed: response data length = " << (int)Ne;

                strcpy(RESPONSE, empty_string);

                for (uint32_t i = 0; i < Ne; i++)
                {

                    response_integer[i] = r_apdu[i];
                }

                for (uint32_t j = 0; j < Ne; j++)
                {

                    dec_to_hex(response_integer[j], RESPONSE);
                }

                TRYEMV << "\n[R] : " << RESPONSE;
            }

            sw_int[0] = sw[0];
            sw_int[1] = sw[1];

            sprintf(sw_string1, "%02X", sw_int[0]);
            sprintf(sw_string2, "%02X", sw_int[1]);

            TRYEMV << "\n [SW] : " << sw_string1 << " " << sw_string2;
        }

        emv_status = newEmvTag(&head, r_apdu, Ne, false);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {

            CHECK_PSE << "\nEMV parsing error code: " << EMV_STATUS_error;
            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        emv_status = getSfi(head, &sfi);

        if (emv_status == EMV_OK)
        {

            cnt = 2;
            record = 1;

            cnt_integer = cnt;
            record_integer = record;
            sfi_integer = sfi;
            sfi_hex = (sfi << 3) | 4;

            sprintf(SFI_HEX, "%02X", sfi_hex);

            do
            {

                TRYEMV << cnt_integer << ". Issuing Read Record command (record = " << record_integer << ", sfi = " << sfi_integer << " )";
                TRYEMV << "\n[C] 00 B2 " << SFI_HEX;

                emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);

                if (emv_status == EMV_OK)
                {

                    emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                    if (record == 1)
                        head->next = tail = temp;
                    else
                    {
                        tail->next = temp;
                        tail = tail->next;
                    }

                    if (Ne)
                    {

                        TRYEMV << "\nAPDU command executed: response data length = " << (int)Ne;

                        strcpy(RESPONSE, empty_string);

                        for (uint32_t i = 0; i < Ne; i++)
                        {

                            response_integer[i] = r_apdu[i];
                        }

                        for (uint32_t j = 0; j < Ne; j++)
                        {

                            dec_to_hex(response_integer[j], RESPONSE);
                        }

                        TRYEMV << "\n[R] : " << RESPONSE;
                    }

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%02X", sw_int[0]);
                    sprintf(sw_string2, "%02X", sw_int[1]);

                    TRYEMV << "\n [SW] : " << sw_string1 << " " << sw_string2;
                }
                else
                {

                    if (*sw16_ptr != 0x90)
                    {

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%X", sw_int[0]);
                        sprintf(sw_string2, "%X", sw_int[1]);

                        TRYEMV << "\nThere is no records.\n"
                               << "\n[SW] : " << sw_string1 << " " << sw_string2;
                    }
                }

                record++;
                cnt++;

            } while (emv_status == EMV_OK);
        }

        emv_status = getAid(head, aid, &aid_len);

        if (emv_status == EMV_OK)
        {

            TRYEMV << " \n--------------------------------------------------------------------------------------------------------------------\n";
            TRYEMV << (int)cnt++ << ". Issuing Select the application command:\n";
            TRYEMV << "  [C] 00 A4 04 00 ";

            Ne = 256;
            status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, aid, aid_len, r_apdu, &Ne, 1, sw);
            Status = UFR_Status2String(status);

            if (status != UFR_OK)
            {

                TRYEMV << " Error while executing APDU command, uFR status is: " << Status;

                wxString msg = "";
                msg << "Reading error ";
                wxMessageBox(msg, _("ERROR"));
                break;
            }
            else
            {

                if (*sw16_ptr != 0x90)
                {

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%X", sw_int[0]);
                    sprintf(sw_string2, "%X", sw_int[1]);

                    TRYEMV << "\nCould not continue execution due to an APDU error"
                           << "\n[SW] : " << sw_string1 << " " << sw_string2;

                    wxString msg = "";
                    msg << "Reading error ";
                    wxMessageBox(msg, _("ERROR"));
                    break;
                }

                //std:cerr<<"7";

                if (Ne)
                {
                    TRYEMV << "\nAPDU command executed: response data length = " << (int)Ne;

                    strcpy(RESPONSE, empty_string);

                    for (uint32_t i = 0; i < Ne; i++)
                    {

                        response_integer[i] = r_apdu[i];
                    }

                    for (uint32_t j = 0; j < Ne; j++)
                    {

                        dec_to_hex(response_integer[j], RESPONSE);
                    }

                    TRYEMV << "\n[R] : " << RESPONSE;
                }

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%02X", sw_int[0]);
                sprintf(sw_string2, "%02X", sw_int[1]);

                TRYEMV << "\n [SW] : " << sw_string1 << " " << sw_string2;

                //std:cerr<<"8";
            }
        }

        emv_status = newEmvTag(&temp, r_apdu, Ne, false);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {

            TRYEMV << " EMV parsing error code: " << EMV_STATUS_error;
            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        if (!head_attached)
        {
            head->next = tail = temp;
            head_attached = true;
        }
        else
        {
            tail->next = temp;
            tail = tail->next;
        }

        TRYEMV << " \n--------------------------------------------------------------------------------------------------------------------\n";
        TRYEMV << (int)cnt++ << ". Formating Get Processing Options instruction (checking PDOL).\n";
        TRYEMV << " --------------------------------------------------------------------------------------------------------------------\n";

        emv_status = formatGetProcessingOptionsDataField(temp, &gpo_data_field, &gpo_data_field_size);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        //std:cerr<<"9";

        if (emv_status)
        {

            TRYEMV << " EMV parsing error code: " << EMV_STATUS_error;
            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        TRYEMV << (int)cnt++ << ". Issuing Get Processing Options command:\n";
        TRYEMV << "  [C] 80 A8 00 00\n";

        Ne = 256;
        status = APDUTransceive(0x80, 0xA8, 0x00, 0x00, gpo_data_field, gpo_data_field_size, r_apdu, &Ne, 1, sw);

        //std::cerr<<"10";

        if (status != UFR_OK)
        {

            TRYEMV << " Error while executing APDU command, uFR status is: " << Status;

            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }
        else
        {

            if (*sw16_ptr != 0x90)
            {

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%X", sw_int[0]);
                sprintf(sw_string2, "%X", sw_int[1]);

                TRYEMV << "\nCould not continue execution due to an APDU error"
                       << "\n[SW] : " << sw_string1 << " " << sw_string2;

                wxString msg = "";
                msg << "Reading error ";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            if (Ne)
            {
                TRYEMV << "\nAPDU command executed: response data length = " << (int)Ne;

                strcpy(RESPONSE, empty_string);

                for (uint32_t i = 0; i < Ne; i++)
                {

                    response_integer[i] = r_apdu[i];
                }

                for (uint32_t j = 0; j < Ne; j++)
                {

                    dec_to_hex(response_integer[j], RESPONSE);
                }

                TRYEMV << "\n[R] : " << RESPONSE;
            }

            sw_int[0] = sw[0];
            sw_int[1] = sw[1];

            sprintf(sw_string1, "%02X", sw_int[0]);
            sprintf(sw_string2, "%02X", sw_int[1]);

            TRYEMV << "\n [SW] : " << sw_string1 << " " << sw_string2;
        }

        emv_status = newEmvTag(&temp, r_apdu, Ne, false);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {

            TRYEMV << " EMV parsing error code: " << EMV_STATUS_error;

            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        tail->next = temp;
        tail = tail->next;

        TRYEMV << " \n--------------------------------------------------------------------------------------------------------------------\n";
        TRYEMV << " GET AFL :\n";

        emv_status = getAfl(temp, &afl_list, &afl_list_count);

        if (emv_status == EMV_ERR_TAG_NOT_FOUND)

            emv_status = getAflFromResponseMessageTemplateFormat1(temp, &afl_list, &afl_list_count);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {

            TRYEMV << " EMV parsing error code: " << EMV_STATUS_error;

            wxString msg = "";
            msg << "Reading error ";
            wxMessageBox(msg, _("ERROR"));

            break;
        }

        afl_list_item = afl_list;

        while (afl_list_item)
        {

            for (int r = afl_list_item->record_first; r <= afl_list_item->record_last; r++)
            {

                sfi_integer = afl_list_item->sfi;

                TRYEMV << " \n--------------------------------------------------------------------------------------------------------------------\n";
                TRYEMV << (int)cnt << ". Issuing Read Record command (record = " << r << ", sfi = " << sfi_integer << ")\n";
                TRYEMV << "  [C] 00 B2 \n";

                emv_status = emvReadRecord(r_apdu, &Ne, afl_list_item->sfi, r, sw);

                if (emv_status == EMV_OK)
                {

                    emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                    if (emv_status == EMV_OK)
                    {

                        tail->next = temp;
                        tail = tail->next;
                    }

                    if (Ne)
                    {

                        TRYEMV << "\nAPDU command executed: response data length = " << (int)Ne;

                        strcpy(RESPONSE, empty_string);

                        for (uint32_t i = 0; i < Ne; i++)
                        {

                            response_integer[i] = r_apdu[i];
                        }

                        for (uint32_t j = 0; j < Ne; j++)
                        {

                            dec_to_hex(response_integer[j], RESPONSE);
                        }

                        TRYEMV << "\n[R] : " << RESPONSE;

                        if (r == 1 && afl_list_item->sfi == 2)
                        {
                            CCN1 = "";
                            CCN2 = "";
                            CCN3 = "";
                            CCN4 = "";
                            CCN1 << RESPONSE[10] << RESPONSE[11] << RESPONSE[12] << RESPONSE[13];
                            CCN2 << RESPONSE[14] << RESPONSE[15] << RESPONSE[16] << RESPONSE[17];
                            CCN3 << RESPONSE[18] << RESPONSE[19] << RESPONSE[20] << RESPONSE[21];
                            CCN4 << RESPONSE[22] << RESPONSE[23] << RESPONSE[24] << RESPONSE[25];
                        }
                    }

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%02X", sw_int[0]);
                    sprintf(sw_string2, "%02X", sw_int[1]);

                    TRYEMV << "\n [SW] : " << sw_string1 << " " << sw_string2;
                }
                else
                {

                    if (*sw16_ptr != 0x90)
                    {

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%X", sw_int[0]);
                        sprintf(sw_string2, "%X", sw_int[1]);

                        TRYEMV << "\nCould not continue execution due to an APDU error"
                               << "\n[SW] : " << sw_string1 << " " << sw_string2;
                    }
                }

                cnt++;
            }

            afl_list_item = afl_list_item->next;
        }

    } while (0);

    TRYEMV << "\n--------------------------------------------------------------------------------------------------------------------\n";

    if (afl_list)
        emvAflListCleanup(afl_list);
    if (gpo_data_field)
        free(gpo_data_field);
    if (head)
        emvTreeCleanup(head);

    TRYEMV << "=====================================================================\n";

    s_block_deselect(100);
}

void apdu_credit_card_exampleFrame::OnButton6Click(wxCommandEvent &event)
{

    if (RadioButton1->GetValue())
    {
        tryEmvPseCardRead("1PAY.SYS.DDF01", "PSE1");

        EMV_CARD->SetValue(TRYEMV);
    }
    else if (RadioButton2->GetValue())
    {

        tryEmvPseCardRead("2PAY.SYS.DDF01", "PSE2");

        EMV_CARD->SetValue(TRYEMV);
    }
    else
    {
        wxString msg = "";
        msg << "You must select Payment System Enviroment ";
        wxMessageBox(msg, _("ERROR"));
    }
}

void apdu_credit_card_exampleFrame::OnButton7Click(wxCommandEvent &event)
{

    EMV_CARD->SetValue("");
    TRYEMV = "";
}

void apdu_credit_card_exampleFrame::OnButton8Click(wxCommandEvent &event)
{

    if (RadioButton1->GetValue())
    {

        tryEmvPseLog("1PAY.SYS.DDF01", "PSE1");

        EMV_LOG->SetValue(EMVLOG);

        for (int column = 0; column < 5; column++)
        {

            for (int row = 0; row < 10; row++)
            {

                Transactions_Grid->SetCellValue(row, column, "");
            }
        }
    }
    else if (RadioButton2->GetValue())
    {

        tryEmvPseLog("2PAY.SYS.DDF01", "PSE2");

        EMV_LOG->SetValue(EMVLOG);

        for (int column = 0; column < 5; column++)
        {

            for (int row = 0; row < 10; row++)
            {

                Transactions_Grid->SetCellValue(row, column, "");
            }
        }
    }
    else
    {

        wxString msg = "";
        msg << "You must select Payment System Enviroment ";
        wxMessageBox(msg, _("ERROR"));
    }
}

void apdu_credit_card_exampleFrame::tryEmvPseLog(const char *df_name, const char *szTitlePse)
{

    EMV_STATUS emv_status;
    UFR_STATUS status;
    bool head_attached = false;
    emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
    emv_tree_node_t *log_list_item = NULL;
    uint8_t *log_list_data = NULL, *log_list_ptr = NULL;
    uint16_t log_list_len;
    uint8_t r_apdu[RAW_RES_MAX_LEN];
    uint32_t Ne; // without SW
    uint8_t sw[2], aid[MAX_AID_LEN];
    uint8_t sfi, log_records, record, cnt = 1, aid_len;
    uint16_t *sw16_ptr = (uint16_t *)&sw;
    char empty_string[RAW_RES_MAX_LEN] = "";

    for (int column = 0; column < 5; column++)
    {

        for (int row = 0; row < 10; row++)
        {

            Transactions_Grid->SetCellValue(row, column, "");
        }
    }

    EMVLOG = "";

    EMVLOG << " ===================================================================\n";
    EMVLOG << " Read and parse EMV log on card supporting " << szTitlePse;
    EMVLOG << "\n--------------------------------------------------------------------------------------------------------------------\n";

    do
    {

        status = SetISO14443_4_Mode();

        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {

            EMVLOG << " Error while switching into ISO 14443-4 mode, uFR status is: " << Status;

            wxString msg = "";
            msg << "Reading error";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        EMVLOG << (int)cnt++ << ". Issuing Select PSE command:" << df_name << "\n";
        EMVLOG << "  [C] 00 A4 04 00 \n";

        Ne = 256;
        status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);

        if (status != UFR_OK)
        {

            EMVLOG << " Error while executing APDU command, uFR status is: " << Status;

            wxString msg = "";
            msg << "Reading error";
            wxMessageBox(msg, _("ERROR"));
            break;
        }
        else
        {

            if (*sw16_ptr != 0x90)
            {

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%X", sw_int[0]);
                sprintf(sw_string2, "%X", sw_int[1]);

                EMVLOG << "\nCould not continue execution due to an APDU error"
                       << "\n[SW] : " << sw_string1 << " " << sw_string2;

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            if (Ne)
            {

                EMVLOG << "\nAPDU command executed: response data length = " << (int)Ne;

                strcpy(RESPONSE, empty_string);

                for (uint32_t i = 0; i < Ne; i++)
                {

                    response_integer[i] = r_apdu[i];
                }

                for (uint32_t j = 0; j < Ne; j++)
                {

                    dec_to_hex(response_integer[j], RESPONSE);
                }

                EMVLOG << "\n[R] : " << RESPONSE;
            }

            sw_int[0] = sw[0];
            sw_int[1] = sw[1];

            sprintf(sw_string1, "%02X", sw_int[0]);
            sprintf(sw_string2, "%02X", sw_int[1]);

            EMVLOG << "\n [SW] : " << sw_string1 << " " << sw_string2;
        }

        emv_status = newEmvTag(&head, r_apdu, Ne, false);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {

            EMVLOG << " EMV parsing error code: " << EMV_STATUS_error;

            wxString msg = "";
            msg << "Reading error";
            wxMessageBox(msg, _("ERROR"));
            break;
        }

        emv_status = getSfi(head, &sfi);

        if (emv_status == EMV_OK)
        {

            // There is SFI:
            record = 1;
            cnt_integer = cnt;
            record_integer = record;
            sfi_integer = sfi;
            sfi_hex = (sfi << 3) | 4;

            sprintf(SFI_HEX, "%02X", sfi_hex);

            do
            {

                EMVLOG << cnt_integer << ". Issuing Read Record command (record = " << record_integer << ", sfi = " << sfi_integer << " )";
                EMVLOG << "\n[C] 00 B2 " << SFI_HEX;

                emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);

                if (emv_status == EMV_OK)
                {

                    emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                    if (record == 1)
                        head->next = tail = temp;
                    else
                    {
                        tail->next = temp;
                        tail = tail->next;
                    }

                    if (Ne)
                    {

                        EMVLOG << "\nAPDU command executed: response data length = " << (int)Ne;

                        strcpy(RESPONSE, empty_string);

                        for (uint32_t i = 0; i < Ne; i++)
                        {

                            response_integer[i] = r_apdu[i];
                        }

                        for (uint32_t j = 0; j < Ne; j++)
                        {

                            dec_to_hex(response_integer[j], RESPONSE);
                        }

                        EMVLOG << "\n[R] : " << RESPONSE;
                    }

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%02X", sw_int[0]);
                    sprintf(sw_string2, "%02X", sw_int[1]);

                    EMVLOG << "\n [SW] : " << sw_string1 << " " << sw_string2;
                }
                else
                {

                    if (*sw16_ptr != 0x90)
                    {

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%X", sw_int[0]);
                        sprintf(sw_string2, "%X", sw_int[1]);

                        EMVLOG << "\nThere is no records.\n"
                               << "\n[SW] : " << sw_string1 << " " << sw_string2;
                    }
                }

                record++;
                cnt++;

            } while (emv_status == EMV_OK);
        }

        emv_status = getAid(head, aid, &aid_len);

        if (emv_status == EMV_OK)
        {

            EMVLOG << " \n--------------------------------------------------------------------------------------------------------------------\n";
            EMVLOG << (int)cnt++ << ". Issuing Select the application command:\n";
            EMVLOG << "  [C] 00 A4 04 00 ";

            Ne = 256;
            status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, aid, aid_len, r_apdu, &Ne, 1, sw);
            Status = UFR_Status2String(status);

            if (status != UFR_OK)
            {

                EMVLOG << " Error while executing APDU command, uFR status is: " << Status;

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }
            else
            {

                if (*sw16_ptr != 0x90)
                {

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%X", sw_int[0]);
                    sprintf(sw_string2, "%X", sw_int[1]);

                    EMVLOG << "\nCould not continue execution due to an APDU error"
                           << "\n[SW] : " << sw_string1 << " " << sw_string2;

                    wxString msg = "";
                    msg << "Reading error";
                    wxMessageBox(msg, _("ERROR"));
                    break;
                }

                if (Ne)
                {
                    EMVLOG << "\nAPDU command executed: response data length = " << (int)Ne;

                    strcpy(RESPONSE, empty_string);

                    for (uint32_t i = 0; i < Ne; i++)
                    {

                        response_integer[i] = r_apdu[i];
                    }

                    for (uint32_t j = 0; j < Ne; j++)
                    {

                        dec_to_hex(response_integer[j], RESPONSE);
                    }

                    EMVLOG << "\n[R] : " << RESPONSE;
                }

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%02X", sw_int[0]);
                sprintf(sw_string2, "%02X", sw_int[1]);

                EMVLOG << "\n [SW] : " << sw_string1 << " " << sw_string2;
            }

            emv_status = newEmvTag(&temp, r_apdu, Ne, false);

            sprintf(EMV_STATUS_error, "%08X", emv_status);

            if (emv_status)
            {

                EMVLOG << " EMV parsing error code: " << EMV_STATUS_error;

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            if (!head_attached)
            {
                head->next = tail = temp;
                head_attached = true;
            }
            else
            {
                tail->next = temp;
                tail = tail->next;
            }
        }

        emv_status = getLogEntry(temp, &sfi, &log_records);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status != EMV_OK)
        {

            EMVLOG << "Can't find Log Entry\n\n";
            EMVLOG << " EMV parsing error code: " << EMV_STATUS_error;
        }
        else
        {

            EMVLOG << "\n--------------------------------------------------------------------------------------------------------------------\n";
            EMVLOG << (int)cnt++ << ". Issuing Get Log Format command:\n";
            EMVLOG << "  [C] 80 CA 9F 4F 00\n";

            Ne = 256;
            status = APDUTransceive(0x80, 0xCA, 0x9F, 0x4F, NULL, 0, r_apdu, &Ne, 1, sw);

            if (status != UFR_OK)
            {

                EMVLOG << " Error while executing APDU command, uFR status is: " << Status;

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }
            else
            {

                if (*sw16_ptr != 0x90)
                {

                    sw_int[0] = sw[0];
                    sw_int[1] = sw[1];

                    sprintf(sw_string1, "%X", sw_int[0]);
                    sprintf(sw_string2, "%X", sw_int[1]);

                    EMVLOG << "\nCould not continue execution due to an APDU error"
                           << "\n[SW] : " << sw_string1 << " " << sw_string2;

                    wxString msg = "";
                    msg << "Reading error";
                    wxMessageBox(msg, _("ERROR"));
                    break;
                }

                if (Ne)
                {
                    EMVLOG << "\nAPDU command executed: response data length = " << (int)Ne;

                    strcpy(RESPONSE, empty_string);

                    for (uint32_t i = 0; i < Ne; i++)
                    {

                        response_integer[i] = r_apdu[i];
                    }

                    for (uint32_t j = 0; j < Ne; j++)
                    {

                        dec_to_hex(response_integer[j], RESPONSE);
                    }

                    EMVLOG << "\n[R] : " << RESPONSE;
                }

                sw_int[0] = sw[0];
                sw_int[1] = sw[1];

                sprintf(sw_string1, "%02X", sw_int[0]);
                sprintf(sw_string2, "%02X", sw_int[1]);

                EMVLOG << "\n [SW] : " << sw_string1 << " " << sw_string2;
            }

            emv_status = newEmvTag(&temp, r_apdu, Ne, false);

            if (emv_status)
            {

                EMVLOG << " EMV parsing error code: " << EMV_STATUS_error;

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            tail->next = temp;
            tail = tail->next;

            log_list_item = temp->tl_list_format;

            if (!log_list_item)
            {

                EMVLOG << "Can't find Log Entry\n";

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            emv_status = getListLength(temp, &log_list_len);

            if (emv_status != EMV_OK)
            {

                EMVLOG << "Can't find Log Entry\n";

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            log_list_data = (uint8_t *)malloc(log_list_len * log_records);

            if (!log_list_data)
            {

                EMVLOG << "Out of memory error\n\n";

                wxString msg = "";
                msg << "Reading error";
                wxMessageBox(msg, _("ERROR"));
                break;
            }

            memset(log_list_data, 0, log_list_len);

            // There is SFI:

            for (record = 1; record < log_records + 1; record++)
            {

                cnt_integer = cnt;
                record_integer = record;
                sfi_integer = sfi;
                sfi_hex = (sfi << 3) | 4;

                sprintf(SFI_HEX, "%02X", sfi_hex);

                EMVLOG << "\n--------------------------------------------------------------------------------------------------------------------\n";
                EMVLOG << cnt_integer << ". Issuing Read Record command (record = " << record_integer << ", sfi = " << sfi_integer << " )";
                EMVLOG << "\n[C] 00 B2 " << SFI_HEX;

                emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);

                if (emv_status == EMV_OK)
                {

                    if (Ne)
                    {

                        memcpy(&log_list_data[(record - 1) * log_list_len], r_apdu, log_list_len);

                        EMVLOG << "\nAPDU command executed: response data length = " << (int)Ne;

                        strcpy(RESPONSE, empty_string);

                        for (uint32_t i = 0; i < Ne; i++)
                        {

                            response_integer[i] = r_apdu[i];
                        }

                        for (uint32_t j = 0; j < Ne; j++)
                        {

                            dec_to_hex(response_integer[j], RESPONSE);
                        }

                        EMVLOG << "\n[R] : " << RESPONSE;

                        if (*sw16_ptr == 0x90)
                        {
                            {

                                uint16_t ATCounter = 0;
                                uint16_t ATCounter_pos;
                                uint16_t ATCounter_len;
                                bool ATCounter_exist = isExistATCounter(log_list_item, &ATCounter_pos, &ATCounter_len);

                                if (log_list_data)
                                {

                                    log_list_ptr = log_list_data;

                                    for (int i = 0; i < log_records; i++)
                                    {

                                        if (ATCounter_exist)
                                        {

                                            if (ATCounter_len == 2)
                                                ATCounter = ((uint16_t)(log_list_ptr[ATCounter_pos]) << 8) + log_list_ptr[ATCounter_pos + 1];
                                            LOG_VIEW = "";
                                            LOG_VIEW << ATCounter;
                                            Transactions_Grid->SetCellValue(i, 0, LOG_VIEW);
                                        }

                                        log_list_ptr += log_list_len;
                                    }
                                }
                            }

                            LOG_VIEW = "";
                            LOG_VIEW << RESPONSE[22] << RESPONSE[23];
                            LOG_VIEW << "." << RESPONSE[20] << RESPONSE[21];
                            LOG_VIEW << ".20" << RESPONSE[18] << RESPONSE[19];

                            Transactions_Grid->SetCellValue(record_integer - 1, 1, LOG_VIEW);

                            LOG_VIEW = "";
                            LOG_VIEW << RESPONSE[42] << RESPONSE[43] << ":";
                            LOG_VIEW << RESPONSE[44] << RESPONSE[45] << ":";
                            LOG_VIEW << RESPONSE[46] << RESPONSE[47];

                            Transactions_Grid->SetCellValue(record_integer - 1, 2, LOG_VIEW);

                            LOG_VIEW = "";

                            int position = 0;

                            for (int i = 2; i < 12 && RESPONSE[i] == '0'; i++)
                            {

                                position = i;
                            }

                            if (RESPONSE[position + 1] != '0')
                            {

                                for (int j = position + 1; j < 12; j++)
                                {

                                    LOG_VIEW << RESPONSE[j];
                                }
                            }
                            else if (RESPONSE[position] == '0')
                            {

                                LOG_VIEW << "0";
                            }

                            uint32_t tmp_uint32;
                            uint16_t TransactionCurrency_len;
                            uint16_t TransactionCurrency_pos;
                            bool TransactionCurrency_exist = isExistTransactionCurrency(log_list_item, &TransactionCurrency_pos, &TransactionCurrency_len);

                            LOG_VIEW << "." << RESPONSE[12] << RESPONSE[13];

                            Transactions_Grid->SetCellValue(record_integer - 1, 3, LOG_VIEW);

                            LOG_VIEW = "";

                            log_list_ptr = log_list_data;

                            if (TransactionCurrency_exist)
                            {

                                tmp_uint32 = findCurrencyIndexByNumCode(bin_bcd_to_i(&log_list_ptr[TransactionCurrency_pos], 2));

                                wxString currency = "";

                                currency << iso4217_currency_codes[tmp_uint32].alpha_code;

                                LOG_VIEW << currency;

                                Transactions_Grid->SetCellValue(record_integer - 1, 4, LOG_VIEW);

                                LOG_VIEW = "";
                            }
                        }

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%02X", sw_int[0]);
                        sprintf(sw_string2, "%02X", sw_int[1]);

                        EMVLOG << "\n [SW] : " << sw_string1 << " " << sw_string2;
                    }
                }
                else
                {

                    if (*sw16_ptr != 0x90)
                    {

                        sw_int[0] = sw[0];
                        sw_int[1] = sw[1];

                        sprintf(sw_string1, "%X", sw_int[0]);
                        sprintf(sw_string2, "%X", sw_int[1]);

                        EMVLOG << "\nThere is no records.\n"
                               << "\n[SW] : " << sw_string1 << " " << sw_string2;

                        LOG_VIEW << "0";

                        Transactions_Grid->SetCellValue(record_integer - 1, 0, LOG_VIEW);

                        LOG_VIEW = "";

                        LOG_VIEW << "00.00.0000";

                        Transactions_Grid->SetCellValue(record_integer - 1, 1, LOG_VIEW);

                        LOG_VIEW = "";

                        LOG_VIEW << "00:00:00";

                        Transactions_Grid->SetCellValue(record_integer - 1, 2, LOG_VIEW);

                        LOG_VIEW = "";

                        LOG_VIEW << "0.00";

                        Transactions_Grid->SetCellValue(record_integer - 1, 3, LOG_VIEW);

                        LOG_VIEW = "";

                        LOG_VIEW << "---";

                        Transactions_Grid->SetCellValue(record_integer - 1, 4, LOG_VIEW);

                        LOG_VIEW = "";
                    }
                }

                cnt++;
                log_list_ptr += log_list_len;
            }
        }

    } while (0);

    if (head)
        emvTreeCleanup(head);
    if (log_list_data)
    {
        free(log_list_data);
        log_list_data = NULL;
    }

    s_block_deselect(100);
}

void apdu_credit_card_exampleFrame::OnButton9Click(wxCommandEvent &event)
{

    EMVLOG = "";
    EMV_LOG->SetValue("");
}

uint32_t hex2int(char *hex)
{

    uint32_t val = 0;

    while (*hex)
    {
        uint8_t byte = *hex++;

        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;

        val = (val << 4) | (byte & 0xF);
    }

    return val;
}

void apdu_credit_card_exampleFrame::OnButton10Click2(wxCommandEvent &event)
{
    if (RadioButton1->GetValue())
    {

        GetCreditCardNumber("1PAY.SYS.DDF01", "PSE1");

        CC_n1->SetValue(CCN1.substr(0, 4));
        CC_n2->SetValue(CCN2.substr(0, 4));
        CC_n3->SetValue(CCN3.substr(0, 4));
        CC_n4->SetValue(CCN4.substr(0, 4));

        Transactions_Grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

        tryEmvPseLog("1PAY.SYS.DDF01", "PSE1");
    }
    else if (RadioButton2->GetValue())
    {

        GetCreditCardNumber("2PAY.SYS.DDF01", "PSE2");

        CC_n1->SetValue(CCN1.substr(0, 4));
        CC_n2->SetValue(CCN2.substr(0, 4));
        CC_n3->SetValue(CCN3.substr(0, 4));
        CC_n4->SetValue(CCN4.substr(0, 4));

        Transactions_Grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

        tryEmvPseLog("2PAY.SYS.DDF01", "PSE2");
    }
    else
    {

        wxString msg = "";
        msg << "You must select Payment System Enviroment";
        wxMessageBox(msg, _("ERROR"));
    }
}

void apdu_credit_card_exampleFrame::OnButton11Click2(wxCommandEvent &event)
{
    CC_n1->SetValue("");
    CC_n2->SetValue("");
    CC_n3->SetValue("");
    CC_n4->SetValue("");

    CCN1 = "";
    CCN2 = "";
    CCN3 = "";
    CCN4 = "";

    LOG_VIEW = "";

    for (int column = 0; column < 5; column++)
    {

        for (int row = 0; row < 10; row++)
        {

            Transactions_Grid->SetCellValue(row, column, "");
        }
    }
}

void GetCreditCardNumber(const char *df_name, const char *szTitlePse)
{

    EMV_STATUS emv_status;
    UFR_STATUS status;
    bool head_attached = false;
    emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
    afl_list_item_t *afl_list = NULL, *afl_list_item = NULL;
    uint8_t afl_list_count;
    uint8_t r_apdu[RAW_RES_MAX_LEN];
    uint32_t Ne; // without SW
    uint8_t sw[2], aid[MAX_AID_LEN];
    uint8_t sfi, record, cnt = 1, aid_len;
    uint16_t *sw16_ptr = (uint16_t *)&sw;
    uint8_t *gpo_data_field = NULL;
    uint16_t gpo_data_field_size;
    char empty_string[RAW_RES_MAX_LEN] = "";
    CCN1 = "";
    CCN2 = "";
    CCN3 = "";
    CCN4 = "";

    cnt = 1;

    do
    {

        status = SetISO14443_4_Mode();

        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {
            break;
        }

        Ne = 256;
        status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);
        Status = UFR_Status2String(status);

        if (status != UFR_OK)
        {
            break;
        }
        else
        {

            if (*sw16_ptr != 0x90)
            {
                break;
            }
        }

        emv_status = newEmvTag(&head, r_apdu, Ne, false);

        sprintf(EMV_STATUS_error, "%08X", emv_status);

        if (emv_status)
        {
            break;
        }

        emv_status = getSfi(head, &sfi);

        if (emv_status == EMV_OK)
        {

            cnt = 2;
            record = 1;

            cnt_integer = cnt;
            record_integer = record;
            sfi_integer = sfi;
            sfi_hex = (sfi << 3) | 4;

            do
            {

                emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);

                if (emv_status == EMV_OK)
                {

                    emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                    if (record == 1)
                        head->next = tail = temp;
                    else
                    {
                        tail->next = temp;
                        tail = tail->next;
                    }
                }

                record++;
                cnt++;

            } while (emv_status == EMV_OK);
        }

        emv_status = getAid(head, aid, &aid_len);

        if (emv_status == EMV_OK)
        {

            Ne = 256;
            status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, aid, aid_len, r_apdu, &Ne, 1, sw);
            Status = UFR_Status2String(status);

            if (status != UFR_OK)
            {
                break;
            }
            else
            {

                if (*sw16_ptr != 0x90)
                {
                    break;
                }
            }
        }

        emv_status = newEmvTag(&temp, r_apdu, Ne, false);

        if (emv_status)
        {
            break;
        }

        if (!head_attached)
        {
            head->next = tail = temp;
            head_attached = true;
        }
        else
        {
            tail->next = temp;
            tail = tail->next;
        }

        emv_status = formatGetProcessingOptionsDataField(temp, &gpo_data_field, &gpo_data_field_size);

        if (emv_status)
        {
            break;
        }

        Ne = 256;
        status = APDUTransceive(0x80, 0xA8, 0x00, 0x00, gpo_data_field, gpo_data_field_size, r_apdu, &Ne, 1, sw);

        if (status != UFR_OK)
        {
            break;
        }
        else
        {

            if (*sw16_ptr != 0x90)
            {
                break;
            }
        }

        emv_status = newEmvTag(&temp, r_apdu, Ne, false);

        if (emv_status)
        {
            break;
        }

        tail->next = temp;
        tail = tail->next;

        emv_status = getAfl(temp, &afl_list, &afl_list_count);

        if (emv_status == EMV_ERR_TAG_NOT_FOUND)

            emv_status = getAflFromResponseMessageTemplateFormat1(temp, &afl_list, &afl_list_count);

        if (emv_status)
        {

            break;
        }

        afl_list_item = afl_list;

        while (afl_list_item)
        {

            for (int r = afl_list_item->record_first; r <= afl_list_item->record_last; r++)
            {

                emv_status = emvReadRecord(r_apdu, &Ne, afl_list_item->sfi, r, sw);

                if (emv_status == EMV_OK)
                {

                    emv_status = newEmvTag(&temp, r_apdu, Ne, false);

                    if (emv_status == EMV_OK)
                    {

                        tail->next = temp;
                        tail = tail->next;
                    }

                    if (Ne)
                    {

                        strcpy(RESPONSE, empty_string);

                        for (uint32_t i = 0; i < Ne; i++)
                        {

                            response_integer[i] = r_apdu[i];
                        }

                        for (uint32_t j = 0; j < Ne; j++)
                        {

                            dec_to_hex(response_integer[j], RESPONSE);
                        }

                        TRYEMV << "\n[R] : " << RESPONSE;

                        if (r == 1 && afl_list_item->sfi == 2)
                        {
                            CCN1 = "";
                            CCN2 = "";
                            CCN3 = "";
                            CCN4 = "";
                            CCN1 << RESPONSE[10] << RESPONSE[11] << RESPONSE[12] << RESPONSE[13];
                            CCN2 << RESPONSE[14] << RESPONSE[15] << RESPONSE[16] << RESPONSE[17];
                            CCN3 << RESPONSE[18] << RESPONSE[19] << RESPONSE[20] << RESPONSE[21];
                            CCN4 << RESPONSE[22] << RESPONSE[23] << RESPONSE[24] << RESPONSE[25];
                        }
                    }
                }

                cnt++;
            }

            afl_list_item = afl_list_item->next;
        }

    } while (0);

    TRYEMV << "\n--------------------------------------------------------------------------------------------------------------------\n";

    if (afl_list)
        emvAflListCleanup(afl_list);
    if (gpo_data_field)
        free(gpo_data_field);
    if (head)
        emvTreeCleanup(head);

    TRYEMV << "=====================================================================\n";

    s_block_deselect(100);
}
