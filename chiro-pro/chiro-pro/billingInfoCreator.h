#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <sstream>
#include <algorithm> 
namespace filesystem = std::filesystem;


struct date {
	int d, m, y;
};
class billingInfoCreator {
private:
	std::fstream in_out;
	std::ofstream outMed;
	std::fstream outReg;
	std::fstream outOth;
	std::string path;
	std::string billingfile,billingfileMed,billingfileReg,billingfileOther;
	std::string information[6];
	const int months[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
public:
	billingInfoCreator(std::string dirpath,std::string billfilename,
		std::string billfileMed, std::string billgfileReg, std::string billfileOther) {
		path = dirpath;
		billingfile = billfilename;
		billingfileMed= billfileMed;
		billingfileReg= billgfileReg;
		billingfileOther= billfileOther;
	}
	std::string* gatherInfo(std::fstream &inout) {

		std::string infoline;
		std::string templine;
		while (!inout.eof()) {
			std::getline(inout, infoline);
			std::stringstream ss(infoline);
			while (getline(ss, templine,'>')) {
				std::string line = templine;
				transform(line.begin(), line.end(), line.begin(), ::toupper);
				if (line.find("PATIENT NAME") != std::string::npos) {
					information[0] = templine;
				}
				if (line.find("DATE OF FIRST VISIT") != std::string::npos) {
					information[1] = templine;
				}
				if (line.find("LATEST DATE OF VISIT") != std::string::npos) {
					information[2] = templine;
				}
				if (line.find("INSURANCE")!= std::string::npos) {
					information[5] = templine;
				}
				if (line.find("<<") != std::string::npos) {
				
						information[4] += templine + "\n";
					
				}
			}
		}
		cleanLines();
		return information;
	}
	void documentCreation() {
		in_out.open(billingfile,std::ios::out,std::ios::trunc);
		in_out.close();
		outMed.open(billingfileMed, std::ios::out, std::ios::trunc);
		outMed << "Medicare \n";
		outMed << "--------------------------------------------------------------------------------------------------------------------------------\n";
		outMed << "Patient\t\t\tDate of Service\t\tCPT Code [Modifier]\t\tDx Codes\t\t\tInitial Date of Tx\t\tDays\n";
		outMed.close();
		outReg.open(billingfileReg, std::ios::out, std::ios::trunc);
		outReg << "Regence \n";
		outReg << "--------------------------------------------------------------------------------------------------------------------------------\n";
		outReg << "Patient\t\t\tDate of Service\t\tCPT Code [Modifier]\t\tDx Codes\t\t\tInitial Date of Tx\t\tDays\n";
		outReg.close();
		outOth.open(billingfileOther, std::ios::out, std::ios::trunc);
		outOth << "Other \n";
		outOth << "--------------------------------------------------------------------------------------------------------------------------------\n";
		outOth << "Patient\t\t\tDate of Service\t\tCPT Code [Modifier]\t\tDx Codes\t\t\tInitial Date of Tx\t\tDays\n";
		outOth.close();

		for (const auto& entry : filesystem::directory_iterator(path)) {
			
			in_out.open(entry);
			gatherInfo(in_out);
			in_out.close();
			std::transform(information[5].begin(), information[5].end(), information[5].begin(), ::toupper);
			if (information[5].find("MEDICARE")!=std::string::npos) {
				seperateCptCodes(information[4]);
				outMed.open(billingfileMed, std::ios::app);

				outMed << information[0] << "\t\t";
				outMed << information[2] << "\t\t";
				int pos =rearangeDxCodes(0);
				
				outMed << information[3] << "\t\t\t\t";
				pos += 8;
				outMed << information[4] << "\t";
				outMed.seekp(pos);
				outMed << information[1] << "\t\t\t";
				int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
				outMed << days << "\n";
				outMed.close();
			}
			if (information[5].find("REGENCE") != std::string::npos) {
				seperateCptCodes(information[4]);
				outReg.open(billingfileReg, std::ios::app);

				outReg << information[0] << "\t\t";
				outReg << information[2] << "\t\t";
				outReg << information[3] << "\t\t\t\t";
				outReg << information[4] << "\t";
				outReg << information[1] << "\t\t\t";
				int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
				outReg << days << "\n";
				outReg.close();
			}
			if (information[5].find("OTHER") != std::string::npos) {
				seperateCptCodes(information[4]);
				outOth.open(billingfileOther, std::ios::app);

				outOth << information[0] << "\t\t";
				outOth << information[2] << "\t\t";
				outOth << information[3] << "\t\t\t\t";
				outOth << information[4] << "\t";
				outOth << information[1] << "\t\t\t";
				int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
				outOth << days << "\n";
				outOth.close();
			}

		}
		appendFiles();
	}
	date intdateformater(std::string dt) {
		size_t position=0;
		int i = 0;
		std::string tokens[4];
		while ((position = dt.find(" ")) != std::string::npos) {
			tokens[i] = dt.substr(0, position);
			dt= dt.substr(position+1,dt.size());
			i++;
		}
		tokens[3] = dt;
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
		return {day,mon,year};

	}
	void cleanLines() {
		size_t position = 0;
		size_t position2 = 0;
		std::string templine;
		//std::string tokens[5];
		for (int i = 0; i < 5; i++) {
			if (( information[i].find("<<")) != std::string::npos) {
				if (( information[i].find("\t")) != std::string::npos) {
					std::stringstream ss(information[i]);
					information[i] = "";
					while (getline(ss, templine, '\n')) {
						if ((position = templine.find("<<")) != std::string::npos) {
							if ((position2 = templine.find("\t")) != std::string::npos) {

								templine = templine.substr(position + 1, position2);
								if ((position2 = templine.find("\t")) != std::string::npos) {
									information[i] += templine.substr(0, position2)+",";
									information[i].erase(std::remove(information[i].begin(), information[i].end(), '<'), information[i].end());
									information[i].erase(std::remove(information[i].begin(), information[i].end(), '\t'), information[i].end());
								}
							}
						}
					}
				}
			}
			
			
			
			if ((position = information[i].find('<')) != std::string::npos) {
				information[i] = information[i].substr(position + 1, information[i].size());


				
			}
		}

	}


	bool isNumber(std::string str) {
		std::string::const_iterator it = str.begin();
		while (it != str.end() && std::isdigit(*it))
		it++;
		return !str.empty() && it == str.end();
	}
	int getDayDifferance(date init,date last) {

		long int n1 = init.y * 365 + init.d;

		for (int i = 0; i < init.m - 1;i++) {
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
	int countLeapYears(date dt) {
		int years = dt.y;
		if (dt.m <= 2) {
			years--;
		}
		return (years / 4) - (years / 100) + (years / 400);
	}
	void seperateCptCodes(std::string str) {
		std::stringstream ss(str);
		information[3] = "";
		std::string templine;
		while (getline(ss, templine, ',')) {
			if (isNumber(templine)) {
				size_t pos = information[4].find(templine);
				information[4].erase(pos, templine.size()+1);
				information[3] += templine;
			}
		}
	}
	void printDxCodes(std::fstream out) {////needs work
		std::stringstream ss(information[4]);
		std::string templine;
		int Dxcount = 0,iterationCount=0;
		while (getline(ss, templine, ',')) {
			Dxcount++;
		}
		if (Dxcount == 1) {
			out << information[4] << "\t\t\t";
		}
		if (Dxcount == 2) {
			out << information[4] << "\t\t";
		}
		if (Dxcount > 2) {
			while (getline(ss, templine, ',')) {
				
				if ((iterationCount % 3) == 0) {
					out << templine << "\n";
				}
				else {
					out << templine;
				}
				iterationCount++;
			}
		}
		
	}
	void appendFiles() {
		in_out.open(billingfile);
		std::ifstream Medicare;
		std::ifstream Regence;
		Medicare.open(billingfileMed);
		Regence.open(billingfileReg);
		in_out << Medicare.rdbuf() <<"\n"<< Regence.rdbuf();
	}

	int rearangeDxCodes(int type) {
		size_t NumberOfCommas = std::count(information[4].begin(), information[4].end(),',');
		size_t ofsett=0;
		std::stringstream ss(information[4]);
		information[4] = "";
		std::string templine;
		int position;
		int i = 0;
		int test = static_cast<int>(NumberOfCommas);
		while (getline(ss, templine, ',')) {
		
			information[4]+= templine + ",";
			ofsett += templine.size() + 1;
			if (i % 2 != 0) {
				information[4] += "\n\t\t\t\t\t\t\t\t\t\t";
			}
			if (i == 1&&type ==0) {
				int x =static_cast<int>(ofsett);
				position = outMed.tellp();
				position += x;
			}
			i ++;
		}
		return position;
	}

	//for (size_t i = 0; i < NumberOfCommas; i++) {
};

