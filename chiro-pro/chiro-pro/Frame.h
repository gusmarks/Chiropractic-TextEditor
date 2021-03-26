#pragma once

#ifndef __Frame_H
#define __Frame_H
//includes, all wx/ includes are required by the various aspects of wxWidgets used
//all "" includes designate the custom header files used to make the application
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "DialogHelper.h"
#include "ButtonPanel.h"
#include "FunctionHelper.h"
#include "ButtonSet.h"
#include "buttonPanel.h"


//MainFrame extends the wxFrame class to make a window
class MainFrame : public wxFrame
{
//the private variables hardly need to be chanced and thus are placed here ---add more into this section later----
private:
	wxButton* ButtonToEdit;
	wxString fileName;
public:
	// all public variable are referanced offten and for development purposes are in the public catigory 
	wxTextCtrl* MainEditBox;
	wxMenuBar* MainMenu;
	wxString ButtonSetNames[10] = {"Rob"};
	int SetIndex = 1;
	wxChoice* userSelection;
	std::vector<ButtonSet*> ButtonSetList;
	wxString currentButtonSetName;
	ButtonSet* currentButtonSet;
	wxBoxSizer* ButtonSetSizer;
	wxBoxSizer* sizer;
	wxBoxSizer* ControlSizer;
	DialogHelper* popUpHandeler;
	FuncHelper* functionHelper;
	bool Signed = false;
	// function headers i try to keep only event functions, someof witch are only wrapper methods
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void openFile(wxCommandEvent& event);
	void saveFile(wxCommandEvent& event);
	void saveFileAs(wxCommandEvent& event);
	void closeFile(wxCommandEvent& event); 
	void AddButton(wxCommandEvent& event);
	void ButtonWrite(wxCommandEvent& event);
	void swapPanels(ButtonPanel* newPanel);
	void swapHelper(wxCommandEvent& event);
	void swapToPreviousPanel(wxCommandEvent& event);
	void clickURLinTextCtrl(wxMouseEvent& evt);
	void SwapButtonSet(wxCommandEvent& evt);
	void Sign(wxCommandEvent& WXUNUSED(event));
	void onRightClick(wxMouseEvent& event);
	void onPopUpCLick(wxCommandEvent& event);
	void destroyPanels(wxString usr);
	bool DoseUserExist(wxString usr);
	void newSet(wxCommandEvent& event);
	void SavePanelsAndButtons(wxCommandEvent& event);
	/// <summary>
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
	TEXT_Main = wxID_HIGHEST + 1,
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
	BUTTON_Panel,
	BUTTON_Back,
	BUTTON_NewSet,
	CHOICE_SWAP_Set,
	MENU_EditButtonName,
	MENU_EditButtonText,
	MENU_SaveButtons,
};
#endif