#pragma once
#ifndef __Button_H
#define __Button_H
#include <vector>
#include <wx/wx.h>
#include <wx/panel.h>
#include "FunctionHelper.h"
#include "DialogHelper.h"
#include <iostream>
#include <fstream>
#include <string>

class ButtonPanel : public wxPanel
{
private:
	ButtonPanel* prev;
public:
	ButtonPanel(wxWindow* parent,wxString name,std::string layoutName,int ind);
	~ButtonPanel() {

		for (size_t i = 0; i < QlinkList->size(); i++) {
			ButtonSizer->Detach(QlinkList->at(i));
			QlinkList->at(i)->~wxButton();
		}
	}
	
	std::vector<wxButton*> QlinkList[51];
	ButtonPanel* SubPanelList[5];
	int QLinkIndex;
	int SubPanelIndex;
	int thisPanelIndex;
	wxFlexGridSizer* ButtonSizer;
	std::string layoutName;
	std::ifstream ButtonLayoutin;
	std::ofstream ButtonLayoutout;
	FuncHelper* functionHelper;
	DialogHelper* popUpHandeler;

	void AddButton(wxString ButtonName, wxString ButtonText);

	void LoadButtons();
	void saveButtons();
	ButtonPanel* getPrev();
	void setPrev(ButtonPanel* panel);
	//int findIndex();
	std::string getLayoutName() {
		return layoutName;
	}
	int getButtonIndex() {
		return QLinkIndex;
	}
	void setButtonIndex(int newIND) {
		QLinkIndex = newIND;
	}
};
#endif