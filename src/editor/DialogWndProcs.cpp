#include "editor.h"
#include <Windows.h>
#include <commctrl.h>

extern Application* g_app;

INT_PTR CALLBACK DialogProcAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetDlgItemText(hDlg, IDC_EDIT1, L"artemybasov@gmail.com");
	}return (INT_PTR)TRUE;
	//case WM_SHOWWINDOW:
	//	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		case IDOK:
		case IDCANCEL:
			DestroyWindow(hDlg);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE; // Return FALSE for unhandled messages
}

