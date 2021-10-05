#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "DialogHelper.h"
#include <fstream>
#include <iostream>
#include <sstream>


class DialogEditor : public wxDialog {
private:
	wxRichTextCtrl* MainDisplayBox;
	wxButton* save;
	std::string name;

	DialogHelper* popupHandeler;
	wxBoxSizer* vSizer;
	wxBoxSizer* hSizer;
public:
	DialogEditor(wxWindow* parent, wxWindowID id,std::string name)
		:wxDialog(parent, id, "Dx/CPT editor", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "") {

		save = new wxButton(this, wxID_ANY, "save", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		MainDisplayBox = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(800, 400), wxRE_MULTILINE, wxDefaultValidator);
		vSizer = new wxBoxSizer(wxVERTICAL);
		hSizer = new wxBoxSizer(wxHORIZONTAL);
		MainDisplayBox->Show();
		save->Show();
		hSizer->Add(save);
		vSizer->Add(MainDisplayBox);
		vSizer->Add(hSizer);
		hSizer->Layout();
		//set the sizer
		SetSizer(vSizer);
		this->name = name;
		Layout();

		save->Bind(wxEVT_BUTTON, &DialogEditor::SaveDialogText, this);
		


	}
	void openGenFile() {
	std::ifstream openFileOfCodes;
	std::string content;
	try {
		openFileOfCodes.open(name, std::ofstream::app);
		if (openFileOfCodes.is_open()) {

			std::string temp;
			while (std::getline(openFileOfCodes, temp)) {

				content += temp + "\n";
			}
			openFileOfCodes.close();
		}
		else {
			popupHandeler->errorMessage("file failed to open inturnaly");
		}
		MainDisplayBox->WriteText(content);
	}
	catch (...) {
		popupHandeler->errorMessage("an error occured in dialog editor");
	}
}

	void openCodeFile() {
		if (name == "Ankle") {
			name = "Dialogs/cptcodes/cptcodes-ankle.txt";
		}
		if (name == "Cervical Spine") {
			name = "Dialogs/cptcodes/cptcodes-cervicalSpine.txt";
		}
		if (name == "Elbow Forarm") {
			name = "Dialogs/cptcodes/ctpcodes-ElbowForearm.txt";
		}
		if (name == "Foot") {
			name = "Dialogs/cptcodes/cptcodes-foot.txt";
		}
		if (name == "Hand") {
			name = "Dialogs/cptcodes/cptcodes-hand.txt";
		}
		if (name == "Head") {
			name = "Dialogs/cptcodes/cptcodes-head.txt";
		}
		if (name == "Hip-Thigh") {
			name = "Dialogs/cptcodes/cptcodes-hip_thigh.txt";
		}
		if (name == "Knee") {
			name = "Dialogs/cptcodes/ctpcodes-knee.txt";
		}
		if (name == "Lumbosacral Spine") {
			name = "Dialogs/cptcodes/cptcodes-LumbosacralSpine.txt";
		}
		if (name == "Shoulder") {
			name = "Dialogs/cptcodes/cptcodes-shoulder.txt";
		}
		if (name == "Thoacic Spine") {
			name = "Dialogs/cptcodes/cptcodes-thoacicSpine.txt";
		}
		if (name == "Wrist") {
			name = "Dialogs/cptcodes/cptcodes-Wrist.txt";
		}
		if (name == "Manipulation") {
			name = "Dialogs/cptcodes/cptChiropractic-Manipulation-CPT-Coding.txt";
		}
		if (name == "Management") {
			name = "Dialogs/cptcodes/cptEvaluation-and-Management-Codes.txt";
		}
		if (name == "Misc") {
			name = "Dialogs/cptcodes/cpt-misc.txt";
		}
		if (name == "Rehabilitation") {
			name = "Dialogs/cptcodes/cptcode-Physical-Medicine-Rehabilitation.txt";
		}
		if (name == "Xray") {
			name = "Dialogs/cptcodes/cptext-xray.txt";
		}
		if (name == "Place of Service") {
			name = "Dialogs/cptcodes/cpt-placeofService.txt";
		}
		if (name == "Extentions") {
			name = "Dialogs/cptcodes/cprcode-ext.txt";
		}
		if (name == "Mods") {
			name = "Dialogs/cptcodes/cptcodes-mods.txt";
		}
		try {
			std::ifstream openFileOfCodes;
			std::string content;
			openFileOfCodes.open(name, std::ofstream::app);
			if (openFileOfCodes.is_open()) {

				std::string temp;
				while (std::getline(openFileOfCodes, temp)) {

					content += temp + "\n";
				}
				openFileOfCodes.close();
			}
			else {
				popupHandeler->errorMessage("file failed to open inturnaly");
			}
			MainDisplayBox->WriteText(content);
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in dialog editor");
		}
	}
	void SaveDialogText(wxCommandEvent& event) {
		try {
			if (MainDisplayBox->SaveFile(name, wxRICHTEXT_TYPE_ANY)) {
				wxMessageDialog dialog(NULL, wxT("The file saved successfuly"), wxT("save file"), wxOK);
				if (dialog.ShowModal() == wxID_OK) {
					MainDisplayBox->Clear();
					//DialogCount++;
					this->Close();
				}
			}
			else {
				wxMessageDialog dialog(NULL, wxT("The file Failed to save"), wxT("save file"), wxOK);
				if (dialog.ShowModal() == wxID_OK) {
				}
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in dialog editor");
		}
	}
	

};
