#include "DialogHelper.h"
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
bool DialogHelper::confirmIntentAddButton() {
    wxMessageDialog dialog(NULL, wxT("would you like a text button, or a Linking Button?"), wxT("Question"),
        wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog.SetYesNoLabels((wxMessageDialogBase::ButtonLabel)"text", (wxMessageDialogBase::ButtonLabel)"Link");
    switch (dialog.ShowModal()) {
    case wxID_YES:
        wxLogStatus(wxT("You pressed \"Text\""));
        return true;
        break;
    case wxID_NO:
        wxLogStatus(wxT("You pressed \"Link\""));
        return false;
        break;
    default:       wxLogError(wxT("Unexpected wxMessageDialog return code!"));
    }
    return false;
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
}