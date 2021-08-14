#include "BodyImageDialog.h"
#include <iostream>
#include <fstream>
#include <wx/wx.h>
BEGIN_EVENT_TABLE(myImagePanel, wxPanel)
EVT_PAINT(myImagePanel::paintEvent)
EVT_BUTTON(wxID_ANY, myImagePanel::swapImages)
END_EVENT_TABLE()
void myImagePanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
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
    bodyImageDialog* parent = (bodyImageDialog*)this->GetParent();
    if (parent) {
        parent->updateDisplay();
    }
}