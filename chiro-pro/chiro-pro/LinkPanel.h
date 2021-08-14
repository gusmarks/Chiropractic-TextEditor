#pragma once
// include wx for ui componants, and add nav link for the hyperlink navigation
#include <wx/wx.h>
#include "NavLink.h"

class linkPanel : public wxPanel {
private:
	// a boxsizer to order the componants
	wxBoxSizer* linkSizer;
public:
	//constructors
	linkPanel() {}
	linkPanel(wxWindow* parent, wxWindowID id, wxString name) :
		wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, name) {
		//the boxsizer is set to horizontal
		linkSizer = new wxBoxSizer(wxHORIZONTAL);
		this->SetSizer(linkSizer);
	}
	// adds a navlink to the sizer and calls layout
	void addToLinkSizer(NavLink* nav) {
		linkSizer->Add(nav);
		linkSizer->Layout();
	}
	//empties the sizer and calls layout to reorder the panel, rarly used
	void clearLinkSizer() {
		linkSizer->Clear();
		linkSizer->Layout();
	}//calls layout on the linkpanel
	void linkLayout() {
		linkSizer->Layout();
	}
	//adds a specified amount of space to the sizer
	void linklAddSpace(int x, int y) {
		linkSizer->Add(x, y);
		linkSizer->Layout();
	}

};