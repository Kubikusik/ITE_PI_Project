#pragma once

#include <windows.h>
#include <commdlg.h>
#include <string>

class FileManager {
public:
    static std::string GetLocation();
	static std::string SaveLocation();
};

std::string FileManager::GetLocation() {
    OPENFILENAME ofn;       // Struktura zawieraj�ca informacje o oknie dialogowym
    wchar_t szFile[260];    // Bufor na nazw� pliku
    ZeroMemory(&ofn, sizeof(ofn)); //Wyzerowanie pami�ci bufora pliku
	ofn.lStructSize = sizeof(ofn); //Rozmiar struktury w bajtach
    ofn.hwndOwner = NULL; //Ekran dialogowy jest bez w�a�ciciela
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

std::string FileManager::SaveLocation() {
    OPENFILENAME ofn;       // Struktura zawieraj�ca informacje o oknie dialogowym
    wchar_t szFile[260];    // Bufor na nazw� pliku
    ZeroMemory(&ofn, sizeof(ofn)); //Wyzerowanie pami�ci bufora pliku
    ofn.lStructSize = sizeof(ofn); //Rozmiar struktury w bajtach
    ofn.hwndOwner = NULL; //Ekran dialogowy jest bez w�a�ciciela
    ofn.lpstrFile = szFile; //
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"XML Files\0*.xml\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn)) {
        std::wstring ws(ofn.lpstrFile);
        std::string filePath(ws.begin(), ws.end());
        if (filePath.find(".xml") == std::string::npos) {
            filePath += ".xml";
        }
        return filePath;
    }
    else {
        return std::string();
    }
}