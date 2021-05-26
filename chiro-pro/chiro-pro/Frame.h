#pragma once

#ifndef __Frame_H
#define __Frame_H
//includes, all wx/ includes are required by the various aspects of wxWidgets used
//all "" includes designate the custom header files used to make the application
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/richtext/richtextctrl.h>
#include "DialogHelper.h"
#include "ButtonPanel.h"
#include "FunctionHelper.h"
#include "ButtonSet.h"
#include "buttonPanel.h"
#include "NavLink.h"
#include <wx/hyperlink.h>
#include "myRichTextCtrl.h"


//MainFrame extends the wxFrame class to make a window
class MainFrame : public wxFrame
{
// private variable are stored to keep them from being accessd by other means
private:
	//buttonToEdit allows the user to edit the lable and contents of the button
	wxButton* ButtonToEdit;
	//this file name saves the current document being worked on
	wxString fileName;
	//the text box on the main screen, currently cant be manualy edited
wxRichTextCtrl* MainEditBox;
	// the menu bar with save, load, etc functions
	wxMenuBar* MainMenu;
	wxToolBar* toolBar;
	// Button sets are defined by a name, this will be the primary set of buttons and panels used by a given user
	// there is a maximum of 10 user sets, recored in the ButtonSetNames array, the SetCount variable(consider wraping them togeather)
	// the ButtonSetList holds the user data once loaded, and the currentButtonSet references the set being used at any given time by the user
	wxString ButtonSetNames[10] = {};
	int SetCount = 0;
	std::vector<ButtonSet*> ButtonSetList;
	ButtonSet* currentButtonSet;
	// user selection controls what ButtonSet is being used
	wxChoice* userSelection;
	// a set of sizers to orginize the page, ButtonSetSizer aranges the set changer and the new set button, MainSizer holds all other sizers
	//and and the text box, finaly ControlSizer aranges the loose buttons 
	wxBoxSizer* ButtonSetSizer;
	wxBoxSizer* Mainsizer;
	wxBoxSizer* ControlSizer;
	wxBoxSizer* LinkSizer;

	
	//the DialogHelper allows for popup use, while function handeler, deals with functions that dont quite fit in the main files 
	DialogHelper* popUpHandeler;
	FuncHelper* functionHelper;
	// check to see if the document has been signed or not
	bool Signed = false;
	// input and output streams to read txt files that store information
	std::ifstream setInfoIn;
	std::ofstream setInfoOut;
	// navlinks are hyperlinks that swap to and from panels inside a set.
	NavLink* Link1;
	NavLink* Link2;
	NavLink* Link3;
public:
	//Constuctor for the frame
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	// function headers for event functions.
	void openFile(wxCommandEvent& event);
	void saveFile(wxCommandEvent& event);
	void saveFileAs(wxCommandEvent& event);
	void closeFile(wxCommandEvent& event); 

	void saveButtonSetInfo(std::string path);
	void loadButtonSetInfo(std::string path);
	void SavePanelsAndButtons(wxCommandEvent& event);
	void AddButton(wxCommandEvent& event);
	void ButtonWrite(wxCommandEvent& event);
	void swapPanels(ButtonPanel* newPanel);
	void swapHelper(wxCommandEvent& event);

	void clickURLinTextCtrl(wxMouseEvent& evt);

	void SwapButtonSet(wxCommandEvent& evt);
	void newSet(wxCommandEvent& event);

	void Sign(wxCommandEvent& WXUNUSED(event));
	void onRightClick(wxMouseEvent& event);
	void onPopUpCLick(wxCommandEvent& event);

	void OnBold(wxCommandEvent& event);
	void OnUpdateBold(wxUpdateUIEvent& event);
	void OnItalic(wxCommandEvent& event);
	void OnUnderline(wxCommandEvent& event);
	void OnStrikethrough(wxCommandEvent& event);
	void OnSuperscript(wxCommandEvent& event);
	void OnSubscript(wxCommandEvent& event);

	
	void LinkNavigation(wxHyperlinkEvent& evt);
	void BuildNavLinks(ButtonPanel* panelToWork);

	void destroyPanels(wxString usr);

	void OnAlignRight(wxCommandEvent& WXUNUSED(event));
	void OnAlignLeft(wxCommandEvent& WXUNUSED(event));
	void OnAlignCentre(wxCommandEvent& WXUNUSED(event));

	void DialogButton(wxCommandEvent& WXUNUSED(event));
	void EandM(wxCommandEvent& WXUNUSED(event));

/// //new file method, this method clears the texteditor and filename for saving.
///allowing people to make and save new files
/// </summary>
/// <param name="WXUNUSED"></param>
	void newFile(wxCommandEvent& WXUNUSED(event))
	{
		if (popUpHandeler->confirmIntent("are you sure you want to open a new file?")) {
			MainEditBox->Clear();
			MainFrame::setFilename("");
		}
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
	//getButtonToEdit returns the referance to a button, used to edit said button
	wxButton* getButtonToEdit() {
		return ButtonToEdit;
	}
	//setButtonToEditName changes the name of the button the user is editing
	void setButtonToEditName(wxString Name) {
		this->getButtonToEdit()->SetLabel(Name);
	}
	//setButtonToEditText chaned the text of the button the user is editing
	void setButtonToEditText(wxString Text) {
		this->getButtonToEdit()->SetName(Text);
	}
	//checks if the file has already been signed
	bool isSigned() {
		return this->Signed;
	}
	//tells the object weather or not the document has been singed.
	void setSigned(bool Sign) {
		this->Signed = Sign;
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
	BUTTON_Back,
	BUTTON_NewSet,
	CHOICE_SWAP_Set,
	MENU_EditButtonName,
	MENU_EditButtonText,
	MENU_SaveButtons,
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
#endif