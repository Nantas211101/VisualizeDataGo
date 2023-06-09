#include "GUI_FileSelector.hpp"
HWND hwnd;
TCHAR szFileName[MAX_PATH];

namespace GUI {

FileSelector::FileSelector(std::string filter, std::string ext)
    : filter(filter),
      ext(ext){

      };

std::optional<std::string> FileSelector::select() {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = this->filter.c_str();
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = this->ext.c_str();

    if (GetOpenFileName(&ofn) == TRUE) {
        // User selected a file
        return ofn.lpstrFile;
    } else {
        // User cancelled the dialog
        return std::nullopt;
    }
}

} // namespace GUI