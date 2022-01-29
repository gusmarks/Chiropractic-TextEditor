#pragma once
//include wx for ui componats, choicdlg for choice dialogs.
//include calender for a popup calender, and body image for a popup of a body to select parts
#include <wx/wx.h>
#include <wx/choicdlg.h>
#include "CalenderDialog.h"
#include "BodyImageDialog.h"
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
struct selectionInformation {
	std::string selectionText;
	int selectionIndex;
};
class DialogHelper {
private:
	//body image, calender and parent are all stored for later use,
	//parent is used in various dialogs to ensure they are on the main frame
	bodyImageDialog* bodyDialogObj;
	CalenderDialog* cal;
	wxWindow* parent;
public:
	//constructor definition
	DialogHelper(wxWindow* parent) {this->parent = parent;}
	/// <summary>
/// the function confirmIntent accepts a string with a question,
/// and produces a popupbox to ask this question. then waits on a yes or no responce, and acts acordingly
/// </summary>
/// <param name="message"></param>
/// <returns></returns>
	bool confirmIntent(wxString message) {

		wxMessageDialog dialog(NULL, message + wxT("unsaved progress will be lost."), wxT("Question"),
			wxNO_DEFAULT | wxYES_NO | wxICON_INFORMATION);
		switch (dialog.ShowModal()) {
		case wxID_YES:
			wxLogStatus(wxT("You pressed \"Yes\""));
			return true;
			break;
		case wxID_NO:
			wxLogStatus(wxT("You pressed \"No\""));
			return false;
			break;
		default:       wxLogError(wxT("Unexpected wxMessageDialog return code!"));
		}
		return false;
	}
	/// <summary>
	/// the function confirmIntentAddButton, takes no parameters, but asks what type of button to add
	/// to the current panel
	/// </summary>
	/// <returns></returns>
	wxString confirmIntentAddButton(wxWindow* parent) {
		wxArrayString options;
		try {
			options.Add("Text Button.");
			options.Add("New Page Button.");
			wxSingleChoiceDialog dialog(parent, wxT("would you like a Text button, or a Page Button?"), wxT("Question"), options);
			//dialog.SetYesNoLabels((wxMessageDialogBase::ButtonLabel)"text", (wxMessageDialogBase::ButtonLabel)"Link");
			switch (dialog.ShowModal()) {
			case wxID_OK:
				wxLogStatus(wxT("You pressed \"Ok\""));
				return dialog.GetStringSelection();
				break;
			case wxID_CANCEL:
				wxLogStatus(wxT("You pressed \"Cancel\""));
				return "Cancel";
				break;
			default:       wxLogError(wxT("Unexpected wxMessageDialog return code!"));
			}
			return "";
		}
		catch (...) {
			errorMessage("an error occured in dialog helper");
		}
		return "";
	}
	/// <summary>
	/// takes in a string with any message, and produces a popup error message this helps prevent crashing
	/// add lets the user know what went wrong
	/// </summary>
	/// <param name="str">str contains the message to display</param>
	/// <returns></returns>
	bool errorMessage(wxString str) {
		wxMessageDialog dialog(NULL, str, wxT("Message"),
			wxOK_DEFAULT | wxOK | wxICON_ERROR);

		switch (dialog.ShowModal()) {
		case wxID_OK:
			wxLogStatus(wxT("you pressed \"okay\""));
			return true;
			break;
		}
		return false;
	}

	bool Message(wxString str) {
		wxMessageDialog dialog(NULL, str, wxT("Message"),
			wxOK_DEFAULT | wxOK | wxICON_INFORMATION);

		switch (dialog.ShowModal()) {
		case wxID_OK:
			wxLogStatus(wxT("you pressed \"okay\""));
			return true;
			break;
		}
		return false;
	}
	/// <summary>
/// this function opens a list of strings, and asks the user to pick as many as they want
/// </summary>
/// <param name="path"> the path links to the fils containg the strings</param>
/// <param name="text">text is all selected strings</param>
/// <returns></returns>
	wxString MultipleChoiceDialog(std::string path, std::string text) {
		std::ifstream choiceIn;
		try {
			choiceIn.open(path, std::fstream::in);
			if (choiceIn.is_open()) {
				wxArrayString choices;
				std::string choice;
				while (choiceIn.peek() != EOF) {
					getline(choiceIn, choice);
					choices.Add(choice);
				}
				wxMultiChoiceDialog dialog(this->parent, text, wxT("Make a Selection"), choices);
				if (dialog.ShowModal() == wxID_OK) {
					wxArrayInt selections = dialog.GetSelections();
					if (selections.IsEmpty()) {
						return "OTHER";
					}
					wxString msg;
					for (size_t n = 0; n < selections.GetCount(); n++) {
						if (n > 0) {
							msg += wxString::Format(wxT(",%s"), choices[selections[n]].c_str());
						}
						else {
							msg += choices[selections[n]].c_str();
						}
					}
					
					return msg;
				}
				else if (dialog.ShowModal() == wxID_CANCEL) {
					return "OTHER";
				}
				return "OTHER";
			}
			return "OTHER";
		}
		catch (...) {
			errorMessage("an error occured in dialog helper");
		}
		return "OTHER";
	}
	/// <summary>
	/// this function pops up a list of strings and asks the user to pick one
	/// </summary>
	/// <param name="path">the path links to the file cntaining the strings</param>
	/// <param name="text">text is the selected string</param>
	/// <returns></returns>
	wxString SingleChoiceDialog(std::string path, std::string text) {
		std::ifstream choiceIn;
		try {
			choiceIn.open(path, std::fstream::in);
			if (choiceIn.is_open()) {
				wxArrayString choices;
				std::string choice;
				while (choiceIn.peek() != EOF) {
					getline(choiceIn, choice);
					choices.Add(choice);
				}
				wxSingleChoiceDialog dialog(this->parent, text, wxT("Make a Selection"), choices);
				if (dialog.ShowModal() == wxID_OK) {

					if (choices.IsEmpty()) {
						return "OTHER";
					}
					int selection = dialog.GetSelection();
					return choices[selection];
				}
				return "OTHER";
			}
			return "OTHER";
		}
		catch (...) {
			errorMessage("an error occured in dialog helper");
		}
		return "OTHER";
	}
	int SingleChoiceDialogIndex(std::string path, std::string text) {
		std::ifstream choiceIn;
		try {
			choiceIn.open(path, std::fstream::in);
			if (choiceIn.is_open()) {
				wxArrayString choices;
				std::string choice;
				while (choiceIn.peek() != EOF) {
					getline(choiceIn, choice);
					choices.Add(choice);
				}
				wxSingleChoiceDialog dialog(this->parent, text, wxT("Make a Selection"), choices);
				if (dialog.ShowModal() == wxID_OK) {

					if (choices.IsEmpty()) {
						return -1;
					}
					int selection = dialog.GetSelection();
					return selection;
				}
				else {
					return -1;
				}
				return -1;
			}
			return -1;
		}
		catch (...) {
			errorMessage("an error occured in dialog helper");
		}
		return -1;
	}
/// this fucntion opens a Dialog to allow the user to select a date, upto todays date
/// 	and returns the date in string form.
/// </summary>
/// 
	wxString Calender() {
		cal = new CalenderDialog(parent, "Date of Accident");
		if (cal->ShowModal() == wxID_OK) {
			return cal->getVal();
		}
		delete cal;
		return "OTHER";
	}
	/// <summary>
/// this function pops up an image of a body, and asks the user to select one or more body parts, and returns the 
/// value as a string 
/// </summary>
/// <param name="name">the name of the dialog</param>
/// <returns>a string of all selected values</returns>
	std::string bodyDialog(wxString name) {
		bodyDialogObj = new bodyImageDialog(parent, wxID_ANY, name);
		if (bodyDialogObj->ShowModal() == wxID_OK) {
			return bodyDialogObj->getSelections();
		}
		delete bodyDialogObj;
		return "OTHER";
	}
	selectionInformation selectPremadeDialog() {
		wxArrayString options;
		try {
			std::string pathName = "Dialogs/0dialogList.txt";
			std::fstream dialogList(pathName);
			int i = 0;
			if (dialogList.is_open()) {
				if (!dialogList.eof()) {
					while (dialogList) {
						if (i > 0) {
							std::string startStr;
							std::string endStr;
							std::getline(dialogList, startStr);
							if (!startStr.empty()) {
								for (size_t j = 0; j < startStr.size(); j++) {
									if (startStr[j] == '\t')
										break;
									endStr += (startStr[j]);
								}

								options.Add(endStr);
							}
						}
						i++;
					}
				}
			}
			else {
				errorMessage("file failed to open inturnaly");
			}
			selectionInformation selection;
			selectionInformation Cancel;
			Cancel.selectionText = "Cancel";
			Cancel.selectionIndex = -1;
			wxSingleChoiceDialog dialog(parent, wxT("what button would you like?"), wxT("Question"), options);

			switch (dialog.ShowModal()) {
			case wxID_OK:
				wxLogStatus(wxT("You pressed \"Ok\""));
				selection.selectionText = dialog.GetStringSelection().ToStdString();
				selection.selectionIndex = dialog.GetSelection();
				return selection;
				break;
			case wxID_CANCEL:
				wxLogStatus(wxT("You pressed \"Cancel\""));
				return Cancel;
				break;
			default:       wxLogError(wxT("Unexpected wxMessageDialog return code!"));
				return Cancel;
			}
			return Cancel;
		}
		catch (...) {
			errorMessage("an error occured in dialog helper");
		}
		return {"cancel",-1};
	}
};