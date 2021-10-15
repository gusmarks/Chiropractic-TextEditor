#pragma once
#ifndef __Button_H
#define __Button_H
//includes to use the gui elements,edit files, and various dialogs, and utility functions
#include <vector>
#include <wx/wx.h>
#include <wx/panel.h>
#include "FunctionHelper.h"
#include "DialogHelper.h"
#include <iostream>
#include <fstream>
#include <string>
//button panel class used to store and access buttons on the main page, they are swaped out to access diferant buttons
class ButtonPanel : public wxPanel {
private:
	//previous panel used to swap back to the previous one, this is useful for navigating the seperate panels
	ButtonPanel* prev;
	//the level shows how many panels in a line there are. the maximum amount is 3 deep.
	//and each panel with level 1 or 2 can hold max 7 sub panels. this leads to a total maximum of 200 panels
	int level;
	int maxSubPanels = 7;
	//parent is to make sure the panels are displayed on the main window
	wxWindow* parent;
	//;ayout name tells what filename holds the contents of the buttons
	std::string layoutName;
	//qLinks are the buttons on the panel, named to be "quick links" or qLinks
	std::vector<wxButton*> qLinkList;
	//qLinkCount and subPanelCount keeptrack of how many buttons are there and how many panels are. 
	//subPanelList holds all of the subPanels.
	int qLinkCount = 0;
	int subPanelCount = 0;
	std::vector<ButtonPanel*> subPanelList;
	//buttonSizer stores are organizes the buttons on the panel, id dose so in 3 rows and 15 colums, offering a total of 
	//45 Qlinks in any one panel. 
	wxFlexGridSizer* buttonSizer;
	//buttonlayout in and out are to load and save button and panel information stored in files.
	std::ifstream buttonLayoutin;
	std::ofstream buttonLayoutout;
	//function helper lets us use utility functions that dont really relate the the class
	//dialog helper lets us use defined dialogs, primarily for pop ups.
	FuncHelper* functionHelper;
	DialogHelper* popUpHandeler;
public:
	//constructor & destructor definitions
	ButtonPanel(wxWindow* parent,wxString name,std::string layoutName,int ind):wxPanel(parent,wxID_ANY,wxDefaultPosition,
		wxSize(600,69),wxTAB_TRAVERSAL,name) {
		try {
			//keep track of the parent
			this->parent = parent;
			//initulize the button sizer, then set the sizer to the panel, this applies the sizer the panel to actualy change the layout
			buttonSizer = new wxFlexGridSizer(3, 9, 0, 0);
			this->SetSizer(buttonSizer);
			//layout name is the filename of the button information
			this->layoutName = layoutName;
			functionHelper = new FuncHelper();
			popUpHandeler = new DialogHelper(parent);
			//make level deturmins the level of the panel when it is created, this is based on if there is a previous element.
			this->makeLevel();
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}

	}
	~ButtonPanel() {
		try {
			//this function deletes the panel, by destroying all the buttons.
			for (size_t i = qLinkList.size(); i > 0; i--) {
				buttonSizer->Detach(qLinkList.at(i - 1));
				qLinkList.at(i - 1)->~wxButton();
			}
			delete functionHelper;
			delete popUpHandeler;
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	//getlayoutname returns the layoutname variable
	std::string getLayoutName() {
		return layoutName;
	}
	//checks to see if there is a previous panel
	bool hasPrev() {
		if (this->prev != NULL) {
			return true;
		}
		return false;
	}
	//returns the previous panel, assuming there is one
	ButtonPanel* getPrev() {
		if (this->hasPrev()) {
			return this->prev;
		}
		return NULL;
	}
	//sets the previous panel this sould only be called once per panel
	void setPrev(ButtonPanel* panel) {
		this->prev = panel;

	}
	//getPanelCount returns the number of subpanels in the given panel
	//getQLinkCount returns the number of buttons the the given panel
	//setQLinkCOunt changes the count is primarily used to incriment or decrement the count
	int getPanelCount() {
		return subPanelCount;
	}
	int getQLinkCount() {
		return qLinkCount;
	}
	void setQLinkCount(int newIND) {
		qLinkCount = newIND;
	}
	//getLevel returns the level of the panel
	//setLevel sets the level of the panel
	//getParent return the parent of the panel
	//getQLinkList returns the list of buttons
	int getLevel() {
		return level;
	}
	void setLevel(int lvl) {
		level = lvl;
	}
	wxWindow* getParent() {
		return parent;
	}
	std::vector<wxButton*> getQLinkList() {
		return qLinkList;
	}
	std::vector<ButtonPanel*> getSubPanelList() {
		return subPanelList;
	}
	//getPanelAtIndex returns the subpanel at a given index
	ButtonPanel* getPanelAtIndex(int ind) {
		if (ind < 8) {
			if (subPanelList.at(ind) != NULL) {
				return subPanelList.at(ind);
			}
			return NULL;
		}
		return NULL;
	}
	//getGridSizer returns the flexgridsizer
	wxFlexGridSizer* getGridSizer() {
		return buttonSizer;
	}
	//setSubPanel, places a Panel into the subpanellist of the panel provided there is room
	void setSubPanel(int index, ButtonPanel* panel) {
		if (index < maxSubPanels)
			subPanelList.push_back(panel);
	}
	//makeLevel sets the level initualy based on the previous elements
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
	//addQLink adds a button to the panel
	//addNewPanel adds a new subpanel to the panel
	//addLoadedPanel helps load in panels from file
	void addQLink(wxString ButtonName, wxString ButtonText)
	{
		try {
			if (!functionHelper->isNumber(ButtonText.ToStdString())) {
				std::string text = ButtonText.ToStdString();
				std::string check = "Dialog-ID";
				if (text.find(check) != std::string::npos) {
					wxButton* newButton = new wxButton(this
						, wxID_ANY, ButtonName, wxDefaultPosition, wxSize(150, 23), wxBG_STYLE_COLOUR, wxDefaultValidator, ButtonText);

					newButton->SetForegroundColour(*wxBLUE);
					qLinkList.push_back(newButton);
					//add the button to the button sizer
					buttonSizer->Add(qLinkList.back(), wxLeft);
					//update layout of page and button index
					buttonSizer->Layout();
				}
				else {
					qLinkList.push_back(new wxButton(this
						, wxID_ANY, ButtonName, wxDefaultPosition, wxSize(150, 23), 0, wxDefaultValidator, ButtonText));
					//add the button to the button sizer
					buttonSizer->Add(qLinkList.back(), wxLeft);
					//update layout of page and button index
					buttonSizer->Layout();
				}


			}
			else {
				wxButton* newButton = new wxButton(this,
					wxID_ANY, ButtonName, wxDefaultPosition, wxSize(150, 23), 0, wxDefaultValidator, ButtonText);
				newButton->SetForegroundColour(wxColor(255, 0, 0));
				newButton->Refresh();
				qLinkList.push_back(newButton);
				//newButton->Bind()
				buttonSizer->Add(qLinkList.back(), wxLeft);
				//update layout of page and button index
				buttonSizer->Layout();
				//Layout();

			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void addNewPanel(wxWindow* parent, wxString name, wxString filePath) {
		std::string fileName;
		try {
			if (this->level == 0) {

				fileName = this->getLayoutName();
				fileName = fileName.substr(0, fileName.size() - 4);
				fileName.append("-panels/LayoutLevel2-");
				fileName += std::to_string(subPanelCount + 1);
				fileName.append(".txt");
			}
			if (this->level == 1) {

				fileName = this->getLayoutName();
				fileName = fileName.substr(0, fileName.size() - 4);
				fileName.append("-panels/LayoutLevel3-");
				fileName += std::to_string(subPanelCount + 1);
				fileName.append(".txt");
			}

			if (subPanelCount <= 7) {
				ButtonPanel* newPanel = new ButtonPanel(parent, name, fileName, subPanelCount);
				newPanel->setPrev(this);
				this->setSubPanel(subPanelCount, newPanel);
				newPanel->makeLevel();
				newPanel->Hide();
				subPanelCount++;

			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void addLoadedPanel(wxWindow* parent, wxString name, wxString filePath) {
		try {
			if (subPanelCount <= 7) {
				ButtonPanel* newPanel = new ButtonPanel(parent, name, filePath.ToStdString(), subPanelCount);
				newPanel->setPrev(this);
				this->setSubPanel(subPanelCount, newPanel);
				newPanel->makeLevel();
				newPanel->Hide();
				subPanelCount++;
				newPanel->loadQLinks();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	// loadQlinks loads the buttons into the panel from file
	//saveQLinks saves the button information to the file
	//save subpanels saves the subpanel information to file
	void loadQLinks()
	{
		try {
			//open the button layout file assuming it is closed already
			if (!this->buttonLayoutin.is_open()) {
				this->buttonLayoutin.open(layoutName, std::fstream::in);
				if (!this->buttonLayoutin.is_open()) {
					buttonLayoutin.close();
					return;
				}
			}
			//two strings name and lable holding the lable and text
			std::string name;
			std::string maintext;

			qLinkCount = 0;//give value to index
			//while there is still text in the layout file keep taking in two lines into out string variables
			// creating buttons from those strings and adding them to the array. 
			//lastly updating th layout of the page and closing the layout file
			if (buttonLayoutin.is_open()) {
				while (this->buttonLayoutin.peek() != EOF) {
					//get input
					getline(this->buttonLayoutin, name);
					getline(this->buttonLayoutin, maintext);

					//make button
					if (functionHelper->isNumber(maintext)) {
						this->qLinkList.push_back(new wxButton(this, wxID_ANY, name, wxDefaultPosition, wxSize(150, 23), wxBG_STYLE_COLOUR, wxDefaultValidator, maintext));
						this->qLinkList.back()->SetForegroundColour(*wxRED);
						std::string pathName;
						getline(this->buttonLayoutin, pathName);
						this->addLoadedPanel(parent, name, pathName);

					}
					else if (functionHelper->isPopup(maintext)) {
						this->qLinkList.push_back(new wxButton(this, wxID_ANY, name, wxDefaultPosition, wxSize(150, 23), wxBG_STYLE_COLOUR, wxDefaultValidator, maintext));
						this->qLinkList.back()->SetForegroundColour(*wxBLUE);
					}
					else if (functionHelper->isText(maintext)) {
						this->qLinkList.push_back(new wxButton(this, wxID_ANY, name, wxDefaultPosition, wxSize(150, 23), 0, wxDefaultValidator, maintext));
					}
					//add button to array
					this->buttonSizer->Add(qLinkList.back(), wxLeft);
					//update layout
					buttonSizer->Layout();
					//update index
					qLinkCount++;
				}
				//close file
				this->buttonLayoutin.close();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
			this->buttonLayoutin.close();
		}
	}
	void saveQLinks() {
		//if the layout file is not open, open it
		try {
			if (!this->buttonLayoutout.is_open()) {
				buttonLayoutout.open(this->layoutName, std::fstream::out);
			}
			if (buttonLayoutout.is_open()) {
				int size = qLinkList.size();
				for (int i = 0; i < size; i++) {
					if (this->qLinkList.at(i) != nullptr) {

						this->buttonLayoutout << this->qLinkList.at(i)->GetLabel();
						this->buttonLayoutout << "\n";
						this->buttonLayoutout << this->qLinkList.at(i)->GetName();
						this->buttonLayoutout << "\n";
						if (this->qLinkList.at(i)->GetName().IsNumber()) {
							std::string layout = this->subPanelList.front()->getLayoutName();
							this->buttonLayoutout << layout;
							this->buttonLayoutout << "\n";
						}
					}
				}
			}
			//clsoe file
			this->buttonLayoutout.close();
			if (this->level < 3 && this->subPanelCount>0) {
				this->saveSubPanels();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
	void saveSubPanels() {
		try {
			for (size_t i = 0; i < subPanelList.size(); i++) {
				subPanelList.at(i)->saveQLinks();
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}

	void removeButtonWithTextOf(wxString text) {
		try {
			size_t size = qLinkList.size();
			for (size_t i = 0; i < size; i++) {
				if (qLinkList.at(i)->GetName() == text) {
					qLinkList.erase(qLinkList.begin() + i);
					size--;
				}
			}
		}
		catch (...) {
			popUpHandeler->errorMessage("an error occured in buttonpanel");
		}
	}
};
#endif