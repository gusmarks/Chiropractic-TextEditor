
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
#include "DialogHelper.h"
#include "ButtonPanel.h"
#include "Frame.h"
#include "FunctionHelper.h"
#include "LinkPanel.h"
#include "DialogCreator.h"

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
EVT_MENU(MENU_RemoveButton,MainFrame::onPopUpCLick)
EVT_MENU(MENU_SaveButtons, MainFrame::savePanelsAndButtons)
EVT_MENU(MENU_BillingDoc, MainFrame::gatherBillingInformation)
EVT_MENU(MENU_EditCodes, MainFrame::openCPT_DxEditor)
EVT_MENU(MENU_EditGenDialog, MainFrame::openGenDialogEditor)
EVT_MENU(ID_FORMAT_BOLD, MainFrame::onBold)
EVT_MENU(ID_FORMAT_ITALIC, MainFrame::onItalic)
EVT_MENU(ID_FORMAT_UNDERLINE, MainFrame::onUnderline)
EVT_MENU(ID_FORMAT_ALIGN_LEFT, MainFrame::onAlignLeft)
EVT_MENU(ID_FORMAT_ALIGN_CENTRE, MainFrame::onAlignCentre)
EVT_MENU(ID_FORMAT_ALIGN_RIGHT, MainFrame::onAlignRight)
EVT_BUTTON(BUTTON_Add, MainFrame::addButton)
EVT_BUTTON(BUTTON_Sign, MainFrame::sign)
EVT_BUTTON(BUTTON_NewSet, MainFrame::newSet)
EVT_CHOICE(CHOICE_SWAP_Set, MainFrame::swapButtonSet)
EVT_HYPERLINK(LINK_NAVIGATE, MainFrame::linkNavigation)
EVT_TEXT_URL(TEXT_Main,MainFrame::dialogURLHelper)
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
/// <param name="title"> the name of the frame to be displayed</param>
/// <param name="pos">the position on the screen to render the frame</param>
/// <param name="size"> how large the frame is</param>
MainFrame::MainFrame(const wxString& title,
    const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
    //initulize image handelers, allows the use of icons, and bitmaps to apply pictures 
    wxInitAllImageHandlers();
    //loads the basic data for button sets
    loadButtonSetInfo("SetInfoAll.txt");
    if (buttonSetList.empty()) {
        MainFrame::quit();
        return;
    }
    //-----------menu-status start-----------
    //statusbar at the bottom of page has 2 cells
    //menu bar at the top ofthe page has 1 tab named File menu and visualy represented as File
    //the File tab has 10 options: new file, open file,close file, save file, save file as, gather billing information, 
    //edit dx/cpt codes, edit general dialogs,save button layout and quit.
    mainMenu = new wxMenuBar();
    wxMenu* FileMenu = new wxMenu();

    FileMenu->Append(MENU_New, wxT("&New Patient File"),
        wxT("Create a new file"));
    FileMenu->Append(MENU_Open, wxT("&Open Existing Patient File"),
        wxT("Open an existing file"));
    FileMenu->Append(MENU_Close, wxT("&Close Patient file"),
        wxT("Close the current document"));
    FileMenu->Append(MENU_Save, wxT("&Save File"),
        wxT("Save the current document"));
    FileMenu->Append(MENU_SaveAs, wxT("Save File &As"),
        wxT("Save the current document under a new file name"));
    FileMenu->Append(MENU_BillingDoc, wxT("&Create Billing Document"),
        wxT("Create a new Billing file"));
    FileMenu->Append(MENU_SaveButtons, wxT(" &Save the button layout"),
        wxT("Save the button layout"));
    FileMenu->Append(MENU_Quit, wxT(" &Quit"),
        wxT("Quit the editor"));
    wxMenu* EditMenu = new wxMenu();

    EditMenu->Append(MENU_EditCodes, wxT("&Edit Dx/CPT Codes"),
        wxT("Edit Dx/CPT Codes."));
    EditMenu->Append(MENU_EditGenDialog, wxT("&Edit General Dialogs"),
        wxT("Edit Dx/CPT Codes."));

    //formatting drop down
    wxMenu* formatMenu = new wxMenu;
    formatMenu->AppendCheckItem(ID_FORMAT_BOLD, _("&Bold\tCtrl+B"));
    formatMenu->AppendCheckItem(ID_FORMAT_ITALIC, _("&Italic\tCtrl+I"));
    formatMenu->AppendCheckItem(ID_FORMAT_UNDERLINE, _("&Underline\tCtrl+U"));
    formatMenu->AppendSeparator();
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_LEFT, _("L&eft Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_RIGHT, _("&Right Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_CENTRE, _("&Centre"));
    formatMenu->AppendSeparator();
  
    //attach the menu items to the frame
    mainMenu->Append(FileMenu, wxT("File"));
    mainMenu->Append(EditMenu, wxT("Edit"));
    mainMenu->Append(formatMenu, wxT("Format"));
    SetMenuBar(mainMenu);

    //the tool bar allows for quick editing of formatting variables
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

    controls = new controlPanel(this, wxID_ANY, "Control Panel");
    links = new linkPanel(controls, wxID_ANY, "Link Panel");

    wxButton* addButton = new wxButton(controls, BUTTON_Add, "New Button", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "add");
    wxButton* SignButton = new wxButton(controls, BUTTON_Sign, "Sign", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "Apply Signiture to document");
    wxButton* swapToPanelOne = new wxButton(controls, Button_ControlPanel, "Subjective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "0");
    wxButton* swapToPanelTwo = new wxButton(controls, Button_ControlPanel, "Objective", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
    wxButton* swapToPanelThree = new wxButton(controls, Button_ControlPanel, "Assessment", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "2");
    wxButton* swapToPanelFour = new wxButton(controls, Button_ControlPanel, "Plan", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "3");
    swapToPanelOne->Bind(wxEVT_BUTTON, &MainFrame::startSwapHelper, this);
    swapToPanelTwo->Bind(wxEVT_BUTTON, &MainFrame::startSwapHelper, this);
    swapToPanelThree->Bind(wxEVT_BUTTON, &MainFrame::startSwapHelper, this);
    swapToPanelFour->Bind(wxEVT_BUTTON, &MainFrame::startSwapHelper, this);

    //make a box sizer to add the buttons too
    
    //add the buttons to the box sizer
    controls->addToControlSizer(links, wxALIGN_CENTER);
    controls->addToControlSizer(addButton);
    controls->addToControlSizer(SignButton);
    controls->addToControlSizer(swapToPanelOne);
    controls->addToControlSizer(swapToPanelTwo);
    controls->addToControlSizer(swapToPanelThree);
    controls->addToControlSizer(swapToPanelFour);
    controls->controlLayout();

    //create a choice box so the user can select what set of buttons they want, and a button to create new button sets
    userSelection = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(100,30), 2, MainFrame::buttonSetNames, 0, wxDefaultValidator, "userCHoice");
    wxButton* newUser = new wxButton(this, BUTTON_NewSet, "New Set", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "New Set");
    //new sizer for the wxCHoice and the new set button
    buttonSetSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSetSizer->Add(userSelection);
    buttonSetSizer->Add(newUser);
    //set the selection and mind the setswaping function
    userSelection->SetSelection(0);
    userSelection->Bind(wxEVT_CHOICE, &MainFrame::swapButtonSet, this);
    //instanciate the first set
    currentButtonSet = buttonSetList.at(0);

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////text editor/////////////////////////////////
    mainEditBox = new wxRichTextCtrl(this,TEXT_Main, "", wxDefaultPosition, wxSize(770, 600), 0, wxDefaultValidator, "");
   
    mainEditBox->SetInsertionPoint(0);
    wxFont font(wxFontInfo(12).Family(wxFONTFAMILY_ROMAN));
    mainEditBox->SetFont(font);
    mainEditBox->SetMargins(10, 10);
    
    //handelers help save in other file formats
    wxRichTextBuffer::SetFloatingLayoutMode(false);
    wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler);
    wxRichTextBuffer::AddHandler(new wxRichTextHTMLHandler);

    //an object to make and handel popups, error messages and questions
    popUpHandeler = new DialogHelper(this);
    functionHelper = new FuncHelper();
    int count =ManualgetDialogCount("Dialogs/0dialogCount.txt");
    DialogCreationHelper = new DialogCreator(this,wxID_ANY,count);
    
    ///////////////////////////button layout//////////////////////////////
 
    //loades the panels and buttons for the first set
    currentButtonSet->loadPanelsAndButtons(currentButtonSet->getSetName());

    //builds the first navlink, this mostly shows were the user currently is. as clicking it would bring you to the same panel
 
    buildNavLinks(currentButtonSet->getCurrentPanel());
  


    // set the current panel object to be the first panel, and tell it to show.
    currentButtonSet->getCurrentPanel()->Show();
   
    /////////////////////////////////////////////////////////////////
    ///////////////////////// arange sizers//////////////////////////

    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(buttonSetSizer,1, wxALIGN_CENTER);
    mainSizer->Add(controls, 1, wxEXPAND);
    mainSizer->Add(currentButtonSet->getCurrentPanel(),5, wxEXPAND);
    mainSizer->Add(toolBar, 1, wxEXPAND);
    mainSizer->AddSpacer(5);
    mainSizer->Add(mainEditBox, 5,wxALIGN_CENTER);

    //set the sizer object "sizer" as the main sizer and update its layout, finaly centering the objects in the frame.
    mainSizer->Layout();
    SetSizer(mainSizer);
    Centre();
    Maximize();
    bindAllButtons();
    startingSate();
  
}
/////////////////////end Frame Constructor/////////////
////////////////////////methods////////////////////////
/// <summary>
/// the function adds a button to the buttonPanel. these are called dynamic buttons
/// each button will need a lable and text, these are prompted from the user
/// the button is added and the layout updated
/// the buttons can perform several tasks, it could simply write a line of text, it could link to a seperate panel
/// or it could bring up a dialog. 
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::addButton(wxCommandEvent& event)
{
    wxString buttonName;
    wxString buttonText;
    //ask user what type of button they want, text addes text to the document,link adds a link to a new panel
   wxString selection = popUpHandeler->confirmIntentAddButton(this);
   selectionInformation premadeSelection;
   int buttonCount = currentButtonSet->getCurrentPanel()->getQLinkCount();
   if (buttonCount >= 30) {
       popUpHandeler->errorMessage("exceding 30 buttons, cannot place");
       return;
   }
   try {
       //promp user for name and text of text button
       if (selection == "Text Button.") {
           premadeSelection = popUpHandeler->selectPremadeDialog();
           if (premadeSelection.selectionText == "New Paragraph") {
               //open the dialog creator to make a new paragraph name the button to name the new paragraph(could be an issue)
               openDialogCreator();
               buttonName = wxGetTextFromUser("Enter name for Button & Dialog", "Must have name(errors may occur if unnamed)", "", NULL, wxDefaultCoord, wxDefaultCoord, true);
               buttonText = "Dialogs/DialogFile" + std::to_string(DialogCreationHelper->getDialogCount()) + ".txt";
               //save the button details
               std::fstream fileReader("Dialogs/0dialogList.txt", fstream::app);
               if (fileReader.is_open()) {
                   fileReader <<  buttonName.ToStdString();
                   fileReader << " \tDialogs/DialogFile" + std::to_string(DialogCreationHelper->getDialogCount()) + ".txt\n";
               }
               fileReader.close();
               //save the aditional information, the buttons name and the count of all dialogs
               fileReader.open("Dialogs/0dialogNames.txt", fstream::app);
               if (fileReader.is_open())
                   fileReader << buttonName.ToStdString() + "\n";
               fileReader.close();
               fileReader.open("Dialogs/0dialogCount.txt");
               if (fileReader.is_open())
                   fileReader << std::to_string(DialogCreationHelper->getDialogCount());
               fileReader.close();
           }
           // if the user chose to cancel this will make sure not to save it
           else if (premadeSelection.selectionText == "Cancel") {
               buttonName = "";
           }
           //add this!
           else if (premadeSelection.selectionText != "") {
               buttonName = premadeSelection.selectionText;
               buttonText = ("Dialogs/DialogFile" + std::to_string(premadeSelection.selectionIndex-1) + ".txt");

           }

           //adds the button to te page and binds the to the apropirate functions
           if (buttonName != "") {
               currentButtonSet->getCurrentPanel()->addQLink(buttonName, buttonText);
               int QIndex = currentButtonSet->getCurrentPanel()->getQLinkCount();
               currentButtonSet->getCurrentPanel()->setQLinkCount(QIndex + 1);

               currentButtonSet->getCurrentPanel()->getQLinkList().back()->
                   Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);

               currentButtonSet->getCurrentPanel()->getQLinkList().back()->
                   Bind(wxEVT_BUTTON, &MainFrame::readDialogFile, this);
           }
       }
       //if the user chose new page then a button will be added that is bound to a function that swaps the page to the clicked one
       else if (selection == "New Page Button.") {
           if (currentButtonSet->getCurrentPanel()->getLevel() >= 2) {
               popUpHandeler->errorMessage("max panel depth reached");
               return;
           }
           wxString ButtonName =
               wxGetTextFromUser("Enter text for button name", "Must have name(errors may occur if unnamed)", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
           if (ButtonName != "" && currentButtonSet->getCurrentPanel()->getLevel() < 2) {
               //gets the panel index and adds the new button, also the new panel that the button will link to.
               int panelCount = currentButtonSet->getCurrentPanel()->getPanelCount();
               std::string panelCountStr = std::to_string(panelCount);
               currentButtonSet->getCurrentPanel()->addQLink(ButtonName, panelCountStr);

               currentButtonSet->getCurrentPanel()->addNewPanel(this, ButtonName, currentButtonSet->getPath());

               //bind the editing function, and update the button index for the panel
               int QIndex = currentButtonSet->getCurrentPanel()->getQLinkCount();
               currentButtonSet->getCurrentPanel()->getQLinkList().at(QIndex)->
                   Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);

               currentButtonSet->getCurrentPanel()->getQLinkList().at(QIndex)->
                   Bind(wxEVT_BUTTON, &MainFrame::swapHelper, this);


               currentButtonSet->getCurrentPanel()->setQLinkCount(QIndex + 1);

           }
           else if (ButtonName == "") {
             
           }

       }
   }
   catch (...) { popUpHandeler->errorMessage("an error occured in main.cpp"); }
}
/// <summary>
/// adds a Name, and date+time to the file, as a time stamp and signiture --fix function--
/// </summary>
/// <param name="WXUNUSED"></param>
void MainFrame::sign(wxCommandEvent& WXUNUSED(event)) {
    try {
        if (isFileOpen()) {
            // stop all styling and add a new line
            mainEditBox->EndAllStyles();
            mainEditBox->EndURL();
            mainEditBox->WriteText("\n");
            wxBitmap signiture;
            currentButtonSet->getSigniturePath();
            //load a document containing pixels for a signiture
            signiture.LoadFile(currentButtonSet->getSigniturePath(), wxBITMAP_TYPE_ANY);
            // arrange descriptive text, and the current date
            mainEditBox->AppendText("\n Electronicly Signed by:" + currentButtonSet->getSetName() + "\n");
            mainEditBox->WriteText(" Signiture:");
            mainEditBox->WriteImage(signiture);
            mainEditBox->AppendText("\n Latest Date of Visit<" + functionHelper->getDateToSignNoTime() + ">");
        }
        else {
            popUpHandeler->errorMessage("There is no file open.\n Please open a file to write.");
        }
    }
    catch (...) { popUpHandeler->errorMessage("an error occured in main.cpp"); }
}
/// <summary>
/// this function allows the user to make a right click on a button and offer them 2 choices edit lable or edit text
/// </summary>
/// <param name="event"></param>
void MainFrame::onRightClick(wxMouseEvent& event) {
    //get the selected button
    buttonToEdit = (wxButton*)event.GetEventObject();
    wxMenu menu;
    std::string name =buttonToEdit->GetName().ToStdString();
    // depending on what type of button it is, all the user to alter it
    //if its a new page button all the user to edit the name
    if (functionHelper->isNumber(name)) {
        menu.Append(MENU_EditButtonName, "edit Name of button");
        
        
    }
    // if its a text button allow the name to be edited and the button to be removed
    else  if (functionHelper->isText(name)) {
        menu.Append(MENU_EditButtonName, "edit Name of button");
        menu.Append(MENU_RemoveButton, "remove button");
    }

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
    case MENU_EditButtonName: {
        //if the edit name option was chocsen then prompt the user for a new name, as long as it was not empty
        //the new name will replace the old one
        buttonName =
            wxGetTextFromUser("Enter text for button name", " ", buttonToEdit->GetLabel(), NULL, wxDefaultCoord, wxDefaultCoord, true);
        if (!buttonName.IsEmpty()) {
            MainFrame::setbuttonToEditName(buttonName);
        }

        break;
    }
            //make new case to remove button
    case MENU_RemoveButton: {
        std::vector<wxButton*> vect = currentButtonSet->getCurrentPanel()->getQLinkList();
        int x = 0;
        for (int i = 0; i != (int)vect.size(); i++) {
            wxString buttonName = currentButtonSet->getCurrentPanel()->getQLinkList().at(i)->GetName();
            if (buttonName == buttonToEdit->GetName()) {
                x = i;
            }
        }
        vect.erase(vect.begin() + x);
        int QIndex = currentButtonSet->getCurrentPanel()->getQLinkCount();
        currentButtonSet->getCurrentPanel()->setQLinkCount(QIndex - 1);
        MainFrame::removebuttonToEdit();
        currentButtonSet->getCurrentPanel()->getGridSizer()->Layout();
        Update();
        break;
    }
    }
}
/// <summary>
/// swapPanels takes in a ButtonPanel and makes it the active panel
/// </summary>
/// <param name="newPanel"></param>
void MainFrame::swapPanels(ButtonPanel* newPanel) {
    if (newPanel != currentButtonSet->getCurrentPanel()) {
        //chance the navlinks based on the panel being swaped to
        buildNavLinks(newPanel);
        //detach old panel and several control sizers
        mainSizer->Detach(controls);
        mainSizer->Detach(currentButtonSet->getCurrentPanel());
        mainSizer->Detach(buttonSetSizer);
        //hide the current panel
        currentButtonSet->getCurrentPanel()->Hide();
        //set the new current panel
        currentButtonSet->setCurrentPanel(newPanel);
        // reattach all sizers and the new panel
        mainSizer->Prepend(currentButtonSet->getCurrentPanel(), 5, wxEXPAND);
        mainSizer->Prepend(controls, 1, wxEXPAND);
        mainSizer->Prepend(buttonSetSizer, 1, wxALIGN_CENTER);
        currentButtonSet->getCurrentPanel()->Show();
        mainSizer->Layout();
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
void MainFrame::startSwapHelper(wxCommandEvent& event) {
    //gets the current event object being a button, the button has an index relating the the assosidated panel 
    wxButton* btton = (wxButton*)event.GetEventObject();
    //get the index and turn it to an integer
    wxString ButtonName = btton->GetName();
    int PanelIndexToSwapTo = wxAtoi(ButtonName.ToStdString());
    // gets the panel and swaps to it
    swapPanels(currentButtonSet->getPanelAt(PanelIndexToSwapTo));
}
/// <summary>
/// using a wxChoice control, the user can change what button set they are using 
/// </summary>
/// <param name="evt"></param>
void MainFrame::swapButtonSet(wxCommandEvent& evt)
{
    //get the new selection in the wxChoice field and uses it to get the index to use
    wxChoice* choi = (wxChoice*)evt.GetEventObject();
    int setIndex = choi->GetSelection();
    // if the Buttonset in question's files exist and it is not the current buttonset then destroy the old panels set the new 
    //current buttonset and load its panels and buttons
    if (functionHelper->DoseUserExist(buttonSetNames[setIndex]) && currentButtonSet->getSetName() != buttonSetNames[setIndex]) {
        //remove the panels from the current set
        destroyPanels(currentButtonSet->getSetName());
        //chance the current set
        currentButtonSet = buttonSetList.at(setIndex);
        //load panels for the new current set
        currentButtonSet->loadPanelsAndButtons(currentButtonSet->getSetName());
        buildNavLinks(currentButtonSet->getCurrentPanel());
        //detach several sizers and reattach them to update the layout properly
        mainSizer->Detach(buttonSetSizer);
        mainSizer->Detach(controls);
        mainSizer->Detach(currentButtonSet->getCurrentPanel());

        mainSizer->Prepend(currentButtonSet->getCurrentPanel(), 5, wxEXPAND);
        mainSizer->Prepend(controls, 1, wxEXPAND);
        mainSizer->Prepend(buttonSetSizer, 0, wxALIGN_CENTER);
        bindAllButtons();

        mainSizer->Layout();
    }
    else if (!functionHelper->DoseUserExist(buttonSetNames[setIndex])) {// if the set dose not exist or if we selected the same user
        popUpHandeler->errorMessage("set dose not exist");
    }
    else if (currentButtonSet->getSetName() == buttonSetNames[setIndex]) {
        popUpHandeler->errorMessage("this is the same user");
    }
}
/// <summary>
/// //systematicly destroy all panels and buttons for a given set. ---consider moving this to the buttonset class
/// </summary>
/// <param name="set"></param>
void MainFrame::destroyPanels(wxString set) {
    //loops over the button sets panels, and detaches all panels, then calls the destructor of each panel
    for (int i = 0; i < currentButtonSet->getPanelListSize(); i++) {
        mainSizer->Detach(currentButtonSet->getPanelAtIndex(i));
        currentButtonSet->getPanelAtIndex(i)->~ButtonPanel();
        mainSizer->Layout();
        Update();
    }
    //clear the list of panels
    currentButtonSet->clearPanelList();
}
void MainFrame::newSet(wxCommandEvent& event) {
    //props the user for a name for the new button set, then adds the new set to the set list
    //opens an ofstream and writes a new file for the button set
    if (setCount <= 9) {
        wxString setName =
            wxGetTextFromUser("Please enter name of new set", " ", "", NULL, wxDefaultCoord, wxDefaultCoord, true);
        if (!setName.empty()) {
            buttonSetNames[setCount] = setName;
            //create the filename and pathname for the new users information
            std::string filename = "panelLayout/panelLayout" + setName.ToStdString() + ".txt";
            std::string pathname = "panelLayout/panelLayout" + setName.ToStdString();
            std::string sigPath = getSignitureImage();
            //create the new set
            buttonSetList.push_back(new ButtonSet(this, setName.ToStdString(), setCount, pathname, sigPath, 4));
            //store the information for the set, in directories with text files
            ofstream newSetStream(filename);
            int check = mkdir(pathname.c_str());
            for (int i = 0; i < 4; i++) {
                std::string buttonfilename = pathname + "/layout" + (std::to_string(i + 1)) + ".txt";
                ofstream basicFileStream(buttonfilename);
                std::string buttonPathname = pathname + "/layout" + (std::to_string(i + 1)) + "-panels";
                int check = mkdir(buttonPathname.c_str());
                for (int j = 0; j < 7; j++) {
                    buttonfilename = buttonPathname + "/LayoutLevel2-" + (std::to_string(j + 1)) + ".txt";
                    ofstream basicFileStream(buttonfilename);
                    std::string LvL2ButtonPathname = buttonPathname + "/LayoutLevel2-" + (std::to_string(j + 1)) + "-panels";
                    int check = mkdir(LvL2ButtonPathname.c_str());
                    for (int k = 0; k < 7; k++) {
                        std::string LvL3Buttonfilename = LvL2ButtonPathname + "/LayoutLevel3-" + (std::to_string(k + 1)) + ".txt";
                        ofstream basicFileStream(LvL3Buttonfilename);
                    }
                }
            }
            //set the set selection control
            size_t size = sizeof(buttonSetNames) / sizeof(buttonSetNames[0]);
            userSelection->Clear();
            for (size_t i = 0; i < size; i++) {
                if (buttonSetNames[i] != wxT("")) {
                    userSelection->Append(buttonSetNames[i]);
                }
            }
            std::string names[4] = { "Subjective","Objective","Assesment","Plan" };
            //sets the original set as the first and saves basic set information
            userSelection->SetSelection(0);
            for (int i = 0; i < 4; i++) {
                newSetStream << names[i];
                newSetStream << "\n";
                newSetStream << (pathname + "/layout" + std::to_string(i + 1) + ".txt");
                newSetStream << "\n";
            }
            setCount++;
            newSetStream.close();
            saveButtonSetInfo("SetInfoAll.txt");
        }
    }
}
void MainFrame::newSet() {
    //props the user for a name for the new button set, then adds the new set to the set list
  //opens an ofstream and writes a new file for the button set
    if (setCount <= 9) {
        wxString setName =
            wxGetTextFromUser("Please enter name of new User", " ", "", NULL, wxDefaultCoord, wxDefaultCoord, true);
        if (!setName.empty()) {
            buttonSetNames[setCount] = setName;
            //create the filename and pathname for the new users information
            std::string filename = "panelLayout/panelLayout" + setName.ToStdString() + ".txt";
            std::string pathname = "panelLayout/panelLayout" + setName.ToStdString();
            std::string sigPath = getSignitureImage();
            //create the new set
            buttonSetList.push_back(new ButtonSet(this, setName.ToStdString(), setCount, pathname, sigPath, 4));
            //store the information for the set, in directories with text files
            ofstream newSetStream(filename);
            int check = mkdir(pathname.c_str());
            for (int i = 0; i < 4; i++) {
                std::string buttonfilename = pathname + "/layout" + (std::to_string(i + 1)) + ".txt";
                ofstream basicFileStream(buttonfilename);
                std::string buttonPathname = pathname + "/layout" + (std::to_string(i + 1)) + "-panels";
                int check = mkdir(buttonPathname.c_str());
                for (int j = 0; j < 7; j++) {
                    buttonfilename = buttonPathname + "/LayoutLevel2-" + (std::to_string(j + 1)) + ".txt";
                    ofstream basicFileStream(buttonfilename);
                    std::string LvL2ButtonPathname = buttonPathname + "/LayoutLevel2-" + (std::to_string(j + 1)) + "-panels";
                    int check = mkdir(LvL2ButtonPathname.c_str());
                    for (int k = 0; k < 7; k++) {
                        std::string LvL3Buttonfilename = LvL2ButtonPathname + "/LayoutLevel3-" + (std::to_string(k + 1)) + ".txt";
                        ofstream basicFileStream(LvL3Buttonfilename);
                    }
                }
            }
            //set the set selection control
            size_t size = sizeof(buttonSetNames) / sizeof(buttonSetNames[0]);

            std::string names[4] = { "Subjective","Objective","Assesment","Plan" };
            //sets the original set as the first and saves basic set information
            //userSelection->SetSelection(0);
            for (int i = 0; i < 4; i++) {
                newSetStream << names[i];
                newSetStream << "\n";
                newSetStream << (pathname + "/layout" + std::to_string(i + 1) + ".txt");
                newSetStream << "\n";
            }
            setCount++;
            newSetStream.close();
            saveButtonSetInfo("SetInfoAll.txt");
        }
    }
}
/// <summary>
/// saves the panels and buttons for the current button set
/// </summary>
/// <param name="event"></param>
void MainFrame::savePanelsAndButtons(wxCommandEvent & event) {
    currentButtonSet->savePanelsAndButtonsNP();
    //saveButtonSetInfo("SetInfoAll.txt");
}
void MainFrame::saveButtonSetInfo(std::string path) {
    //open the fstream used to save the sets data when called on to do so
    try {
        setInfoOut.open(path, std::ofstream::out | std::ofstream::trunc);
        if (setInfoOut.fail()) {
            popUpHandeler->errorMessage("failed to save layout");
            setInfoOut.close();
            return;
        }
        // if the file opend properly save all the information to the users file path
        if (!setInfoOut.fail()) {
            for (size_t i = 0; i < buttonSetList.size(); i++) {
                setInfoOut << buttonSetList.at(i)->getSetName().ToStdString();
                setInfoOut << "\n";
                setInfoOut << buttonSetList.at(i)->getPanelCount();
                setInfoOut << "\n";
                setInfoOut << buttonSetList.at(i)->getPath();
                setInfoOut << "\n";
                setInfoOut.close();
            }
        }
    }
    catch (...) { popUpHandeler->errorMessage("an error occured in main.cpp"); }
    setInfoOut.close();
}
/// <summary>
/// loads the information for defined button sets
/// </summary>
/// <param name="path"></param>
void MainFrame::loadButtonSetInfo(std::string path) {
    //open the file path containing the setinformation to be loaded
    std::string filepath = path;
    setInfoIn.open(filepath);
    std::string setNo;
    std::string setName;
    std::string setPath;
    std::string sigPath;
    
    //load in and save the information for the newly loaded set
    if (setInfoIn.is_open()) {
        if (setInfoIn.peek() != std::ifstream::traits_type::eof()) {
            while (setInfoIn.peek() != EOF) {
                getline(setInfoIn, setName);
                getline(setInfoIn, setNo);
                getline(setInfoIn, setPath);
                getline(setInfoIn, sigPath);

                int SetNo = std::stoi(setNo);
                buttonSetNames[setCount] = setName;
                buttonSetList.push_back(new ButtonSet(this, setName, setCount, setPath, sigPath, SetNo));
                setCount++;

            }
        }
        else {
            newSet();
        }
        //test for errors
        if (!buttonSetList.empty()) {
            currentButtonSet = buttonSetList.at(0);
            setInfoIn.close();
        }

    }else{ 
        setInfoIn.close();
        popUpHandeler->errorMessage("an error occured in main.cpp"); 
    }
}
/// <summary>
/// allows the Navlinks to swap panels to thier stores panel 
/// </summary>
/// <param name="evt"></param>
void MainFrame::linkNavigation(wxHyperlinkEvent& evt) {
    NavLink* tempLink = (NavLink*)evt.GetEventObject();
    swapPanels(tempLink->getPanel());
}
/// <summary>
/// binds all the buttons loaded from file to thier appropriate methods
/// </summary> 
void MainFrame::bindAllButtons() {
    //loop over all button sets
    for (size_t k = 0; k < buttonSetList.size(); k++) {
        size_t size = buttonSetList.at(k)->getPanelList().size();
        //loop over all panels in a set
        for (size_t j = 0; j < size; j++) {
            ButtonPanel* tempPanelLevel1 = buttonSetList.at(k)->getPanelList().at(j);
            //loop over all buttons on the panel
            for (size_t i = 0; i < tempPanelLevel1->getQLinkList().size(); i++) {
                wxButton* button = tempPanelLevel1->getQLinkList().at(i);
                std::string name = button->GetName().ToStdString();
                //bind each button to the functions that apply
                button->Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);
                if (functionHelper->isNumber(name)) {
                    button->Bind(wxEVT_BUTTON, &MainFrame::swapHelper, this);
                }
                
                else if (functionHelper->isText(name)) {
                    button->Bind(wxEVT_BUTTON, &MainFrame::readDialogFile, this);
                }
            }
            for (size_t l = 0; l < tempPanelLevel1->getSubPanelList().size(); l++) {
                ButtonPanel* tempPanelLevel2 = tempPanelLevel1->getSubPanelList().at(l);
                for (size_t i = 0; i < tempPanelLevel2->getQLinkList().size(); i++) {
                    wxButton* button2 = tempPanelLevel2->getQLinkList().at(i);
                    std::string name = button2->GetName().ToStdString();
                    button2->Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);
                    if (functionHelper->isNumber(name)) {
                        button2->Bind(wxEVT_BUTTON, &MainFrame::swapHelper, this);
                    }
                }
                for (size_t l = 0; l < tempPanelLevel2->getSubPanelList().size(); l++) {
                    ButtonPanel* tempPanelLevel3 = tempPanelLevel2->getSubPanelList().at(l);
                    for (size_t i = 0; i < tempPanelLevel3->getQLinkList().size(); i++) {
                        wxButton* button3 = tempPanelLevel3->getQLinkList().at(i);
                        std::string name = button3->GetName().ToStdString();
                        button3->Bind(wxEVT_RIGHT_DOWN, &MainFrame::onRightClick, this);
                        if (functionHelper->isNumber(name)) {
                            button3->Bind(wxEVT_BUTTON, &MainFrame::swapHelper, this);
                        }
                    }
                }
            }
        }
    }
}
/// 
///when the user clicks light blue text the a list of popups will be displayed, 
///upon selection a new pop up will show and all the user to change the text
void MainFrame::dialogURLHelper(wxTextUrlEvent& evt) {
    int CptOrDx = 0;

    wxString dialogChoice = popUpHandeler->SingleChoiceDialog("DialogInformation/DialogSelection-2.txt", "select a popup.");
    wxString result;
    if (dialogChoice == "Allergies") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt", "Allergies");
        CptOrDx = 1;
    }
    if (dialogChoice == "Body") {
        result = popUpHandeler->bodyDialog("Select a location(s)");
        CptOrDx = 1;
    }
    if (dialogChoice == "Cervical Adjustment") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Cervicaladjustment.txt", "Cervical Adjustment");
        CptOrDx = 1;
    }
    if (dialogChoice == "Compicating Factors") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/CompicatingFactors.txt", "Compicating Factors");
        CptOrDx = 1;
    }
    if (dialogChoice == "Contraindiction") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/contraindication.txt", "contraindication");
        CptOrDx = 1;
    }
    if (dialogChoice == "Diet & Nutrition") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/DietNutrition.txt", "Diet & Nutrition");
        CptOrDx = 1;
    }
    if (dialogChoice == "Dificulty Performing") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/dificultyPerforming.txt", "Dificulty Performing");
        CptOrDx = 1;
    }
    if (dialogChoice == "Drugs Medication") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/drugsMedication.txt", "Drugs Medication");
        CptOrDx = 1;
    }
    if (dialogChoice == "Exercise Routine") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/ExerciseRoutine.txt", "Exercise Routine");
        CptOrDx = 1;
    }
    if (dialogChoice == "Freqency of pain") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/FreqOfPain.txt", "Freqency of pain");
        CptOrDx = 1;
    }
    if (dialogChoice == "Family History") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/familyHistory.txt", "family History");
        CptOrDx = 1;
    }
    if (dialogChoice == "Goals") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/goals.txt", "Goals");
        CptOrDx = 1;
    }

    if (dialogChoice == "Improve/Decline") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/BetterorWorse.txt", "Improve/Decline");
        CptOrDx = 1;
    }
    if (dialogChoice == "Injury") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Injury.txt", "Injury");
        CptOrDx = 1;
    }
    if (dialogChoice == "Life Affected") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/LifeAffected.txt", "Life Affected");
        CptOrDx = 1;
    }
    if (dialogChoice == "Percent or vas") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/percentorvas.txt", "Percent or vas");
        CptOrDx = 1;
    }
    if (dialogChoice == "Postural Change") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/posturalChange.txt", "Postural Change");
        CptOrDx = 1;
    }
    if (dialogChoice == "Quality of Discomfort") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/QualityOfDiscomfort.txt", "Quality of Discomfort");
        CptOrDx = 1;
    }
    if (dialogChoice == "Rate of Improvement") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/rateofImprove.txt", "rateofImprove");
        CptOrDx = 1;
    }
    if (dialogChoice == "Restrictions") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/restrictions.txt", "Restrictions");
        CptOrDx = 1;
    }

    if (dialogChoice == "ROM region") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/ROMregion.txt", "ROM region");
        CptOrDx = 1;
    }
    if (dialogChoice == "Social habits") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/socialhabits.txt", "Social habits");
        CptOrDx = 1;
    }
    if (dialogChoice == "Surgical history") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/surgicalHistory.txt", "Surgical history");
        CptOrDx = 1;
    }
    if (dialogChoice == "Specific Muscles") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/SpecificMuseles.txt", "Specific Muscles");
        CptOrDx = 1;
    }
    if (dialogChoice == "Side") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/side.txt", "Side");
        CptOrDx = 1;
    }
    if (dialogChoice == "Spine") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/spineSegmentsL-R.txt", "Spine");
        CptOrDx = 1;
    }
    if (dialogChoice == "Tone Intensity") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/ToneIntensity.txt", "Tone Intensity");
        CptOrDx = 1;
    }
    if (dialogChoice == "Work") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/work.txt", "Work");
        CptOrDx = 1;
    }
    if (dialogChoice == "Vas") {
        result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Vas.txt", "VAS");
        CptOrDx = 1;
    }
    if (dialogChoice == "Ankle") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-ankle.txt", "Dx Ankel");
        CptOrDx = 2;
    }
    if (dialogChoice == "Cervical Spine") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-cervicalSpine.txt", " DxCervical Spine");
        CptOrDx = 2;
    }
    if (dialogChoice == "Elbow Forarm") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/ctpcodes-ElbowForearm.txt", "Dx Elbow Forarm");
        CptOrDx = 2;
    }
    if (dialogChoice == "Foot") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-foot.txt", "Dx Foot");
        CptOrDx = 2;
    }
    if (dialogChoice == "Hand") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-hand.txt", "Dx Hand");
        CptOrDx = 2;
    }
    if (dialogChoice == "Head") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-head.txt", "Dx Head");
        CptOrDx = 2;
    }
    if (dialogChoice == "Hip-Thigh") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-hip_thigh.txt", "Dx Hip-Thigh");
        CptOrDx = 2;
    }
    if (dialogChoice == "Knee") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/ctpcodes-knee.txt", "Dx Knee");
        CptOrDx = 2;
    }
    if (dialogChoice == "Lumbosacral Spine") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-LumbosacralSpine.txt", "Dx Lumbosacral Spine");
        CptOrDx = 2;
    }
    if (dialogChoice == "Shoulder") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-shoulder.txt", "Dx Shoulder");
        CptOrDx = 2;
    }
    if (dialogChoice == "Thoacic Spine") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-thoacicSpine.txt", "Dx Thoacic Spine");
        CptOrDx = 2;
    }
    if (dialogChoice == "Wrist") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcodes-Wrist.txt", "Dx Wrist");
        CptOrDx = 2;
    }
    if (dialogChoice == "Manipulation") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptChiropractic-Manipulation-CPT-Coding.txt", " Manipulation");
        CptOrDx = 2;
    }
    if (dialogChoice == "Management") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptEvaluation-and-Management-Codes.txt", " Management");
        CptOrDx = 2;
    }
    if (dialogChoice == "Misc") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cpt-misc.txt", "Misc");
        CptOrDx = 2;
    }
    if (dialogChoice == "Rehabilitation") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptcode-Physical-Medicine-Rehabilitation.txt", "Rehabilitation");
        CptOrDx = 2;
    }
    if (dialogChoice == "Xray") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cptext-xray.txt", "Xray");
        CptOrDx = 2;
    }
    if (dialogChoice == "Place of Service") {
        result = popUpHandeler->MultipleChoiceDialog("Dialogs/cptcodes/cpt-placeofService.txt", "Place of Service");
        CptOrDx = 2;
    }
    if (dialogChoice != "OTHER") {
        int urlS = evt.GetURLStart();
        int urlE = evt.GetURLEnd() + 1;
        mainEditBox->Remove(urlS, urlE);
        if (CptOrDx==1) {
            mainEditBox->WriteText("<" + result + ">");
        }
        else if (CptOrDx == 2) {
            mainEditBox->WriteText("<<" + result + ">>");
        }

        
        mainEditBox->EndURL();
        mainEditBox->EndAllStyles();
        mainEditBox->WriteText("\n");
    }

}
/// <summary>
/// opens the dialog creation dialog
/// </summary>
void MainFrame::openDialogCreator() {
    if (DialogCreationHelper->ShowModal() == wxID_OK) {    
    }
}
void MainFrame::openCPT_DxEditor(wxCommandEvent& WXUNUSED(event)) {
    //present a selection of cpt-dx code files, when selected a dialog editor will open with the apropriate file information
    wxString str = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/names.txt", "select a code catagory to edit.");
    std::string name = str.ToStdString();
    if (str != "OTHER" && !str.empty()) {
        DialogEditingHelper = new DialogEditor(this, wxID_ANY, name);
        DialogEditingHelper->openCodeFile();
        if (DialogEditingHelper->ShowModal() == wxID_OK) {


        }
        delete DialogEditingHelper;
    }

}
void MainFrame::openGenDialogEditor(wxCommandEvent& WXUNUSED(event)) {
    //present a selection of paragraph files, when selected a dialog editor will open with the apropriate file information
    int index = popUpHandeler->SingleChoiceDialogIndex("Dialogs/0DialogNames.txt", "select a Dialog to edit.");
    if (index != -1) {
        index += 1;
        std::string name = "Dialogs/DialogFile"+ std::to_string(index)+".txt";
    
        DialogEditingHelper = new DialogEditor(this, wxID_ANY, name);
        DialogEditingHelper->openGenFile();
        if (DialogEditingHelper->ShowModal() == wxID_OK) {


        }
        delete DialogEditingHelper;
    }
}
void MainFrame::readDialogFile(wxCommandEvent& event) {
    if (isFileOpen()) {
        //reads the information associated with the button from file
        wxButton* buttn = (wxButton*)event.GetEventObject();
        std::string file = buttn->GetName().ToStdString();
        std::ifstream fileReader(file);
        std::vector<std::string> fileLines;
        int i = 0;
        std::string line;
        if (fileReader.is_open()) {
            while (getline(fileReader, line, '~')) {
                if (line != "") {
                    fileLines.push_back(line);
                }
            }
        }
        //writes the information to the screen
        DialogWriter(fileLines);
        fileReader.close();
    }
    else {
        popUpHandeler->errorMessage("There is no file open.\n Please open a file to write.");
    }
}
/// <summary>
/// dialog writer takes in a set of lines, and deturmines what dialog or what formmating to use and 
/// writes it to the rich text control
/// </summary>
/// <param name="lines"></param>
void MainFrame::DialogWriter(std::vector<std::string> lines) {
    size_t i = 0;
    while (i < lines.size()) {
        wxString result;
        int linkInd = lines.at(i).find("<");
        if (lines.at(i).find("<") != std::string::npos) {
            std::string str = lines.at(i);

            for (int i = 0; i < 3; i++) {
                str.pop_back();
            }
            str = str.substr(3, str.size());

            if (lines.at(i).at(1) == 'b') {
                mainEditBox->BeginBold();
                mainEditBox->WriteText(str);
                mainEditBox->EndBold();
            }
            if (lines.at(i).at(1) == 'u') {
                mainEditBox->BeginUnderline();
                mainEditBox->WriteText(str);
                mainEditBox->EndUnderline();
            }
            if (lines.at(i).at(1) == 'i') {
                mainEditBox->BeginItalic();
                mainEditBox->WriteText(str);
                mainEditBox->EndItalic();
            }

            if (str == "Dialog-ID-Allergies") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt", "Allergies");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Body") {
                result = popUpHandeler->bodyDialog("body");

                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Calender") {
                result = popUpHandeler->Calender();
             
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Cervical-Adjustment") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/Cervicaladjustment.txt", "Cervical Adjustment");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Complicating-Factors") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/CompicatingFactors.txt", "Compicating Factors");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Contraindiction") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/contraindication.txt", "Contraindication");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Nutrition") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/DietNutrition.txt", "Diet and Nutrition");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Dificulty-Performing") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/dificultyPerforming.txt", "Dificulty Performing Tasks");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Drugs-Medication") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/drugsMedication.txt", "Drugs and Medication");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Exercise") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/ExerciseRoutine.txt", "Exercise Routine");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Family-History") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/familyHistory.txt", "Family History");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Pain") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/FreqOfPain.txt", "Frequency of Pain");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Goals") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/goals.txt", "Goals");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Improve/Decline") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/BetterorWorse.txt", "Improve/Decline");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Injury") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/Injury.txt", "Injury");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Life-Affected") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/LifeAffected.txt", "Life Affected");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Percentorvas") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/percentorvas.txt", "Percent or Vas");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-PosturalChange") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/posturalChange.txt", "Postural Change");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-QualityofDiscomfort") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/QualityOfDiscomfort.txt", "Quality of Discomfort");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Improve") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/rateofImprove.txt", "Rate of Improvement");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Restrictions") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/restrictions.txt", "Restrictions");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-ROMregion") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/ROMregion.txt", "ROM Region");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Social") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/socialhabits.txt", "Social Habits");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Surgical") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/surgicalHistory.txt", "Surgical History");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Muscle") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/SpecificMuseles.txt", "Specific Museles");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Side") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/side.txt", "Side");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();

            }
            if (str == "Dialog-ID-Spine") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/spineSegmentsL-R.txt", "Spine SegmentsL-R");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();

            }
            if (str == "Dialog-ID-ToneIntensity") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/ToneIntensity.txt", "Tone Intensity");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Work") {
                result = popUpHandeler->MultipleChoiceDialog("DialogInformation/work.txt", "Work");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "Dialog-ID-Vas") {
                result = popUpHandeler->SingleChoiceDialog("DialogInformation/Vas.txt", "Vas");
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Ankle") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-ankle.txt", "Ankle-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-CervicalSpine") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-cervicalSpine.txt", "Cervical-Spine-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-ElbowForarm") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/ctpcodes-ElbowForearm.txt", "Elbow-Forarm-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Foot") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-foot.txt", "Foot-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Hand") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-hand.txt", "Hand-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Head") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-head.txt", "Head-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Hip-Thigh") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-hip_thigh.txt", "Hip-Thigh-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Knee") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/ctpcodes-knee.txt", "Knee-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Lumbosacral-Spine") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-LumbosacralSpine.txt", "Lumbosacral-Spine-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Shoulder") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-shoulder.txt", "Shoulder-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-ThoacicSpine") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-thoacicSpine.txt", "Thoacic-Spine-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Wrist") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcodes-Wrist.txt", "Wrist");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Manipulation") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptChiropractic-Manipulation-CPT-Coding.txt", "Manipulation-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Management") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptEvaluation-and-Management-Codes.txt", "Management-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Misc") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cpt-misc.txt", "Misc-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Rehabilitation") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptcode-Physical-Medicine-Rehabilitation.txt", "Rehabilitation");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-Xray") {
                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cptext-xray.txt", "Xray-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
            if (str == "CPT-ID-PlaceofService") {

                result = popUpHandeler->SingleChoiceDialog("Dialogs/cptcodes/cpt-placeofService.txt", "Place of Service-CPT");
                result = "<<" + result + ">>";
                mainEditBox->BeginTextColour(wxColour(52, 128, 235));
                mainEditBox->BeginURL(result);
                mainEditBox->WriteText(result);
                mainEditBox->EndURL();
                mainEditBox->EndTextColour();
            }
        }
        else {
            mainEditBox->WriteText(lines.at(i));
        }
        i++;
    }
}