#include "BodyImageDialog.h"
#include <iostream>
#include <fstream>
#include <wx/wx.h>

myImageDialog::myImageDialog() {}
myImageDialog::myImageDialog(wxWindow* parent, wxWindowID id, wxString name) :
    wxDialog(parent, id, "hi", wxDefaultPosition, wxSize(600, 650), wxDEFAULT_DIALOG_STYLE, name) {

    mainPanel = new myImagePanel(this, wxT("PainLocations.png"), wxBITMAP_TYPE_PNG);
    Cancel = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    Okay = new wxButton(this, wxID_OK, "Okay", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    displayBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 100), wxTE_READONLY);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(Cancel);
    buttonSizer->AddSpacer(20);
    buttonSizer->Add(Okay);
    sizer->Add(mainPanel, 2, wxEXPAND);
    sizer->Add(displayBox);
    sizer->Add(buttonSizer);
    this->Layout();
    this->SetSizer(sizer);
}
std::string myImageDialog::getSelections() {
    return mainPanel->collectValues();
}
void myImageDialog::updateDisplay() {
    displayBox->Clear();
    displayBox->WriteText(mainPanel->collectValues());
}
BEGIN_EVENT_TABLE(myImagePanel, wxPanel)
EVT_PAINT(myImagePanel::paintEvent)
EVT_BUTTON(wxID_ANY, myImagePanel::swapImages)
END_EVENT_TABLE()

myImagePanel::myImagePanel() {}
myImagePanel::myImagePanel(wxWindow* parent, wxString file, wxBitmapType format) :
    wxPanel(parent, wxEVT_SET_FOCUS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL) {
    MainImage.LoadFile(file, format);
    MainImage.SetDepth(0);
    circle.LoadFile("circlemap.jpg", wxBITMAP_TYPE_ANY);
    redcircle.LoadFile("circlemapred.png", wxBITMAP_TYPE_ANY);
    MakeButtons();
}
void myImagePanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}
void myImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}
void myImagePanel::render(wxDC& dc)
{
    dc.DrawBitmap(MainImage, 0, 0, false);
}
void myImagePanel::swapImages(wxCommandEvent& evt) {
    ImageButton* temp = (ImageButton*)evt.GetEventObject();
    int ind = temp->getIndex();
    if (ClickCircles[ind]->getSelected() == false) {
        ClickCircles[ind]->SetBitmap(redcircle);
        ClickCircles[ind]->toggleSelected();
    }
    else {
        ClickCircles[ind]->SetBitmap(circle);
        ClickCircles[ind]->toggleSelected();
    }
    myImageDialog* parent = (myImageDialog*)this->GetParent();
    if (parent) {
        parent->updateDisplay();
    }
}
void myImagePanel::MakeButtons() {
    std::string str = "";
    int X, Y;
    std::ifstream LocationXY("ButtonLocations-BodyImage.txt");
    if (LocationXY.is_open()) {
        for (int i = 0; !LocationXY.eof(); i++) {
            LocationXY >> X;
            LocationXY >> Y;
            LocationXY >> str;
            ClickCircles[i] = new ImageButton(this, wxID_ANY, circle, wxPoint(X, Y), i, str);
        }
    }
}
std::string myImagePanel::collectValues() {
    int size = sizeof(ClickCircles) / sizeof(ClickCircles[0]);
    std::string values = "";
    for (int i = 0; i < size; i++) {
        if (ClickCircles[i]->getSelected()) {
            values += " " + ClickCircles[i]->getPartName();
        }
    }
    return values;
}
ImageButton::ImageButton(wxWindow* parent, wxWindowID id, wxBitmap image, wxPoint location, int ind, std::string str) :
    wxBitmapButton(parent, id, image, location, wxSize(10, 10), 0, wxDefaultValidator, wxButtonNameStr) {
    setSelected(false);
    index = ind;
    this->setPartName(str);
}