#pragma once
//include wx for the ui componants, and LinkPanel ro next that panel within the control panel
#include <wx/wx.h>
#include "LinkPanel.h"
class controlPanel : public wxPanel {
private:
	//a box sizer to hold all elements
	wxBoxSizer* controlSizer;

public:
	//constructors
	controlPanel() {}
	controlPanel(wxWindow* parent, wxWindowID id, wxString name) :
		wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxBORDER, name) {
		//set the box sizer to horizontal
		controlSizer = new wxBoxSizer(wxHORIZONTAL);
		this->SetSizer(controlSizer);
	}
	//returns a referance to the boxsizer
	wxBoxSizer* getControlSizer() {
		return controlSizer;
	}
	//adds elements to the boxsizer, and calls the layout function to re order the panel
	void addToControlSizer(wxButton* button) {
		controlSizer->Add(button, wxALIGN_CENTER);
		controlSizer->Layout();
	}
	//a second adding function, this one adds the link panel, with a specified alignment
	void addToControlSizer(linkPanel* pan,wxAlignment align) {
		controlSizer->Add(pan, align);
		controlSizer->Layout();
	}
	//fully empties the control sizer, rarly used
	void clearControlSizer() {
		controlSizer->Clear();
		controlSizer->Layout();
	}
	//calls layout on the sizer
	void controlLayout() {
		controlSizer->Layout();
	}
	//adds a specified amount of blank space in the sizer
	void controlAddSpace(int x, int y) {
		controlSizer->Add(x, y);
		controlSizer->Layout();
	}
};
