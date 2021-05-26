#pragma once
#include <wx/wx.h>
#include <wx/calctrl.h>

class CalenderDialog :public wxDialog {
public:
	CalenderDialog(wxWindow* parent, wxString title);
	wxCalendarCtrl* cal;
	wxBoxSizer* vert;
	wxBoxSizer* horz;

	wxString getVal() {
		return cal->GetDate().FormatDate();
	}
private:
};