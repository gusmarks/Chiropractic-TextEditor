#include "clipboard.h"

BEGIN_EVENT_TABLE(ClipboardDialog, wxFrame)

EVT_BUTTON(BUTTON_Okay, ClipboardDialog::Okay)
EVT_BUTTON(BUTTON_Closedialog, ClipboardDialog::cancelDialog)
EVT_BUTTON(BUTTON_Newline, ClipboardDialog::NewLine)
END_EVENT_TABLE()


ClipboardDialog::ClipboardDialog(const wxString& title, const wxPoint& pos, const wxSize& size) 
	: wxFrame((wxFrame*)NULL, -1, title, pos, size)
{

	newLine = new wxButton(this, BUTTON_Newline, "New Line", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
	okay = new wxButton(this, BUTTON_Okay, "Okay", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
	cancel = new wxButton(this, BUTTON_Closedialog, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "1");
	bottomButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	vertSizer = new wxBoxSizer(wxVERTICAL);

	selectionStrings = wxArrayString();

	selections = new wxListBox();
	selections->Create(this, wxID_ANY, wxDefaultPosition,wxSize(400,300), selectionStrings,wxLB_NEEDED_SB|wxLB_MULTIPLE|wxLB_SORT);
	
	bottomButtonSizer->Add(okay);
	bottomButtonSizer->Add(cancel);
	vertSizer->Add(newLine);
	vertSizer->Add(selections);
	vertSizer->Add(bottomButtonSizer);
	vertSizer->Layout();
	SetSizer(vertSizer);
	Centre();
}
void ClipboardDialog::cancelDialog(wxCommandEvent& event) {
	//return wxEmptyString;
}
void ClipboardDialog::Okay(wxCommandEvent& event) {
	if (selections->GetSelection() != wxNOT_FOUND) {
		//return selections->GetString(selections->GetSelection());
	}
	//return wxEmptyString;
}
void ClipboardDialog::NewLine(wxCommandEvent& event) {
	wxString newChoice = wxGetTextFromUser("Enter a line to add to the choices", " ", "enter here", NULL, wxDefaultCoord, wxDefaultCoord, true);
	
	selections->Append(newChoice);
	selectionStrings.Add(newChoice);
	selections->Update();
}