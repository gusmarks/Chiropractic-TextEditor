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

//a struct to hold the information of a date in time, D,M,Y
struct date {
	int d, m, y;
};
class billingInfoCreator {
	//streams for the input and output of files, and strings to represent the paths for each section
private:
	std::fstream in_out;
	std::ofstream outMed;
	std::fstream outCom;
	std::fstream outAuto;
	std::fstream outLni;
	std::fstream insuranceMedicareStream;
	std::fstream insuranceCommStream;
	std::fstream insuranceAutoStream;
	std::fstream insuranceLnIStream;
	std::vector<std::string>insuranceMedicare;
	std::vector<std::string>insuranceComm;
	std::vector<std::string>insuranceAuto;
	std::vector<std::string>insuranceLnI;
	std::string path;
	std::string billingfile,billingfileMed,billingfileComm,billingfileAuto, billingfileLnI;
	std::string information[6];
	const int months[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	DialogHelper* popupHandeler;
	FuncHelper* funcHelp;
public:
	//constructor for the infor-Creator assigns all paths to variables
	billingInfoCreator(std::string dirpath,std::string billfilename,
		std::string billfileMed, std::string billgfileCom, std::string billfileAuto, std::string billfileLnI) {
		try {
			path = dirpath;
			billingfile = billfilename;
			billingfileMed = billfileMed;
			billingfileComm = billgfileCom;
			billingfileAuto = billfileAuto;
			billingfileLnI = billfileLnI;
			funcHelp = new FuncHelper();
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured making the billing creator");
		}
	}
	//this function gathers the information for the main billing documents from patient files.
	std::string* gatherInfo(std::fstream& inout) {
		try {
			for (int i = 0; i < (sizeof(information) / sizeof(information[0])); i++) {
				information[i].clear();
			}
			information->clear();
			std::string infoline;
			std::string templine;
			if (inout.is_open()) {
				while (!inout.eof()) {
					std::getline(inout, infoline);
					std::stringstream ss(infoline);
					while (getline(ss, templine, '>')) {
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
						if (line.find("INSURANCE") != std::string::npos) {
							information[5] = templine;
						}
						if (line.find("<<") != std::string::npos) {

							information[4] += templine + "\n";

						}
					}
				}
			}
			else {
				popupHandeler->errorMessage("Information collector could not open patient file.");
			}
			cleanLines();
			return information;
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in cathering patient information.");
			
		}
		return information;
	}
	void GatherInsurance() {
		insuranceAutoStream.open("DialogInformation/InsuranceAuto.txt");
		if (insuranceAutoStream.is_open()) {
			int i = 0;
			std::string str;
			while (std::getline(insuranceAutoStream, str)) {
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				insuranceAuto.push_back(str);
				i++;
			}
			insuranceAutoStream.close();
		}
		insuranceMedicareStream.open("DialogInformation/InsuranceMedicare.txt");
		if (insuranceMedicareStream.is_open()) {
			int i = 0;
			std::string str;
			while (std::getline(insuranceMedicareStream, str)) {
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				insuranceMedicare.push_back(str);
				i++;
			}
			insuranceMedicareStream.close();
		}
		insuranceCommStream.open("DialogInformation/InsuranceCommercial.txt");
		if (insuranceCommStream.is_open()) {
			int i = 0;
			std::string str;
			while (std::getline(insuranceCommStream,str)) {
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				insuranceComm.push_back(str);
				i++;
			}
			insuranceCommStream.close();
		}
		insuranceLnIStream.open("DialogInformation/InsuranceLnI.txt");
		if (insuranceLnIStream.is_open()) {
			int i = 0;
			std::string str;
			while (std::getline(insuranceLnIStream, str)) {
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				insuranceLnI.push_back(str);
				i++;
			}
			insuranceLnIStream.close();
		}
	}
	//thisfuction is ment to take the information gathered in the previous function and make 3 seperate billing files
	void documentCreation() {
		try {
			
			in_out.open(billingfile, std::ios::out, std::ios::trunc);
			in_out.close();
			outMed.open(billingfileMed, std::ios::out, std::ios::trunc);
			if (outMed.is_open()) {
				outMed << "Medicare Inssurance  \n";
				outMed << "--------------------------------------------------------------------------------------------------------------------------------\n";
				outMed << "Patient\t\t\t\tInsurance \t\t\t\tDate of Service\t\tInitial Date of Tx\tDays\tCPT Code [Modifier]\tDx Codes\n";
			}
			outMed.close();
			outCom.open(billingfileComm, std::ios::out, std::ios::trunc);
			if (outCom.is_open()) {
				outCom << "Commercial Inssurance  \n";
				outCom << "--------------------------------------------------------------------------------------------------------------------------------\n";
				outCom << "Patient\t\t\t\tInsurance \t\t\t\tDate of Service\t\tInitial Date of Tx\tDays\tCPT Code [Modifier]\tDx Codes\n";
			}
			outCom.close();
			outAuto.open(billingfileAuto, std::ios::out, std::ios::trunc);
			if (outAuto.is_open()) {
				outAuto << "Auto Inssurance  \n";
				outAuto << "--------------------------------------------------------------------------------------------------------------------------------\n";
				outAuto << "Patient\t\t\t\tInsurance \t\t\t\tDate of Service\t\tInitial Date of Tx\tDays\tCPT Code [Modifier]\tDx Codes\n";
			}
			outAuto.close();
			outLni.open(billingfileLnI, std::ios::out, std::ios::trunc);
			if (outLni.is_open()) {
				outLni << "LNI Inssurance  \n";
				outLni << "--------------------------------------------------------------------------------------------------------------------------------\n";
				outLni << "Patient\t\t\t\tInsurance \t\t\t\tDate of Service\t\tInitial Date of Tx\tDays\tCPT Code [Modifier]\tDx Codes\n";
			}
			outLni.close();

			for (const auto& entry : filesystem::directory_iterator(path)) {
				//gather the information
				in_out.open(entry);
				if (in_out.is_open()) {
					gatherInfo(in_out);
				}
				GatherInsurance();
				in_out.close();
				size_t position = information[5].find('<');
				if (position !=std::string::npos) {
					information[5] = information[5].erase(0, position+1);
				}
				//turn information 5 into uppercase letters
				std::transform(information[5].begin(), information[5].end(), information[5].begin(), ::toupper);
				if (funcHelp->compaireArrayToElement(insuranceMedicare,information[5])) {
					seperateCptCodes(information[4]);
					outMed.open(billingfileMed, std::ios::app);
					if (outMed.is_open()) {
						controlSpacingInNameAndInsurance();
						outMed << information[0];
						outMed << information[5];
						outMed << information[1] << "\t\t";
						outMed << information[2] << "\t\t";
						int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
						outMed << days << "\t";
						int pos = rearangeDxCodes(0);
						outMed << information[3] << "\t\t";
						pos += 8;
						outMed.seekp(pos);
						outMed << information[4];

					}
					outMed.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceComm,information[5])) {
					seperateCptCodes(information[4]);
					outCom.open(billingfileComm, std::ios::app);
					if (outCom.is_open()) {
						controlSpacingInNameAndInsurance();
						outCom << information[0];
						outCom << information[5];
						outCom << information[1] << "\t\t";
						outCom << information[2] << "\t\t";
						int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
						outCom << days << "\t";
						int pos = rearangeDxCodes(0);
						outCom << information[3] << "\t\t";
						pos += 8;
						outCom.seekp(pos);
						outCom << information[4];

					}
					outCom.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceAuto, information[5])) {
					seperateCptCodes(information[4]);
					outAuto.open(billingfileAuto, std::ios::app);
					if (outAuto.is_open()) {
						controlSpacingInNameAndInsurance();
						outAuto << information[0];
						outAuto << information[5];
						outAuto << information[1] << "\t\t";
						outAuto << information[2] << "\t\t";
						int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
						outAuto << days << "\t";
						int pos = rearangeDxCodes(0);
						outAuto << information[3] << "\t\t";
						pos += 8;
						outAuto.seekp(pos);
						outAuto << information[4] ;
					}
					outAuto.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceLnI, information[5])) {
					seperateCptCodes(information[4]);
					outLni.open(billingfileLnI, std::ios::app);
					if (outLni.is_open()) {
						controlSpacingInNameAndInsurance();
						outLni << information[0];
						outLni << information[5];
						outLni << information[1] << "\t\t";
						outLni << information[2] << "\t\t";
						int days = getDayDifferance(intdateformater(information[1]), intdateformater(information[2]));
						outLni << days << "\t";
						int pos = rearangeDxCodes(0);
						outLni << information[3] << "\t\t";
						pos += 8;
						outLni.seekp(pos);
						outLni << information[4];
					}
					outLni.close();
				}

			}
			appendFiles();
			popupHandeler->Message("Billing Document Created. find in files");
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator");
		}
	}
	date intdateformater(std::string dt) {
		try {
			size_t position = 0;
			int i = 0;
			std::string tokens[4];
			while ((position = dt.find(" ")) != std::string::npos) {
				if (i == 2) { 
					dt.erase(dt.begin());
					position = dt.find(" ");
				}
				tokens[i] = dt.substr(0, position);
				dt = dt.substr(position + 1, dt.size());
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
			return { day,mon,year };
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-date formatter");
			
		}
		return { 0,0,0 };
	}
	void cleanLines() {
		size_t position = 0;
		size_t position2 = 0;
		std::string templine;
		try {
			for (int i = 0; i < 5; i++) {
				if ((information[i].find("<<")) != std::string::npos) {
					if ((information[i].find("\t")) != std::string::npos) {
						std::stringstream ss(information[i]);
						information[i] = "";
						while (getline(ss, templine, '\n')) {
							if ((position = templine.find("<<")) != std::string::npos) {
								if ((position2 = templine.find("\t")) != std::string::npos) {

									templine = templine.substr(position + 1, position2);
									if ((position2 = templine.find("\t")) != std::string::npos) {
										information[i] += templine.substr(0, position2) + ",";
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
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-line cleaner");
		}
	}

	bool isNumber(std::string str) {
		try {	
			int position;
			if (position = str.find(' ') != std::string::npos) {
				str.erase(position);
			}
		std::string::const_iterator it = str.begin();
		while (it != str.end() && std::isdigit(*it))
			it++;
		return !str.empty() && it == str.end();
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-number validator");
		}
		return false;
	}
	int getDayDifferance(date init,date last) {
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
	void seperateCptCodes(std::string str) {
		try {
			std::stringstream ss(str);
			information[3] = "";
			std::string templine;
			while (getline(ss, templine, ',')) {
				if (isNumber(templine)) {
					size_t pos = information[4].find(templine);
					information[4].erase(pos, templine.size() + 1);
					information[3] += templine;
				}
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-seperator");
		}
	}
	void printDxCodes(std::fstream out) {
		try {
			std::stringstream ss(information[4]);
			std::string templine;
			int Dxcount = 0, iterationCount = 0;
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
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-print method");
		}
		
	}
	void appendFiles() {
		try {
			in_out.open(billingfile);
			std::ifstream Medicare;
			std::ifstream Comercial;
			std::ifstream Auto;
			std::ifstream LnI;
			Medicare.open(billingfileMed);
			Comercial.open(billingfileComm);
			Auto.open(billingfileAuto);
			LnI.open(billingfileLnI);
			in_out << Medicare.rdbuf() << "\n" << Comercial.rdbuf()<<"\n"<<Auto.rdbuf()<<"\n"<<LnI.rdbuf();
			Medicare.close();
			Comercial.close();
			Auto.close();
			LnI.close();
			in_out.close();
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-append method");
		}
	}

	int rearangeDxCodes(int type) {
		try {
			size_t NumberOfCommas = std::count(information[4].begin(), information[4].end(), ',');
			size_t ofsett = 0;
			std::stringstream ss(information[4]);
			information[4] = "";
			std::string templine;
			int position=0;
			int i = 0;
			int test = static_cast<int>(NumberOfCommas);
			while (getline(ss, templine, ',')) {

				information[4] += templine + ",";
				ofsett += templine.size() + 1;
				if (i % 2 != 0) {
					information[4] += "\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
				}
				if (i == 1 && type == 0) {
					int x = static_cast<int>(ofsett);
					position = outMed.tellp();
					position += x;
				}
				if (i == 1 && type == 1) {
					int x = static_cast<int>(ofsett);
					position = outMed.tellp();
					position += x;
				}
				if (i == 1 && type == 2) {
					int x = static_cast<int>(ofsett);
					position = outMed.tellp();
					position += x;
				}
				i++;
			}
			information[4] += "\n";
			return position;
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-rearange dx");
			return NULL;
		}
	}
	void controlSpacingInNameAndInsurance() {
		if (information[0].size() > 24 && information[0].size() <= 32) {
			information[0] += "\t";
		}
		if (information[0].size() >= 16 && information[0].size() <= 24) {
			information[0] += "\t\t";
		}
		if (information[0].size() > 8 && information[0].size() < 16) {
			information[0] += "\t\t\t";
		}
		if (information[0].size() <= 8) {
			information[0] += "\t\t\t\t";
		}
		if (information[5].size() > 25 && information[5].size() <= 35) {
			information[5] += "\t";
		}
		if (information[5].size() == 25) {
			information[5] += "\t\t";
		}
		if (information[5].size() >= 17 && information[5].size() <= 24) {
			information[5] += "\t\t\t";
		}
		if (information[5].size() >= 8 && information[5].size() < 17) {
			information[5] += "\t\t\t\t";
		}
		if (information[5].size() < 8) {
			information[5] += "\t\t\t\t\t";
		}
		
		
		
	}
	
};

