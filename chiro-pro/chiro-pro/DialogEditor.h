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
	wxButton* addDialog, * save, * bold, * underline, * italic;
	std::string name;

	DialogHelper* popupHandeler;
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
public:
	DialogEditor(wxWindow* parent, wxWindowID id,std::string name)
		:wxDialog(parent, id, "Dx/CPT editor", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "") {

		addDialog = new wxButton(this, wxID_ANY, "addDialog", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		save = new wxButton(this, wxID_ANY, "save", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		bold = new wxButton(this, wxID_ANY, "bold", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		underline = new wxButton(this, wxID_ANY, "underline", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		italic = new wxButton(this, wxID_ANY, "italic", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		//create the text box the paragraph will be writen in
		MainDisplayBox = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 400), wxRE_MULTILINE, wxDefaultValidator);
		//show and add the components to the sizer
		save->Show();
		addDialog->Show();
		bold->Show();
		underline->Show();
		italic->Show();
		hSizer->Add(save);
		hSizer->Add(addDialog);
		hSizer->Add(bold);
		hSizer->Add(underline);
		hSizer->Add(italic);
		vSizer->Add(MainDisplayBox);
		vSizer->Add(hSizer);
		hSizer->Layout();
		//set the sizer
		SetSizer(vSizer);
		Layout();
		//bind the buttons
		addDialog->Bind(wxEVT_BUTTON, &DialogEditor::AddDialogToText, this);
		save->Bind(wxEVT_BUTTON, &DialogEditor::SaveDialogText, this);
		bold->Bind(wxEVT_BUTTON, &DialogEditor::addBoldtoText, this);
		underline->Bind(wxEVT_BUTTON, &DialogEditor::addUnderlinetoText, this);
		italic->Bind(wxEVT_BUTTON, &DialogEditor::addItalictoText, this);

		popupHandeler = new DialogHelper(this);
		


	}
	void AddDialogToText(wxCommandEvent& event) {
		try {
			wxString dialogChoice = popupHandeler->SingleChoiceDialog("DialogInformation/DialogSelection-2.txt", "select a popup.");
			dialogChoice = TranslateDialogNames(dialogChoice);
			if (dialogChoice != "OTHER") {
				MainDisplayBox->WriteText("~<H>");
				MainDisplayBox->WriteText(dialogChoice);
				MainDisplayBox->WriteText("<H>~");
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in dialog creator");
		}
	}
	void openGenFile(std::string namee) {
	std::ifstream openFileOfCodes;
	std::string content;
	this->name = namee;
	try {
		openFileOfCodes.open(namee, std::ofstream::app);
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
	wxString TranslateDialogNames(wxString name) {
		if (name == "Allergies") {
			return "Dialog-ID-Allergies";
		}
		if (name == "Body") {
			return "Dialog-ID-Body";
		}
		if (name == "Cervical Adjustment") {
			return "Dialog-ID-Cervical-Adjustment";
		}
		if (name == "Calender") {
			return "Dialog-ID-Calender";
		}
		if (name == "Compicating Factors") {
			return "Dialog-ID-Complicating-Factors";
		}
		if (name == "Contraindiction") {
			return "Dialog-ID-Contraindiction";
		}
		if (name == "Diet & Nutrition") {
			return "Dialog-ID-Nutrition";
		}
		if (name == "Dificulty Performing") {
			return "Dialog-ID-Dificulty-Performing";
		}
		if (name == "Drugs Medication") {
			return "Dialog-ID-Drugs-Medication";
		}
		if (name == "Exercise Routine") {
			return "Dialog-ID-Exercise";
		}
		if (name == "Family History") {
			return "Dialog-ID-Family-History";
		}//19 left
		if (name == "Freqency of pain") {
			return "Dialog-ID-Pain";
		}
		if (name == "Goals") {
			return "Dialog-ID-Goals";
		}
		if (name == "Improve/Decline") {
			return "Dialog-ID-Improve/Decline";
		}
		if (name == "Injury") {
			return "Dialog-ID-Injury";
		}
		if (name == "Life Affected") {
			return "Dialog-ID-Life-Affected";
		}
		if (name == "Percent or vas") {
			return "Dialog-ID-Percentorvas";
		}
		if (name == "Postural Change") {
			return "Dialog-ID-PosturalChange";
		}
		if (name == "Quality of Discomfort") {
			return "Dialog-ID-QualityofDiscomfort";
		}
		if (name == "Rate of Improvement") {
			return "Dialog-ID-Improve";
		}
		if (name == "Restrictions") {
			return "Dialog-ID-Restrictions";
		}//9 left
		if (name == "ROM region") {
			return "Dialog-ID-ROMregion";
		}
		if (name == "Social habits") {
			return "Dialog-ID-Social";
		}
		if (name == "Surgical history") {
			return "Dialog-ID-Surgical";
		}
		if (name == "Tone Intensity") {
			return "Dialog-ID-ToneIntensity";
		}
		if (name == "Side") {
			return "Dialog-ID-Side";
		}
		if (name == "Spine") {
			return "Dialog-ID-Spine";
		}
		if (name == "Specific Muscles") {
			return "Dialog-ID-Muscle";
		}
		if (name == "Work") {
			return "Dialog-ID-Work";
		}
		if (name == "Vas") {
			return "Dialog-ID-Vas";
		}
		//----------codes--------------
		if (name == "Ankle") {
			return "CPT-ID-Ankle";
		}
		if (name == "Cervical Spine") {
			return "CPT-ID-CervicalSpine";
		}
		if (name == "Elbow Forarm") {
			return "CPT-ID-ElbowForarm";
		}
		if (name == "Foot") {
			return "CPT-ID-Foot";
		}
		if (name == "Hand") {
			return "CPT-ID-Hand";
		}
		if (name == "Head") {
			return "CPT-ID-Head";
		}
		if (name == "Hip-Thigh") {
			return "CPT-ID-Hip-Thigh";
		}
		if (name == "Knee") {
			return "CPT-ID-Knee";
		}
		if (name == "Lumbosacral Spine") {
			return "CPT-ID-Lumbosacral-Spine";
		}
		if (name == "Shoulder") {
			return "CPT-ID-Shoulder";
		}
		if (name == "Thoacic Spine") {
			return "CPT-ID-ThoacicSpine";
		}
		if (name == "Wrist") {
			return "CPT-ID-Wrist";
		}
		if (name == "Manipulation") {
			return "CPT-ID-Manipulation";
		}
		if (name == "Management") {
			return "CPT-ID-Management";
		}
		if (name == "Misc") {
			return "CPT-ID-Misc";
		}
		if (name == "Rehabilitation") {
			return "CPT-ID-Rehabilitation";
		}
		if (name == "Xray") {
			return "CPT-ID-Xray";
		}
		if (name == "Place of Service") {
			return "CPT-ID-PlaceofService";
		}
		if (name == "OTHER") {
			popupHandeler->errorMessage("value not accepted");
		}

		return "OTHER";
	}
	void addBoldtoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<b>");
		MainDisplayBox->WriteText("<b>~");
	}
	//adds an underline to the text
	void addUnderlinetoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<u>");
		MainDisplayBox->WriteText("<u>~");
	}
	//adds italic to the text
	void addItalictoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<i>");
		MainDisplayBox->WriteText("<i>~");
	}
};
