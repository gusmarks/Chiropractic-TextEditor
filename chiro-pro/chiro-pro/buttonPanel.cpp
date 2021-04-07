
#include <wx/wx.h>
#include <wx/panel.h>
#include "ButtonPanel.h"
#include "Frame.h"
#include <string>

ButtonPanel::ButtonPanel(wxWindow* parent, wxString name, std::string layoutName, int ind) : wxPanel(parent, wxID_ANY, wxDefaultPosition,
	wxDefaultSize, wxTAB_TRAVERSAL, name)
{
    QLinkCount = 0;
    thisPanelIndex = ind;
	ButtonSizer = new wxFlexGridSizer(3, 17, 0, 0);
	this->SetSizer(ButtonSizer);
    this->layoutName = layoutName;
    functionHelper = new FuncHelper();
    popUpHandeler = new DialogHelper();
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
            this->QlinkList->push_back( new wxButton(this, BUTTON_Panel, name, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, maintext));
            this->QlinkList->back()->SetBackgroundColour(*wxCYAN);
        }
        else {
            this->QlinkList->push_back(new wxButton(this, BUTTON_Write, name, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, maintext));
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
ButtonPanel* ButtonPanel::getPrev() {
    return this->prev;
}
void ButtonPanel::setPrev(ButtonPanel* panel) {
    this->prev = panel;
}
void ButtonPanel::AddButton(wxString ButtonName,wxString ButtonText)
{      
    if (!functionHelper->isNumber(ButtonText.ToStdString())) {
        QlinkList->push_back(new wxButton(this
            , BUTTON_Write, ButtonName, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, ButtonText));
        //add the button to the button sizer
        ButtonSizer->Add(QlinkList->back(), wxLeft);
        //update layout of page and button index
        ButtonSizer->Layout();
        
    }
    else {
        //int QIndex = QLinkIndex;
        wxButton* newButton = new wxButton(this,
            BUTTON_Panel, ButtonName, wxDefaultPosition, wxSize(80.5, 23), 0, wxDefaultValidator, ButtonText);
        QlinkList->push_back(newButton);
        QlinkList->back()->SetBackgroundColour(*wxCYAN);
        ButtonSizer->Add(QlinkList->back(), wxLeft);
        //update layout of page and button index
        ButtonSizer->Layout();
        
    }
}