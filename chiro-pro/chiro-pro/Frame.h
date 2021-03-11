#pragma once
#ifndef __Frame_H
#define __Frame_H
#include <wx/frame.h>
//#include <wx/richtext/richtextctrl.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "DialogHelper.h"
#include "Buttons.h"


class MainFrame : public wxFrame
{

private:
	wxButton* ButtonToEdit;
	wxString fileName;
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	wxTextCtrl* MainEditBox;
	wxMenuBar* MainMenu;
	//int QLinkIndex = 0;
	int panelIndex = 0;
	wxBoxSizer* sizer;
	wxBoxSizer* ControlSizer;
	DialogHelper* popUpHandeler;
	ButtonPanel* currentPanel;

	std::vector<ButtonPanel*> panelList;
	bool Signed = false;
	

	bool isSigned();
	void setSigned(bool Sign);


	
	void quit(wxCommandEvent& event);
	void SaveButtons(wxCommandEvent& event);
	void newFile(wxCommandEvent& event);
	void openFile(wxCommandEvent& event);
	void saveFile(wxCommandEvent& event);
	//void SaveFile();
	void saveFileAs(wxCommandEvent& event);
	void closeFile(wxCommandEvent& event); 
	void AddButton(wxCommandEvent& event);
	void ButtonWrite(wxCommandEvent& event);
	
	void Sign(wxCommandEvent& WXUNUSED(event));
	void swapButtonPanels(wxCommandEvent& event);
	void onRightClick(wxMouseEvent& event);
	void onPopUpCLick(wxCommandEvent& event);
	wxString getFilename();
	void setFilename(wxString name);
	void appendFilename(wxString app);
	wxButton* getButtonToEdit();
	void setButtonToEditName(wxString Name);
	void setButtonToEditText(wxString Text);
	void swapPanels(ButtonPanel* newPanel);
	void swapHelper(wxCommandEvent& event);
	void swapToPreviousPanel(wxCommandEvent& event);
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
	MENU_EditButtonName,
	MENU_EditButtonText,
	MENU_SaveButtons
};
#endif