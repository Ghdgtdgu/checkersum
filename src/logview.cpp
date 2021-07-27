// Ghdgtdgu Studio Vo.4 - CheckerSum
// by Ghdgtdgu, July 2021

// logview.cpp
// 显示和日志文件显示相关的内容.

#include "checkersum.h"

// 日志文件对话框回调函数 (保留): LogViewProc

// 参数:
// HWND hLogView: 代表窗口的句柄.
// UINT uMessage: 本次消息.
// WPARAM wParam: 参数1.
// LPARAM lParam: 参数2.

// 返回值:
// BOOL: 如果消息在程序内部被捕获, 则返回FALSE; 否则返回TRUE.
BOOL WINAPI LogViewProc(HWND hLogView, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static PROGRAM_KERNEL_PTR pkp;

    switch(uMessage) {
		case WM_COMMAND:
			switch(wParam) {
				case IDC_BUTTON4:
					SendMessage(hLogView, WM_CLOSE, NULL, NULL);
					break;

                case IDC_BUTTON5:
                    LogViewSaveFile(hLogView, pkp);
					SendMessage(hLogView, WM_CLOSE, NULL, NULL);
                    break;
			}
			break;
		
        case WM_INITDIALOG:
            pkp = (PROGRAM_KERNEL_PTR) lParam;

            LogViewSetText(hLogView, pkp);
            break;

        case WM_CLOSE:
            EndDialog(hLogView, ERR_SUCCED);
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

// 编辑框内容准备函数: LogViewSetText

// 参数:
// HWND hWnd: 对话框窗口句柄
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体

// 返回值:
// VRESULT: 保证返回ERR_SUCCED.
VRESULT LogViewSetText(HWND hWnd, PROGRAM_KERNEL_PTR pkp)
{
    unsigned short szBufferLarge[MAX_LARGESIZE];
    HWND hWinCtrl;

    wsprintf(szBufferLarge,
             L"Checksum=0x%08X\r\nChecksum0=0x%08X, Filesize=%d byte\r\n\r\nCallBack Times: %d\r\nUsing time=%dms (Compress=%dms, Callback=%dms)\r\n\0",
             pkp->cs.checksum1, pkp->cs.checksum0, pkp->cs.filesize, pkp->cs.ct_callback, pkp->cs.time0, pkp->cs.time1, pkp->cs.time_cb);

    SetDlgItemText(hWnd, IDC_EDIT2, szBufferLarge);
    //SendDlgItemMessage(hWnd, IDC_EDIT2, EM_SETREADONLY, TRUE, NULL);

    hWinCtrl = GetDlgItem(hWnd, IDC_EDIT1);
    SetWindowLong(hWinCtrl, GWL_EXSTYLE, WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE);

    return ERR_SUCCED;
}

// 保存日志文件内容函数: LogViewSaveFile

// 参数:
// HWND hWnd: 对话框窗口句柄
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体

// 返回值:
// VRESULT: 如果出现指针错误, 则返回ERR_POINTERR
//          如果出现文件读写错误, 则返回ERR_FILERW
//          否则返回ERR_SUCCED.
VRESULT LogViewSaveFile(HWND hWnd, PROGRAM_KERNEL_PTR pkp)
{
    OPENFILENAME ofn_sv;
    unsigned short szBufferLarge[MAX_LARGESIZE], szBufferStr[MAX_STRSIZE];
    unsigned short szBufferPath[MAX_PATHSIZE];
    int tsize, wsize;

    HANDLE hFile;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    tsize = GetWindowTextLength(GetDlgItem(hWnd, IDC_EDIT2));
    if(tsize > MAX_LARGESIZE) {
        return ErrMsgCallBack(ERR_UNKNOWN, hWnd, 0);
    }
    GetDlgItemText(hWnd, IDC_EDIT2, szBufferLarge, tsize);
    GetCurrentDirectory(MAX_STRSIZE, (LPWSTR)&szBufferStr);
	
	RtlZeroMemory(&szBufferPath, MAX_PATHSIZE);
	RtlZeroMemory(&ofn_sv, sizeof(OPENFILENAME));
	ofn_sv.lStructSize = sizeof(OPENFILENAME);
	ofn_sv.hInstance = pkp->hInstance;
	ofn_sv.hwndOwner = hWnd;
	ofn_sv.lpstrFilter = L"All Files(*.*)\0*.*\0\0";
	ofn_sv.lpstrFile = szBufferPath;
	ofn_sv.nMaxFile = MAX_PATHSIZE;
	ofn_sv.lpstrTitle = L"Save";
	ofn_sv.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;

    if(GetSaveFileName(&ofn_sv) == TRUE) {
        hFile = CreateFile(szBufferPath, GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE) {
err:
            return ErrMsgCallBack(ERR_FILERW, hWnd, 0);
        }

        if(WriteFile(hFile, szBufferLarge, tsize*sizeof(unsigned short), (DWORD *)&wsize, NULL) == 0) {
            CloseHandle(hFile);
            goto err;
        }

        CloseHandle(hFile);
    }

    return ERR_SUCCED;
}
