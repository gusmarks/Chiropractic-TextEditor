#pragma once
#include <wx/wx.h>
class DialogHelper {
public:

	DialogHelper() {

	}
	bool confirmIntent(wxString message);
	bool confirmIntentAddButton();
	bool errorMessage(wxString str);
};