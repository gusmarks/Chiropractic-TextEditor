#pragma once
#ifndef __Button_H
#define __Button_H
#include <wx/wx.h>
#include <wx/panel.h>
//#include "Frame.h"
#include <iostream>
#include <fstream>
#include <string>

class ButtonPanel : public wxPanel
{
private:
	ButtonPanel* prev;
public:
	ButtonPanel(wxWindow* parent,wxString name,std::string layoutName,int ind);
	wxButton* QlinkList[51];
	ButtonPanel* SubPanelList[5];
	int QLinkIndex;
	int SubPanelIndex;
	int index;
	wxFlexGridSizer* ButtonSizer;
	std::string layoutName;
	std::ifstream ButtonLayoutin;
	std::ofstream ButtonLayoutout;

	void LoadButtons();
	void saveButtons();
	ButtonPanel* getPrev();
	void setPrev(ButtonPanel* panel);
	int findIndex();
	
};
#endif