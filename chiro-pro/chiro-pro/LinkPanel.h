#pragma once
#include <wx/wx.h>
#include "NavLink.h"

class linkPanel : public wxPanel {
private:
	wxBoxSizer* linkSizer;
public:
	linkPanel() {}
	linkPanel(wxWindow* parent, wxWindowID id, wxString name) :
		wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, name) {
		linkSizer = new wxBoxSizer(wxHORIZONTAL);
		this->SetSizer(linkSizer);
	}
	void addToLinkSizer(NavLink* nav) {
		linkSizer->Add(nav);
		linkSizer->Layout();
	}
	void clearLinkSizer() {
		linkSizer->Clear();
		linkSizer->Layout();
	}
	void linkLayout() {
		linkSizer->Layout();
	}
	void linklAddSpace(int x, int y) {
		linkSizer->Add(x, y);
		linkSizer->Layout();
	}

};