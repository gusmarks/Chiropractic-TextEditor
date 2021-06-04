#include "myRichTextCtrl.h"
#include <wx/wx.h>



MyRichTextCtrl::MyRichTextCtrl(wxWindow* parent, wxWindowID id) :
    wxRichTextCtrl(parent, id, "richtext", wxDefaultPosition, wxSize(600, 600),
     0,   wxDefaultValidator, "")
{
    isTextBold = false;
    isTextItalic = false;
    isTextUnderlined = false;

}
//MyRichTextCtrl::MyRichTextCtrl():wxRichTextCtrl(NULL,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE,wxDefaultValidator,wxTextCtrlNameStr) {}

