#include "ArchiveManager.h"
#include <wx/wx.h>

BEGIN_EVENT_TABLE(ArchiveManager, wxDialog)
EVT_BUTTON(ID_ARCHIVE_BILL, ArchiveManager::Billed_Unbilled)
EVT_BUTTON(ID_ARCHIVE_PAY, ArchiveManager::payed_Unpayed)
EVT_BUTTON(ID_ARCHIVE_ARCHIVE, ArchiveManager::archiveFilesFunction)

END_EVENT_TABLE()