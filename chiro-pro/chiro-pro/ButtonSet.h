#pragma once
#include <vector>
#include "ButtonPanel.h"
//#include "DialogHelper.h"

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
		popUpHandeler = new DialogHelper(parent);
		this->parent = parent;
		path = Directory;
		setName = name;
		panelCount = panelNo;
	}
	~ButtonSet() {
		delete popUpHandeler;
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
	std::vector<ButtonPanel*> getPanelList() {
		return panelList;
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
	ButtonPanel* getPanelAt(int i) {
		return panelList[i];
	}

// a set of panel manipulating functions defined in ButtonSet.cpp
	void saveAllPanels(wxString set) {
		try {
			std::string PanelLayout = this->path + ".txt";
			if (!this->PanelLayoutout.is_open()) {
				PanelLayoutout.open(PanelLayout, std::fstream::out);//modify to swap users later
			}
			int size = panelList.size();
			for (int i = 0; i < size; i++) {
				if (this->panelList[i] != nullptr) {

					this->PanelLayoutout << this->panelList[i]->GetName();
					this->PanelLayoutout << "\n";
					this->PanelLayoutout << this->panelList[i]->getLayoutName();
					this->PanelLayoutout << "\n";
					//this->PanelLayoutout << this->panelList[i]->getPrev()->GetName();

				}
			}

			//clsoe file
			this->PanelLayoutout.close();
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void loadBasePanels(wxString set) {
		
		setName;
		std::ifstream PanelLayoutin;
		std::string setname = set.ToStdString();
		std::string fileName = path + ".txt";
		try {
			//open the button layout file assuming it is closed already
			//if (!PanelLayoutin) {

			PanelLayoutin.open(fileName, std::fstream::in);
			//}
			if (PanelLayoutin.fail()) {
				popUpHandeler->errorMessage("fail to open file");
			}
			else {
				//two strings name and lable holding the lable and text
				std::string name;
				std::string LayoutName;
				this->setPanelCount(0);

				while (PanelLayoutin.peek() != EOF) {
					//get input
					getline(PanelLayoutin, name);
					getline(PanelLayoutin, LayoutName);

					panelList.push_back(new ButtonPanel(parent, name, LayoutName, panelCount));
					panelList.back()->Hide();
					panelList.back()->makeLevel();
					panelCount++;
				}

				currentPanel = panelList.at(0);
				currentPanel->Show();

				PanelLayoutin.close();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void saveButtons() {
		try {
			for (size_t i = 0; i < panelList.size(); i++) {
				panelList[i]->saveQLinks();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void loadButtons() {
		try {
			for (int i = 0; i < panelCount; i++) {
				panelList.at(i)->loadQLinks();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void loadPanelsAndButtons(wxString set) {
		loadBasePanels(set);
		loadButtons();
	}
	//saves the layout for the user, and will be loaded upon the next use of the program
	void savePanelsAndButtons(wxCommandEvent& event) {
		saveAllPanels(setName);
		saveButtons();
	}//same as last but with no event nessisary
	void savePanelsAndButtonsNP() {
		saveAllPanels(setName);
		saveButtons();
	}
	//removes all panels from the set*
	void clearPanelList() {
		panelList.clear();
	}
};