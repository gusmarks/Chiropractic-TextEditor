#pragma once
#include <wx/wx.h>
#include "LinkPanel.h"
class controlPanel : public wxPanel {
private:
	wxBoxSizer* controlSizer;

public:
	controlPanel() {}
	controlPanel(wxWindow* parent, wxWindowID id, wxString name) :
		wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxBORDER, name) {
		controlSizer = new wxBoxSizer(wxHORIZONTAL);
		this->SetSizer(controlSizer);
	}
	wxBoxSizer* getControlSizer() {
		return controlSizer;
	}
	void addToControlSizer(wxButton* button) {
		controlSizer->Add(button, wxALIGN_CENTER);
		controlSizer->Layout();
	}
	void addToControlSizer(linkPanel* pan,wxAlignment align) {
		controlSizer->Add(pan, align);
		controlSizer->Layout();
	}

	void clearControlSizer() {
		controlSizer->Clear();
		controlSizer->Layout();
	}

	void controlLayout() {
		controlSizer->Layout();
	}
	void controlAddSpace(int x, int y) {
		controlSizer->Add(x, y);
		controlSizer->Layout();
	}

};
