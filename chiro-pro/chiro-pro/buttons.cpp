
#include <wx/wx.h>
#include <wx/panel.h>
#include "Buttons.h"
#include "Frame.h"
#include <string>



ButtonPanel::ButtonPanel(wxWindow* parent, wxString name, std::string layoutName, int ind) : wxPanel(parent, wxID_ANY, wxDefaultPosition,
	wxDefaultSize, wxTAB_TRAVERSAL, name)
{
	QLinkIndex = 0;
    index = ind;
	ButtonSizer = new wxFlexGridSizer(3, 17, 0, 0);
	this->SetSizer(ButtonSizer);
    this->layoutName = layoutName;
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
    QLinkIndex = 0;//give value to index
    //while there is still text in the layout file keep taking in two lines into out string variables
    // creating buttons from those strings and adding them to the array. 
    //lastly updating th layout of the page and closing the layout file

    while (this->ButtonLayoutin.peek() != EOF) {
        //get input
        getline(this->ButtonLayoutin, name);
        getline(this->ButtonLayoutin, maintext);
        //make button
        this->QlinkList[QLinkIndex] = new wxButton(this, BUTTON_Write, name, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, maintext);
        //add button to array
        this-> ButtonSizer->Add(QlinkList[QLinkIndex], wxLeft);
        //update layout
        ButtonSizer->Layout();
        //update index
        QLinkIndex++;
    }
    //close file
    this->ButtonLayoutin.close();
}
void ButtonPanel::saveButtons() {
    //if the layout file is not open, open it
    if (!this->ButtonLayoutout.is_open()) {
        ButtonLayoutout.open(this->layoutName, std::fstream::out);
    }
    // for the size of the array save each button into a text file named layout
    int size = sizeof(this->QlinkList) / sizeof(this->QlinkList);
    for (int i = 0; i < size; i++) {
        if (this->QlinkList[i] != nullptr) {

            this->ButtonLayoutout << this->QlinkList[i]->GetLabel();
            this->ButtonLayoutout << "\n";
            this->ButtonLayoutout << this->QlinkList[i]->GetName();
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

