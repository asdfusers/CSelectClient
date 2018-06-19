#include "stdafx.h"
#include "DlgBox.h"
#include "resource.h"


CDlgBox::Login CDlgBox::log = { 0, };
wchar_t CDlgBox::number[10] = L"";
CDlgBox::CDlgBox()
{
	
}


CDlgBox::~CDlgBox()
{
	
}

void CDlgBox::LoginDlg()
{
	HWND hwnd = 0;
	HINSTANCE ghInstance = 0;

	DialogBox(ghInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
}

void CDlgBox::RoomListDlg()
{
	HWND hwnd = 0;
	HINSTANCE ghInstance = 0;

	DialogBox(ghInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DialogProc2);
}

BOOL CDlgBox::DialogProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	
	switch (message)
	{

	case WM_COMMAND:
	{
		switch (wp)
		{
		case IDC_BUTTON1:
		{

			GetDlgItemText(hwnd, IDC_EDIT_ID, log.ID, 10);
			GetDlgItemText(hwnd, IDC_EDIT_PASSWORD, log.password, 10);
			EndDialog(hwnd, IDC_BUTTON1);
			return TRUE;
		}
		break;
		}
	}
	}
	return FALSE;
}

BOOL CDlgBox::DialogProc2(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	switch (message)
	{

	case WM_COMMAND:
	{
		switch (wp)
		{
		case IDC_BUTTON1:
		{
			GetDlgItemText(hwnd, IDC_EDIT1, (LPWSTR)number, 10);
			EndDialog(hwnd, IDC_BUTTON1);
			return TRUE;
		}
		break;
		}
	}
	}
	return FALSE;
}

int CDlgBox::getNumber()
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, number, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, number, -1, pStr, strSize, 0, 0);

	std::string asdf;
	for (int i = 0; i < sizeof(pStr); i++)
	{
		asdf += pStr[i];
	}

	return std::stoi(asdf);
}
