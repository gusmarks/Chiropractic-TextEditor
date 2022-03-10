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

class billingInfoCreator {
	//streams for the input and output of files, and strings to represent the paths for each section
private:
	std::fstream in_out, outMed, outCom, outAuto, outLni;
	std::fstream insuranceMedicareStream, insuranceCommStream, insuranceAutoStream, insuranceLnIStream;
	std::vector<std::string>insuranceMedicare, insuranceComm, insuranceAuto, insuranceLnI;
	std::string path;
	std::string billingfile, billingfileMed, billingfileComm, billingfileAuto, billingfileLnI;
	std::vector<std::vector<std::string>> information;
	DialogHelper* popupHandeler;
	FuncHelper* funcHelp;
public:
	//constructor for the infor-Creator assigns all paths to variables
	billingInfoCreator(std::string dirpath, std::string billfilename,
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
	std::vector <std::vector<std::string>>  gatherInfo(std::fstream& inout,std::string filename) {
		try {
			int pos1 = filename.find("_");
			filename.erase(filename.begin(),filename.begin() + pos1+1);
			filename.erase(filename.end()-4, filename.end());
			for (int j = 0; j < 7; j++) {
				std::vector<std::string> temp;
				information.push_back(temp);
			}
			for (size_t i = 0; i < information.size(); i++) { information[i].clear(); }
			std::string infoline;
			std::string templine;
			if (inout.is_open()) {
				while (!inout.eof()) {
					std::getline(inout, infoline);
					std::stringstream LineStream(infoline);
					while (getline(LineStream, templine, '}')) {
						std::string token;
						std::stringstream TokenStream(templine);
						int i = 0;
						while (getline(TokenStream, token, '\t')) {
							std::string line = token;
							transform(line.begin(), line.end(), line.begin(), ::toupper);
							if (line.find("PATIENT NAME") != std::string::npos) {
								information.at(0).push_back(token);
							}
							if (line.find("DATE OF FIRST VISIT") != std::string::npos) {
								information.at(1).push_back(filename);
							}
							if (line.find("LATEST DATE OF VISIT") != std::string::npos) {
								std::string date = funcHelp->getDateToSign().substr(3);
								int timePos = date.find(':');
								date.erase(date.begin() + timePos, date.begin() + timePos + 6);
								date.pop_back();
								information.at(2).push_back(date);
							}
							if (line.find("INSURANCE") != std::string::npos) {
								information.at(5).push_back(token);
							}
							if (line.find("ELECTRONICLY SIGNED BY:") != std::string::npos) {
								std::string Doctor = "";
								int docPos = token.find(':');
								token.erase(token.begin(),token.begin()+docPos+1);
								information.at(6).push_back(token);
							}
							size_t position = 0;
							if (( position = line.find("{{")) != std::string::npos) {

								information.at(4).push_back(token.substr(position +2,token.size()));
							}
							if (i % 2 == 0) { i++; }
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
				
				if (entry.is_directory()) {
					continue;
				}
				//gather the information
				in_out.open(entry);
				
				if (in_out.is_open()) {
					gatherInfo(in_out, entry.path().stem().string());
				}
				if (information.at(2).empty()) {
					continue;
				}
				GatherInsurance();
				in_out.close();
				size_t position = information.at(5).at(0).find('{');
				if (position !=std::string::npos) {
					information.at(5).at(0) = information.at(5).at(0).erase(0, position+1);
				}
				//turn information 5 into uppercase letters
				std::transform(information.at(5).at(0).begin(), information.at(5).at(0).end(), information.at(5).at(0).begin(), ::toupper);
				if (funcHelp->compaireArrayToElement(insuranceMedicare,information.at(5).at(0))) {
					seperateCptCodes(information[4]);
					outMed.open(billingfileMed, std::ios::app);
					if (outMed.is_open()) {
						controlSpacingInNameAndInsurance();
						int largersize = 0;
						rearangeCptCodes(0);
						rearangeDxCodes(0);
						if (information.at(3).size() >= information.at(4).size()) {
							largersize = information.at(3).size();
						}
						else {
							largersize = information.at(4).size();
						}
						int i = 0;
						do{
							if (i == 0) {
								outMed << information.at(0).at(0);
								outMed << information.at(5).at(0);
								outMed << information.at(1).at(0) << "\t\t";
								outMed << information.at(2).at(0) << "\t\t";
								int days = funcHelp->getDayDifferance(funcHelp->intdateformater(information.at(1).at(0)), funcHelp->intdateformater(information.at(2).at(0)));
								outMed << days << "\t";
							}
							else {
								outMed << "\t\t\t\t";
								outMed << "\t\t\t\t\t";
								outMed << "\t\t\t";
								outMed << "\t\t\t\t";
							}
							if ((size_t)i < information.at(3).size()) {
								outMed << information.at(3).at(i) + "\t";
								if (information.at(3).at(i).size() < 7) {
									outMed << "\t\t";
								}
								if (information.at(3).at(i).size() >= 7 && information.at(3).at(i).size() <= 10) {
									outMed << "\t";
								}
							}
							else {
								outMed << "\t\t\t";
							}
							if ((size_t)i < information.at(4).size()) {
								outMed << information.at(4).at(i);
							}
							else {
								outMed << "\t\t";
							}
							i++;
						} while (i<largersize);
						outMed << "\t" + information.at(6).at(0)+"\n";
					}

					outMed.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceComm,information.at(5).at(0))) {
					seperateCptCodes(information[4]);
					outCom.open(billingfileComm, std::ios::app);
					if (outCom.is_open()) {
						controlSpacingInNameAndInsurance();
						int largersize = 0;
						rearangeCptCodes(0);
						rearangeDxCodes(0);
						if (information.at(3).size() >= information.at(4).size()) {
							largersize = information.at(3).size();
						}
						else {
							largersize = information.at(4).size();
						}
						int i = 0;
						do{
							if (i == 0) {
								outCom << information.at(0).at(0);
								outCom << information.at(5).at(0);
								outCom << information.at(1).at(0) << "\t\t";
								outCom << information.at(2).at(0) << "\t\t";
								
								int days = funcHelp->getDayDifferance(funcHelp->intdateformater(information.at(1).at(0)), funcHelp->intdateformater(information.at(2).at(0)));
								outCom << days << "\t";
							}
							else {
								outCom << "\t\t\t\t";
								outCom << "\t\t\t\t\t";
								outCom << "\t\t\t";
								outCom << "\t\t\t\t";
							}
							if ((size_t)i < information.at(3).size()) {
								outCom << information.at(3).at(i) + "\t";
								if (information.at(3).at(i).size() < 7) {
									outCom << "\t\t";
								}
								if (information.at(3).at(i).size() >= 7 && information.at(3).at(i).size() <= 10) {
									outCom << "\t";
								}
							}
							else {
								outCom << "\t\t\t";
							}
							if ((size_t)i < information.at(4).size()) {
								outCom << information.at(4).at(i);
							}
							else {
								outCom << "\t\t";
							}
							i++;
						} while (i<largersize);
						outCom << "\t" + information.at(6).at(0) + "\n";
					}

					outCom.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceAuto, information.at(5).at(0))) {
					seperateCptCodes(information[4]);
					outAuto.open(billingfileAuto, std::ios::app);
					if (outAuto.is_open()) {
						controlSpacingInNameAndInsurance();
						int largersize = 0;
						rearangeCptCodes(0);
						rearangeDxCodes(0);
						if (information.at(3).size() >= information.at(4).size()) {
							largersize = information.at(3).size();
						}
						else {
							largersize = information.at(4).size();
						}
						int i = 0;
						do{
							if (i == 0) {
								outAuto << information.at(0).at(0);
								outAuto << information.at(5).at(0);
								outAuto << information.at(1).at(0) << "\t\t";
								outAuto << information.at(2).at(0) << "\t\t";
								int days = funcHelp->getDayDifferance(funcHelp->intdateformater(information.at(1).at(0)), funcHelp->intdateformater(information.at(2).at(0)));
								outAuto << days << "\t";
								
							}
							else {
								outAuto << "\t\t\t\t";
								outAuto << "\t\t\t\t\t";
								outAuto << "\t\t\t";
								outAuto << "\t\t\t\t";
							}
							if ((size_t)i < information.at(3).size()) {
								outAuto << information.at(3).at(i) + "\t";
								if (information.at(3).at(i).size() < 7) {
									outAuto << "\t\t";
								}
								if (information.at(3).at(i).size() >= 7 && information.at(3).at(i).size() <= 10) {
									outAuto << "\t";
								}
							}
							else {
								outAuto << "\t\t\t";
							}
							if ((size_t)i < information.at(4).size()) {
								outAuto << information.at(4).at(i);
							}
							else {
								outAuto << "\t\t";
							}
							i++;
						} while (i < largersize);
						outAuto << "\t" + information.at(6).at(0) + "\n";
					}
					
					outAuto.close();
				}
				if (funcHelp->compaireArrayToElement(insuranceLnI, information.at(5).at(0))) {
					seperateCptCodes(information[4]);
					outLni.open(billingfileLnI, std::ios::app);
					if (outLni.is_open()) {
						controlSpacingInNameAndInsurance();
						int largersize = 0;
						rearangeCptCodes(0);
						rearangeDxCodes(0);
						if (information.at(3).size() >= information.at(4).size()) {
							largersize = information.at(3).size();
						}
						else {
							largersize = information.at(4).size();
						}
						int i = 0;
						do{
							if (i == 0) {
								outLni << information.at(0).at(0);
								outLni << information.at(5).at(0);
								outLni << information.at(1).at(0) << "\t\t";
								outLni << information.at(2).at(0) << "\t\t";
								int days = funcHelp->getDayDifferance(funcHelp->intdateformater(information.at(1).at(0)), funcHelp->intdateformater(information.at(2).at(0)));
								outLni << days << "\t";
								
							}
							else {
								outLni << "\t\t\t\t";
								outLni << "\t\t\t\t\t";
								outLni << "\t\t\t";
								outLni << "\t\t\t\t";
							}
							if ((size_t)i < information.at(3).size()) {
								outLni << information.at(3).at(i)+"\t";
								if (information.at(3).at(i).size() <7) {
									outLni << "\t\t";
								}
								if (information.at(3).at(i).size() >= 7&& information.at(3).at(i).size()<=10) {
									outLni << "\t";
								}
							}
							else {
								outLni << "\t\t\t";
							}
							if ((size_t)i < information.at(4).size()) {
								outLni << information.at(4).at(i);
							}
							else {
								outLni << "\t\t";
							}
							i++;
						} while (i < largersize);
						outLni << "\t" + information.at(6).at(0) + "\n";
					}
					outLni.close();
				}
				information.clear();
			}
			
			appendFiles();
			popupHandeler->Message("Billing Document Created. find in files");
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-Doc");
		}
	}
	void cleanLines() {
		size_t position = 0;
		size_t position2 = 0;
		std::string templine;
		try {
			for (size_t i = 0; i < information.size(); i++) {
				for (size_t j = 0; j < information.at(i).size(); j++) {
					if ((information.at(i).at(j).find("{{")) != std::string::npos) {
						std::stringstream ss(information.at(i).at(j));
						information.at(i).at(j) = "";
						while (getline(ss, templine, '{')) {
							if (templine != "") {
								information.at(i).at(j) += templine;
							}
						}
					}
					if ((position = information.at(i).at(j).find('{')) != std::string::npos) {
						information.at(i).at(j) = information.at(i).at(j).substr(position + 1, information.at(i).at(j).size());
					}
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
	void seperateCptCodes(std::vector<std::string> vec) {
		try {
			int count = 0;
			for (size_t i = 0; i < vec.size(); i++) {
				std::string str = vec.at(i);
				
					if (str.find('[') != std::string::npos || isNumber(str)) {
						information.at(4).erase(information.at(4).begin()+(count));
						count--;
						information.at(3).push_back(str+",");
					}
					count++;
			}
		
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-seperator");
		}
	}
	void appendFiles() {
		try {
			std::ofstream mainBillingFile;
			mainBillingFile.open(billingfile);
			if (mainBillingFile.is_open()) {
				std::fstream Medicare;
				std::fstream Comercial;
				std::fstream Auto;
				std::fstream LnI;
				std::string tempstr;
				Medicare.open(billingfileMed, std::ios::in);
				Comercial.open(billingfileComm, std::ios::in);
				Auto.open(billingfileAuto, std::ios::in);
				LnI.open(billingfileLnI, std::ios::in);
				if (Medicare.is_open() && Comercial.is_open() && Auto.is_open() && LnI.is_open()) {
					while (getline(Medicare, tempstr)) {
						mainBillingFile << tempstr;
						mainBillingFile << "\n";
					}
					mainBillingFile << "\n";
					while (getline(Comercial, tempstr)) {
						mainBillingFile << tempstr;
						mainBillingFile << "\n";
					}
					mainBillingFile << "\n";
					while (getline(Auto, tempstr)) {
						mainBillingFile << tempstr;
						mainBillingFile << "\n";
					}
					mainBillingFile << "\n";
					while (getline(LnI, tempstr)) {
						mainBillingFile << tempstr;
						mainBillingFile << "\n";
					}
					
					
					//in_out << Medicare.rdbuf() << "\n" << Comercial.rdbuf() << "\n" << Auto.rdbuf() << "\n" << LnI.rdbuf();
					Medicare.close();
					Comercial.close();
					Auto.close();
					LnI.close();
					in_out.close();
				}
			}
			else {
				popupHandeler->errorMessage("billingDocument.txt failed to open.");
					
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-append method");
		}
	}
	void rearangeCptCodes(int type) {
		try{
			if (information.at(3).size() != 0) {
				for (size_t i = 0; i < information.at(3).size() - 1; i++) {
					information.at(3).at(i) += information.at(3).at(i + 1);
					information.at(3).erase(information.at(3).begin() + (i + 1));
				}
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-rearange cpt");
		}
	}
	void rearangeDxCodes(int type) {
		try {
			if (information.at(4).size() != 0) {
				
				for (size_t i = 0; i < information.at(4).size() - 1; i++) {
					information.at(4).at(i) += ",";
					if (i == information.at(4).size() - 2) {
						information.at(4).at(i) += information.at(4).at(i + 1) + ".";
					}
					else {
						information.at(4).at(i) += information.at(4).at(i + 1) + ",";
					}
					information.at(4).erase(information.at(4).begin() + (i + 1));
				}
			}
		}
		catch (...) {
			popupHandeler->errorMessage("an error occured in the document creator-rearange dx");
		}
	}
	void controlSpacingInNameAndInsurance() {
		if (information.at(0).at(0).size() > 24 && information.at(0).at(0).size() <= 32) {
			information.at(0).at(0) += "\t";
		}
		if (information.at(0).at(0).size() >= 16 && information.at(0).at(0).size() <= 24) {
			information.at(0).at(0) += "\t\t";
		}
		if (information.at(0).at(0).size() > 8 && information.at(0).at(0).size() < 16) {
			information.at(0).at(0) += "\t\t\t";
		}
		if (information.at(0).at(0).size() <= 8) {
			information.at(0).at(0) += "\t\t\t\t";
		}
		if (information.at(5).at(0).size() > 25 && information.at(5).at(0).size() <= 35) {
			information.at(5).at(0) += "\t";
		}
		if (information.at(5).at(0).size() == 25) {
			information.at(5).at(0) += "\t\t";
		}
		if (information.at(5).at(0).size() >= 17 && information.at(5).at(0).size() <= 24) {
			information.at(5).at(0) += "\t\t\t";
		}
		if (information.at(5).at(0).size() >= 8 && information.at(5).at(0).size() < 17) {
			information.at(5).at(0) += "\t\t\t\t";
		}
		if (information.at(5).at(0).size() < 8) {
			information.at(5).at(0) += "\t\t\t\t\t";
		}
		
		
		
	}	
};