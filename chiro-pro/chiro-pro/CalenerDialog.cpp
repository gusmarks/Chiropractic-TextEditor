#include <wx/wx.h>
#include <wx/calctrl.h>
#include "CalenderDialog.h"


CalenderDialog::CalenderDialog(wxWindow* parent, wxString title) : 
	wxDialog(parent, wxID_ANY, title,wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE) {

	cal = new wxCalendarCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize,
		wxCAL_MONDAY_FIRST, "");
	horz = new wxBoxSizer(wxHORIZONTAL);
	vert = new wxBoxSizer(wxVERTICAL);

	wxButton* ok = new wxButton(this, wxID_OK, "Ok");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Cancel");
	horz->Add(ok);
	horz->Add(cancel);

	vert->Add(cal,0,wxALL);
	vert->Add(horz, 2, wxALL);
	ok->Show();
	cancel->Show();
	SetSizer(vert);
	vert->Fit(this);
	SetAutoLayout(true);
	Layout();
}