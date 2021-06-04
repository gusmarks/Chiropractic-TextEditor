//includes
// wxprec is requierd for the wx widgets files to be found
// app.h, Buttons.h  and frame.h are included files that define methods used in this file
// fstream allow us to open and close certain files

//include bitmaps to add images to controls
#include "bitmaps/bold.xpm"
#include "bitmaps/italic.xpm"
#include "bitmaps/underline.xpm"
#include "bitmaps/alignleft.xpm"
#include "bitmaps/alignright.xpm"
#include "bitmaps/centre.xpm"
//include rich text styles
#include <wx/richtext/richtextstyles.h>
//include custom classess
#include "app.h"
#include "NavLink.h"
#include "myRichTextCtrl.h"
#include "ButtonPanel.h"
#include "Frame.h"
#include "clipboard.h"
#include "FunctionHelper.h"
//include general functionaly and wx specific functions
#include <wx/wfstream.h>
#include <wx/wxprec.h>
#include <fstream>
#include <ctime>
#include <wx/msgdlg.h>
#include <wx/url.h>
#include <wx/icon.h>
#include <filesystem>

#include "wx/richtext/richtextctrl.h"
#include "wx/richtext/richtextstyles.h"
#include "wx/richtext/richtextxml.h"
#include "wx/richtext/richtexthtml.h"
#include "wx/richtext/richtextformatdlg.h"
#include "wx/richtext/richtextsymboldlg.h"
#include "wx/richtext/richtextstyledlg.h"
#include "wx/richtext/richtextprint.h"
#include "wx/richtext/richtextimagedlg.h"


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

EVT_MENU(ID_FORMAT_BOLD, MainFrame::OnBold)
EVT_MENU(ID_FORMAT_ITALIC, MainFrame::OnItalic)
EVT_MENU(ID_FORMAT_UNDERLINE, MainFrame::OnUnderline)

EVT_MENU(ID_FORMAT_ALIGN_LEFT, MainFrame::OnAlignLeft)
EVT_MENU(ID_FORMAT_ALIGN_CENTRE, MainFrame::OnAlignCentre)
EVT_MENU(ID_FORMAT_ALIGN_RIGHT, MainFrame::OnAlignRight)

EVT_BUTTON(BUTTON_Add, MainFrame::AddButton)
EVT_BUTTON(BUTTON_Write, MainFrame::ButtonWrite)
EVT_BUTTON(BUTTON_Sign, MainFrame::Sign)
EVT_BUTTON(BUTTON_NewSet, MainFrame::newSet)

EVT_CHOICE(CHOICE_SWAP_Set, MainFrame::SwapButtonSet)
EVT_HYPERLINK(LINK_NAVIGATE, MainFrame::LinkNavigation)

EVT_BUTTON(BUTTON_Dialog,MainFrame::EandM)
EVT_TEXT_URL(TEXT_Main,MainFrame::clickURLinTextCtrl)
/*
EVT_MENU(ID_FORMAT_STRIKETHROUGH, MainFrame::OnStrikethrough)
EVT_MENU(ID_FORMAT_SUPERSCRIPT, MainFrame::OnSuperscript)
EVT_MENU(ID_FORMAT_SUBSCRIPT, MainFrame::OnSubscript)*/
END_EVENT_TABLE()
void loadButtonSetInfo(std::string path);
/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////main application///////////////////////////
// Initializes the MainApplication class and tells our program to run it
IMPLEMENT_APP(MainApplication) 

bool MainApplication::OnInit()
{
    //main frame constructor
     m_styleSheet = new wxRichTextStyleSheet();
    MainFrame* MainWin = new MainFrame(wxT("DocuMaster"), wxPoint(1, 1),
        wxSize(300, 200)); // Create an instance of our frame, or window
    MainWin->Show(TRUE); // show the window
    MainWin->SetIcon(wxIcon("SAMPLE"));
   // MainWin->SetIcon(wxIcon());
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
    wxInitAllImageHandlers();
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

    wxMenu* formatMenu = new wxMenu;
    formatMenu->AppendCheckItem(ID_FORMAT_BOLD, _("&Bold\tCtrl+B"));
    formatMenu->AppendCheckItem(ID_FORMAT_ITALIC, _("&Italic\tCtrl+I"));
    formatMenu->AppendCheckItem(ID_FORMAT_UNDERLINE, _("&Underline\tCtrl+U"));
    formatMenu->AppendSeparator();
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_LEFT, _("L&eft Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_RIGHT, _("&Right Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_CENTRE, _("&Centre"));
    formatMenu->AppendSeparator();
    //formatMenu->AppendCheckItem(ID_FORMAT_STRIKETHROUGH, _("Stri&kethrough"));
    //formatMenu->AppendCheckItem(ID_FORMAT_SUPERSCRIPT, _("Superscrip&t"));
    //formatMenu->AppendCheckItem(ID_FORMAT_SUBSCRIPT, _("Subscrip&t"));
    //attach the menu items to the frame
    MainMenu->Append(FileMenu, wxT("File"));
    MainMenu->Append(formatMenu, wxT("format"));
    SetMenuBar(MainMenu);

   
    toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxNO_BORDER);
   
    toolBar->AddCheckTool(ID_FORMAT_BOLD, wxEmptyString, wxBitmap(bold_xpm), wxNullBitmap, _("Bold"));
    toolBar->AddCheckTool(ID_FORMAT_ITALIC, wxEmptyString, wxBitmap(italic_xpm), wxNullBitmap, _("Italic"));
    toolBar->AddCheckTool(ID_FORMAT_UNDERLINE, wxEmptyString, wxBitmap(underline_xpm), wxNullBitmap, _("Underline"));
    toolBar->AddSeparator();
    toolBar->AddCheckTool(ID_FORMAT_ALIGN_LEFT, wxEmptyString, wxBitmap(alignleft_xpm), wxNullBitmap, _("Align Left"));
    toolBar->AddCheckTool(ID_FORMAT_ALIGN_CENTRE, wxEmptyString, wxBitmap(centre_xpm), wxNullBitmap, _("Centre"));
    toolBar->AddCheckTool(ID_FORMAT_ALIGN_RIGHT, wxEmptyString, wxBitmap(alignright_xpm), wxNullBitmap, _("Align Right"));
    toolBar->AddSeparator();
    

    toolBar->Realize();
    ////////////////////////menu-status end////////////////////////

    //make 7 buttons, one to add dynamic buttons, one to sign the document
    // 4 buttons to swap panels of dynamic buttons, and one to go to the previous panel.

    wxButton* AddButton = new wxButton(this, BUTTON_Add, "New Button", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "add");
    wxButton* SignButton = new wxButton(this, BUTTON_Sign, "Sign", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "Apply Signiture to document");
    wxButton* swapToPanelOne = new wxButton(this, BUTTON_Panel, "Subjective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "0");
    wxButton* swapToPanelTwo = new wxButton(this, BUTTON_Panel, "Objective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
    wxButton* swapToPanelThree = new wxButton(this, BUTTON_Panel, "Assessment", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "2");
    wxButton* swapToPanelFour = new wxButton(this, BUTTON_Panel, "Plan", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "3");
  

    //make a box sizer to add the buttons too
    ControlSizer = new wxBoxSizer(wxHORIZONTAL);
    //add the buttons to the box sizer
 
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
    currentButtonSet = ButtonSetList.at(0);

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////text editor/////////////////////////////////
    MainEditBox = new wxRichTextCtrl(this,TEXT_Main, "", wxDefaultPosition, wxSize(700, 600), 0, wxDefaultValidator, "");
   
    MainEditBox->SetInsertionPoint(0);
    wxFont font(wxFontInfo(12).Family(wxFONTFAMILY_ROMAN));
    MainEditBox->SetFont(font);
    MainEditBox->SetMargins(10, 10);
   

    wxRichTextBuffer::SetFloatingLayoutMode(false);
    wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler);
    wxRichTextBuffer::AddHandler(new wxRichTextHTMLHandler);

    
    //MainEditBox->Bind(wxEVT_LEFT_DCLICK, &MainFrame::clickURLinTextCtrl, this);
    //an object to make and handel popups, error messages and questions
    popUpHandeler = new DialogHelper(this);
    functionHelper = new FuncHelper();
    ///////////////////////////button layout//////////////////////////////
 
    //loades the panels and buttons for the first set
    currentButtonSet->loadPanelsAndButtons(currentButtonSet->getSetName());

    //builds the first navlink, this mostly shows were the user currently is. as clicking it would bring you to the same panel
    LinkSizer = new wxBoxSizer(wxHORIZONTAL);
    BuildNavLinks(currentButtonSet->getCurrentPanel());
    ControlSizer->Prepend(LinkSizer);


    // set the current panel object to be the first panel, and tell it to show.
    currentButtonSet->getCurrentPanel()->Show();
   
    /////////////////////////////////////////////////////////////////
    ///////////////////////// arange sizers//////////////////////////

    Mainsizer = new wxBoxSizer(wxVERTICAL);
    Mainsizer->Add(ButtonSetSizer,1, wxALIGN_CENTER);
    Mainsizer->Add(ControlSizer, 1, wxALIGN_CENTER);
    Mainsizer->Add(currentButtonSet->getCurrentPanel(),5, wxEXPAND);
    Mainsizer->Add(toolBar, 1, wxEXPAND);
    Mainsizer->AddSpacer(10);
    Mainsizer->Add(MainEditBox, 5,wxALIGN_CENTER);

    //set the sizer object "sizer" as the main sizer and update its layout, finaly centering the objects in the frame.
    Mainsizer->Layout();
    SetSizer(Mainsizer);
    Centre();
    Maximize();
   
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
            openFileDialog(this, _("Open Xml file"), "", "",
                "Xml files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
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
        wxString file = openFileDialog.GetPath();

        MainEditBox->LoadFile(file);
        //"PatientFileLoad/test.xml"
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
        //MainFrame::appendFilename(".doc");
        wxString xmlName= MainFrame::getFilename() + ".xml", docName= MainFrame::getFilename() + ".doc";
            MainEditBox->SaveFile("PatientFileParse/"+MainFrame::getFilename()+".txt");
            MainEditBox->SaveFile("PatientFileLoad/"+MainFrame::getFilename() + ".xml");
            MainEditBox->SaveFile("PatientFileView/"+MainFrame::getFilename() + ".html");
           
    }
    else {
        MainEditBox->SaveFile(MainFrame::getFilename() + ".txt");
        MainEditBox->SaveFile(MainFrame::getFilename() + ".xml");
    }   
}
/// <summary>
///this method opens a dialog box that lets the user search thier file system and 
/// save a file with a custom name regardless of if there is filename saved.
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::saveFileAs(wxCommandEvent& WXUNUSED(event))
{
    //set up rich text buffer
    wxFileDialog
        saveFileDialog(this, _("Save Doc file"), "", "",
            "xml files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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
/// the buttons can perform several tasks, it could simply write a line of text, it could link to a seperate panel
/// or it could bring up a dialog. 
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::AddButton(wxCommandEvent& event)
{
    //ask user what type of button they want, text addes text to the document,link adds a link to a new panel
   wxString selection = popUpHandeler->confirmIntentAddButton(this);
        //promp user for name and text of text button
   if (selection == "text output.") {
       wxString buttonName =
           wxGetTextFromUser("Enter text for button name", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
       wxString buttonText =
           wxGetTextFromUser("Enter text for button to add", "", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
       //adds the button
       if (buttonName!=""||buttonText!="") {
           currentButtonSet->getCurrentPanel()->AddButton(buttonName, buttonText);
       }
   }
   else if (selection == "new page.") {
       if (currentButtonSet->getCurrentPanel()->getLevel() >= 2) {
           popUpHandeler->errorMessage("max panel depth reached");
           return;
       }
       wxString ButtonName =
           wxGetTextFromUser("Enter text for button name", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
       if (ButtonName != "" && currentButtonSet->getCurrentPanel()->getLevel() < 2) {
           //gets the panel index and adds the new button, also the new panel that the button will link to.
           int panelCount = currentButtonSet->getCurrentPanel()->getPanelCount();
           std::string panelCountStr = std::to_string(panelCount);
           currentButtonSet->getCurrentPanel()->AddButton(ButtonName, panelCountStr);
           
           currentButtonSet->getCurrentPanel()->addNewPanel(this, ButtonName, currentButtonSet->getPath());

           //bind the editing function, and update the button index for the panel
           int QIndex = currentButtonSet->getCurrentPanel()->getButtonIndex();
           currentButtonSet->getCurrentPanel()->QlinkList->at(QIndex)->
               Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);

           currentButtonSet->getCurrentPanel()->QlinkList->at(QIndex)->
               Bind(wxEVT_BUTTON, &MainFrame::swapHelper, this);


           currentButtonSet->getCurrentPanel()->setButtonIndex(QIndex + 1);
           
       }
       else if (ButtonName == "") {
           popUpHandeler->errorMessage("no name");
       }
       
   }
   else if (selection == "popup.") {
       wxString ButtonName =
           wxGetTextFromUser("Enter text for button name", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
       currentButtonSet->getCurrentPanel()->AddButton(ButtonName, "Dialog-ID-pain");
   }
       
}
/// <summary>////////////////////////////////////////////////
/// this function makes a button write its texr to the texteditor/
/// </summary>/////////////////////////////////////////////////
/// <param name="event"></param>
void MainFrame::ButtonWrite(wxCommandEvent& event) {
    wxButton* temp = (wxButton*)event.GetEventObject();
    if(temp->GetName()!="")
        MainEditBox->WriteText(("\n" + temp->GetName() + "\n"));
}/////////////////////////////end button write///////////////////
/// <summary>
/// adds a Name, and date+time to the file, as a time stamp and signiture --fix function--
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::Sign(wxCommandEvent& WXUNUSED(event)) {
    if(!Signed)
        MainEditBox->AppendText("\n" + currentButtonSet->getSetName() + "\n" + functionHelper->getDateToSign());
    else if (Signed) {
        //remove old signiture, and apply new one
    }
    
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
    if (newPanel != currentButtonSet->getCurrentPanel()) {
        //chance the navlinks based on the panel being swaped to
        BuildNavLinks(newPanel);
        //detach old panel and several control sizers
        Mainsizer->Detach(ControlSizer);
        Mainsizer->Detach(currentButtonSet->getCurrentPanel());
        Mainsizer->Detach(ButtonSetSizer);
        //hide the current panel
        currentButtonSet->getCurrentPanel()->Hide();
        //set the new current panel
        currentButtonSet->setCurrentPanel(newPanel);
        // reattach all sizers and the new panel
        Mainsizer->Prepend(currentButtonSet->getCurrentPanel(), 2, wxEXPAND);
        Mainsizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        Mainsizer->Prepend(ButtonSetSizer, 0, wxALIGN_CENTER);
        currentButtonSet->getCurrentPanel()->Show();
        Mainsizer->Layout();
        Update();
    }
}
/// <summary>
/// swapHelper gets the index of the panel to swap to and passes the new panel to swapPanels
/// </summary>
/// <param name="event"></param>
void MainFrame::swapHelper(wxCommandEvent& event){
    //gets the current event object being a button, the button has an index relating the the assosidated panel 
    wxButton* btton = (wxButton*)event.GetEventObject(); 
    //get the index and turn it to an integer
    wxString ButtonName = btton->GetName();
    int PanelIndexToSwapTo = wxAtoi(ButtonName.ToStdString());
    // gets the panel and swaps to it
    swapPanels(currentButtonSet->getCurrentPanel()->getPanelAtIndex(PanelIndexToSwapTo));
}
//double click on the end of a line of text, this opens a dialog and asks the user to select an option, 
//it then replaces the double clicked line with the selected line. ----needs work---- 
void MainFrame::clickURLinTextCtrl(wxTextUrlEvent& evt) {
    
    int urlS =evt.GetURLStart();
    int urlE = evt.GetURLEnd()+1;
    MainEditBox->Remove(urlS,urlE);
    MainEditBox->WriteText(evt.GetString());
    //MainEditBox->WriteText(wxString::Format(wxT("%ld"),MainEditBox->GetInsertionPoint()));

}

/// <summary>
/// using a wxChoice control, the user can change what button set they are using ----- needs work ----
/// </summary>
/// <param name="evt"></param>
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
        Mainsizer->Detach(ButtonSetSizer);
        Mainsizer->Detach(ControlSizer);
        Mainsizer->Detach(currentButtonSet->getCurrentPanel());

        Mainsizer->Prepend(currentButtonSet->getCurrentPanel(), 2, wxEXPAND);
        Mainsizer->Prepend(ControlSizer, 0, wxALIGN_CENTER);
        Mainsizer->Prepend(ButtonSetSizer, 0, wxALIGN_CENTER);

        Mainsizer->Layout();
    }
    else if(!functionHelper->DoseUserExist(ButtonSetNames[setIndex])){// if the set dose not exist or if we selected the same user
        popUpHandeler->errorMessage("set dose not exist");
    }
    else if (currentButtonSet->getSetName() == ButtonSetNames[setIndex]) {
        popUpHandeler->errorMessage("this is the same user");
    }
}
/// <summary>
/// //systematicly destroy all panels and buttons for a given set. ---consider moving this to the buttonset class
/// </summary>
/// <param name="set"></param>
void MainFrame::destroyPanels(wxString set) {
    
    for (int i = 0; i < currentButtonSet->getPanelListSize(); i++) {
        Mainsizer->Detach(currentButtonSet->getPanelAtIndex(i));
        currentButtonSet->getPanelAtIndex(i)->~ButtonPanel();
        Mainsizer->Layout();
        Update();   
    }
    currentButtonSet->clearPanelList();
}
void MainFrame::newSet(wxCommandEvent& event) {
    //props the user for a name for the new button set, then adds the new set to the set list
    //opens an ofstream and writes a new file for the button set
    if (SetCount <= 9) {
        wxString setName =
            wxGetTextFromUser("Please enter name of new set", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
        ButtonSetNames[SetCount] = setName;
        //create the filename and pathname for the new users information
        std::string filename = "panelLayout/panelLayout" + setName.ToStdString() + ".txt";
        std::string pathname = "panelLayout/panelLayout" + setName.ToStdString();
        //create the new set
        ButtonSetList.push_back(new ButtonSet(this, setName.ToStdString(), SetCount, pathname, 4));
        //store the information for the set, in directories with text files
        ofstream newSetStream(filename);
        int check = mkdir(pathname.c_str());
        for (int i = 0; i < 4; i++) {
            std::string buttonfilename = pathname + "/layout" + (std::to_string(i + 1)) + ".txt";
            ofstream basicFileStream(buttonfilename);
        }
        //set the set selection control
        size_t size = sizeof(ButtonSetNames) / sizeof(ButtonSetNames[0]);
        userSelection->Clear();
        for (size_t i = 0; i < size; i++) {
            if (ButtonSetNames[i] != wxT("")) {
                userSelection->Append(ButtonSetNames[i]);
            }
        }
        //sets the original set as the first and saves basic set information
        userSelection->SetSelection(0);
        for (int i = 0; i < 4; i++) {
            newSetStream << "button-List" + std::to_string(i + 1);
            newSetStream << "\n";
            newSetStream << (pathname + "/layout" + std::to_string(i + 1) + ".txt");
            newSetStream << "\n";
        }
        SetCount++;
    }
}
/// <summary>
/// saves the panels and buttons for the current button set
/// </summary>
/// <param name="event"></param>
void MainFrame::SavePanelsAndButtons(wxCommandEvent & event) {
    currentButtonSet->SavePanelsAndButtonsNP();
    saveButtonSetInfo("SetInfoAll.txt");
}
void MainFrame::saveButtonSetInfo(std::string path) {
    //work on this one
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
/// <summary>
/// loads the information for defined button sets
/// </summary>
/// <param name="path"></param>
void MainFrame::loadButtonSetInfo(std::string path) {

    std::string filepath = path;
    setInfoIn.open(filepath);
    std::string setNo;
    std::string setName;
    std::string setPath;
    //int i = 0;
    while (setInfoIn.peek() != EOF) {
        getline(setInfoIn, setName);
        getline(setInfoIn, setNo);
        getline(setInfoIn, setPath);

        int SetNo = std::stoi(setNo);
        ButtonSetNames[SetCount] = setName;
        ButtonSetList.push_back(new ButtonSet(this, setName, SetCount, setPath, SetNo));
        SetCount++;

    }
    currentButtonSet = ButtonSetList.at(0);


}
/// <summary>
/// allows the Navlinks to swap panels to thier stores panel 
/// </summary>
/// <param name="evt"></param>
void MainFrame::LinkNavigation(wxHyperlinkEvent& evt) {
    NavLink* tempLink = (NavLink*)evt.GetEventObject();
    swapPanels(tempLink->getPanel());
}
/// <summary>
/// builds the navlinks based on the currentpanel and how many previous elements thier are
/// </summary>
/// <param name="panelToWork"></param>
void MainFrame::BuildNavLinks(ButtonPanel* panelToWork){

    switch (panelToWork->getLevel()) {
    case 0: 
        if (Link1 == nullptr) {
            Link1 = new NavLink(this, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
        }
        else {
            Link1->setName(panelToWork->GetName());
            Link1->setPanel(panelToWork);
        }
        if (Link2 != nullptr) {
            Link2->Hide();
            Link2 = nullptr;
        }
        if (Link3 != nullptr) {
            Link3->Hide();
            Link3 = nullptr;
        }
        
        LinkSizer->Clear();
        Link1->Hide();
        LinkSizer->Add(Link1);
        Link1->Show();
        LinkSizer->Layout();
        Update();
        break;
    case 1:
        if (Link2 == nullptr) {
            Link2 = new NavLink(this, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
        }
        else {
            Link2->setName(panelToWork->GetName());
            Link2->setPanel(panelToWork);
        }

        LinkSizer->Clear();
        Link1->Hide();
        Link2->Hide();
        LinkSizer->Add(Link1);
        LinkSizer->Add(10, 10);
        LinkSizer->Add(Link2);
        Link1->Show();
        Link2->Show();
        LinkSizer->Layout();
        Update();
        break;
    case 2:
        if (Link3 == nullptr) {
            Link3 = new NavLink(this, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
        }
        else {
            Link3->setName(panelToWork->GetName());
            Link3->setPanel(panelToWork);
        }
        LinkSizer->Clear();
        Link1->Hide();
        Link2->Hide();
        Link3->Hide();
        LinkSizer->Add(Link1);
        LinkSizer->Add(10, 10);
        LinkSizer->Add(Link2);
        LinkSizer->Add(10, 10);
        LinkSizer->Add(Link3);
        Link1->Show();
        Link2->Show();
        Link3->Show();
        LinkSizer->Layout();
        Update();
        break;
    }
}
void MainFrame::OnBold(wxCommandEvent& WXUNUSED(event))
{   
    MainEditBox->ApplyBoldToSelection();
}
void MainFrame::OnItalic(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyItalicToSelection();
}
void MainFrame::OnUnderline(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyUnderlineToSelection();
}
void MainFrame::OnAlignLeft(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_LEFT);
}
void MainFrame::OnAlignCentre(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTRE);
}
void MainFrame::OnAlignRight(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox-> ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_RIGHT);
}
void MainFrame::OnStrikethrough(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_STRIKETHROUGH);
}
void MainFrame::OnSuperscript(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
}
void MainFrame::OnSubscript(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
}
void MainFrame::DialogButton(wxCommandEvent& WXUNUSED(event)) {
    wxString allergies, surgeries, meds, illness, accidents, family,work,social,exercise,diet,locations;
    allergies = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt","patient allergies?");
    surgeries = popUpHandeler->MultipleChoiceDialog("DialogInformation/surgicalHistory.txt", "patient surgical history?");
    meds = popUpHandeler->MultipleChoiceDialog("DialogInformation/drugsMedication.txt", "patient Medications?");
    //illness = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt", "patient allergies?");
    //accidents = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt", "patient allergies?");
    family = popUpHandeler->MultipleChoiceDialog("DialogInformation/familyHistory.txt", "patient's Immediate Family History?");
    work = popUpHandeler->MultipleChoiceDialog("DialogInformation/work.txt", "Work?");
    social = popUpHandeler->MultipleChoiceDialog("DialogInformation/socialhabits.txt", "Social habits? ");
    exercise = popUpHandeler->MultipleChoiceDialog("DialogInformation/ExerciseRoutine.txt", "Exercise routine?");
    diet = popUpHandeler->MultipleChoiceDialog("DialogInformation/DietNutrition.txt", "Diet and Nutrition?");
    locations = popUpHandeler->bodyDialog();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("Past,Family and Social History:");
    MainEditBox->WriteText("\n\t-Past Health History:");
    MainEditBox->WriteText("\n\t\t-Allergies/Sensitivities:");
    MainEditBox->EndBold();

    MainEditBox->BeginURL("Allergies", wxT("Blue"));
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(allergies);
    MainEditBox->EndTextColour();
    MainEditBox->EndURL();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Surgery:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(surgeries);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Medications:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(meds);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Illnesses:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Accidents:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t-Family and Social History:");
    MainEditBox->WriteText("\n\t\t-Family History:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(family);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Work Habits:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(work);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Social Habits:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(social);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Exercise Habits:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(exercise);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n\t\t-Diet and Nutrition Habits:");
    MainEditBox->EndBold();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(diet);
    MainEditBox->EndTextColour();
}
void MainFrame::EandM(wxCommandEvent& WXUNUSED(event)) {
    wxString area, date, injury, freq,quality,vas,Radiate,allergies,prevEpp,goals,locations;
    locations = popUpHandeler->bodyDialog();
    date = popUpHandeler->Calender();

    injury = popUpHandeler->SingleChoiceDialog("DialogInformation/Injury.txt","Mechanism of injury or condition.");
    freq = popUpHandeler->SingleChoiceDialog("DialogInformation/FreqOfPain.txt", "Frequency of pain?");
    quality = popUpHandeler->MultipleChoiceDialog("DialogInformation/QualityOfDiscomfort.txt", "What are the quality of the pain?");
    //radiates
    vas = popUpHandeler->SingleChoiceDialog("DialogInformation/VAS.txt", "What is the VAS?");
    //modifying factors
    //previos episodes
    //previos care
    //recent tests
    goals = popUpHandeler->MultipleChoiceDialog("DialogInformation/goals.txt", "Patient Goals?");
 
    MainEditBox->BeginBold();
    MainEditBox->WriteText("HISTORY: \n");
    MainEditBox->WriteText("Chief Complaint: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(locations);
    MainEditBox->EndTextColour();
    MainEditBox->WriteText(" complaint since ");
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(date);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Mechanism of injury: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(52,128,235));
    MainEditBox->WriteText(injury);
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Frequency/Quality: ");
    MainEditBox->EndBold();
    MainEditBox->WriteText("experiences pain and discomfort ");
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(freq);
    MainEditBox->EndTextColour();
    MainEditBox->WriteText(" of the time and is discribed as");
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(quality);
    MainEditBox->EndTextColour();


    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Radiation of symptoms: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Change in Complaint: ");
    MainEditBox->EndBold();
    MainEditBox->WriteText("Complaint has ");
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();
    MainEditBox->WriteText("since the onset and the pain scale is currently rated at ");
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(vas);
    MainEditBox->EndTextColour();
    MainEditBox->WriteText("(10/10 being the most severe)");

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Modifying Factors: ");
    MainEditBox->EndBold();
    MainEditBox->WriteText("Relived by ");
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();
    MainEditBox->WriteText("and aggravated by ");
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Previous Episodes: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();
    MainEditBox->WriteText("past episodes.");

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Previos Care: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Recent diagnostic tests: ");
    MainEditBox->EndBold();
    MainEditBox->BeginTextColour(wxColour(239, 51, 10));
    MainEditBox->WriteText("placeholder");
    MainEditBox->EndTextColour();

    MainEditBox->BeginBold();
    MainEditBox->WriteText("\n-Patient subjective goal(s): ");
    MainEditBox->EndBold();
    MainEditBox->WriteText("Explains personal goal for staring treatment");
    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText(goals);
    MainEditBox->EndTextColour();

    MainEditBox->BeginTextColour(wxColour(52, 128, 235));
    MainEditBox->WriteText("\n--------------------------------------");
    MainEditBox->EndTextColour();
}