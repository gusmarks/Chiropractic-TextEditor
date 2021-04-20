
#include "NavLink.h"


NavLink::NavLink(wxWindow* parent, wxWindowID ID, wxString name,ButtonPanel* panel) : wxHyperlinkCtrl(parent,ID,name,"",wxDefaultPosition,wxDefaultSize,
	wxHL_DEFAULT_STYLE,"") {
	setPanel(panel);

}


