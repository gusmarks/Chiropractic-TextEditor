#pragma once
#include <wx/wx.h>
#include <wx/dialog.h>
#include <iostream>
#include <fstream>

class ImageButton : public wxBitmapButton {
public:
	ImageButton();
	ImageButton(wxWindow* parent, wxWindowID id, wxBitmap image, wxPoint location, int ind, std::string str) :
		wxBitmapButton(parent, id, image, location, wxSize(10, 10), 0, wxDefaultValidator, wxButtonNameStr) {
		setSelected(false);
		index = ind;
		this->setPartName(str);
	}
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
	myImagePanel() {}
	myImagePanel(wxWindow* parent, wxString file, wxBitmapType format) :
		wxPanel(parent, wxEVT_SET_FOCUS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL) {
		MainImage.LoadFile(file, format);
		MainImage.SetDepth(0);
		circle.LoadFile("circlemap.jpg", wxBITMAP_TYPE_ANY);
		redcircle.LoadFile("circlemapred.png", wxBITMAP_TYPE_ANY);
		MakeButtons();
	}
	void paintEvent(wxPaintEvent& evt);
	void paintNow()
	{
		wxClientDC dc(this);
		render(dc);
	}
	void swapImages(wxCommandEvent& evt);
	void MakeButtons() {
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
	void render(wxDC& dc);
	std::string collectValues() {
		int size = sizeof(ClickCircles) / sizeof(ClickCircles[0]);
		std::string values = "";
		for (int i = 0; i < size; i++) {
			if (ClickCircles[i]->getSelected()) {
				values += " " + ClickCircles[i]->getPartName();
			}
		}
		return values;
	}
private:
	wxBitmap MainImage;
	wxBitmap circle;
	wxBitmap redcircle;
	ImageButton* ClickCircles[83];
	DECLARE_EVENT_TABLE()
};
class myImageDialog : public wxDialog {
public:
	myImageDialog() {}
	myImageDialog(wxWindow* parent, wxWindowID id, wxString name) :
		wxDialog(parent, id, name, wxDefaultPosition, wxSize(600, 650), wxDEFAULT_DIALOG_STYLE, name) {

		mainPanel = new myImagePanel(this, wxT("PainLocations.png"), wxBITMAP_TYPE_PNG);
		Cancel = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		Okay = new wxButton(this, wxID_OK, "Okay", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		displayBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 100), wxTE_READONLY);

		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonSizer->Add(Okay);
		buttonSizer->AddSpacer(20);
		buttonSizer->Add(Cancel);
		sizer->Add(mainPanel, 2, wxEXPAND);
		sizer->Add(displayBox);
		sizer->Add(buttonSizer);
		this->Layout();
		this->SetSizer(sizer);
		Center(wxBOTH);
	}
	std::string getSelections() {
		return mainPanel->collectValues();
	}
	void updateDisplay() {
		displayBox->Clear();
		displayBox->WriteText(mainPanel->collectValues());
	}
private:
	wxButton* Cancel;
	wxButton* Okay;
	myImagePanel* mainPanel;
	wxTextCtrl* displayBox;
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
};
class mySpineDialog :public wxDialog {
private:
	wxPanel* multiSelectionPanel;
	wxButton* okay;
	wxButton* cancel;
public:
	mySpineDialog() {}
	mySpineDialog(wxWindow* parent, wxWindowID id, wxString name) :
		wxDialog(parent, id, name, wxDefaultPosition, wxSize(600, 650), wxDEFAULT_DIALOG_STYLE, name) {
		okay = new wxButton(this, wxID_OK, "Okay", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		cancel = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	}
};