void MyFrame::OnBold(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyBoldToSelection();
}

void MyFrame::OnItalic(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyItalicToSelection();
}

void MyFrame::OnUnderline(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyUnderlineToSelection();
}

void MyFrame::OnStrikethrough(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_STRIKETHROUGH);
}

void MyFrame::OnSuperscript(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
}

void MyFrame::OnSubscript(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyTextEffectToSelection(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
}
//////////////////////////////

void MyFrame::OnUpdateBold(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionBold());
}

void MyFrame::OnUpdateItalic(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionItalics());
}

void MyFrame::OnUpdateUnderline(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionUnderlined());
}

void MyFrame::OnUpdateStrikethrough(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->DoesSelectionHaveTextEffectFlag(wxTEXT_ATTR_EFFECT_STRIKETHROUGH));
}

void MyFrame::OnUpdateSuperscript(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->DoesSelectionHaveTextEffectFlag(wxTEXT_ATTR_EFFECT_SUPERSCRIPT));
}

void MyFrame::OnUpdateSubscript(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->DoesSelectionHaveTextEffectFlag(wxTEXT_ATTR_EFFECT_SUBSCRIPT));
}

void MyFrame::OnAlignLeft(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_LEFT);
}

void MyFrame::OnAlignCentre(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTRE);
}

void MyFrame::OnAlignRight(wxCommandEvent& WXUNUSED(event))
{
    m_richTextCtrl->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_RIGHT);
}

void MyFrame::OnUpdateAlignLeft(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionAligned(wxTEXT_ALIGNMENT_LEFT));
}

void MyFrame::OnUpdateAlignCentre(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionAligned(wxTEXT_ALIGNMENT_CENTRE));
}

void MyFrame::OnUpdateAlignRight(wxUpdateUIEvent& event)
{
    event.Check(m_richTextCtrl->IsSelectionAligned(wxTEXT_ALIGNMENT_RIGHT));
}


void MyFrame::OnFont(wxCommandEvent& WXUNUSED(event))
{
    wxRichTextRange range;
    if (m_richTextCtrl->HasSelection())
        range = m_richTextCtrl->GetSelectionRange();
    else
        range = wxRichTextRange(0, m_richTextCtrl->GetLastPosition()+1);

    int pages = wxRICHTEXT_FORMAT_FONT;

    wxRichTextFormattingDialog formatDlg(pages, this);
    formatDlg.SetOptions(wxRichTextFormattingDialog::Option_AllowPixelFontSize);
    formatDlg.GetStyle(m_richTextCtrl, range);

    if (formatDlg.ShowModal() == wxID_OK)
    {
        formatDlg.ApplyStyle(m_richTextCtrl, range, wxRICHTEXT_SETSTYLE_WITH_UNDO|wxRICHTEXT_SETSTYLE_OPTIMIZE|wxRICHTEXT_SETSTYLE_CHARACTERS_ONLY);
    }
}




 wxMenu* formatMenu = new wxMenu;
    formatMenu->AppendCheckItem(ID_FORMAT_BOLD, _("&Bold\tCtrl+B"));
    formatMenu->AppendCheckItem(ID_FORMAT_ITALIC, _("&Italic\tCtrl+I"));
    formatMenu->AppendCheckItem(ID_FORMAT_UNDERLINE, _("&Underline\tCtrl+U"));
    formatMenu->AppendSeparator();
    formatMenu->AppendCheckItem(ID_FORMAT_STRIKETHROUGH, _("Stri&kethrough"));
    formatMenu->AppendCheckItem(ID_FORMAT_SUPERSCRIPT, _("Superscrip&t"));
    formatMenu->AppendCheckItem(ID_FORMAT_SUBSCRIPT, _("Subscrip&t"));
    formatMenu->AppendSeparator();
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_LEFT, _("L&eft Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_RIGHT, _("&Right Align"));
    formatMenu->AppendCheckItem(ID_FORMAT_ALIGN_CENTRE, _("&Centre"));
    formatMenu->AppendSeparator();
    formatMenu->Append(ID_FORMAT_INDENT_MORE, _("Indent &More"));
    formatMenu->Append(ID_FORMAT_INDENT_LESS, _("Indent &Less"));
    formatMenu->AppendSeparator();
    formatMenu->Append(ID_FORMAT_PARAGRAPH_SPACING_MORE, _("Increase Paragraph &Spacing"));
    formatMenu->Append(ID_FORMAT_PARAGRAPH_SPACING_LESS, _("Decrease &Paragraph Spacing"));
    formatMenu->AppendSeparator();
    formatMenu->Append(ID_FORMAT_LINE_SPACING_SINGLE, _("Normal Line Spacing"));
    formatMenu->Append(ID_FORMAT_LINE_SPACING_HALF, _("1.5 Line Spacing"));
    formatMenu->Append(ID_FORMAT_LINE_SPACING_DOUBLE, _("Double Line Spacing"));
    formatMenu->AppendSeparator();
    formatMenu->Append(ID_FORMAT_FONT, _("&Font..."));
    formatMenu->Append(ID_FORMAT_IMAGE, _("Image Property"));
    formatMenu->Append(ID_FORMAT_PARAGRAPH, _("&Paragraph..."));
    formatMenu->Append(ID_FORMAT_CONTENT, _("Font and Pa&ragraph...\tShift+Ctrl+F"));
    formatMenu->AppendSeparator();
    formatMenu->Append(ID_SWITCH_STYLE_SHEETS, _("&Switch Style Sheets"));
    formatMenu->Append(ID_MANAGE_STYLES, _("&Manage Styles"));
