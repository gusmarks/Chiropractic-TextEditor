#pragma once
#include <vector>
#include "ButtonPanel.h"
#include "DialogHelper.h"

class ButtonSet {

private://private variables, the setName, panelCount,panelList, the currentPanel pointer
	//the save/loading path, a dialog helper for error messages, and a referance to the parent object.
	wxString setName;
	int panelCount = 0;
	std::vector<ButtonPanel*> panelList;
	ButtonPanel* currentPanel;
	std::string path;
	DialogHelper* popUpHandeler;
	wxFrame* parent;

public://an ofstream to save configurations
	std::ofstream PanelLayoutout;
	
	//constructor sets most of the variable
	ButtonSet(wxFrame* parent, std::string name,int setNo,std::string Directory,int panelNo) {
		popUpHandeler = new DialogHelper();
		this->parent = parent;
		path = Directory;
		setName = name;
		panelCount = panelNo;
	}
	//getter function for the panel count variable
	int getPanelCount() {
		return panelCount;
	}
	//setter function for the panel count variable 
	void setPanelCount(int ind) {
		panelCount = ind;
	}
	// adds the passed panel to the the local panel list
	void addToPanelList(ButtonPanel* newPanel) {
		panelList.push_back(newPanel);
	}
	//getter for the panel list variable, gets the item at the passed index
	ButtonPanel* getPanelAtIndex(int Ind) {
		return panelList.at(Ind);
	}
	//return the size of the fector as an integer 
	int getPanelListSize() {
		return (int)panelList.size();
	}
	//returns the current panel variable
	ButtonPanel* getCurrentPanel() {
		return currentPanel;
	}
	//setter for the current panel variable
	void setCurrentPanel(ButtonPanel* newPanel) {
		currentPanel = newPanel;
	}
	wxString getSetName() {
		return setName;
	}
	std::string getPath() {
		return path;
	}
	
// a set of panel manipulating functions defined in ButtonSet.cpp
	void addNewPanel(wxFrame* parent,wxString name);
	void SaveAllPanels(wxString set);
	void loadAllPanels(wxString set);
	void SaveButtons();
	void LoadButtons();
	void loadPanelsAndButtons(wxString set);
	void SavePanelsAndButtons(wxCommandEvent& event);
	void SavePanelsAndButtonsNP();
	void destroyPanels(wxString set);
	void clearPanelList() {
		panelList.clear();
	}
	

};