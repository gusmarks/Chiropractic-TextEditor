#pragma once
#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "DialogHelper.h"
#include <fstream>
#include <iostream>
#include <wx/msgdlg.h>
class DialogCreator : public wxDialog {
public:
	DialogCreator(wxWindow* parent, wxWindowID id, int dialogCount)
		:wxDialog(parent, id, "hello", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "") {
		popupHandeler = new DialogHelper(this);
		addDialog = new wxButton(this, wxID_ANY, "addDialog", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		save = new wxButton(this, wxID_ANY, "save", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		bold = new wxButton(this, wxID_ANY, "bold", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		underline = new wxButton(this, wxID_ANY, "underline", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		italic = new wxButton(this, wxID_ANY, "italic", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		MainDisplayBox = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 400), wxRE_MULTILINE, wxDefaultValidator);
		DialogCount = dialogCount;
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
		SetSizer(vSizer);
		Layout();
		addDialog->Bind(wxEVT_BUTTON, &DialogCreator::AddDialogToText, this);
		save->Bind(wxEVT_BUTTON, &DialogCreator::SaveDialogText, this);
		bold->Bind(wxEVT_BUTTON, &DialogCreator::addBoldtoText, this);
		underline->Bind(wxEVT_BUTTON, &DialogCreator::addUnderlinetoText, this);
		italic->Bind(wxEVT_BUTTON, &DialogCreator::addItalictoText, this);

	}
	void AddDialogToText(wxCommandEvent& event) {
		wxString dialogChoice = popupHandeler->SingleChoiceDialog("DialogInformation/DialogSelection-3.txt", "select a popup.");
		dialogChoice = TranslateDialogNames(dialogChoice);
		MainDisplayBox->WriteText("~<H>");
		MainDisplayBox->WriteText(dialogChoice);
		MainDisplayBox->WriteText("<H>~");

	}
	void SaveDialogText(wxCommandEvent& event) {
		std::string DialogFileName = ("Dialogs/DialogFile" + std::to_string(DialogCount+1) + ".txt");
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
	void addBoldtoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<b>");
		MainDisplayBox->WriteText("<b>~");
	}
	void addUnderlinetoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<u>");
		MainDisplayBox->WriteText("<u>~");
	}
	void addItalictoText(wxCommandEvent& event) {
		MainDisplayBox->WriteText("~<i>");
		MainDisplayBox->WriteText("<i>~");
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
		return "OTHER";
	}
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