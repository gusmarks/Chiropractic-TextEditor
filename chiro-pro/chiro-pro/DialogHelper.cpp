#include "DialogHelper.h"
#include <wx/choicdlg.h>

#include <fstream>

DialogHelper::DialogHelper(wxWindow* parent) {
    this->parent = parent;
}
/// <summary>
/// the function confirmIntent accepts a string with a question,
/// and produces a popupbox to ask this question. then waits on a yes or no responce, and acts acordingly
/// </summary>
/// <param name="message"></param>
/// <returns></returns>
bool DialogHelper::confirmIntent(wxString message) {

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
wxString DialogHelper::confirmIntentAddButton(wxWindow* parent) {
    wxArrayString options;
    options.Add("text output.");
    options.Add("new page.");
    options.Add("popup.");
    wxSingleChoiceDialog dialog(parent, wxT("would you like a text button, or a Linking Button?"), wxT("Question"), options);
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
/// <summary>
/// takes in a string with any message, and produces a popup error message
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
bool DialogHelper::errorMessage(wxString str) {
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
/// <summary>
/// this fucntion opens a Dialog to allow the user to fill a list of text.
///  from this lis tthey can replace anyline in the Main document
/// </summary>
void DialogHelper::openClipBoard() {
    clipBoard =
        new ClipboardDialog("", wxDefaultPosition,wxDefaultSize);
    clipBoard->Show();

}
wxString DialogHelper::FreqOfPain() {
    std::ifstream choiceIn;
    choiceIn.open("DialogInformation/FreqOfPain.txt", std::fstream::in);
    wxArrayString choices;
    std::string choice;
    while (choiceIn.peek() != EOF) {

        getline(choiceIn, choice);
        choices.Add(choice);
    }
    wxMultiChoiceDialog dialog(this->parent, wxT("Frequency of pain?"), wxT("Make a Selection"), choices);
    if (dialog.ShowModal() == wxID_OK) {
        wxArrayInt selections = dialog.GetSelections();
        wxString msg;
        msg.Printf(wxT("you selected %u items:\n"), selections.GetCount());
        for (size_t n = 0; n < selections.GetCount(); n++) {
            msg += wxString::Format(wxT("\t%d: %d (%s)\n"), n, selections[n], choices[selections[n]].c_str());
        }
        wxMessageBox(msg, wxT("got selections"));
    }

    return "";
}
wxString DialogHelper::MechOfInjury() {
    std::ifstream choiceIn;
    choiceIn.open("DialogInformation/Injury.txt", std::fstream::in);
    wxArrayString choices;
    std::string choice;
    while (choiceIn.peek() != EOF) {
        
        getline(choiceIn, choice);
        choices.Add(choice);
    }
    wxMultiChoiceDialog dialog(this->parent, wxT("Mechanism of injury or condition?"), wxT("Make a Selection"), choices);
    if (dialog.ShowModal() == wxID_OK) {
        wxArrayInt selections = dialog.GetSelections(); 
        wxString msg;
        msg.Printf(wxT("you selected %u items:\n"), selections.GetCount());
        for (size_t n = 0; n < selections.GetCount(); n++) {
            msg += wxString::Format(wxT("\t%d: %d (%s)\n"), n, selections[n], choices[selections[n]].c_str());
        }
        wxMessageBox(msg, wxT("got selections"));
    }

    return "";
}
wxString DialogHelper::Alergies_Sensitivities() {
    std::ifstream choiceIn;
    choiceIn.open("DialogInformation/Allergies.txt", std::fstream::in);
    wxArrayString choices;
    std::string choice;
    while (choiceIn.peek() != EOF) {

        getline(choiceIn, choice);
        choices.Add(choice);
    }
    //choices.Add(wxT("one"));
    //choices.Add(wxT("two"));
    wxMultiChoiceDialog dialog(this->parent, wxT("Dose the patient have any known allergies, or sensitivities?"), wxT("Make a Selection"), choices);
    if (dialog.ShowModal() == wxID_OK) {
        wxArrayInt selections = dialog.GetSelections();
        wxString msg;
        msg.Printf(wxT("you selected %u items:\n"), selections.GetCount());
        for (size_t n = 0; n < selections.GetCount(); n++) {
            msg += wxString::Format(wxT("\t%d: %d (%s)\n"), n, selections[n], choices[selections[n]].c_str());
        }
        wxMessageBox(msg, wxT("got selections"));
    }

    return "";
}
wxString DialogHelper::Calender() {
    cal = new CalenderDialog(parent, "Date of Accident");
    cal->Show();
    return "";
}
void DialogHelper::MaxLevel(){
}
