
#include <wx/wx.h>
#include <wx/panel.h>
#include "ButtonPanel.h"
#include "Frame.h"
#include <string>

ButtonPanel::ButtonPanel(wxWindow* parent, wxString name, std::string pathName, int ind) : wxPanel(parent, wxID_ANY, wxDefaultPosition,
	wxSize(100,70), wxTAB_TRAVERSAL, name)
{
    QLinkCount = 0;
	ButtonSizer = new wxFlexGridSizer(3, 15, 0, 0);
	this->SetSizer(ButtonSizer);
    this->pathName = pathName;
    this->layoutName = this->pathName + ".txt";
    functionHelper = new FuncHelper();
    popUpHandeler = new DialogHelper(parent);
    this->makeLevel();
    
}

void ButtonPanel::LoadButtons()
{
    //open the button layout file assuming it is closed already
    if (!this->ButtonLayoutin.is_open()) {
        this->ButtonLayoutin.open(layoutName, std::fstream::in);
    }
    //two strings name and lable holding the lable and text
    std::string name;
    std::string maintext;
  
    QLinkCount = 0;//give value to index
    //while there is still text in the layout file keep taking in two lines into out string variables
    // creating buttons from those strings and adding them to the array. 
    //lastly updating th layout of the page and closing the layout file

    while (this->ButtonLayoutin.peek() !=EOF) {
        //get input
        getline(this->ButtonLayoutin, name);
        getline(this->ButtonLayoutin, maintext);
       
        //make button
        if (functionHelper->isNumber(maintext)) {
            this->QlinkList->push_back( new wxButton(this, BUTTON_Panel, name, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, maintext));
            this->QlinkList->back()->SetBackgroundColour(*wxCYAN);
        }
        else {
            this->QlinkList->push_back(new wxButton(this, BUTTON_Write, name, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, maintext));
        }
        //add button to array
        this-> ButtonSizer->Add(QlinkList->back(), wxLeft);
        //update layout
        ButtonSizer->Layout();
        //update index
        QLinkCount++;
    }
    //close file
    this->ButtonLayoutin.close();
}
void ButtonPanel::saveButtons() {
    //if the layout file is not open, open it
    if (!this->ButtonLayoutout.is_open()) {
        ButtonLayoutout.open(this->layoutName, std::fstream::out);
    }
    int size = QlinkList->size();
    for (int i = 0; i < size; i++) {
        if (this->QlinkList->at(i) != nullptr) {

            this->ButtonLayoutout << this->QlinkList->at(i)->GetLabel();
            this->ButtonLayoutout << "\n";
            this->ButtonLayoutout << this->QlinkList->at(i)->GetName();
            this->ButtonLayoutout << "\n";
            
        }
    }
    //clsoe file
    this->ButtonLayoutout.close();

}

void ButtonPanel::AddButton(wxString ButtonName,wxString ButtonText)
{      
    if (!functionHelper->isNumber(ButtonText.ToStdString())) {
        std::string text = ButtonText.ToStdString();
        std::string check = "Dialog-ID";
        if (text.find(check)!=std::string::npos) {
            QlinkList->push_back(new wxButton(this
                , BUTTON_Dialog, ButtonName, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, ButtonText));
            QlinkList->back()->SetForegroundColour(wxColour(28,86,163));
            //add the button to the button sizer
            ButtonSizer->Add(QlinkList->back(), wxLeft);
            //update layout of page and button index
            ButtonSizer->Layout();
        }
        else {
            QlinkList->push_back(new wxButton(this
                , BUTTON_Write, ButtonName, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, ButtonText));
            //add the button to the button sizer
            ButtonSizer->Add(QlinkList->back(), wxLeft);
            //update layout of page and button index
            ButtonSizer->Layout();
        }

        
    }
    else {
        wxButton* newButton = new wxButton(this,
            wxID_ANY, ButtonName, wxDefaultPosition, wxSize(80.5, 23), wxBG_STYLE_COLOUR, wxDefaultValidator, ButtonText);
        newButton->SetForegroundColour(*wxRED);
        QlinkList->push_back(newButton);
        //newButton->Bind()
        ButtonSizer->Add(QlinkList->back(), wxLeft);
        //update layout of page and button index
        ButtonSizer->Layout();
        Layout();
        
    }
}
void ButtonPanel::addNewPanel(wxFrame* parent, wxString name,wxString filePath) {

    std::string filename = filePath.ToStdString()+"layout" + std::to_string(subPanelCount) + ".txt";
    

    ButtonPanel* newPanel = new ButtonPanel(parent, name, filename, subPanelCount);
    newPanel->setPrev(this);
    this->setSubPanel(subPanelCount, newPanel);
    newPanel->makeLevel();
    newPanel->Hide();
    subPanelCount++;
}

void ButtonPanel::saveSubPanels() {

}

ButtonPanel* ButtonPanel::getPanelAtIndex(int ind) {
    if (ind < 8) {
        if (SubPanelList[ind] != NULL) {
            return SubPanelList[ind];
        }
    }
}
