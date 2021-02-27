#pragma once
#include <wx/frame.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
class ButtonPanel;
//#include "Toolbar.h"
class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	wxTextCtrl* MainEditBox;
	wxMenuBar* MainMenu;
	wxButton* QLinksList[10];
	int QLinkIndex = 0;
	wxBoxSizer* sizer;
	wxBoxSizer* ControlSizer;
	ButtonPanel* buttons1;
	ButtonPanel* buttons2;
	ButtonPanel* buttons3;
	ButtonPanel* buttons4;
	ButtonPanel* currentPanel;


	bool confirmIntent(wxString message);
	void quit(wxCommandEvent& WXUNUSED(event));
	void newFile(wxCommandEvent& event);
	void openFile(wxCommandEvent& event);
	void saveFile(wxCommandEvent& event);
	void SaveFile();
	void saveFileAs(wxCommandEvent& event);
	void closeFile(wxCommandEvent& event); 
	void AddButton(wxCommandEvent& event);
	void ButtonWrite(wxCommandEvent& event);
	
	void Sign(wxCommandEvent& WXUNUSED(event));
	void swapButtonPanels(wxCommandEvent& event);
	void onRightClick(wxMouseEvent& event);
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
	BUTTON_Write
};