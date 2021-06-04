#pragma once
#include <wx/wx.h>
#include "clipboard.h"
#include <wx/choicdlg.h>
#include "CalenderDialog.h"
#include "BodyImageDialog.h"
class DialogHelper {
public:
	myImageDialog* bodyDialogObj;
	//ClipboardDialog* clipBoard;
	CalenderDialog* cal;
	wxWindow* parent;
	DialogHelper(wxWindow* parent);

	void MaxLevel();
	bool confirmIntent(wxString message);
	wxString confirmIntentAddButton(wxWindow* parent);
	bool errorMessage(wxString str);

	wxString MultipleChoiceDialog(std::string path, std::string text);
	wxString SingleChoiceDialog(std::string path, std::string text);
	wxString Calender();
	std::string bodyDialog();
	

};