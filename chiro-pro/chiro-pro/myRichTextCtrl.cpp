#include "myRichTextCtrl.h"
#include <wx/wx.h>



MyRichTextCtrl::MyRichTextCtrl(wxWindow* parent, wxWindowID id) :
    wxRichTextCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxSize(600, 1),
       wxVSCROLL|wxHSCROLL| wxTE_RICH2, wxDefaultValidator, wxTextCtrlNameStr)
{
    isTextBold = false;
    isTextItalic = false;
    isTextUnderlined = false;

}
//MyRichTextCtrl::MyRichTextCtrl():wxRichTextCtrl(NULL,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE,wxDefaultValidator,wxTextCtrlNameStr) {}

