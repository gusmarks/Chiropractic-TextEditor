#pragma once
#include <vector>
#include "ButtonPanel.h"
#include "DialogHelper.h"

class ButtonSet {

private:
	wxString setName;
	int panelIndex = 0;
	std::vector<ButtonPanel*> panelList;
	ButtonPanel* currentPanel;
	std::string dir;
	DialogHelper* popUpHandeler;
	wxFrame* parent;

public:
	std::ofstream PanelLayoutout;
	

	ButtonSet(wxFrame* parent, wxString name,int setNo,std::string Directory) {
		popUpHandeler = new DialogHelper();
		this->parent = parent;
	}
	int getPanelIndex() {
		return panelIndex;
	}
	void setPanelIndex(int ind) {
		panelIndex = ind;
	}
	void addToPanelList(ButtonPanel* newPanel) {
		panelList.push_back(newPanel);
	}
	ButtonPanel* getPanelAtIndex(int Ind) {
		return panelList.at(Ind);
	}
	int getPanelListSize() {
		return (int)panelList.size();
	}
	ButtonPanel* getCurrentPanel() {
		return currentPanel;
	}
	void setCurrentPanel(ButtonPanel* newPanel) {
		currentPanel = newPanel;
	}
	


	void addNewPanel(wxFrame* parent,wxString name);
	void SaveAllPanels(wxString set);
	void loadAllPanels(wxString set);
	void SaveButtons();
	void LoadButtons();
	void loadPanelsAndButtons(wxString set);
	void SavePanelsAndButtons(wxCommandEvent& event);
	void SavePanelsAndButtonsNP();
	void destroyPanels(wxString set);

};