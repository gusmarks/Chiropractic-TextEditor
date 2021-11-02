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
    DialogHelper* popupHandeler;
    std::string BlankSpace = " \n\t\r\f\v";
public:
    bool isPopup(const std::string& str) {
        return (str.find("Dialog-ID")!= std::string::npos);
    }
	bool isNumber(const std::string& str) {
		return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) {return !std::isdigit(c); }) == str.end();
	}
    bool isText(const std::string& str) {
        return !(str.find("Dialog-ID") != std::string::npos)&&!str.empty();
    }
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
};