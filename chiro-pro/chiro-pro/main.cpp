//includes
// wxprec is requierd for the wx widgets files to be found
// app.h, Buttons.h and frame.h are included files that define methods writen in this file
// fstream allow us to open and close certain files
//ctime gives us date and time functions
#include "app.h"
#include "Buttons.h"
#include "Frame.h"
#include <wx/wfstream.h>
#include <wx/wxprec.h>
#include <fstream>
#include <ctime>
#include <time.h>
#include <chrono>
using namespace std;

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

////////////////////////////////////event table////////////////////////////////////////////
// this event table connects widget ids to functions.
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(MENU_New, MainFrame::newFile)
EVT_MENU(MENU_Open, MainFrame::openFile)
EVT_MENU(MENU_Close, MainFrame::closeFile)
EVT_MENU(MENU_Save, MainFrame::saveFile)
EVT_MENU(MENU_SaveAs, MainFrame::saveFileAs)
EVT_MENU(MENU_Quit, MainFrame::quit)
EVT_BUTTON(BUTTON_Add, MainFrame::AddButton)
EVT_BUTTON(BUTTON_Write, MainFrame::ButtonWrite)
EVT_BUTTON(BUTTON_Sign, MainFrame::Sign)
EVT_BUTTON(BUTTON_Swap, MainFrame::swapButtonPanels)
EVT_MENU(MENU_EditButtonName, MainFrame::onPopUpCLick)
EVT_MENU(MENU_EditButtonText, MainFrame::onPopUpCLick)
//EVT_RIGHT_DOWN( MainFrame::onRightClick)
END_EVENT_TABLE()
std::string getDateToSign();
//time_t getDate();
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////variables/////////////////////////////////////////
// we add a fleg grid for the dynamicly added buttons, two strings that are for filenames one is hardcoded. 
//2 streams for reading and writing files



/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////main application///////////////////////////
IMPLEMENT_APP(MainApplication) // Initializes the MainApplication class and tells our program
// to run it
bool MainApplication::OnInit()
{
    //main frame constructor
    MainFrame* MainWin = new MainFrame(wxT("DocuMaster"), wxPoint(1, 1),
        wxSize(300, 200)); // Create an instance of our frame, or window
    MainWin->Show(TRUE); // show the window
    SetTopWindow(MainWin);// and finally, set it as the main window
    
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////main frame method/////////////////////////

/// <summary>
/// 
/// </summary>
/// <param name="title"></param>
/// <param name="pos"></param>
/// <param name="size"></param>
MainFrame::MainFrame(const wxString& title,
    const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
    
    //-----------menu-status start-----------
    //statusbar at the bottom of page has 2 cells
    //menu bar at the top ofthe page has 1 tab named File menu and visualy represented as File
    //the File tab has 6 options: new file, open file,close file, save file, save file as, and quit.
    CreateStatusBar(2);
    MainMenu = new wxMenuBar();
    wxMenu* FileMenu = new wxMenu();

    FileMenu->Append(MENU_New, wxT("&New"),
        wxT("Create a new file"));
    FileMenu->Append(MENU_Open, wxT("&Open"),
        wxT("Open an existing file"));
    FileMenu->Append(MENU_Close, wxT("&Close"),
        wxT("Close the current document"));
    FileMenu->Append(MENU_Save, wxT("&Save"),
        wxT("Save the current document"));
    FileMenu->Append(MENU_SaveAs, wxT("Save &As"),
        wxT("Save the current document under a new file name"));
    FileMenu->Append(MENU_Quit, wxT("Save button layout and &Quit"),
        wxT("Quit the editor"));

    MainMenu->Append(FileMenu, wxT("File"));
    SetMenuBar(MainMenu);
    ////////////////////////menu-status end////////////////////////

    //make 6 buttons, one to add dynamic buttons, one to sign the document
    // 4 buttons to swap panels of dynamic buttons
   
    wxButton* AddButton = new wxButton(this, BUTTON_Add, "add", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "add");
    wxButton* SignButton = new wxButton(this, BUTTON_Sign, "Sign", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "Apply Signiture to document");
    wxButton* swapToPanelOne = new wxButton(this, BUTTON_Swap, "Panel 1", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
    wxButton* swapToPanelTwo = new wxButton(this, BUTTON_Swap, "Panel 2", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "2");
    wxButton* swapToPanelThree = new wxButton(this, BUTTON_Swap, "Panel 3", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "3");
    wxButton* swapToPanelFour = new wxButton(this, BUTTON_Swap, "Panel 4", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "4");
    //make a box sizer to add the buttons too
    ControlSizer = new wxBoxSizer(wxHORIZONTAL);
    //add the buttons to the box sizer
    ControlSizer->Add(AddButton);
    ControlSizer->Add(SignButton);
    ControlSizer->Add(swapToPanelOne);
    ControlSizer->Add(swapToPanelTwo);
    ControlSizer->Add(swapToPanelThree);
    ControlSizer->Add(swapToPanelFour);
    ///////////////////////////////////////////////////////////////
    
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////text editor/////////////////////////////////
    //add a wxtextctrl this is the main text editor
    MainEditBox = new wxTextCtrl(this, TEXT_Main,
        wxT(""), wxDefaultPosition, wxSize(600,1),
        wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
    Maximize();


    ///////////////////////////button layout//////////////////////////////
    buttons1 = new ButtonPanel(this, "button-List", "layouts/layout.txt");
    buttons2 = new ButtonPanel(this, "button-List2", "layouts/layout2.txt");
    buttons2->Hide();
    buttons3 = new ButtonPanel(this, "button-List3", "layouts/layout3.txt");
    buttons3->Hide();
    buttons4 = new ButtonPanel(this, "button-List4", "layouts/layout4.txt");
    buttons4->Hide();
    currentPanel = buttons1;
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    /////dynamic button loading. this reads the layout files and addes the content to the panels/////////
   
    buttons1->LoadButtons();
    buttons2->LoadButtons();
    buttons3->LoadButtons();
    buttons4->LoadButtons();
    /////////////////////////////////////////////////////////////////
    ///////////////////////// arange sizers//////////////////////////

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(ControlSizer, 0, wxALIGN_CENTER);
    sizer->Add(currentPanel, 1, wxEXPAND);
    sizer->Add(MainEditBox, 8, wxALIGN_CENTER,wxBORDER);
    
    //set the mainsizer as the frame sizer to align content
    //and center the layout
    sizer->Layout();
    SetSizer(sizer);
    Centre();
}
/////////////////////////////////////////////////////////////////
/////////////////////end Frame Constructor/////////////
////////////////////////methods////////////////////////
wxString MainFrame::getFilename() {
    return this->fileName;
}
void MainFrame::setFilename(wxString name) {
    this->fileName = name;
}
void MainFrame::appendFilename(wxString app) {
    this->fileName += app;
}
wxButton* MainFrame::getButtonToEdit() {
    return ButtonToEdit;
}
void MainFrame::setButtonToEditName(wxString Name){
    this->getButtonToEdit()->SetLabel(Name);
}
void MainFrame::setButtonToEditText(wxString Text) {
    this->getButtonToEdit()->SetName(Text);
}
bool MainFrame::isSigned() {
    return this->Signed;
}
bool MainFrame::setSigned(bool Sign) {
    this->Signed = Sign;
}

/// <summary>
/// //new file method, this method clears the texteditor and filename for saving.
///allowing people to make and save new files
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::newFile(wxCommandEvent& WXUNUSED(event))
{
    if (confirmIntent("are you sure you want to open a new file?")) {

        MainEditBox->Clear();
        MainFrame::setFilename("");
    }
}
/// </summary>
/// this method opens a dialog box that lets the user search thier file system 
/// and open any.doc file
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::openFile(wxCommandEvent& WXUNUSED(event))
{
    if (confirmIntent("are you sure you want to open a new file and close this one?")) {


        wxFileDialog
            openFileDialog(this, _("Open Doc file"), "", "",
                "Doc files (*.doc)|*.doc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;     // the user changed idea...

        // proceed loading the file chosen by the user;
        // this can be done with e.g. wxWidgets input streams:
        wxFileInputStream input_stream(openFileDialog.GetPath());

        if (!input_stream.IsOk())
        {
            wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
            return;
        }
        wxString file = openFileDialog.GetFilename();

        MainEditBox->LoadFile(file);
    }
}

/// <summary>
/// this is esentualy the same as new file. clearing the text editor and the filename
/// too let the user have a blank file
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::closeFile(wxCommandEvent& WXUNUSED(event))
{
    if (confirmIntent("are you sure you want to close the file?")) {
        MainEditBox->DiscardEdits();
        MainEditBox->Clear();
        MainFrame::setFilename("");
    }
}

/// <summary>
/// if filename variable is empty prompt the user to enter a name then add .doc to the end
/// if there is already a name save under the name.
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::saveFile(wxCommandEvent& WXUNUSED(event))
{
    //SignAndSave();
    
    if (MainFrame::getFilename().empty()) {
        MainFrame::setFilename(
            wxGetTextFromUser("Enter Name of File.do not include file extension", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true));

        MainFrame::appendFilename(".doc");
     
            MainEditBox->SaveFile(MainFrame::getFilename());
    }
    else {
        MainEditBox->SaveFile(MainFrame::getFilename());
    }
    
}

/// <summary>
/// save a file with a custom name regardless of if there is filename saved.
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::saveFileAs(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog
        saveFileDialog(this, _("Save Doc file"), "", "",
            "Doc files (*.doc)|*.doc", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    // save the current contents in the file;
    // this can be done with e.g. wxWidgets output streams:
    wxFileOutputStream output_stream(saveFileDialog.GetPath());
    setFilename(saveFileDialog.GetFilename());
    if (!output_stream.IsOk())
    {
        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }
}
/// <summary>
/// the quit function saves the button configuration to a file
/// then closes the program
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::quit(wxCommandEvent& event)
{
    buttons1->saveButtons();
    buttons2->saveButtons();
    buttons3->saveButtons();
    buttons4->saveButtons();
    
    event.Skip();
    
    Close(TRUE); // Tells the OS to quit running this process
}

/// <summary>
/// the function adds a button to the topbar. these are called dynamic buttons
/// each button will need a lable and text, these are prompted from the user
/// the button is added and the layout updated
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::AddButton(wxCommandEvent& event)
{
    //promp user for name of button and button text
    wxString buttonName=
    wxGetTextFromUser("Enter text for button name", " ","enter here",NULL,wxDefaultCoord, wxDefaultCoord,true);
    wxString buttonText =
    wxGetTextFromUser("Enter text for button to add", "", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
    // make button and add the button to array
    currentPanel->QlinkList[QLinkIndex] = new wxButton(currentPanel, BUTTON_Write, buttonName,wxDefaultPosition,wxSize(80.5,23),0,wxDefaultValidator,buttonText);
    currentPanel->QlinkList[QLinkIndex]->Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick,this);

    //add the button to the button sizer
    currentPanel->ButtonSizer->Add(currentPanel->QlinkList[QLinkIndex],wxLeft);
    //update layout of page and button index
   
    currentPanel->ButtonSizer->Layout();
    currentPanel->Layout();
    currentPanel->QLinkIndex++;
}
/// <summary>////////////////////////////////////////////////
/// this function makes a button write its texr to the texteditor/
/// </summary>/////////////////////////////////////////////////
/// <param name="event"></param>
void MainFrame::ButtonWrite(wxCommandEvent& event) {
    wxButton* temp = (wxButton*)event.GetEventObject();
 
    MainEditBox->WriteText("\n" + temp->GetName() + "\n");

}/////////////////////////////end button write///////////////////
/// <summary>
/// this function creates a dialog and asks the user if this action is really what they want.
/// if yes it returns true, if no it returns false.
/// </summary>
/// <param name="message"></param>
/// <returns></returns>
bool MainFrame::confirmIntent(wxString message) {

    wxMessageDialog dialog(NULL, message+wxT("unsaved progress will be lost."), wxT("Question"),
        wxNO_DEFAULT | wxYES_NO | wxICON_INFORMATION); 
    switch (dialog.ShowModal()) { 
    case wxID_YES: 
        wxLogStatus(wxT("You pressed \"Yes\"")); 
        return true;
        break;
    case wxID_NO:
        wxLogStatus(wxT("You pressed \"No\"")); 
        return false;
        break;   
    default:       wxLogError(wxT("Unexpected wxMessageDialog return code!")); }
    return false;
}
/// <summary>
/// adds a Name, and date+time to the file, as a time stamp and signiture
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::Sign(wxCommandEvent& WXUNUSED(event)) {
    
        MainEditBox->WriteText("\n" + (wxString)"G Marks" + "\n" + getDateToSign());
    
}
/// <summary>
/// there are 4 panels 3 of witch are always hidden, this function changes which ones are hidden
/// this effectifly swaps the panels, allowing the user to have an alternitive set of buttons.
/// </summary>
/// <param name="event"></param>
void MainFrame::swapButtonPanels(wxCommandEvent& event) {
    int x = 0;
    MainFrame::sizer->Detach(currentPanel);
    //MainFrame::sizer->Detach(ControlSizer);
    wxButton* temp = (wxButton*)event.GetEventObject();
    wxString switchVariable = temp->GetName();
    x = stoi((string)switchVariable);
    switch (x) {
    case 1:
        MainFrame::sizer->Detach(ControlSizer);
        currentPanel = buttons1;

        buttons2->Hide();
        buttons3->Hide();
        buttons4->Hide();

        MainFrame::sizer->Prepend(currentPanel, 1, wxEXPAND);
        MainFrame::sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        buttons1->Show();
        break;

    case 2:
        MainFrame::sizer->Detach(ControlSizer);
        currentPanel = buttons2;

        buttons1->Hide();
        buttons3->Hide();
        buttons4->Hide();

        MainFrame::sizer->Prepend(currentPanel, 1, wxEXPAND);
        MainFrame::sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        currentPanel->Show();
        break;

    case 3:
        MainFrame::sizer->Detach(ControlSizer);
        currentPanel = buttons3;

        buttons1->Hide();
        buttons2->Hide();
        buttons4->Hide();

        MainFrame::sizer->Prepend(currentPanel, 1, wxEXPAND);
        MainFrame::sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);

        buttons3->Show();
        break;
    case 4:
        MainFrame::sizer->Detach(ControlSizer);
        currentPanel = buttons4;

        buttons1->Hide();
        buttons2->Hide();
        buttons3->Hide();

        MainFrame::sizer->Prepend(currentPanel, 1, wxEXPAND);
        MainFrame::sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        buttons4->Show();
        break;
    }
    sizer->Layout();
}
/// <summary>
/// this function returns the current date and time 
/// </summary>
/// <returns></returns>
std::string getDateToSign() {
    time_t current;
    time(&current);
    
    return  ctime(&current) ;
}
/// <summary>
/// this function allows the user to make a right click on a button and offer them 2 choices edit lable or edit text
/// </summary>
/// <param name="event"></param>
void MainFrame::onRightClick(wxMouseEvent& event) {

    ButtonToEdit = (wxButton*)event.GetEventObject();

    wxMenu menu;
    menu.Append(MENU_EditButtonName, "edit Label of Button");
    menu.Append(MENU_EditButtonText, "edit text of Button");
    PopupMenu(&menu);
}
/// <summary>
/// this function interprites the command from onRightCLick, and processes it accoridingly 
/// </summary>
/// <param name="event"></param>
void MainFrame::onPopUpCLick(wxCommandEvent& event) {
    wxString buttonName;
    wxString buttonText;
    switch (event.GetId()) {
        case MENU_EditButtonName:
            buttonName =
            
            wxGetTextFromUser("Enter text for button name", " ", ButtonToEdit->GetLabel(), NULL, wxDefaultCoord, wxDefaultCoord, true);
            if (!buttonName.IsEmpty()) {
                MainFrame::setButtonToEditName(buttonName);
            }
            break;
        case MENU_EditButtonText:
            buttonText =
            wxGetTextFromUser("Enter text for button Text", " ", ButtonToEdit->GetName(), NULL, wxDefaultCoord, wxDefaultCoord, true);
            if (!buttonText.IsEmpty()) {
                MainFrame::setButtonToEditText(buttonText);
            }
            break;

    }
}