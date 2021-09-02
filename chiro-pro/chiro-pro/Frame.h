#pragma once
#ifndef __Frame_H
#define __Frame_H
//includes, all wx/ includes are required by the various aspects of wxWidgets used
//all "" includes designate the custom header files used to make the application
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/hyperlink.h>
#include <ctime>
#include <wx/wfstream.h>
//custome includes
#include "DialogHelper.h"
#include "DialogCreator.h"
#include "DialogEditor.h"
#include "ButtonPanel.h"
#include "FunctionHelper.h"
#include "ButtonSet.h"

#include "NavLink.h"
#include "LinkPanel.h"
#include "ControlPanel.h"
#include "billingInfoCreator.h"
enum
{
	TEXT_Main,
	MENU_New,
	MENU_Open,
	MENU_Close,
	MENU_Save,
	MENU_SaveAs,
	MENU_Quit,
	BUTTON_Add,
	BUTTON_Minus,
	BUTTON_Sign,
	BUTTON_Swap,
	BUTTON_Write,
	BUTTON_Dialog,
	BUTTON_Panel,
	Button_ControlPanel,
	BUTTON_Back,
	BUTTON_NewSet,
	CHOICE_SWAP_Set,
	MENU_EditButtonName,
	MENU_EditButtonText,
	MENU_RemoveButton,
	MENU_SaveButtons,
	MENU_chanegDialog,
	MENU_BillingDoc,
	MENU_EditCodes,
	MENU_EditGenDialog,
	LINK_NAVIGATE,
	ID_FORMAT_BOLD,
	ID_FORMAT_ITALIC,
	ID_FORMAT_UNDERLINE,
	ID_FORMAT_STRIKETHROUGH,
	ID_FORMAT_SUPERSCRIPT,
	ID_FORMAT_SUBSCRIPT,
	ID_RICHTEXT_STYLE_COMBO,
	PANEL_TEXT,
	ID_FORMAT_ALIGN_LEFT,
	ID_FORMAT_ALIGN_CENTRE,
	ID_FORMAT_ALIGN_RIGHT,

};
//MainFrame extends the wxFrame class to make a window
class MainFrame : public wxFrame
{
// private variable are stored to keep them from being accessd by other means
private:
	//buttonToEdit allows the user to edit the lable and contents of the button
	wxButton* buttonToEdit;
	//this file name saves the current document being worked on
	wxString fileName;
	//the text box on the main screen, currently cant be manualy edited
	wxRichTextCtrl* mainEditBox;
	// the menu bar with save, load, etc functions
	wxMenuBar* mainMenu;
	wxToolBar* toolBar;
	// Button sets are defined by a name, this will be the primary set of buttons and panels used by a given user
	// there is a maximum of 10 user sets, recored in the ButtonSetNames array, the SetCount variable(consider wraping them togeather)
	// the ButtonSetList holds the user data once loaded, and the currentButtonSet references the set being used at any given time by the user
	wxString buttonSetNames[10] = {};
	int setCount = 0;
	std::vector<ButtonSet*> buttonSetList;
	ButtonSet* currentButtonSet;
	// user selection controls what ButtonSet is being used
	wxChoice* userSelection;
	// a set of sizers to orginize the page, ButtonSetSizer aranges the set changer and the new set button, MainSizer holds all other sizers
	//and and the text box, finaly ControlSizer aranges the loose buttons 
	wxBoxSizer* buttonSetSizer, *controlSizer, *mainSizer, *linkSizer;
	//control panel holds all of the buttons used to manipulate buttons
	controlPanel* controls;
	//the linkpanel holds the links that allow the user to navagate the diferant button panels
	linkPanel* links;
	//the DialogHelper allows for popup use, while function handeler, deals with functions that dont quite fit in the main files 
	DialogHelper* popUpHandeler;
	//function helper performs various auxilery functions
	FuncHelper* functionHelper;
	//dialogcreation helper, opens and controls the dialog creator allowing the user to make all buttons and popups 
	//easy to customize and make.
	DialogCreator* DialogCreationHelper;
	DialogEditor* DialogEditingHelper;
	// check to see if the document has been signed or not
	bool Signed = false;
	//check if somthing is bold?
	bool isBold=false,isItalic=false,isunderline=false;
	// input and output streams to read txt files that store information about the user set
	std::ifstream setInfoIn;
	std::ofstream setInfoOut;
	// navlinks are hyperlinks that swap to and from panels inside a set.
	NavLink* link1, *link2, *link3;
public:
	//Constuctor for the frame
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	// function headers for event functions.
	/// </summary>
/// this method opens a dialog box that lets the user search thier file system 
/// and open any.xml file
/// </summary>
/// <param name="WXUNUSED"></param>
	void openFile(wxCommandEvent& WXUNUSED(event))
	{
		if (fileName == ""||popUpHandeler->confirmIntent("are you sure you want to open a new file and close this one?")) {
			wxFileDialog
				openFileDialog(this, _("Open Xml file"), "", "",
					"Xml files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			if (openFileDialog.ShowModal() == wxID_CANCEL)
				return;     // the user changed idea...

			// proceed loading the file chosen by the user;
			// this can be done with e.g. wxWidgets input streams:
			wxFileInputStream input_stream(openFileDialog.GetPath());
			wxString filename = openFileDialog.GetFilename();
			filename.erase(filename.Length()-4,4);
			fileName = filename;
			if (!input_stream.IsOk())
			{
				wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
				return;
			}
			wxString file = openFileDialog.GetPath();

			mainEditBox->Enable();
			mainEditBox->SetBackgroundColour(wxColour(255, 255, 255));
			mainEditBox->SetEditable(true);

			mainEditBox->LoadFile(file);

		}
		
	}
	/// <summary>
/// if filename variable is empty prompt the user to enter a name then add .doc to the end
/// if there is already a name save under the name.
/// </summary>
/// <param name="WXUNUSED"></param>
	void saveFile(wxCommandEvent& WXUNUSED(event))
	{
		if (MainFrame::getFilename().empty()) {
			MainFrame::setFilename(
				wxGetTextFromUser("Enter Name of File.do not include file extension", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true));
			//MainFrame::appendFilename(".doc");
			wxString xmlName = MainFrame::getFilename() + ".xml", docName = MainFrame::getFilename() + ".doc";
			mainEditBox->SaveFile("PatientFileParse/" + MainFrame::getFilename() + ".txt");
			//mainEditBox->SaveFile("PatientFileParse/" + MainFrame::getFilename() + ".docx");
			mainEditBox->SaveFile("PatientFileLoad/" + MainFrame::getFilename() + ".xml");
			mainEditBox->SaveFile("PatientFileView/" + MainFrame::getFilename() + ".html");

		}
		else {
			mainEditBox->SaveFile("PatientFileParse/" + MainFrame::getFilename() + ".txt");
			//mainEditBox->SaveFile("PatientFileParse/" + MainFrame::getFilename() + ".docx",wxRICHTEXT_TYPE_ANY);
			mainEditBox->SaveFile("PatientFileLoad/" + MainFrame::getFilename() + ".xml");
			mainEditBox->SaveFile("PatientFileView/" + MainFrame::getFilename() + ".html");
		}
	}
	/// <summary>
	///this method opens a dialog box that lets the user search thier file system and 
	/// save a file with a custom name regardless of if there is filename saved.
	/// </summary>
	/// <param name="WXUNUSED"></param>
	void saveFileAs(wxCommandEvent& WXUNUSED(event))
	{
		//set up rich text buffer
		wxFileDialog
			saveFileDialog(this, _("Save Doc file"), "", "",
				"xml files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL) {
			return;     // the user changed idea...
		}

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
/// this is esentualy the same as new file. clearing the text editor and the filename
/// too let the user have a blank file
/// </summary>
/// <param name="WXUNUSED"></param>
	void closeFile(wxCommandEvent& WXUNUSED(event))
	{
		if (popUpHandeler->confirmIntent("are you sure you want to close the file?")) {
			mainEditBox->DiscardEdits();
			mainEditBox->Clear();
			mainEditBox->Disable();
			mainEditBox->SetBackgroundColour(wxColour(211, 211, 211));
			mainEditBox->SetEditable(false);
			MainFrame::setFilename("");
		}
	}

	//saves the information about the current button set to file
	void saveButtonSetInfo(std::string path);
	//loads the information about the selected set from file
	void loadButtonSetInfo(std::string path);
	//saves the buttons and panel information to file, for the currest user set
	void savePanelsAndButtons(wxCommandEvent& event);
	//adds a button to the current panel
	void addButton(wxCommandEvent& event);
	//void ButtonWrite(wxCommandEvent& event);
	//swaps the panel the user is currently using for the selected panel
	void swapPanels(ButtonPanel* newPanel);
	//swap heper just provides some addition details for swap panels
	void swapHelper(wxCommandEvent& event);
	//start swap helper provides additional details for swap panels relating the first time the panels load
	void startSwapHelper(wxCommandEvent& event);
	// swaps the user set to the selected one
	void swapButtonSet(wxCommandEvent& evt);
	//makes a new user set, with the appropriate files and directories
	void newSet(wxCommandEvent& event);
	//signs the document, adds some descriptive text, and an image signiture
	void sign(wxCommandEvent& WXUNUSED(event));
	//on right click brings up a menu when right clicking some componants, this allows the user to edit certain componants
	void onRightClick(wxMouseEvent& event);
	// on popupclick dose very similar to on right click for pop ups
	void onPopUpCLick(wxCommandEvent& event);
	//the linknavigation lets the user navigate panels of buttons
	void linkNavigation(wxHyperlinkEvent& evt);
	//openDialog opens the dialogcreator, consider renaming
	void openDialogCreator();
	void openCPT_DxEditor(wxCommandEvent& WXUNUSED(event));
	void openGenDialogEditor(wxCommandEvent& WXUNUSED(event));
	//read dialog file, reads in a file and passes the information to the dialog writer
	void readDialogFile(wxCommandEvent& event);
	//dialog writer writes the lines of a file to the text box, it also includes dialogs
	void DialogWriter(std::vector<std::string>);

	/// <summary>
/// builds the navlinks based on the currentpanel and how many previous elements thier are
/// </summary>
/// <param name="panelToWork"></param>
	void buildNavLinks(ButtonPanel* panelToWork) {
		wxString name = panelToWork->GetName();
		switch (panelToWork->getLevel()) {
		case 0: {
			if (link1 == nullptr) {
				link1 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				
				link1->setName(name);
				link1->setPanel(panelToWork);
			}
			if (link2 != nullptr) {
				link2->Hide();
				link2 = nullptr;
			}
			if (link3 != nullptr) {
				link3->Hide();
				link3 = nullptr;
			}

			links->clearLinkSizer();
			//linksizer->Clear();
			link1->Hide();
			links->addToLinkSizer(link1);
			//linksizer->Add(link1);
			link1->Show();
			links->linkLayout();
			//linksizer->Layout();
			Layout();
			break;
		}
		case 1: {
			if (link2 == nullptr) {
				link2 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				link2->setName(panelToWork->GetName());
				link2->setPanel(panelToWork);
			}

			if (link3 != nullptr) {
				link3->Hide();
				link3 = nullptr;
			}

			links->clearLinkSizer();
			link1->Hide();
			link2->Hide();
			links->addToLinkSizer(link1);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link2);
			link1->Show();
			link2->Show();
			links->linkLayout();
			Update();
			break;
		}
		case 2: {
			if (link3 == nullptr) {
				link3 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				link3->setName(panelToWork->GetName());
				link3->setPanel(panelToWork);
			}
			links->clearLinkSizer();
			link1->Hide();
			link2->Hide();
			link3->Hide();
			links->addToLinkSizer(link1);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link2);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link3);
			link1->Show();
			link2->Show();
			link3->Show();
			links->linkLayout();
			Update();
			break;
		}

		}
	}
	bool isFileOpen() {
		return mainEditBox->IsEditable();
	}

	/// <summary>
	/// distroy panels will systimaticly delete all the the users panels this is used to swap user sets
	/// </summary>
	/// <param name="usr"></param>
	void destroyPanels(wxString usr);


	//onbold will make the selected text and future text bold until toggled off
	void onBold(wxCommandEvent& WXUNUSED(event))
	{
		if (isBold == false) {
			mainEditBox->BeginBold();
			isBold = true;
		}
		else if (isBold == true) {
			mainEditBox->EndBold();
			isBold = false;
		}
			
		mainEditBox->ApplyBoldToSelection();
	}
	//onItalic will make the selected text and future text italic until toggled off
	void onItalic(wxCommandEvent& WXUNUSED(event))
	{
		if (isItalic == false) {
			mainEditBox->BeginItalic();
			isItalic = true;
		}
		else if (isItalic == true) {
			mainEditBox->EndItalic();
			isBold = isItalic;
		}
		mainEditBox->ApplyItalicToSelection();
	}
	//onUnderline will male the selected text and future text underlined until toggled off
	void onUnderline(wxCommandEvent& WXUNUSED(event))
	{
		if (isunderline == false) {
			mainEditBox->BeginUnderline();
			isunderline = true;
		}
		else if (isunderline == true) {
			mainEditBox->EndUnderline();
			isunderline = false;
		}
		mainEditBox->ApplyUnderlineToSelection();
	}
	//onStrikethrough will male the selected text and future text striken through until toggled off
	void onStrikethrough(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_STRIKETHROUGH);
	}
	//onSuperscript will male the selected text and future text superscript until toggled off
	void onSuperscript(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	}
	//onSubscript will male the selected text and future text subscript until toggled off
	void onSubscript(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
	}
	//onAlignLeft will male the selected text and future text alighned left until toggled off
	void onAlignLeft(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_LEFT);
	}
	//onAlignCentre will male the selected text and future text aligned center until toggled off
	void onAlignCentre(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTRE);
	}
	//onAlignRight will male the selected text and future text aligned right until toggled off
	void onAlignRight(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_RIGHT);
	}

	//this bids all buttons the the appropriate functions, needs work
	void bindAllButtons();
	//this will re do the results of a dialog
	bool dialogButtonHelper();
	//this directs the url click to the dialog button helper function
	void dialogURLHelper(wxTextUrlEvent& evt);
	//this binds all pup up buttons, consider removing
	void popupButtonBind(std::string str, wxButton* btton);
	//this rebinds all pop up buttons, consider removing
	void popupButtonRebind(std::string str,wxButton* btton);
	// undbind buttons called between rebinding
	void unbindPopupButton(wxString str, wxButton* btton);

	/// <summary>
	/// this function opens the dialog count file and reads in the previos dialog count
	/// this keeps the count accurate when the program opens and closes
	/// </summary>
	int ManualgetDialogCount(std::string file) {
		std::ifstream fileReader(file);
		std::string fileCount;
		getline(fileReader, fileCount);
		if (fileCount != "") {
			return std::stoi(fileCount);
		}
		return 0;
	}
	/// //new file method, this method clears the texteditor and filename for saving.
	///allowing people to make and save new files
	/// </summary>
	/// <param name="WXUNUSED"> an unused Command event</param>
	void newFile(wxCommandEvent& WXUNUSED(event))
	{
		if (popUpHandeler->confirmIntent("are you sure you want to open a new file?")) {
			wxString name=wxGetTextFromUser("Enter Patient name", " ", "", NULL, wxDefaultCoord, wxDefaultCoord, true);
			wxString insurance= popUpHandeler->SingleChoiceDialog("DialogInformation/Insurance.txt","Select A Provider.");
			if (name == "") {
				return;
			}

			mainEditBox->Clear();
			MainFrame::setFilename(name);
			mainEditBox->Enable();
			mainEditBox->BeginBold();
			mainEditBox->WriteText("Patient Name: ");
			mainEditBox->EndBold();
			mainEditBox->WriteText("<"+name+">");
			mainEditBox->WriteText("   ");
			mainEditBox->BeginBold();
			mainEditBox->WriteText("Date of First Visit: ");
			mainEditBox->EndBold();
			mainEditBox->WriteText("<"+functionHelper->getDateToSignNoTime()+">");
			mainEditBox->WriteText("   ");
			mainEditBox->BeginBold();
			mainEditBox->WriteText("Insurance: ");
			mainEditBox->EndBold();
			mainEditBox->WriteText("<"+insurance+">");
			mainEditBox->SetBackgroundColour(wxColour(255, 255, 255));
			mainEditBox->SetEditable(true);
		}
	}
	void startingSate() {
		mainEditBox->Clear();
		MainFrame::setFilename("");
		mainEditBox->Disable();
		mainEditBox->SetBackgroundColour(wxColour(211, 211, 211));
		mainEditBox->SetEditable(false);
	}
	//getFilename is a getter method for the filename variable used for saving and loading textfiles
	wxString getFilename() {
		return this->fileName;
	}
	//setFIlename is a settermethod for the filename variable used for saving and loading textfiles
	void setFilename(wxString name) {
		this->fileName = name;
	}
	//appendDilename is a function to add somthing to the filename, typicaly an extention
	void appendFilename(wxString app) {
		this->fileName += app;
	}
	//getbuttonToEdit returns the referance to a button, used to edit said button
	wxButton* getbuttonToEdit() {
		return buttonToEdit;
	}
	//setbuttonToEditName changes the name of the button the user is editing
	void setbuttonToEditName(wxString Name) {
		this->getbuttonToEdit()->SetLabel(Name);
	}
	//setbuttonToEditText chaned the text of the button the user is editing
	void setbuttonToEditText(wxString Text) {
		this->getbuttonToEdit()->SetName(Text);
	}
	void removebuttonToEdit() {
		wxString text = this->getbuttonToEdit()->GetName();
		
		currentButtonSet->getCurrentPanel()->removeButtonWithTextOf(text);
		this->getbuttonToEdit()->Destroy();
	}
	//checks if the file has already been signed
	bool isSigned() {
		return this->Signed;
	}
	//tells the object weather or not the document has been singed.
	void setSigned(bool Sign) {
		this->Signed = Sign;
	}



	void gatherBillingInformation(wxCommandEvent& event) {
		billingInfoCreator BIC = billingInfoCreator("PatientFileParse", "billingInfo/billingDocument.txt",
			"billingInfo/billingDocument-Medicare.txt", "billingInfo/billingDocument-Regence.txt",
			"billingInfo/billingDocument-Other.txt");
		BIC.documentCreation();
	}
	/// <summary>
/// the quit function saves the button configuration to a file
/// then closes the program
/// </summary>
/// <param name="WXUNUSED"></param>
	void quit(wxCommandEvent& event)
	{
		event.Skip();
		Close(TRUE); // Tells the OS to quit running this process
	}
	
		DECLARE_EVENT_TABLE()
};

#endif