// Ghdgtdgu Studio Vo.4 - CheckerSum
// by Ghdgtdgu, July 2021

// errmsg.cpp
// 处理和程序错误有关的操作.

#include    "checkersum.h"

// 错误消息结构体 (struct tagERR_THROWMSG:ERR_THROWMSG:*ERR_THROWMSG_PTR)
// 建议缩写: etm, &etmp

// LPCWSTR lpszText: 消息窗口的内容
// LPCWSTR lpszTitle: 消息窗口的标题
// UINT uMode: 消息窗口的属性

struct tagERR_THROWMSG {
    LPCWSTR lpszText;
    LPCWSTR lpszTitle;
    UINT uMode;
} typedef ERR_THROWMSG, *ERR_THROWMSG_PTR;

// 下列定义错误的属性
ERR_THROWMSG etm_Arr[ERR_COUNT] = {
    // ERR_SUCCED
    {
        NULL, 
        NULL, 
        0
    },

    // ERR_UNKNOWN
    {
        L"An unknown error occurred in the program.",
        L"Error!",
        MB_ICONERROR|MB_OK
    },

    // ERR_POINTERR
    {
        L"Null or unavailable pointer in argument to function.",
        L"Error!",
        MB_ICONERROR|MB_OK
    },

    // ERR_FILERW
    {
        L"File R/W error!",
        L"Error!",
        MB_ICONERROR|MB_OK
    },

    // ERR_FILESIZE
    {
        L"The file is too large!",
        L"Error!",
        MB_ICONERROR|MB_OK
    },
};

// 错误回调函数: ErrMsgCallBack

// 参数:
// VRESULT vrsl: 程序返回的错误号
// HWND hWnd: 错误消息窗口从属的主窗口
// int dwFlag: 属性. 请见头文件中的定义.

// 返回值:
// VRESULT: 如果返回的错误号为ERR_SUCCED, 则返回ERR_SUCCED,
//          否则返回ERR_FAILED
VRESULT ErrMsgCallBack(VRESULT vrsl, HWND hWnd, int dwFlag)
{
    if(vrsl != ERR_SUCCED) {
        if((dwFlag & EMCB_IMVISIBLE) != EMCB_IMVISIBLE) {
            ErrMsgShow(vrsl, hWnd, (dwFlag & EMCB_EXIT));
        }

        // 如果程序必须退出, 就通过ExitProcess来传递错误码
        // 否则直接return
        if((dwFlag & EMCB_EXIT) == EMCB_EXIT) {
            ExitProcess(-1);
            return -2;
        }
    }
    return vrsl;
}

// 错误窗口调用函数: ErrMsgShow

// 参数:
// VRESULT vrsl: 错误号. 可以索引错误属性.
// HWND hWnd: 错误消息窗口从属的主窗口
// int dwExit: 是否退出程序. 这会影响到窗口的显示.

// 返回值:
// VRESULT: 确定返回ERR_SUCCED.
VRESULT ErrMsgShow(VRESULT vrsl, HWND hWnd, int dwExit)
{
    UINT uMode0;
    unsigned short szBufferStr[MAX_STRSIZE];

    if(dwExit == 0) {
        wsprintf(szBufferStr, L"%s\nCODE: 0x%04X", etm_Arr[vrsl].lpszText, vrsl);
        uMode0 = etm_Arr[vrsl].uMode;
    } else if(dwExit == 1) {
        // 程序必须退出时,
        // 窗口文字会多出一句话, 并且图标锁定为MB_ICONERROR
        wsprintf(szBufferStr, L"%s\nThe program is about to exit.\nCODE: 0x%04X", etm_Arr[vrsl].lpszText, vrsl);
        uMode0 = MB_ICONERROR|MB_OK;
    }

    MessageBox(
        hWnd,
        szBufferStr,
        etm_Arr[vrsl].lpszTitle,
        uMode0
    );

	return ERR_SUCCED;
}
