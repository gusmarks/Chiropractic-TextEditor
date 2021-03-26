#pragma once
#include <string>
class FuncHelper {

public:
	FuncHelper() {

	}
	bool isNumber(const std::string& str) {
		return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) {return !std::isdigit(c); }) == str.end();
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
   // std::string getDateToSign() {
     //   time_t current;
      //  time(&current);

       // return  ctime(&current);
    //}
};