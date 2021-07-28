#pragma once

#ifndef __Frame_H
#define __Frame_H
//includes, all wx/ includes are required by the various aspects of wxWidgets used
//all "" includes designate the custom header files used to make the application
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/richtext/richtextctrl.h>
#include "DialogHelper.h"
#include "DialogCreator.h"
#include "ButtonPanel.h"
#include "FunctionHelper.h"
#include "ButtonSet.h"
#include "buttonPanel.h"
#include "NavLink.h"
#include "LinkPanel.h"
#include "ControlPanel.h"
#include <wx/hyperlink.h>
#include <ctime>
enum
{
	TEXT_Main,
	MENU_New,
	MENU_Open,
	MENU_Close,
	MENU_Save,
	MENU_SaveAs,
	MENU_Quit,
	BUTTON_Add,
	BUTTON_Minus,
	BUTTON_Sign,
	BUTTON_Swap,
	BUTTON_Write,
	BUTTON_Dialog,
	BUTTON_Panel,
	Button_ControlPanel,
	BUTTON_Back,
	BUTTON_NewSet,
	CHOICE_SWAP_Set,
	MENU_EditButtonName,
	MENU_EditButtonText,
	MENU_RemoveButton,
	MENU_SaveButtons,
	MENU_chanegDialog,
	LINK_NAVIGATE,
	ID_FORMAT_BOLD,
	ID_FORMAT_ITALIC,
	ID_FORMAT_UNDERLINE,
	ID_FORMAT_STRIKETHROUGH,
	ID_FORMAT_SUPERSCRIPT,
	ID_FORMAT_SUBSCRIPT,
	ID_RICHTEXT_STYLE_COMBO,
	PANEL_TEXT,
	ID_FORMAT_ALIGN_LEFT,
	ID_FORMAT_ALIGN_CENTRE,
	ID_FORMAT_ALIGN_RIGHT,

};
//MainFrame extends the wxFrame class to make a window
class MainFrame : public wxFrame
{
// private variable are stored to keep them from being accessd by other means
private:
	//buttonToEdit allows the user to edit the lable and contents of the button
	wxButton* buttonToEdit;
	//this file name saves the current document being worked on
	wxString fileName;
	//the text box on the main screen, currently cant be manualy edited
	wxRichTextCtrl* mainEditBox;
	// the menu bar with save, load, etc functions
	wxMenuBar* mainMenu;
	wxToolBar* toolBar;
	// Button sets are defined by a name, this will be the primary set of buttons and panels used by a given user
	// there is a maximum of 10 user sets, recored in the ButtonSetNames array, the SetCount variable(consider wraping them togeather)
	// the ButtonSetList holds the user data once loaded, and the currentButtonSet references the set being used at any given time by the user
	wxString buttonSetNames[10] = {};
	int setCount = 0;
	std::vector<ButtonSet*> buttonSetList;
	ButtonSet* currentButtonSet;
	// user selection controls what ButtonSet is being used
	wxChoice* userSelection;
	// a set of sizers to orginize the page, ButtonSetSizer aranges the set changer and the new set button, MainSizer holds all other sizers
	//and and the text box, finaly ControlSizer aranges the loose buttons 
	wxBoxSizer* buttonSetSizer, *controlSizer, *mainSizer, *linkSizer;

	controlPanel* controls;
	linkPanel* links;
	//the DialogHelper allows for popup use, while function handeler, deals with functions that dont quite fit in the main files 
	DialogHelper* popUpHandeler;
	FuncHelper* functionHelper;
	DialogCreator* DialogCreationHelper;
	// check to see if the document has been signed or not
	bool Signed = false;
	bool isBold=false;
	// input and output streams to read txt files that store information
	std::ifstream setInfoIn;
	std::ofstream setInfoOut;
	// navlinks are hyperlinks that swap to and from panels inside a set.
	NavLink* link1, *link2, *link3;
public:
	//Constuctor for the frame
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	// function headers for event functions.
	void openFile(wxCommandEvent& event);
	void saveFile(wxCommandEvent& event);
	void saveFileAs(wxCommandEvent& event);
	void closeFile(wxCommandEvent& event); 

	void saveButtonSetInfo(std::string path);
	void loadButtonSetInfo(std::string path);
	void savePanelsAndButtons(wxCommandEvent& event);
	void addButton(wxCommandEvent& event);
	void ButtonWrite(wxCommandEvent& event);
	void swapPanels(ButtonPanel* newPanel);
	void swapHelper(wxCommandEvent& event);
	void startSwapHelper(wxCommandEvent& event);

	void swapButtonSet(wxCommandEvent& evt);
	void newSet(wxCommandEvent& event);

	void sign(wxCommandEvent& WXUNUSED(event));
	void onRightClick(wxMouseEvent& event);
	void onPopUpCLick(wxCommandEvent& event);

	void linkNavigation(wxHyperlinkEvent& evt);
	void openDialog();
	void readDialogFile(wxCommandEvent& event);
	void DialogWriter(std::vector<std::string>);

	/// <summary>
/// builds the navlinks based on the currentpanel and how many previous elements thier are
/// </summary>
/// <param name="panelToWork"></param>

	void buildNavLinks(ButtonPanel* panelToWork) {
		wxString name = panelToWork->GetName();
		switch (panelToWork->getLevel()) {
		case 0: {
			if (link1 == nullptr) {
				link1 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				
				link1->setName(name);
				link1->setPanel(panelToWork);
			}
			if (link2 != nullptr) {
				link2->Hide();
				link2 = nullptr;
			}
			if (link3 != nullptr) {
				link3->Hide();
				link3 = nullptr;
			}

			links->clearLinkSizer();
			//linksizer->Clear();
			link1->Hide();
			links->addToLinkSizer(link1);
			//linksizer->Add(link1);
			link1->Show();
			links->linkLayout();
			//linksizer->Layout();
			Layout();
			break;
		}
		case 1: {
			if (link2 == nullptr) {
				link2 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				link2->setName(panelToWork->GetName());
				link2->setPanel(panelToWork);
			}

			if (link3 != nullptr) {
				link3->Hide();
				link3 = nullptr;
			}

			links->clearLinkSizer();
			link1->Hide();
			link2->Hide();
			links->addToLinkSizer(link1);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link2);
			link1->Show();
			link2->Show();
			links->linkLayout();
			Update();
			break;
		}
		case 2: {
			if (link3 == nullptr) {
				link3 = new NavLink(links, LINK_NAVIGATE, panelToWork->GetName(), panelToWork);
			}
			else {
				link3->setName(panelToWork->GetName());
				link3->setPanel(panelToWork);
			}
			links->clearLinkSizer();
			link1->Hide();
			link2->Hide();
			link3->Hide();
			links->addToLinkSizer(link1);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link2);
			links->linklAddSpace(10, 10);
			links->addToLinkSizer(link3);
			link1->Show();
			link2->Show();
			link3->Show();
			links->linkLayout();
			Update();
			break;
		}

		}
	}

	void destroyPanels(wxString usr);

	void onBold(wxCommandEvent& WXUNUSED(event))
	{
		if (isBold == false) {
			mainEditBox->BeginBold();
			isBold = true;
		}
		else if (isBold == true) {
			mainEditBox->EndBold();
			isBold = false;
		}
			
		//mainEditBox->ApplyBoldToSelection();
	}
	void onItalic(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyItalicToSelection();
	}
	void onUnderline(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyUnderlineToSelection();
	}
	void onStrikethrough(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_STRIKETHROUGH);
	}
	void onSuperscript(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	}
	void onSubscript(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
	}
	void onAlignLeft(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_LEFT);
	}
	void onAlignCentre(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTRE);
	}
	void onAlignRight(wxCommandEvent& WXUNUSED(event))
	{
		mainEditBox->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_RIGHT);
	}


	void bindAllButtons();
	bool dialogButtonHelper();
	void dialogURLHelper(wxTextUrlEvent& evt);
	void popupButtonBind(std::string str, wxButton* btton);
	void popupButtonRebind(std::string str,wxButton* btton);
	void unbindPopupButton(wxString str, wxButton* btton);


	void chiefEandM(wxCommandEvent& WXUNUSED(event));
	void nextEandM(wxCommandEvent& WXUNUSED(event));
	void pastHealthHistory(wxCommandEvent& WXUNUSED(event));
	void dailyObjective(wxCommandEvent& WXUNUSED(event));

	int ManualgetDialogCount(std::string file) {
		std::ifstream fileReader(file);
		std::string fileCount;
		getline(fileReader, fileCount);
		if (fileCount != "") {
			return std::stoi(fileCount);
		}
		return 0;
	}

	// all of these call a pop up and write a line of text respective to the name the user selected
	void allergies(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/Allergies.txt", "Allergies");
		mainEditBox->WriteText("patients Aalergies are: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void body(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->bodyDialog("Select Locations.");
		mainEditBox->WriteText("Cervical Adjustment: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void betterorWorse(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/BetterorWorse.txt", "Improve/Decline");
		mainEditBox->WriteText("Improve/Decline: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void cervicaladjustment(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/Cervicaladjustment.txt", "Cervical Adjustment?");
		wxString spine = popUpHandeler->MultipleChoiceDialog("DialogInformation/spineSegmentsL-R.txt", "Cervical Adjustment?");
		mainEditBox->BeginBold();
		mainEditBox->WriteText("\nCervical:");
		mainEditBox->EndBold();
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();

		mainEditBox->WriteText("Adjustments to :");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(spine);
		mainEditBox->WriteText(spine);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void compicatingFactors(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/CompicatingFactors.txt", "Compicating Factors");
		mainEditBox->WriteText("Compicating Factors: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void contraindiction(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/contraindication.txt", "contraindiction");
		mainEditBox->WriteText("Contraindiction is: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void dietNutrition(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/DietNutrition.txt", "what is your diet like?");
		mainEditBox->WriteText("the patients diet is: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void dificultyPerforming(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/dificultyPerforming.txt", "Dificulty Performing");
		mainEditBox->WriteText("Dificulty Performing: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void drugsMedication(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/drugsMedication.txt", "Drugs Medication");
		mainEditBox->WriteText("Drugs Medication: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void familyHistory(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/familyHistory.txt", "Family History");
		mainEditBox->WriteText("the Patient has a family history of: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void freqofPain(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->SingleChoiceDialog("DialogInformation/FreqOfPain.txt", "Frequency of pain?");
		mainEditBox->WriteText("pain/discomfort is: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
		mainEditBox->WriteText("(100%)being the worst. \n");
	}
	void restrictions(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/restrictions.txt", "Restrictions?");
		mainEditBox->WriteText("Restrictions: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void rateofImprove(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->SingleChoiceDialog("DialogInformation/rateofImprove.txt", "what is your improvment/decline?");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void side(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->SingleChoiceDialog("DialogInformation/side.txt", "Select Side?");
		mainEditBox->WriteText("Side: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void socialhabits(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/socialhabits.txt", "what is your social habits?");
		mainEditBox->WriteText("the patients social behavior is: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void specificMuscles(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/SpecificMuseles.txt", "Select muscle groups?");
		mainEditBox->WriteText("Muscle groups: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void spineDialog(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/spineSegmentsL-R.txt", "what is your diet like?");
		mainEditBox->WriteText("the reports spinal issues in: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void surgicalHistory(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/surgicalHistory.txt", "what is your surgical history?");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void tautTender(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->bodyDialog("Taut&Tender");
		mainEditBox->WriteText("\n\tUpon inspection the following areas were found with Taut and Tender Fibers: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void exerciseRoutine(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/ExerciseRoutine.txt", "Exercise Routine");
		mainEditBox->WriteText("Exercise Routine: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void goals(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/goals.txt", "Chiropractic Goals");
		mainEditBox->WriteText("Patients Goals: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void injury(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/Injury.txt", "Injury");
		mainEditBox->WriteText("Patient injuries: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void lifeAffected(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/LifeAffected.txt", "Life Affected");
		mainEditBox->WriteText("Life affected: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void percentOrVas(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/percentorvas.txt", "Percent or VAS");
		mainEditBox->WriteText("Percent of pain or VAS: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void posturalChange(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/posturalChange.txt", "posturalChange?");
		mainEditBox->WriteText("Change in Posture: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void qualityOfDiscomfort(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/QualityOfDiscomfort.txt", "Quality of Discomfort");
		mainEditBox->WriteText("Quality of discomfort/pain is discribed as: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void ROMRegion(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/ROMregion.txt", "ROM region");
		mainEditBox->WriteText("ROM: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void toneIntensity(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/ToneIntensity.txt", "Tone Intensity");
		mainEditBox->WriteText("Tone Intensity: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void vas(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/Vas.txt", "VAS");
		mainEditBox->WriteText("VAS: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}
	void work(wxCommandEvent& WXUNUSED(event)) {
		wxString str = popUpHandeler->MultipleChoiceDialog("DialogInformation/work.txt", "Select work habits?");
		mainEditBox->WriteText("Work habits include: \n");
		mainEditBox->BeginTextColour(wxColour(52, 128, 235));
		mainEditBox->BeginURL(str);
		mainEditBox->WriteText(str);
		mainEditBox->EndURL();
		mainEditBox->EndTextColour();
	}

/// //new file method, this method clears the texteditor and filename for saving.
///allowing people to make and save new files
/// </summary>
/// <param name="WXUNUSED"></param>
	void newFile(wxCommandEvent& WXUNUSED(event))
	{
		if (popUpHandeler->confirmIntent("are you sure you want to open a new file?")) {
			wxString name=wxGetTextFromUser("Enter Patient name", " ", "", NULL, wxDefaultCoord, wxDefaultCoord, true);
			if (name == "") {
				return;
			}

			mainEditBox->Clear();
			MainFrame::setFilename(name);
			mainEditBox->Enable();
			mainEditBox->BeginBold();
			mainEditBox->WriteText("Patient Name: ");
			mainEditBox->EndBold();
			mainEditBox->WriteText(name);
			mainEditBox->WriteText("   ");
			mainEditBox->BeginBold();
			mainEditBox->WriteText("Date of First Visit: ");
			mainEditBox->EndBold();
			mainEditBox->WriteText(functionHelper->getDateToSignNoTime());
			mainEditBox->SetBackgroundColour(wxColour(255, 255, 255));
			mainEditBox->SetEditable(true);
		}
	}
	void startingSate() {
		mainEditBox->Clear();
		MainFrame::setFilename("");
		mainEditBox->Disable();
		mainEditBox->SetBackgroundColour(wxColour(211, 211, 211));
		mainEditBox->SetEditable(true);
	}
	//getFilename is a getter method for the filename variable used for saving and loading textfiles
	wxString getFilename() {
		return this->fileName;
	}
	//setFIlename is a settermethod for the filename variable used for saving and loading textfiles
	void setFilename(wxString name) {
		this->fileName = name;
	}
	//appendDilename is a function to add somthing to the filename, typicaly an extention
	void appendFilename(wxString app) {
		this->fileName += app;
	}
	//getbuttonToEdit returns the referance to a button, used to edit said button
	wxButton* getbuttonToEdit() {
		return buttonToEdit;
	}
	//setbuttonToEditName changes the name of the button the user is editing
	void setbuttonToEditName(wxString Name) {
		this->getbuttonToEdit()->SetLabel(Name);
	}
	//setbuttonToEditText chaned the text of the button the user is editing
	void setbuttonToEditText(wxString Text) {
		this->getbuttonToEdit()->SetName(Text);
	}
	void removebuttonToEdit() {
		this->getbuttonToEdit()->Destroy();
	}
	//checks if the file has already been signed
	bool isSigned() {
		return this->Signed;
	}
	//tells the object weather or not the document has been singed.
	void setSigned(bool Sign) {
		this->Signed = Sign;
	}
	/// <summary>
/// the quit function saves the button configuration to a file
/// then closes the program
/// </summary>
/// <param name="WXUNUSED"></param>
	void quit(wxCommandEvent& event)
	{
		event.Skip();
		Close(TRUE); // Tells the OS to quit running this process
	}
	
		DECLARE_EVENT_TABLE()
};

#endif