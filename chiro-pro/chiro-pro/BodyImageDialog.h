#pragma once
#include <wx/wx.h>
#include <wx/dialog.h>


class ImageButton : public wxBitmapButton {
public:
	ImageButton();
	ImageButton(wxWindow* parent, wxWindowID id, wxBitmap image, wxPoint location, int ind, std::string str);
	void toggleSelected() {
		if (isSelected == false) {
			isSelected = true;
		}
		else {
			isSelected = false;
		}
	}
	void setSelected(bool S) { isSelected = S; }
	bool getSelected() { return isSelected; }
	int getIndex() { return index; }
	std::string getPartName() { return partName; }
	void setPartName(std::string name) { partName = name; }
private:
	bool isSelected;
	int index;
	std::string partName;

};

class myImagePanel : public wxPanel {
public:
	myImagePanel();
	myImagePanel(wxWindow* parent, wxString file, wxBitmapType format);
	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void swapImages(wxCommandEvent& evt);
	void MakeButtons();
	void render(wxDC& dc);
	std::string collectValues();
private:
	wxBitmap MainImage;
	wxBitmap circle;
	wxBitmap redcircle;
	ImageButton* ClickCircles[83];
	DECLARE_EVENT_TABLE()
};
class myImageDialog : public wxDialog {
public:
	myImageDialog();
	myImageDialog(wxWindow* parent, wxWindowID id, wxString name);
	std::string getSelections();
	void updateDisplay();
private:
	wxButton* Cancel;
	wxButton* Okay;
	myImagePanel* mainPanel;
	wxTextCtrl* displayBox;
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
};
