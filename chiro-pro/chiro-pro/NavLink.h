#pragma once

#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "ButtonPanel.h" 

class NavLink : public wxHyperlinkCtrl {
private:
	ButtonPanel* Panel;
public:
	NavLink(){}
	NavLink(wxWindow* parent, wxWindowID ID, wxString name,ButtonPanel* panel);
	ButtonPanel* getPanel() {
		return Panel;
	}
	void setPanel(ButtonPanel* newPanel) {
		Panel = newPanel;
	}
	void setName(wxString newName) {
		SetLabel(newName);
	}
	wxString getName() {
		return this->GetLabel();
	}


};
