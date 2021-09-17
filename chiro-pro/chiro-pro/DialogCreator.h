#pragma once
#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "DialogHelper.h"
#include <fstream>
#include <iostream>
#include <wx/msgdlg.h>
/// <summary>
/// dialog creator is capable of creating and saving new paragraphs that are attached to buttons.
/// </summary>
class DialogCreator : public wxDialog {
public:
	/// <summary>
	/// this is the constructor for the creator.
	/// </summary>
	/// <param name="parent">parent is the window this is atached to</param>
	/// <param name="id">id is not specific but automaticly allocated</param>
	/// <param name="dialogCount">dialog count shows how many there currently are, added here to help wuth loading information</param>
	DialogCreator(wxWindow* parent, wxWindowID id, int dialogCount)
		:wxDialog(parent, id, "Dialog Creator", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "") {

		//dialog helper to provide auxilery functions
		popupHandeler = new DialogHelper(this);
		//create the buttons that will interface with the creator
		addDialog = new wxButton(this, wxID_ANY, "addDialog", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		save = new wxButton(this, wxID_ANY, "save", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		bold = new wxButton(this, wxID_ANY, "bold", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		underline = new wxButton(this, wxID_ANY, "underline", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		italic = new wxButton(this, wxID_ANY, "italic", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		//create the text box the paragraph will be writen in
		MainDisplayBox = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 400), wxRE_MULTILINE, wxDefaultValidator);
		DialogCount = dialogCount;
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
		addDialog->Bind(wxEVT_BUTTON, &DialogCreator::AddDialogToText, this);
		save->Bind(wxEVT_BUTTON, &DialogCreator::SaveDialogText, this);
		bold->Bind(wxEVT_BUTTON, &DialogCreator::addBoldtoText, this);
		underline->Bind(wxEVT_BUTTON, &DialogCreator::addUnderlinetoText, this);
		italic->Bind(wxEVT_BUTTON, &DialogCreator::addItalictoText, this);

	}
	~DialogCreator() {
		delete popupHandeler;
	}
	//adds a dialog from a predefined list to the text box
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
	//save the parahgraph for future use
	void SaveDialogText(wxCommandEvent& event) {
		try {
			std::string DialogFileName = ("Dialogs/DialogFile" + std::to_string(DialogCount + 1) + ".txt");
			if (MainDisplayBox->SaveFile(DialogFileName, wxRICHTEXT_TYPE_ANY)) {


				wxMessageDialog dialog(NULL, wxT("The file saved successfuly"), wxT("save file"), wxOK);
				if (dialog.ShowModal() == wxID_OK) {
					MainDisplayBox->Clear();
					DialogCount++;
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
			popupHandeler->errorMessage("an error occured in dialog creator");
		}
	}
	//adds a bold line to the text
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
	//this takes a word and makes it into the "code" for it, this " code" 
	//is understood by the reading end to produce the right output
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
	/// <summary>
	///returns the dialog count
	/// </summary>
	/// <returns></returns>
	int getDialogCount() {
		return DialogCount;
	}
private:
	wxRichTextCtrl* MainDisplayBox;
	wxButton* addDialog, * save, * bold, * underline, * italic;
	int DialogCount;
	DialogHelper* popupHandeler;
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

};