#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "dialogHelper.h"
#include <fileSystem>
namespace fileSys = std::filesystem;
using std::filesystem::exists;
using std::filesystem::create_directory;
class FuncHelper {
private:
    struct date { int d, m, y; };
    DialogHelper* popupHandeler;
    std::string BlankSpace = " \n\t\r\f\v";
    const int months[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
public:
    bool isPopup(const std::string& str) {return (str.find("Dialog-ID")!= std::string::npos);}
	bool isNumber(const std::string& str) {return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) {return !std::isdigit(c); }) == str.end();}
    bool isText(const std::string& str) {return !(str.find("Dialog-ID") != std::string::npos)&&!str.empty(); }
    void createMissingFiles() {
        std::vector<std::string> filesToCheck = {"PatientFileView","PatientFileLoad","PatientFileParse",
            "panelLayout","Dialogs","billingInfo","bitmaps","DialogInformation","SetInfoAll.txt"};
        std::vector<bool> filesChecked;
        for (size_t i=0; i < filesToCheck.size(); i++) {
            if (!exists(filesToCheck.at(i)) && i == 0) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 1) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 2) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 3) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 4) {
                fileSys::create_directory(filesToCheck.at(i));
                std::ofstream dialogInfo(filesToCheck.at(i)+"0dialogCount.txt");
                dialogInfo << 0;
                dialogInfo.close();
                std::ofstream dialogInfo2(filesToCheck.at(i) + "0dialogList.txt");
                dialogInfo2 << "New Paragraph \n----------------------- ";
                dialogInfo.close();
                std::ofstream dialogInfo3(filesToCheck.at(i) + "0DialogNames.txt");
                dialogInfo3.close();
            }
            if (!exists(filesToCheck.at(i)) && i == 5) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 6) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 7) {
                fileSys::create_directory(filesToCheck.at(i));
            }
            if (!exists(filesToCheck.at(i)) && i == 7) {
                std::ofstream setInfo(filesToCheck.at(i), std::ios::out);
                setInfo.close();
            }
        }
    }
    long getLineNo(std::string& str, std::string delim) {
        try {
            std::vector<std::string> stringsNoBlankLines;
            std::string::size_type pos = 0;
            std::string::size_type prev = 0;

            while ((pos = str.find(delim, prev)) != std::string::npos) {
                stringsNoBlankLines.push_back(str.substr(prev));
                prev = pos + 1;
            }
            stringsNoBlankLines.push_back(str.substr(prev));
            return (long)stringsNoBlankLines.size();
        }
        catch (...) {
            popupHandeler->errorMessage("an error occured in function helper");
        }
    }
    /// <summary>
/// this function returns the current date and time 
/// </summary>
/// <returns></returns>
    std::string getDateToSign() {
        time_t current;
        time(&current);

        return  ctime(&current);
    }
    std::string getDateToSignNoTime() {
        time_t current;
        time(&current);
        std::string time= ctime(&current);
        time.erase(time.end() - 14, time.end() - 5);

        time.erase(std::remove(time.begin(),time.end(),'\n'),time.end());
        return  time;
    }
    bool DoseUserExist(wxString usr) {
        //confirm the existance of button set files with an ifstream, by opening it and chacking if it fails
        try {
            std::ifstream setStream;
            std::string user = usr.ToStdString();
            std::string fileName = "panelLayout/panelLayout" + user + ".txt";

            setStream.open(fileName, std::fstream::in);
            if (setStream.fail()) {
                setStream.close();
                return false;
            }
            else {
                setStream.close();
                return true;
            }
            return false;
        }
        catch (...) {
            popupHandeler->errorMessage("an error occured in function helper");
        }
        return false;
    }
    bool compaireArrayToElement(std::vector<std::string> insuranceList,std::string patientInsurance) {
        for (size_t i = 0; i < insuranceList.size(); i++) {
            if (patientInsurance == insuranceList.at(i)) {
                return true;
            }
        }
        return false;
    }
    std::string leftTrim(std::string str) {
        while (isblank(str[0])) {
            str = str.substr(1, str.size());
        }
        return str;
    }
    std::string rightTrim(std::string str) {
        while (isblank(str[str.size()-1])) {
            str = str.substr(0, str.size()-1);
        }
        return str;
    }
    std::string trim(std::string strFinal) {
        return leftTrim(rightTrim(strFinal));
    }
    std::vector<std::string> checkForFileNameMatches(std::string fileName,std::vector<std::string> matches) {
        std::string path = "PatientFileParse/";
        std::string archivePath = "PatientFileParse/Archive/";
        std::string listFileName = "";
        std::string tempFileName = "";
        transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
        for (const auto& entry : fileSys::directory_iterator(path)) {
            listFileName = entry.path().stem().string();
            int pos = listFileName.find('_');
            tempFileName = listFileName;
            if (pos >= 0) {
                tempFileName.erase(tempFileName.begin() + pos, tempFileName.end());
            }
            std::string tempFileName2 = "";
            for (size_t i = 0; i < tempFileName.length(); i++) {
                if (tempFileName[i] >= 'A' && tempFileName[i] <= 'Z') {
                    tempFileName[i] = (char)(97 + (int)(tempFileName[i] - 'A'));
                }
                
            }
            tempFileName2 = tempFileName;
            if (tempFileName2.compare(fileName)==0) {
                matches.push_back(listFileName);
            }
        }
        if (matches.empty()) {
            for (const auto& entry : fileSys::directory_iterator( archivePath )) {
                std::string listFolderName = entry.path().stem().string();
                std::string tempFolderName = listFolderName;
                std::vector<std::string> patientByDates;
                std::transform(tempFolderName.begin(), tempFolderName.end(), tempFolderName.begin(), ::tolower);
                if (fileName == tempFolderName) {
                    std::string archivePatientPath = archivePath + listFolderName + '/';
                    for (const auto& entry2 : fileSys::directory_iterator(archivePatientPath)) {
                        listFileName = entry2.path().stem().string();

                            matches.push_back(listFileName);
                    }
                    matches.push_back("Archive");
                }
            }
        }
        return matches;
    }
    std::vector<std::string> gatherPatientFolders() {
        std::string listFileName = "";
        std::string tempFileName = "";
        std::vector<std::string> results;
        for (const auto& entry : fileSys::directory_iterator("PatientFileParse/Archive/")) {
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
    std::string findLatestFileName(std::vector<std::string> Patients) {
        std::string currentDate = getDateToSignNoTime();
        currentDate.erase(currentDate.begin(), currentDate.begin()+4);
        std::vector<int> dayDiferances;
        int pos=0;
        for (size_t i = 0; i < Patients.size(); i++) {
            std::string tempPatient = Patients.at(i);
            pos = tempPatient.find('_');
            tempPatient.erase(tempPatient.begin(), tempPatient.begin()+pos+1);
            tempPatient.erase(tempPatient.end()-4, tempPatient.end());
            dayDiferances.push_back(getDayDifferance(intdateformater(tempPatient), intdateformater(currentDate)));
        }
        int index= std::distance(std::begin(dayDiferances),std::min_element(std::begin(dayDiferances),std::end(dayDiferances)));
        return Patients.at(index);
    }
    int getDayDifferance(date init, date last) {
        try {
            long int n1 = init.y * 365 + init.d;

            for (int i = 0; i < init.m - 1; i++) {
                n1 += months[i];
            }
            n1 += countLeapYears(init);

            long int n2 = last.y * 365 + last.d;
            for (int i = 0; i < last.m - 1; i++) {
                n2 += months[i];
            }
            n2 += countLeapYears(last);
            return n2 - n1;
        }
        catch (...) {
            popupHandeler->errorMessage("an error occured in the document creator-day Method");

        }
        return -1;
    }
    date intdateformater(std::string dt) {
        try {
            size_t position = 0;
            int i = 0;
            std::vector<std::string> tokens;
            while ((position = dt.find(" ")) != std::string::npos) {
                
                tokens.push_back(dt.substr(0, position));
                dt = dt.substr(position + 1, dt.size());
                i++;
            }
            tokens.push_back(dt);
            if (tokens.size()==4) {
                int mon = 0;
                std::transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), [](unsigned char c) {return std::tolower(c); });
                if (tokens[1] == "jan") {
                    mon = 1;
                }
                if (tokens[1] == "feb") {
                    mon = 2;
                }
                if (tokens[1] == "mar") {
                    mon = 3;
                }
                if (tokens[1] == "apr") {
                    mon = 4;
                }
                if (tokens[1] == "may") {
                    mon = 5;
                }
                if (tokens[1] == "jun") {
                    mon = 6;
                }
                if (tokens[1] == "jul") {
                    mon = 7;
                }
                if (tokens[1] == "aug") {
                    mon = 8;
                }
                if (tokens[1] == "sep") {
                    mon = 9;
                }
                if (tokens[1] == "oct") {
                    mon = 10;
                }
                if (tokens[1] == "nov") {
                    mon = 11;
                }
                if (tokens[1] == "dec") {
                    mon = 12;
                }
                int day = stoi(tokens[2]);
                int year = stoi(tokens[3]);
                return { day,mon,year };
            }
            else if(tokens.size() == 3){
                int mon = 0;
                std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), [](unsigned char c) {return std::tolower(c); });
                if (tokens[0] == "jan") {
                    mon = 1;
                }
                if (tokens[0] == "feb") {
                    mon = 2;
                }
                if (tokens[0] == "mar") {
                    mon = 3;
                }
                if (tokens[0] == "apr") {
                    mon = 4;
                }
                if (tokens[0] == "may") {
                    mon = 5;
                }
                if (tokens[0] == "jun") {
                    mon = 6;
                }
                if (tokens[0] == "jul") {
                    mon = 7;
                }
                if (tokens[0] == "aug") {
                    mon = 8;
                }
                if (tokens[0] == "sep") {
                    mon = 9;
                }
                if (tokens[0] == "oct") {
                    mon = 10;
                }
                if (tokens[0] == "nov") {
                    mon = 11;
                }
                if (tokens[0] == "dec") {
                    mon = 12;
                }
                int day = stoi(tokens[1]);
                int year = stoi(tokens[2]);
                return { day,mon,year };
            }
        }
        catch (...) {
            popupHandeler->errorMessage("an error occured in the document creator-date formatter");

        }
        return { 0,0,0 };
    }
    int countLeapYears(date dt) {
        try {
            int years = dt.y;
            if (dt.m <= 2) {
                years--;
            }
            return (years / 4) - (years / 100) + (years / 400);
        }
        catch (...) {
            popupHandeler->errorMessage("an error occured in the document creator-years");
        }
    }
};