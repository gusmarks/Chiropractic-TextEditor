#pragma once
#include <filesystem>
#include <wx/button.h>
#include<wx/dialog.h>
#include<wx/listbox.h>
#include <wx/sizer.h>
#include <fileSystem> 
#include <wx/arrstr.h>
namespace fileSys = std::filesystem;

class ArchiveManager : public wxDialog {
private:
	std::string pathParse = "PatientFileParse/", pathView = "PatientFileView/", pathLoad = "PatientFileLoad/";
	std::string pathParseArchive = "PatientFileParse/Archive/", pathViewArchive = "PatientFileView/Archive/",
				pathLoadArchive = "PatientFileLoad/Archive/";
	wxArrayInt PatientIndicies;
	std::string tempPatientFileName = "";

	wxButton* Billed;
	wxButton* Payed;
	wxButton* archiveFiles;
	wxListBox* patientsViewBox;
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	wxArrayString patientsToArchive;
	enum {
		ID_ARCHIVE_BILL,
		ID_ARCHIVE_PAY,
		ID_ARCHIVE_ARCHIVE
	};
public:
	ArchiveManager(wxWindow* parent, wxWindowID id) :wxDialog(parent, id, "Archive Manager", wxDefaultPosition, wxSize(500, 500), wxDEFAULT_DIALOG_STYLE, "") 
		{
		patientsToArchive = gatherPatients();
		Billed = new wxButton(this, ID_ARCHIVE_BILL,"mark as Billed/Unblilled",wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,"");
		Payed = new wxButton(this, ID_ARCHIVE_PAY, "mark as Payed/UnPayed", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		archiveFiles = new wxButton(this, ID_ARCHIVE_ARCHIVE, "archiveFiles", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "");
		patientsViewBox = new wxListBox(this,wxID_ANY, wxDefaultPosition, wxSize(500, 400), patientsToArchive,wxLB_MULTIPLE|wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT, wxDefaultValidator,"Patient View Box");
		hSizer->Add(Billed, wxCENTER);
		hSizer->Add(Payed, wxCENTER);
		hSizer->Add(archiveFiles, wxCENTER);
		vSizer->Add(patientsViewBox, wxCENTER);
		vSizer->Add(hSizer);
		SetSizer(vSizer);
		Layout();
	}
	/// <summary>
	/// this toggles filenames to mark them as billed or unbilled.
	/// imppliment toggler of all selected files
	/// </summary>
	/// <param name="event"></param>
	void Billed_Unbilled(wxCommandEvent& event) {
		patientsViewBox->GetSelections(PatientIndicies);
		size_t size = PatientIndicies.size();
		for (size_t i = 0; i < size; i++) {
			tempPatientFileName = patientsToArchive[PatientIndicies[i]];
			int tempSize = (int)tempPatientFileName.size();
			char check = tempPatientFileName[tempSize -3];
			
			if (check == '0') {
				tempPatientFileName[tempPatientFileName.size() - 3] = '1';
				patientsViewBox->SetString(PatientIndicies[i],
					tempPatientFileName);
				std::string pP = pathParse + patientsToArchive[PatientIndicies[i]].ToStdString() + ".txt";
				std::string pP2 = pathParse + tempPatientFileName + ".txt";
				std::string pL = pathLoad + patientsToArchive[PatientIndicies[i]].ToStdString() + ".xml";
				std::string pL2 = pathLoad + tempPatientFileName + ".xml";
				std::string pV = pathView + patientsToArchive[PatientIndicies[i]].ToStdString() + ".html";
				std::string pV2 = pathView + tempPatientFileName + ".html";
				fileSys::rename(pP, pP2);
				fileSys::rename(pL, pL2);
				fileSys::rename(pV, pV2);
				patientsToArchive[PatientIndicies[i]] = tempPatientFileName;

			}
			else if(check=='1') {
				tempPatientFileName[tempPatientFileName.size() - 3] = '0';
				patientsViewBox->SetString(PatientIndicies[i],
					tempPatientFileName);
				std::string pP = pathParse + patientsToArchive[PatientIndicies[i]].ToStdString() + ".txt";
				std::string pP2 = pathParse + tempPatientFileName + ".txt";
				std::string pL = pathLoad + patientsToArchive[PatientIndicies[i]].ToStdString() + ".xml";
				std::string pL2 = pathLoad + tempPatientFileName + ".xml";
				std::string pV = pathView + patientsToArchive[PatientIndicies[i]].ToStdString() + ".html";
				std::string pV2 = pathView + tempPatientFileName + ".html";
				fileSys::rename(pP, pP2);
				fileSys::rename(pL, pL2);
				fileSys::rename(pV, pV2);
				patientsToArchive[PatientIndicies[i]] = tempPatientFileName;
			}
			
		}

	}
	void payed_Unpayed(wxCommandEvent& event) {
		patientsViewBox->GetSelections(PatientIndicies);
		size_t size = PatientIndicies.size();
		for (size_t i = 0; i < size; i++) {
			tempPatientFileName = patientsToArchive[PatientIndicies[i]];
			int tempSize = (int)tempPatientFileName.size();
			char check = tempPatientFileName[tempSize - 1];

			if (check == '0') {
				tempPatientFileName[tempPatientFileName.size() - 1] = '1';
				patientsViewBox->SetString(PatientIndicies[i],
					tempPatientFileName);
				std::string pP = pathParse + patientsToArchive[PatientIndicies[i]].ToStdString() + ".txt";
				std::string pP2 = pathParse + tempPatientFileName + ".txt";
				std::string pL = pathLoad + patientsToArchive[PatientIndicies[i]].ToStdString() + ".xml";
				std::string pL2 = pathLoad + tempPatientFileName + ".xml";
				std::string pV = pathView + patientsToArchive[PatientIndicies[i]].ToStdString() + ".html";
				std::string pV2 = pathView + tempPatientFileName + ".html";
				fileSys::rename(pP, pP2);
				fileSys::rename(pL, pL2);
				fileSys::rename(pV, pV2);
				patientsToArchive[PatientIndicies[i]] = tempPatientFileName;
			}else if (check == '1') {
				tempPatientFileName[tempPatientFileName.size() - 1] = '0';
				patientsViewBox->SetString(PatientIndicies[i],
					tempPatientFileName);
				std::string pP = pathParse + patientsToArchive[PatientIndicies[i]].ToStdString() + ".txt";
				std::string pP2 = pathParse + tempPatientFileName + ".txt";

				std::string pL = pathLoad + patientsToArchive[PatientIndicies[i]].ToStdString() + ".xml";
				std::string pL2 = pathLoad + tempPatientFileName + ".xml";

				std::string pV = pathView + patientsToArchive[PatientIndicies[i]].ToStdString() + ".html";
				std::string pV2 = pathView + tempPatientFileName + ".html";
				fileSys::rename(pP, pP2);
				fileSys::rename(pL, pL2);
				fileSys::rename(pV, pV2);
				patientsToArchive[PatientIndicies[i]] = tempPatientFileName;
			}
			//std::string p = pathParse + patientsToArchive[PatientIndicies[i]].ToStdString() + ".txt";
			//std::string p2 = pathParse + tempPatientFileName + ".txt";
			//fileSys::rename(p, p2);
		}
	}
	void archiveFilesFunction(wxCommandEvent& event) {
		patientsViewBox->GetSelections(PatientIndicies);
		size_t size = PatientIndicies.size();
		for (size_t i = 0; i < size; i++) {
			tempPatientFileName = patientsToArchive[PatientIndicies[i]];
			int tempSize = (int)tempPatientFileName.size();
			char checkBilled = tempPatientFileName[tempSize - 3];
			char checkPayed = tempPatientFileName[tempSize - 1];
			if (checkBilled == '1' && checkPayed == '1') {
				int pos = tempPatientFileName.find("_");
				std::string patientName = tempPatientFileName.substr(0, pos);
				if (checkForMatchingFolderAndCreateFolder(patientName)) {
					std::string pP = pathParse + tempPatientFileName + ".txt";
					std::string pP2 = pathParseArchive + patientName + "/" + tempPatientFileName + ".txt";
					std::string pL = pathLoad + tempPatientFileName + ".xml";
					std::string pL2 = pathLoadArchive + patientName + "/" + tempPatientFileName + ".xml";
					std::string pV = pathView + tempPatientFileName + ".html";
					std::string pV2 = pathViewArchive + patientName + "/" + tempPatientFileName + ".html";
					fileSys::rename(pP, pP2);
					fileSys::rename(pL, pL2);
					fileSys::rename(pV, pV2);
					patientsViewBox->Delete(PatientIndicies[i]);
				}
				else {
					std::string pP = pathParse + tempPatientFileName + ".txt";
					std::string pP2 = pathParseArchive + patientName + "/" + tempPatientFileName + ".txt";
					std::string pL = pathLoad + tempPatientFileName + ".xml";
					std::string pL2 = pathLoadArchive + patientName + "/" + tempPatientFileName + ".xml";
					std::string pV = pathView + tempPatientFileName + ".html";
					std::string pV2 = pathViewArchive + patientName + "/" + tempPatientFileName + ".html";
					fileSys::rename(pP, pP2);
					fileSys::rename(pL, pL2);
					fileSys::rename(pV, pV2);
					patientsViewBox->Delete(PatientIndicies[i]);
				}
			}
			else {

			}
		}
	}
	bool checkForMatchingFolderAndCreateFolder(std::string fileName) {
		std::string tempFileName = fileName;
		std::vector<std::string> archiveFolders = gatherPatientFolders();
		std::string pathNameParse,pathNameLoad,pathNameView;
		pathNameParse.append(pathParseArchive);
		pathNameParse.append(tempFileName);
		pathNameLoad.append(pathLoadArchive);
		pathNameLoad.append(tempFileName);
		pathNameView.append(pathViewArchive);
		pathNameView.append(tempFileName);

		std::transform(tempFileName.begin(),tempFileName.end(),tempFileName.begin(),::tolower);

		for (size_t i = 0; i < archiveFolders.size(); i++) {
			std::string tempFolder = archiveFolders.at(i);
			std::transform(tempFolder.begin(), tempFolder.end(), tempFolder.begin(), ::tolower);
			if (tempFolder == tempFileName) {
				return true;
			}

		}
		fileSys::create_directory(pathNameParse);
		fileSys::create_directory(pathNameLoad);
		fileSys::create_directory(pathNameView);
		return false;
	}
	wxArrayString gatherPatients() {
		std::string listFileName = "";
		wxArrayString results;
		for (const auto& entry : fileSys::directory_iterator(pathParse)) {
			if (!fileSys::is_directory(entry)) {
				listFileName = entry.path().stem().string();
				results.Add(listFileName);
			}

		}
		if (!results.empty()) {
			return results;
		}
		else{
			results.Add("empty");
			return results;
		}
	}
	std::vector<std::string> gatherPatientFolders() {
		std::string listFileName = "";
		std::string tempFileName = "";
		std::vector<std::string> results;
		for (const auto& entry : fileSys::directory_iterator(pathParseArchive)) {
			if (fileSys::is_directory(entry)) {
				listFileName = entry.path().stem().string();
				results.push_back(listFileName);
			}

		}
		if (!results.empty()) {
			return results;
		}
		else {
			results.push_back("empty");
			return results;
		}
	}
	void setPatientsToArchive(wxArrayString arr) {
		patientsToArchive = arr;
		patientsViewBox->Clear();
		patientsViewBox->InsertItems(patientsToArchive, 0);
	}
	wxDECLARE_EVENT_TABLE();
};