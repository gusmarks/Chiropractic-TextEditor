#pragma once
#include <wx/wx.h>
#include <wx/calctrl.h>
class CalenderDialog :public wxDialog {
public:
	/// <summary>
	/// constructor for the calender dialog, when invoked opening a calender for the user to select the date.
	/// </summary>
	/// <param name="parent">wxwindow* to be placed on</param>
	/// <param name="title">title is the name of the dialog</param>
	CalenderDialog(wxWindow* parent, wxString title) :
		wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {
		cal = new wxCalendarCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize,
			wxCAL_MONDAY_FIRST, "");
		horz = new wxBoxSizer(wxHORIZONTAL);
		vert = new wxBoxSizer(wxVERTICAL);
		wxButton* ok = new wxButton(this, wxID_OK, "Ok");
		wxButton* cancel = new wxButton(this, wxID_CANCEL, "Cancel");
		horz->Add(ok);
		horz->Add(cancel);
		vert->Add(cal, 0, wxALL);
		vert->Add(horz, 2, wxALL);
		ok->Show();
		cancel->Show();
		SetSizer(vert);
		vert->Fit(this);
		SetAutoLayout(true);
		Centre(wxBOTH);
		Layout();
	}
	/// <summary>
	/// returns the selected date
	/// </summary>
	wxString getVal() {return cal->GetDate().FormatDate();}
private:
	//cal is a wxcalenderctrl, imbeded into the dialog, vert is a vertical boxsizer used to arange the components
	//horz is a horizontal boxsizer used to arange the components 
	wxCalendarCtrl* cal;
	wxBoxSizer* vert;
	wxBoxSizer* horz;
};