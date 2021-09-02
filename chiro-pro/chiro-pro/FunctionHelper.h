#pragma once
#include <string>
#include <iostream>
#include <fstream>
class FuncHelper {

public:
	FuncHelper() {

	}
    bool isPopup(const std::string& str) {
        return (str.find("Dialog-ID")!= std::string::npos);
    }
	bool isNumber(const std::string& str) {
		return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) {return !std::isdigit(c); }) == str.end();
	}
    bool isText(const std::string& str) {
        return !(str.find("Dialog-ID") != std::string::npos)&&!str.empty();
    }

    long getLineNo(std::string& str, std::string delim) {

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

    }
};