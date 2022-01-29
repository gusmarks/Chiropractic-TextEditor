#pragma once
#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "ButtonPanel.h"
class NavLink : public wxHyperlinkCtrl {
private:
	ButtonPanel* Panel;
public:
	/// <summary>
	/// constructor for a hyperlink used to navigate panels, each one stores a referance to a panel and switched to it when clicked
	/// </summary>
	/// <param name="parent">wxwindow to be placed on</param>
	/// <param name="ID">id is the uniqe id given to the object</param>
	/// <param name="name">name is the displayed label on the link</param>
	/// <param name="panel">panel is stored in the link to know where to go to when clicked</param>
	NavLink(wxWindow* parent, wxWindowID ID, wxString name, ButtonPanel* panel) : wxHyperlinkCtrl(parent, ID, name, "", wxDefaultPosition, wxDefaultSize,
		wxHL_DEFAULT_STYLE, "") {setPanel(panel);}
	/// <summary>
	/// returns the panel of the link
	/// </summary>
	/// <returns></returns>
	ButtonPanel* getPanel() {return Panel;}
	/// <summary>
	/// sets the oanel of the link
	/// </summary>
	/// <param name="newPanel"></param>
	void setPanel(ButtonPanel* newPanel) {Panel = newPanel;}
	/// <summary>
	/// sets the name of the link that will be displayed
	/// </summary>
	/// <param name="newName"></param>
	void setName(wxString newName) {SetLabel(newName);}
	/// <summary>
	/// returns the name of the lonk
	/// </summary>
	/// <returns></returns>
	wxString getName() {return this->GetLabel();}
};
