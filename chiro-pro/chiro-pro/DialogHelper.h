#pragma once
#include <wx/wx.h>
#include "clipboard.h"
class DialogHelper {
public:
	ClipboardDialog* clipBoard;
	DialogHelper();

	
	bool confirmIntent(wxString message);
	bool confirmIntentAddButton();
	bool errorMessage(wxString str);
	
	void openClipBoard();

};