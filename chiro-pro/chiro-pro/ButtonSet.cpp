#include  "ButtonSet.h"

void ButtonSet::SaveButtons() {
    for (size_t i = 0; i < panelList.size(); i++) {
        panelList[i]->saveButtons();
    }
}
void ButtonSet::LoadButtons() {
    
    for (int i = 0; i < panelCount; i++) {
        panelList.at(i)->LoadButtons();
    }
}
void ButtonSet::SaveAllPanels(wxString set) {
    //std::string user = set.ToStdString();
    std::string PanelLayout = "panelLayout/panelLayout" + setName.ToStdString() + ".txt";
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

        }
    }
    //clsoe file
    this->PanelLayoutout.close();
}
void ButtonSet::loadAllPanels(wxString set) {
    setName;
    std::ifstream PanelLayoutin;
    std::string setname = set.ToStdString();
    std::string fileName = path + ".txt";
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
            //std::string index = std::to_string(panelCount + 1);
            //LayoutName += ".txt";
            panelList.push_back(new ButtonPanel(parent, name, LayoutName, panelCount));
            panelList.back()->Hide();//at(panelCount)->Hide();
            panelCount++;
        }
       
        currentPanel = panelList.at(0);
        currentPanel->Show();
        currentPanel->Layout();
        //layout();
        PanelLayoutin.close();
    }
}
void ButtonSet::loadPanelsAndButtons(wxString set) {
    loadAllPanels(set);
    LoadButtons();

}
void ButtonSet::SavePanelsAndButtons(wxCommandEvent& event) {
    SaveAllPanels(setName);
    SaveButtons();

}
void ButtonSet::SavePanelsAndButtonsNP() {
    SaveAllPanels(setName);
    SaveButtons();

}
void ButtonSet::addNewPanel(wxFrame* parent, wxString name) {

    std::string filename = path + ".txt";

    ButtonPanel* newPanel = new ButtonPanel(parent,name, filename, panelCount);
    this->addToPanelList(newPanel);
    panelList.at(panelCount)->Hide();
    panelCount++;
}
