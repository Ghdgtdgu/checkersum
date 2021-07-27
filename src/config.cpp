// Ghdgtdgu Studio Vo. 4 - CheckerSum
// by Ghdgtdgu, July 2021

// config.cpp
// 操作程序的配置文件.

#include    "checkersum.h"

// 应用配置文件函数: ConfigApply

// 参数:
// PROGRAM_KERNEL_PTR pkp: 欲应用的程序中央结构体
// HWND hWnd: 程序主窗口句柄

// 返回值:
// VRESULT: 保证返回ERR_SUCCED.
//          pkp的值已经被验证, 应不会出现无效指针问题.
VRESULT ConfigApply(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    // 如果配置文件内的键值缺失或者超过了预定的范围,
    // 则默认恢复为默认值.

    //unsigned short szBufferTiny[MAX_TINYSIZE];
    int flag, cxWin, cyWin;

    // 是否默认启用进度条
    flag = GetPrivateProfileInt(L"CheckerSum", L"EnabledProgressBar", CFG_KEYLOST, pkp->szProfilePath);
    if(flag == 1) {
        pkp->dwFlag |= FLG_PROGRESS;
    }

    // 程序窗口的坐标
    cxWin = GetPrivateProfileInt(L"CheckerSum", L"XPosition", CFG_KEYLOST, pkp->szProfilePath);
    cyWin = GetPrivateProfileInt(L"CheckerSum", L"YPosition", CFG_KEYLOST, pkp->szProfilePath);

    // 只有当两个键值均无缺失时, 才可以恢复窗口位置.
    if(cxWin != CFG_KEYLOST && cyWin != CFG_KEYLOST) {
        SetWindowPos(hWnd, HWND_TOP, cxWin, cyWin, 0, 0, SWP_NOSIZE);
    }

    return ERR_SUCCED;
}

// 保存配置文件函数: ConfigSave

// 参数:
// PROGRAM_KERNEL_PTR pkp: 欲保存的程序中央结构体
// HWND hWnd: 程序主窗口句柄

// 返回值:
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR. 此时程序会自动退出.
//          其他情况下返回ERR_SUCCED.
VRESULT ConfigSave(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    unsigned short szBufferTiny[MAX_TINYSIZE];
    RECT rectPublic;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, EMCB_EXIT);
    }

    // 是否启用默认进度条
    if((pkp->dwFlag & FLG_PROGRESS) == FLG_PROGRESS) {
        wsprintf(szBufferTiny, L"1");
    } else {
        wsprintf(szBufferTiny, L"0");
    }
    WritePrivateProfileString(L"CheckerSum", L"EnabledProgressBar", szBufferTiny, pkp->szProfilePath);

    // 程序窗口的坐标
    GetWindowRect(hWnd, &rectPublic);

    wsprintf(szBufferTiny, L"%d", rectPublic.left);
    WritePrivateProfileString(L"CheckerSum", L"XPosition", szBufferTiny, pkp->szProfilePath);
    wsprintf(szBufferTiny, L"%d", rectPublic.top);
    WritePrivateProfileString(L"CheckerSum", L"YPosition", szBufferTiny, pkp->szProfilePath);

    return ERR_SUCCED;
}
