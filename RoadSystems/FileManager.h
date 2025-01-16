#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <shobjidl.h> //for IFIleOpenDialog and IFileSaveDialog
#include <string>

class FileManager {
public:
    static std::string GetLocation();
	static std::string SaveLocation();
};

// DOCS: https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog#basic-usage

std::string FileManager::GetLocation() {
	HRESULT hr; //used for error checking
	IFileOpenDialog* pFileOpen; //used for opening file dialog

    // Initialize COM library
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        return std::string();
    }

    // Create the FileOpenDialog object
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
    if (FAILED(hr)) {
        CoUninitialize();
        return std::string();
    }

    // Set the file types to display only. 
    // The user can select other file types, but only the specified types are displayed.
	COMDLG_FILTERSPEC rgSpec[] = {
        { L"XML Files", L"*.xml" },
		{ L"All Files", L"*.*" }
    };
    pFileOpen->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
    pFileOpen->SetFileTypeIndex(1);
    pFileOpen->SetDefaultExtension(L"xml");

    // Show the Open dialog box
    hr = pFileOpen->Show(NULL);
    if (SUCCEEDED(hr)) {
        // Get the file name from the dialog box
		IShellItem* pItem; //used for getting the file path
		hr = pFileOpen->GetResult(&pItem); //get the file path
        if (SUCCEEDED(hr)) {
			PWSTR pszFilePath; //unicode string for file path
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath); //get the file path
            if (SUCCEEDED(hr)) {
                // Convert the file path to a std::string
				std::wstring ws(pszFilePath); //convert to wstring
				std::string filePath(ws.begin(), ws.end()); //convert to string
				CoTaskMemFree(pszFilePath); //free the 
				pItem->Release(); //release the item
				pFileOpen->Release(); //release the dialog
				CoUninitialize(); //uninitialize the COM library
                return filePath; 
            }
            pItem->Release(); 
        }
    }
	//if something went wrong, release the dialog and uninitialize the COM library
    pFileOpen->Release(); 
    CoUninitialize();
    return std::string();
}

std::string FileManager::SaveLocation() {
    HRESULT hr; //used for error checking
    IFileSaveDialog* pFileSave = nullptr; //used for saving file dialog

	// Initialize COM library, prevents memory leaks and 0xcccccccc errors
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        return std::string();
    }

    // Create the FileSaveDialog object
    hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));
    if (FAILED(hr)) {
        CoUninitialize();
        return std::string();
    }

    // Set the file types to display only. 
    // The user can select other file types, but only the specified types are displayed.
    COMDLG_FILTERSPEC rgSpec[] = {
        { L"XML Files", L"*.xml" },
        { L"All Files", L"*.*" }
    };
    pFileSave->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
    pFileSave->SetFileTypeIndex(1);
    pFileSave->SetDefaultExtension(L"xml");

    // Show the Save dialog box
    hr = pFileSave->Show(NULL);
    if (SUCCEEDED(hr)) {
        // Get the file name from the dialog box
        IShellItem* pItem; //used for getting the file path
        hr = pFileSave->GetResult(&pItem); //get the file path
        if (SUCCEEDED(hr)) {
            PWSTR pszFilePath; //unicode string for file path
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath); //get the file path
            if (SUCCEEDED(hr)) {
                // Convert the file path to a std::string
                std::wstring ws(pszFilePath); //convert to wstring
                std::string filePath(ws.begin(), ws.end()); //convert to string
                CoTaskMemFree(pszFilePath); //free the memory
                pItem->Release(); //release the item
                pFileSave->Release(); //release the dialog
                CoUninitialize(); //uninitialize the COM library
                return filePath;
            }
            pItem->Release();
        }
    }
    //if something went wrong, release the dialog and uninitialize the COM library
    pFileSave->Release();
    CoUninitialize();
    return std::string();
}
#endif
#pragma once