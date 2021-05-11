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
	int level;
public:
	ButtonPanel(wxWindow* parent,wxString name,std::string layoutName,int ind);
	~ButtonPanel() {

		for (size_t i = 0; i < QlinkList->size(); i++) {
			ButtonSizer->Detach(QlinkList->at(i));
			QlinkList->at(i)->~wxButton();
		}
	}
	
	std::vector<wxButton*> QlinkList[51];
	ButtonPanel* SubPanelList[7];
	int QLinkCount;
	int subPanelCount =0;
	
	wxFlexGridSizer* ButtonSizer;
	std::string layoutName;
	std::string pathName;
	std::ifstream ButtonLayoutin;
	std::ofstream ButtonLayoutout;
	FuncHelper* functionHelper;
	DialogHelper* popUpHandeler;

	bool hasPrev() {
		if (this->prev != NULL) {
			return true;
		}
		return false;
	}
	ButtonPanel* getPrev() {
		return this->prev;
	}
	void setPrev(ButtonPanel* panel) {
		this->prev = panel;

	}

	std::string getLayoutName() {
		return layoutName;
	}

	int getPanelCount() {
		return subPanelCount;
	}

	int getButtonIndex() {
		return QLinkCount;
	}
	void setButtonIndex(int newIND) {
		QLinkCount = newIND;
	}

	int getLevel() {
		return level;
	}
	void setLevel(int lvl) {
		level = lvl;
	}

	ButtonPanel* getPanelAtIndex(int ind);

	void setSubPanel(int index, ButtonPanel* panel) {
		SubPanelList[index] = panel;
	}

	void makeLevel() {
		if (!this->hasPrev()) {
			level = 0;
		}
		if (this->hasPrev()) {
			level = 1;
			if (this->getPrev()->hasPrev()) {
				level = 2;
			}
		}

	}
	//void clearQlinkList() {}

	void AddButton(wxString ButtonName, wxString ButtonText);
	void addNewPanel(wxFrame* parent, wxString name, wxString filePath);

	void LoadButtons();
	void saveButtons();

	void saveSubPanels();
};
#endif