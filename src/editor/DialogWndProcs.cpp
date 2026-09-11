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
			ShowWindow(hDlg, SW_HIDE);
			//DestroyWindow(hDlg);
			return (INT_PTR)TRUE;
		}
		break;

	case WM_NOTIFY:
	{
		NMHDR* pnmhdr = (NMHDR*)lParam;

		// Verify it's from our tree control
		if (pnmhdr->hwndFrom == g_app->m_hTreeView_objectList)
		{
			switch (pnmhdr->code)
			{
			case NM_DBLCLK:
			{
				HTREEITEM hSel = TreeView_GetSelection(pnmhdr->hwndFrom);
				if (hSel != NULL)
				{
					TVITEM tvItem = { 0 };
					tvItem.mask = TVIF_TEXT;
					tvItem.hItem = hSel;
					tvItem.pszText = (LPWSTR)malloc(256 * sizeof(TCHAR));
					tvItem.cchTextMax = 256;

					SendMessage(pnmhdr->hwndFrom, TVM_GETITEM, 0, (LPARAM)&tvItem);
					//MessageBox(0, tvItem.pszText, 0, 0);
					free(tvItem.pszText);
				}
			}break;
			case TVN_SELCHANGED:
			{
				break;
			}
			}
		}
		break;
	}
	}
	return (INT_PTR)FALSE; // Return FALSE for unhandled messages
}

