#pragma once

#include "wx/wx.h"
#include "wx/richtext/richtextstyles.h"
#include "wx/richtext/richtextctrl.h"
class MyRichTextCtrl : public wxRichTextCtrl
{
public:
    MyRichTextCtrl(wxWindow* parent, wxWindowID id);

   bool isBold() {
        return isTextBold;
    }
   void setBold(bool val) {
       isTextBold = val;
   }
   bool isItalic() {
       return isTextItalic;
   }
   void setItalic(bool val) {
       isTextItalic = val;
   }
   bool isUnderlined() {
       return isTextUnderlined;
   }
   void setUnderlined(bool val) {
       isTextUnderlined = val;
   }
private:
    bool isTextBold;
    bool isTextItalic;
    bool isTextUnderlined;
};    

