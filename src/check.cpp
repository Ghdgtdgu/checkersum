// Ghdgtdgu Studio Vo.4 - CheckerSum
// by Ghdgtdgu, July 2021

// check.cpp 
// 检查调试用函数

#include    "checkersum.h"

#ifdef  __CHECK__

// 显示中央结构体函数: ShowPK

// 参数:
// PROGRAM_KERNEL_PTR pkp: 欲显示的程序中央结构体
// HWND hWnd: 程序主窗口句柄

// 返回值: 
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR.
//          其他情况下返回ERR_SUCCED.
VRESULT ShowPK(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    unsigned short szBufferStr[MAX_STRSIZE];

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    wsprintf(
        szBufferStr,
        L"hInstance=0x%08X\nhFile=0x%08X\nhFileMap=0x%08X\nlpMemory=0x%08X\ndwStatu=%d\ndwFlag=0x%08X",
        pkp->hInstance, pkp->hFile, pkp->hFileMap, pkp->lpMemory, pkp->dwStatu, pkp->dwFlag
    );
    MessageBox(hWnd, szBufferStr, L"ShowPK", MB_OK);
	
	wsprintf(
		szBufferStr,
		L"szFilePath=%s\nFileName=%s\nszProfilePath=%s\n",
		pkp->szFilePath, pkp->szFileName, pkp->szProfilePath
	);
	MessageBox(hWnd, szBufferStr, L"ShowPK", MB_OK);
	
    return ERR_SUCCED;
}

// 显示校验和运算结果结构体函数: ShowCS

// 参数:
// COMPRESS_STUB_PTR csp: 欲显示的校验和运算结果结构体
// HWND hWnd: 程序主窗口句柄

// 返回值: 
// VRESULT: 如果出现了指针错误, 则返回ERR_POINTERR.
//          其他情况下返回ERR_SUCCED.
VRESULT ShowCS(COMPRESS_STUB_PTR csp, HWND hWnd)
{
    unsigned short szBufferStr[MAX_STRSIZE];

    if(csp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    wsprintf(
        szBufferStr,
        L"%08X, %08X, %d\n%d\n%d, %d, %d\n",
        csp->checksum0, csp->checksum1, csp->filesize, csp->ct_callback, csp->time0, csp->time1, csp->time_cb
    );
    MessageBox(hWnd, szBufferStr, L"ShowCS", MB_OK);

    return ERR_SUCCED;
}

#endif
