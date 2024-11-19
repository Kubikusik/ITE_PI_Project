#pragma once

#include <windows.h>
#include <commdlg.h>
#include <string>

class FileManager {
public:
    static std::string GetLocation();
};

std::string FileManager::GetLocation() {
    OPENFILENAME ofn;       // Struktura zawieraj¹ca informacje o oknie dialogowym
    wchar_t szFile[260];    // Bufor na nazwê pliku
    ZeroMemory(&ofn, sizeof(ofn)); //Wyzerowanie pamiêci bufora pliku
	ofn.lStructSize = sizeof(ofn); //Rozmiar struktury w bajtach
    ofn.hwndOwner = NULL; //Ekran dialogowy jest bez w³aœciciela
    ofn.lpstrFile = szFile; //
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"XML Files\0*.xml\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = NULL; 
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        std::wstring ws(ofn.lpstrFile);
        return std::string(ws.begin(), ws.end());
    }
    else {
        return std::string();
    }
}