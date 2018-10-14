#include "stdafx.h"
#include "WindowsCore.h"



WindowsCore::WindowsCore()
{
}


WindowsCore::~WindowsCore()
{
}

void WindowsCore::OpenFileBrowser()

{

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

	
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		pFileOpen->SetOkButtonLabel(L"Import");
		pFileOpen->SetTitle(L"Content Import");

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{

					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{




						size_t outputSize = 150;

						size_t buffersize = 150;

						wchar_t* source = pszFilePath;

						char *Path = new char[outputSize];
						size_t *outputLength = &outputSize;


						wcstombs_s(outputLength, Path, buffersize, source, outputSize);

						CoTaskMemFree(pszFilePath);

						//ImportFileFromPath(Path);




					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();

	}

}

