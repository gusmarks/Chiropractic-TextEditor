#pragma once
#ifndef __BASE_H
#define __BASE_H
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/wx.h>
class MainApplication: public wxApp
{
public: 
	virtual bool OnInit();
	wxRichTextStyleSheet* GetStyleSheet() const { return m_styleSheet; }
	wxRichTextStyleSheet* m_styleSheet;
};


#endif