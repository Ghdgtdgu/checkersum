// Ghdgtdgu Studio Vo. 4
// by Ghdgtdgu, July 2021

// misc.cpp
// 杂项

#include "checkersum.h"

LPCWSTR lpszAboutText = L"CheckerSum\nMade by Ghdgtdgu Studio, 2019-2021\nVisual C++ 6.0, UNICODE, I386\n\nbuild_%d";

// 判断复选框和进度条状态函数: CheckedControl

// 参数:
// PROGRAM_KERNEL_PTR pkp: 欲填入任何内容的程序中央结构体
// HWND hWnd: 程序窗体句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR.
//          其他情况下返回ERR_SUCCED.
VRESULT CheckedControl(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    HWND hWinCtrl;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    hWinCtrl = GetDlgItem(hWnd, IDC_PROGRESS1);
    if((pkp->dwFlag & FLG_PROGRESS) == FLG_PROGRESS) {
        CheckDlgButton(hWnd, IDC_CHECKBOX1, BST_UNCHECKED);
        EnableWindow(hWinCtrl, FALSE);
        pkp->dwFlag ^= FLG_PROGRESS;
    } else {
        CheckDlgButton(hWnd, IDC_CHECKBOX1, BST_CHECKED);
        EnableWindow(hWinCtrl, TRUE);
        pkp->dwFlag |= FLG_PROGRESS;
    }

    return ERR_SUCCED;
}

// 显示关于窗口函数: AboutMsg

// 参数:
// HWND hWnd: 关于窗口从属的主窗口句柄

// 返回值:
// VRESULT: 保证返回ERR_SUCCED.
VRESULT AboutMsg(HWND hWnd)
{
    unsigned short szBufferStr[MAX_STRSIZE];

	#ifdef	__CHECK__
	unsigned short szBufferStr2[MAX_PATHSIZE];
	#endif

    wsprintf(szBufferStr, lpszAboutText, VERSION_BUILD);

    #ifdef  __CHECK__
    GetCurrentDirectory(MAX_PATHSIZE, (LPWSTR)&szBufferStr2);
    wsprintf(szBufferStr, L"%s, %s", szBufferStr, szBufferStr2);
    #endif

    MessageBox(hWnd, szBufferStr, L"About CheckerSum", MB_OK);

    return ERR_SUCCED;
}

// 获取32位整型转换函数: get32
// 来自OSASK SDK, 作者Kawai Hidomi

// 参数:
// unsigned char *buffer: 数字.

// 返回值:
// int: 转换完毕的数字.
int get32(unsigned char *buffer)
{
    // 本函数适用于小端CPU架构.
    return (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0]; 
}
