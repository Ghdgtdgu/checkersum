// Ghdgtdgu Studio Vo. 4 - CheckerSum
// by Ghdgtdgu, July 2021

// install.cpp
// 设定程序的运行.

#include    "checkersum.h"

// 设定程序中央结构体函数: InstProgramKernel

// 参数:
// PROGRAM_KERNEL_PTR pkp: 欲填入任何内容的程序中央结构体
// HWND hWnd: 程序窗体句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR. 此时程序会自动退出.
//          其他情况下返回ERR_SUCCED.
VRESULT InstProgramKernel(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, EMCB_EXIT);
    }

    RtlZeroMemory(pkp, sizeof(PROGRAM_KERNEL));
    pkp->hInstance = GetModuleHandle(NULL);
    pkp->dwStatu = STU_BOOT;

    GetCurrentDirectory(MAX_PATHSIZE, (LPWSTR)&(pkp->szProfilePath));
    wsprintf(pkp->szProfilePath, L"%s\\checkersum.ini", pkp->szProfilePath);

    ConfigApply(pkp, hWnd);

    return ERR_SUCCED;
}

// 初始化程序用户界面函数: InstUI

// 参数:
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体
// HWND hWnd: 程序窗体句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR. 此时程序会自动退出.
//          其他情况下返回ERR_SUCCED.
VRESULT InstUI(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    HWND hWinCtrl;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, EMCB_EXIT);
    }

	SendMessage(hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIcon(NULL, IDI_APPLICATION));

    // 编辑框
    hWinCtrl = GetDlgItem(hWnd, IDC_EDIT1);
    SetWindowLong(hWinCtrl, GWL_EXSTYLE, WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE);
    SendMessage(hWinCtrl, EM_SETREADONLY, FALSE, NULL);
	EnableWindow(hWinCtrl, FALSE);

    RefreshUI(pkp, hWnd);

    return ERR_SUCCED;
}

// 刷新程序用户界面函数：RefreshUI

// 参数:
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体
// HWND hWnd: 程序窗体句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR.
//          其他情况下返回ERR_SUCCED.
VRESULT RefreshUI(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{ 
    DWORD i;
    HWND hWinCtrl;
    HMENU hMenu;
	unsigned short szBufferStr[MAX_STRSIZE];

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    switch(pkp->dwStatu) {
        case STU_BOOT:
            // 开始运算和显示日志文件按钮禁用
            // 根据pkp->dwFlag的内容编辑复选框和进度条状态
            // 之后设置三个标签为合适的内容

            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON2);
            EnableWindow(hWinCtrl, FALSE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON3);
            EnableWindow(hWinCtrl, FALSE);

            hWinCtrl = GetDlgItem(hWnd, IDC_PROGRESS1);
            if((pkp->dwFlag & FLG_PROGRESS) == FLG_PROGRESS) {
                CheckDlgButton(hWnd, IDC_CHECKBOX1, BST_CHECKED);
                EnableWindow(hWinCtrl, TRUE);
            } else {
                CheckDlgButton(hWnd, IDC_CHECKBOX1, BST_UNCHECKED);
                EnableWindow(hWinCtrl, FALSE);
            }

            SetDlgItemText(hWnd, IDC_LABEL1, L"Selected file: (void)");
            SetDlgItemText(hWnd, IDC_LABEL2, L"Done.");
            SetDlgItemText(hWnd, IDC_LABEL3, L"");

            break;

        case STU_SELECTED:
            // 开始运算按钮现在可用
            // 设置三个标签为合适的内容

            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON2);
            EnableWindow(hWinCtrl, TRUE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON3);
            EnableWindow(hWinCtrl, FALSE);
			
			wsprintf(szBufferStr, L"Selected file: %s", pkp->szFileName);
            SetDlgItemText(hWnd, IDC_LABEL1, szBufferStr);
            SetDlgItemText(hWnd, IDC_LABEL2, L"Ready.");
            SetDlgItemText(hWnd, IDC_LABEL3, L"");

			SendDlgItemMessage(hWnd, IDC_PROGRESS1, PBM_SETPOS, (WPARAM)0, NULL);

			break;

        case STU_COMPRESSING:
            // 所有按钮全部不可用
            // 复选框按钮不可用
            // 菜单项全部灰化
            // 请设置三个标签为合适的内容.

            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON1);
            EnableWindow(hWinCtrl, FALSE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON2);
            EnableWindow(hWinCtrl, FALSE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON3);
            EnableWindow(hWinCtrl, FALSE);
            hWinCtrl = GetDlgItem(hWnd, IDC_CHECKBOX1);
            EnableWindow(hWinCtrl, FALSE);

            hMenu = LoadMenu(pkp->hInstance, MAKEINTRESOURCE(IDM_MAIN));
            for(i = 0; i < 3; i++) {
                EnableMenuItem(hMenu, i, MF_GRAYED);
            }

            SetDlgItemText(hWnd, IDC_LABEL2, L"Compressing...");
            SetDlgItemText(hWnd, IDC_LABEL3, L"");

            break;

        case STU_COMPRESSED:
            // 所有按钮全部可用
            // 复选按钮可用
            // 菜单全部还原
            // 现在, 请设置三个标签为合适的内容.

            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON1);
            EnableWindow(hWinCtrl, TRUE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON2);
            EnableWindow(hWinCtrl, TRUE);
            hWinCtrl = GetDlgItem(hWnd, IDC_BUTTON3);
            EnableWindow(hWinCtrl, TRUE);
            hWinCtrl = GetDlgItem(hWnd, IDC_CHECKBOX1);
            EnableWindow(hWinCtrl, TRUE);

            hMenu = LoadMenu(pkp->hInstance, MAKEINTRESOURCE(IDM_MAIN));
            for(i = 0; i < 3; i++) {
                EnableMenuItem(hMenu, i, MF_ENABLED);
            }

            SetDlgItemText(hWnd, IDC_LABEL2, L"Compressed!");

            wsprintf(szBufferStr, L"Checksum=0x%08X(0x%08X+%d), time: %dms",
                     pkp->cs.checksum1, pkp->cs.checksum0, pkp->cs.filesize, pkp->cs.time0);
            SetDlgItemText(hWnd, IDC_LABEL3, szBufferStr);

            break;
    }

    return ERR_SUCCED;
}

// 选择文件界面函数: InstFileName

// 参数:
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体
// HWND hWnd: 程序窗体句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR.
//          如果GetFileSize函数抛出错误, 则返回相应的错误码.
//          其他情况下返回ERR_SUCCED.
VRESULT InstFileName(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    OPENFILENAME ofn;
    unsigned short szBufferStr[MAX_STRSIZE];
    VRESULT iret;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }   
    GetCurrentDirectory(MAX_PATHSIZE, (LPWSTR)&szBufferStr);

    // 准备文件通用对话框结构体
    RtlZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    //ofn.hInstance = pkp->hInstance;
    ofn.lpstrFilter = L"All Files(*.*)\0*.*\0\0";
    ofn.lpstrFile = (LPWSTR)&(pkp->szFilePath);
    ofn.nMaxFile = MAX_PATHSIZE;
    ofn.lpstrFileTitle = (LPWSTR)&(pkp->szFileName);
    ofn.nMaxFileTitle = MAX_STRSIZE;
    ofn.lpstrInitialDir = szBufferStr;
    ofn.lpstrTitle = L"Open";
	ofn.nFilterIndex = 1;
    ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

    if(GetOpenFileName(&ofn) == TRUE) {
        iret = ChkFileSize(pkp->szFilePath, hWnd);

        if(iret == ERR_SUCCED) {
            pkp->dwStatu = STU_SELECTED;
            RefreshUI(pkp, hWnd);
            RtlZeroMemory(&(pkp->cs), sizeof(COMPRESS_STUB));
        } else {
            return iret;
        }
    }

    return ERR_SUCCED;
}