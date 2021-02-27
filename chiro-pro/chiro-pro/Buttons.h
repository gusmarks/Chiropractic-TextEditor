#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include "Frame.h"
#include <iostream>
#include <fstream>
#include <string>

class ButtonPanel : public wxPanel
{
public:
	ButtonPanel(wxWindow* parent,wxString name,std::string layoutName);
	wxButton* QlinkList[30];
	int QLinkIndex;
	wxFlexGridSizer* ButtonSizer;
	std::string layoutName;
	std::ifstream ButtonLayoutin;
	std::ofstream ButtonLayoutout;

	
	void LoadButtons();
	void saveButtons();
};