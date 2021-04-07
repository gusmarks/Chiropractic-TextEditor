//includes
// wxprec is requierd for the wx widgets files to be found
// app.h, Buttons.h  and frame.h are included files that define methods used in this file
// fstream allow us to open and close certain files
//ctime gives us date and time functions
//DocumentIcon ICON "DocumentIcon.ico"
//#include <wx/msw/wx.rc>
#include "app.h"
#include "ButtonPanel.h"
#include "Frame.h"
#include "clipboard.h"
#include "FunctionHelper.h"
#include <wx/wfstream.h>
#include <wx/wxprec.h>
#include <fstream>
#include <ctime>
#include <wx/msgdlg.h>
#include <wx/url.h>
#include <wx/icon.h>
#include <filesystem>

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
EVT_MENU(MENU_EditButtonName, MainFrame::onPopUpCLick)
EVT_MENU(MENU_EditButtonText, MainFrame::onPopUpCLick)
EVT_MENU(MENU_SaveButtons, MainFrame::SavePanelsAndButtons)
EVT_BUTTON(BUTTON_Add, MainFrame::AddButton)
EVT_BUTTON(BUTTON_Write, MainFrame::ButtonWrite)
EVT_BUTTON(BUTTON_Sign, MainFrame::Sign)
EVT_BUTTON(BUTTON_Panel, MainFrame::swapHelper)
EVT_BUTTON(BUTTON_Back, MainFrame::swapToPreviousPanel)
EVT_BUTTON(BUTTON_NewSet, MainFrame::newSet)
EVT_CHOICE(CHOICE_SWAP_Set, MainFrame::SwapButtonSet)
END_EVENT_TABLE()
void loadButtonSetInfo(std::string path);
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
    MainWin->SetIcon(wxIcon("DocumentIcon.ico"));
    SetTopWindow(MainWin);// and finally, set it as the main window
    
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////main frame method/////////////////////////

/// <summary>
/// this is the constructor for the frame, it takes in 3 paramiters to build the frame with.
/// </summary>
/// <param name="title"></param>
/// <param name="pos"></param>
/// <param name="size"></param>
MainFrame::MainFrame(const wxString& title,
    const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
    //loads the basic data for button sets
    loadButtonSetInfo("SetInfoAll.txt");
    //-----------menu-status start-----------
    //statusbar at the bottom of page has 2 cells
    //menu bar at the top ofthe page has 1 tab named File menu and visualy represented as File
    //the File tab has 7 options: new file, open file,close file, save file, save file as, save button layout and quit.
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
    FileMenu->Append(MENU_SaveButtons, wxT(" &Save the button layout"),
        wxT("Save the button layout"));
    FileMenu->Append(MENU_Quit, wxT(" &Quit"),
        wxT("Quit the editor"));

    //attach the menu items to the frame
    MainMenu->Append(FileMenu, wxT("File"));
    SetMenuBar(MainMenu);
    ////////////////////////menu-status end////////////////////////

    //make 7 buttons, one to add dynamic buttons, one to sign the document
    // 4 buttons to swap panels of dynamic buttons, and one to go to the previous panel.

    wxButton* AddButton = new wxButton(this, BUTTON_Add, "add", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "add");
    wxButton* SignButton = new wxButton(this, BUTTON_Sign, "Sign", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "Apply Signiture to document");
    wxButton* swapToPanelOne = new wxButton(this, BUTTON_Panel, "subjective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "0");
    wxButton* swapToPanelTwo = new wxButton(this, BUTTON_Panel, "objective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
    wxButton* swapToPanelThree = new wxButton(this, BUTTON_Panel, "assessment", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "2");
    wxButton* swapToPanelFour = new wxButton(this, BUTTON_Panel, "Plan", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "3");
    wxButton* backButton = new wxButton(this, BUTTON_Back, "back", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "go back");



    //make a box sizer to add the buttons too
    ControlSizer = new wxBoxSizer(wxHORIZONTAL);
    //add the buttons to the box sizer
    ControlSizer->Add(backButton);
    ControlSizer->Add(AddButton);
    ControlSizer->Add(SignButton);
    ControlSizer->Add(swapToPanelOne);
    ControlSizer->Add(swapToPanelTwo);
    ControlSizer->Add(swapToPanelThree);
    ControlSizer->Add(swapToPanelFour);
    //create a choice box so the user can select what set of buttons they want, and a button to create new button sets
    userSelection = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, MainFrame::ButtonSetNames, 0, wxDefaultValidator, "userCHoice");
    wxButton* newUser = new wxButton(this, BUTTON_NewSet, "New Set", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "New Set");
    //new sizer for the wxCHoice and the new set button
    ButtonSetSizer = new wxBoxSizer(wxHORIZONTAL);
    ButtonSetSizer->Add(userSelection);
    ButtonSetSizer->Add(newUser);
    //set the selection and mind the setswaping function
    userSelection->SetSelection(0);
    userSelection->Bind(wxEVT_CHOICE, &MainFrame::SwapButtonSet, this);
    //instanciate the first set
    //currentButtonSetName = ButtonSetNames[0];

    
    //ButtonSetList.push_back(new ButtonSet(this, currentButtonSetName.ToStdString(), SetCount, "panelLayout/panelLayoutRob",8));
    currentButtonSet = ButtonSetList.at(0);
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////text editor/////////////////////////////////
    //add a wxtextctrl this is the main text editor
    MainEditBox = new wxTextCtrl(this, TEXT_Main,
        wxT(""), wxDefaultPosition, wxSize(600,1),
        wxTE_MULTILINE | wxTE_RICH| wxTE_AUTO_URL, wxDefaultValidator, wxTextCtrlNameStr);
    Maximize();
    MainEditBox->Bind(wxEVT_LEFT_DCLICK, &MainFrame::clickURLinTextCtrl, this);
    //an object to make and handel popups, error messages and questions
    popUpHandeler = new DialogHelper();
    functionHelper = new FuncHelper();
    ///////////////////////////button layout//////////////////////////////
 
    //loades the panels and buttons for the first set
    currentButtonSet->loadPanelsAndButtons(currentButtonSet->getSetName());


    // set the current panel object to be the first panel, and tell it to show.
    //currentPanel = panelList[0];
    currentButtonSet->getCurrentPanel()->Show();
   
    /////////////////////////////////////////////////////////////////
    ///////////////////////// arange sizers//////////////////////////

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(ButtonSetSizer,0, wxALIGN_CENTER);
    sizer->Add(ControlSizer, 0, wxALIGN_CENTER);
    sizer->Add(currentButtonSet->getCurrentPanel(), 2, wxEXPAND);
    sizer->Add(MainEditBox, 8, wxALIGN_CENTER,wxBORDER);
  
    //set the sizer object "sizer" as the main sizer and update its layout, finaly centering the objects in the frame.
    sizer->Layout();
    SetSizer(sizer);
    Centre();
}
/////////////////////end Frame Constructor/////////////
////////////////////////methods////////////////////////
/// </summary>
/// this method opens a dialog box that lets the user search thier file system 
/// and open any.doc file
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::openFile(wxCommandEvent& WXUNUSED(event))
{
    if (popUpHandeler->confirmIntent("are you sure you want to open a new file and close this one?")) {
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
    if (popUpHandeler->confirmIntent("are you sure you want to close the file?")) {
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
///this method opens a dialog box that lets the user search thier file system and 
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
/// the function adds a button to the buttonPanel. these are called dynamic buttons
/// each button will need a lable and text, these are prompted from the user
/// the button is added and the layout updated
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::AddButton(wxCommandEvent& event)
{
    //ask user what type of button they want, text addes text to the document,link adds a link to a new panel
    if (popUpHandeler->confirmIntentAddButton()) {
        //promp user for name and text of text button
        wxString buttonName =
            wxGetTextFromUser("Enter text for button name", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
        wxString buttonText =
            wxGetTextFromUser("Enter text for button to add", "", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
        //adds the button
            currentButtonSet->getCurrentPanel()->AddButton(buttonName, buttonText);
      
    }
    else {
        //propts user for the name of the new panel link, the text is the panel index for the current button set
        wxString buttonName =
            wxGetTextFromUser("Enter text for button name", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
    //gets the panel index and adds the new button, also the new panel that the button will link to.
        int panelCount = currentButtonSet->getPanelCount();
        std::string panelCountStr = std::to_string(panelCount);
        currentButtonSet->getCurrentPanel()->AddButton(buttonName, panelCountStr);
        currentButtonSet->addNewPanel(this, "Button-List" + panelCount);
    }
    //bind the editing function, and update the button index for the panel
    int QIndex = currentButtonSet->getCurrentPanel()->getButtonIndex();
    currentButtonSet->getCurrentPanel()->QlinkList->at(QIndex)->
        Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);
    currentButtonSet->getCurrentPanel()->setButtonIndex(QIndex+1);
}
/// <summary>////////////////////////////////////////////////
/// this function makes a button write its texr to the texteditor/
/// </summary>/////////////////////////////////////////////////
/// <param name="event"></param>
void MainFrame::ButtonWrite(wxCommandEvent& event) {
    wxButton* temp = (wxButton*)event.GetEventObject();
 
    MainEditBox->WriteText(("\n" + temp->GetName() + "\n"));

}/////////////////////////////end button write///////////////////
/// <summary>
/// adds a Name, and date+time to the file, as a time stamp and signiture --fix function--
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::Sign(wxCommandEvent& WXUNUSED(event)) {
    
        //MainEditBox->AppendText("\n" + (wxString)"G Marks" + "\n" + functionHelper->getDateToSign());
    
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
            //if the edit name option was chocsen then prompt the user for a new name, as long as it was not empty
            //the new name will replace the old one
            buttonName =
            wxGetTextFromUser("Enter text for button name", " ", ButtonToEdit->GetLabel(), NULL, wxDefaultCoord, wxDefaultCoord, true);
            if (!buttonName.IsEmpty()) {
                MainFrame::setButtonToEditName(buttonName);
            }
            break;
        case MENU_EditButtonText:
            //if the edit text option was chosen then the user will be probpted to enter a new text
            //the text will be replaced with the new text
            buttonText =
            wxGetTextFromUser("Enter text for button Text", " ", ButtonToEdit->GetName(), NULL, wxDefaultCoord, wxDefaultCoord, true);
            if (!buttonText.IsEmpty()) {
                MainFrame::setButtonToEditText(buttonText);
            }
            break;
            //make new case to remove button
    }
}
/// <summary>
/// swapPanels takes in a ButtonPanel and makes it the active panel
/// </summary>
/// <param name="newPanel"></param>
void MainFrame::swapPanels(ButtonPanel* newPanel) {
    ButtonPanel* currentPanel = currentButtonSet->getCurrentPanel();
    sizer->Detach(ControlSizer);
    sizer->Detach(currentPanel);
    sizer->Detach(ButtonSetSizer);
    currentButtonSet->getCurrentPanel()->Hide();
   
    currentButtonSet->setCurrentPanel(newPanel);

    sizer->Prepend(currentButtonSet->getCurrentPanel(), 2, wxEXPAND);
    sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
    sizer->Prepend(ButtonSetSizer, 0, wxALIGN_CENTER);
    currentButtonSet->getCurrentPanel()->Show();
    sizer->Layout();
    Update();  
}
/// <summary>
/// swapHelper gets the index of the panel to swap to and passes the new panel to swapPanels
/// </summary>
/// <param name="event"></param>
void MainFrame::swapHelper(wxCommandEvent& event){
    wxButton* btton = (wxButton*)event.GetEventObject(); 
    wxString ButtonName = btton->GetName();
    int PanelIndexToSwapTo = wxAtoi(ButtonName.ToStdString());
    ButtonPanel* panelToSwapTo= currentButtonSet->getPanelAtIndex(PanelIndexToSwapTo);
    swapPanels(panelToSwapTo);
}
/// <summary>
/// finds the prev element of the panel and passes it the swapPanels. 
/// if there is no prev element set then an error message occurs.
/// </summary>
/// <param name="event"></param>
void MainFrame::swapToPreviousPanel(wxCommandEvent& event) {
    if (currentButtonSet->getCurrentPanel()->getPrev() != nullptr) {
        swapPanels(currentButtonSet->getCurrentPanel()->getPrev());
    }
    else {
        popUpHandeler->errorMessage("there is no panel to go back to");
    }
}
//double click on the end of a line of text, this opens a dialog and asks the user to select an option, 
//it then replaces the double clicked line with the selected line. ----needs work---- 
void MainFrame::clickURLinTextCtrl(wxMouseEvent& evt) {

    wxString tempStr = MainEditBox->GetRange(0, MainEditBox->GetInsertionPoint());
    std::string str = tempStr.ToStdString();
    long lineNo = functionHelper->getLineNo(str, "\n");
    long insP = MainEditBox->GetInsertionPoint();
    wxString lineValue = MainEditBox->GetLineText(lineNo-1);
   // wxString st = (wxString)(""+lineNo);
    //MainEditBox->WriteText(st);
   // if (!lineValue.IsEmpty()||lineValue!="\n") {
        long lineLength = insP - lineValue.ToStdString().size();
        //wxString TextToReplaceWith =
            popUpHandeler->openClipBoard();
        //MainEditBox->Replace(lineLength, insP,TextToReplaceWith);

   // }
}
// using a wxChoice control, the user can change what button set they are using ----- needs work ----
void MainFrame::SwapButtonSet(wxCommandEvent& evt)
{
    //get the new selection in the wxChoice field and uses it to get the index to use
    wxChoice* choi = (wxChoice*)evt.GetEventObject();
    int setIndex = choi->GetSelection();
    // if the Buttonset in question's files exist and it is not the current buttonset then destroy the old panels set the new 
    //current buttonset and load its panels and buttons
    if (functionHelper->DoseUserExist(ButtonSetNames[setIndex]) && currentButtonSet->getSetName() != ButtonSetNames[setIndex]) {
        //remove the panels from the current set
        destroyPanels(currentButtonSet->getSetName());
        //chance the current set
        currentButtonSet = ButtonSetList.at(setIndex);
        //load panels for the new current set
        currentButtonSet->loadPanelsAndButtons(currentButtonSet->getSetName());

        //detach several sizers and reattach them to update the layout properly
        sizer->Detach(ButtonSetSizer);
        sizer->Detach(ControlSizer);
        sizer->Detach(currentButtonSet->getCurrentPanel());

        sizer->Prepend(currentButtonSet->getCurrentPanel(), 2, wxEXPAND);
        sizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        sizer->Prepend(ButtonSetSizer, 0, wxALIGN_CENTER);

        sizer->Layout();
    }
    else if(!functionHelper->DoseUserExist(ButtonSetNames[setIndex])){// if the set dose not exist or if we selected the same user
        popUpHandeler->errorMessage("set dose not exist");
    }
    else if (currentButtonSet->getSetName() == ButtonSetNames[setIndex]) {
        popUpHandeler->errorMessage("this is the same user");
    }
}
void MainFrame::destroyPanels(wxString set) {
    //systematicly destroy all panels and buttons for a given set. ---consider moving this to the buttonset class
    for (int i = 0; i < currentButtonSet->getPanelListSize(); i++) {
        sizer->Detach(currentButtonSet->getPanelAtIndex(i));
        currentButtonSet->getPanelAtIndex(i)->~ButtonPanel();
        sizer->Layout();
        Update();   
    }
    currentButtonSet->clearPanelList();
}
void MainFrame::newSet(wxCommandEvent& event) {
    //props the user for a name for the new button set, then adds the new set to the set list
    //opens an ofstream and writes a new file for the button set
    wxString setName=
        wxGetTextFromUser("Please enter name of new set", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
    ButtonSetNames[SetCount] = setName;
    std::string filename = "panelLayout/panelLayout" + setName.ToStdString() + ".txt";
    std::string pathname = "panelLayout/panelLayout" + setName.ToStdString();
    //std::string setInfoPath = "SetInformation/SetInfo" + setName.ToStdString() + ".txt";

    ButtonSetList.push_back(new ButtonSet(this, setName.ToStdString(), SetCount, pathname,4));

    ofstream newSetStream(filename);
    int check = mkdir(pathname.c_str());
    for (int i = 0; i < 4; i++) {
        std::string buttonfilename = pathname+"/layout" +(std::to_string(i+1))+ ".txt";
        ofstream basicFileStream(buttonfilename);
    }

    size_t size = sizeof(ButtonSetNames) / sizeof(ButtonSetNames[0]);
    userSelection->Clear();
    for (size_t i = 0; i < size; i++) {
        if (ButtonSetNames[i] != wxT("")) {
            userSelection->Append(ButtonSetNames[i]);
        }
    }
    userSelection->SetSelection(0);
    for (int i = 0; i < 4; i++) {
        newSetStream << "button-List"+std::to_string(i+1);
        newSetStream << "\n";
        newSetStream << (pathname+"/layout" + std::to_string(i+1) + ".txt");
        newSetStream << "\n";
    }
    SetCount++;
}
/// <summary>
/// saves the panels and buttons for the current button set
/// </summary>
/// <param name="event"></param>
void MainFrame::SavePanelsAndButtons(wxCommandEvent & event) {
    currentButtonSet->SavePanelsAndButtonsNP();
    //std::string setPathName = "SetInformation/SetInfo" + currentButtonSet->getSetName().ToStdString() + ".txt";
    saveButtonSetInfo("SetInfoAll.txt");
}
void MainFrame::saveButtonSetInfo(std::string path) {

    setInfoOut.open(path,std::ofstream::out|std::ofstream::trunc);
    if (setInfoOut.good()) {

    }
    if (!setInfoOut.fail()) {
        for (size_t i = 0; i < ButtonSetList.size(); i++) {
            setInfoOut << ButtonSetList.at(i)->getSetName().ToStdString();
            setInfoOut << "\n";
            setInfoOut << ButtonSetList.at(i)->getPanelCount();
            setInfoOut << "\n";
            setInfoOut << ButtonSetList.at(i)->getPath();
            setInfoOut << "\n";
        }
    }
}
void MainFrame::loadButtonSetInfo(std::string path) {

    std::string filepath = path;
    setInfoIn.open(filepath);
    std::string panelNo;
    std::string setName;
    std::string setPath;
    //int i = 0;
    while (setInfoIn.peek() != EOF) {
        getline(setInfoIn, setName);
        getline(setInfoIn, panelNo);
        getline(setInfoIn, setPath);

        int PanelNo = std::stoi(panelNo);
        ButtonSetNames[SetCount] = setName;
        ButtonSetList.push_back(new ButtonSet(this, setName, SetCount, setPath, PanelNo));
        SetCount++;

    }
    currentButtonSet = ButtonSetList.at(0);


}