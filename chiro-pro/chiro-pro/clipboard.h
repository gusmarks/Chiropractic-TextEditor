#pragma once
#include <wx/wx.h>

enum {
	BUTTON_Newline,
	BUTTON_Closedialog,
	BUTTON_Okay
};
class ClipboardDialog : public wxFrame {


public:
	ClipboardDialog(const wxString& title, const wxPoint& pos, const wxSize& size);

	wxButton* newLine;
	wxButton* okay;
	wxButton* cancel;
	wxListBox* selections;
	wxArrayString selectionStrings;
	wxBoxSizer* vertSizer;
	wxBoxSizer* bottomButtonSizer;

	void cancelDialog(wxCommandEvent& event);

	void Okay(wxCommandEvent& event);
	void NewLine(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()

	
 };
