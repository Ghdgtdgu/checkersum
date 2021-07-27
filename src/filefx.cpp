// Ghdgtdgu Studio Vo. 4 - CheckerSum
// by Ghdgtdgu, July 2021

// filefx.cpp
// 文件操作以及校验和计算实现. 最困难的部分.

#include    "checkersum.h"

// 计算校验和线程参数结构体 (struct tagCOMPRESS_ARGS|COMPRESS_ARGS|*COMPRESS_ARGS_PTR)
// 建议缩写: ca, *cap

// HWND hWnd: 程序主窗口句柄
// LPVOID lpMemory: 内存地址
// DWORD dwMemSize: 内存容量
// COMPRESS_STUB_PTR csp: 欲存放结果的结构体
// FXCPROC fxcProc: 回调函数. 如果为NULL则不去调用.
// DWORD dwCBTime: 每计算了多少容量的内存之后调用一次回调函数. 必须为4的倍数.
//                 如果不是4的倍数, 本程序会对该数字除以4.
// LPARAM lParam: 回调函数的参数.
struct tagCOMPRESS_ARGS {
    HWND hWnd;
    LPVOID lpMemory;

    DWORD dwMemSize, dwCBTime;
    FXCPROC fxcProc;

    COMPRESS_STUB_PTR csp;
    LPARAM lParam;
} typedef COMPRESS_ARGS, *COMPRESS_ARGS_PTR;

COMPRESS_ARGS ca;

// 判断文件大小函数: ChkFileSize

// 参数:
// LPCWSTR lpszFilePath: 文件路径
// HWND hWnd: 程序主窗口

// 返回值:
// VRESULT: 如果出现空指针, 则返回ERR_POINTERR.
//          如果文件打开错误, 则返回ERR_FILERW.
//          如果文件大小超过2GB, 则返回ERR_FILESIZE.
//          否则返回ERR_SUCCED.
VRESULT ChkFileSize(LPCWSTR lpszFilePath, HWND hWnd)
{
    HANDLE hFile;
    unsigned int fsize;

    if(lpszFilePath == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    }

    hFile = CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE) {
        return ErrMsgCallBack(ERR_FILERW, hWnd, 0);
    }

    fsize = SetFilePointer(hFile, 0, NULL, FILE_END);
    if(fsize >= 0x80000000 || fsize == 0) {
        return ErrMsgCallBack(ERR_FILESIZE, hWnd, 0);
    }

    CloseHandle(hFile);
    return ERR_SUCCED;
}

// 线程原型启动函数: InstFileFx

// 参数:
// PROGRAM_KERNEL_PTR pkp: 程序中央结构体
// HWND hWnd: 程序主窗口句柄

// 返回值:
// VRESULT: 如果出现空指针, 则返回ERR_POINTERR.
//          如果文件打开错误, 则返回ERR_FILERW.
//          否则返回ERR_SUCCED.
VRESULT InstFileFx(PROGRAM_KERNEL_PTR pkp, HWND hWnd)
{
    HANDLE hFile, hFileMap;
    LPVOID lpMemory;
    FXCPROC fxcProc0;
    int fsize, idle, cbtime;

    if(pkp == NULL) {
        return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
    } else {
        if(&(pkp->cs) == NULL) {
            return ErrMsgCallBack(ERR_POINTERR, hWnd, 0);
        }
    }

    hFile = CreateFile(pkp->szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE) {
err:
        return ErrMsgCallBack(ERR_FILERW, hWnd, 0);
    }

    fsize = GetFileSize(hFile, NULL);

    hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if(hFileMap == NULL) {
err2:
        CloseHandle(hFile);
        goto err;
    }

    lpMemory = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
    if(lpMemory == NULL) {
        CloseHandle(hFileMap);
        goto err2;
    }

    pkp->hFile = hFile;
    pkp->hFileMap = hFileMap;
    pkp->lpMemory = lpMemory;
    pkp->dwStatu = STU_COMPRESSING;
    RefreshUI(pkp, hWnd);

	if(fsize <= 4096) {
		cbtime = CBT_COMMON;
	} else {
		cbtime = CBT_4K;
	}
	
    if((pkp->dwFlag & FLG_PROGRESS) == FLG_PROGRESS) {
        fxcProc0 = (FXCPROC) ProcFxProgress;
        
        idle = fsize / cbtime;
        if(fsize % cbtime!= 0) {
            idle++;
        }

        SendDlgItemMessage(hWnd, IDC_PROGRESS1, PBM_SETRANGE32, NULL, (LPARAM)idle);
    } else {
        fxcProc0 = NULL;
    }

    Proto_FxCheckSum(hWnd, lpMemory, fsize, &(pkp->cs), fxcProc0, cbtime, (LPARAM) hWnd);

    return ERR_SUCCED;
}

// 计算校验和线程原型: Proto_FxCheckSum

// 参数:
// HWND hWnd: 程序主窗口句柄
// LPVOID lpMemory: 内存地址
// DWORD dwMemSize: 内存容量
// COMPRESS_STUB_PTR csp: 欲存放结果的结构体
// FXCPROC fxcProc: 回调函数. 如果为NULL则不去调用.
// DWORD dwCBTime: 每计算了多少容量的内存之后调用一次回调函数. 必须为4的倍数.
//                 如果不是4的倍数, 本程序会对该数字除以4.
// LPARAM lParam: 回调函数的参数.

// 返回值: 
// VRESULT: 保证返回ERR_SUCCED.
//          请一定要确认csp和lpMemory可用!
VRESULT Proto_FxCheckSum(HWND hWnd, LPVOID lpMemory, DWORD dwMemSize, COMPRESS_STUB_PTR csp, FXCPROC fxcProc, DWORD dwCBTime, LPARAM lParam)
{
    RtlZeroMemory(&ca, sizeof(COMPRESS_ARGS));
    ca.hWnd = hWnd;
    ca.lpMemory = lpMemory;
    ca.dwMemSize = dwMemSize;
    ca.csp = csp;
    ca.fxcProc = fxcProc;
    ca.dwCBTime = dwCBTime-(dwCBTime%4);
    ca.lParam = lParam;

    CreateThread(NULL, 0, FxCheckSum, (LPVOID)&ca, NULL, NULL);

    return ERR_SUCCED;
}

// 计算校验和线程函数: FxCheckSum (保留)

// 参数:
// LPVOID lpParam: 从CreateThread返回的参数

// 返回值:
// DWORD: 未知. 请返回0;

DWORD WINAPI FxCheckSum(LPVOID lpParam)
{
    COMPRESS_ARGS_PTR cap;
    DWORD i, j;

    DWORD checksum;
    DWORD checksum0, checksum1;
    DWORD time0, time1, time_cb;
    DWORD ct_callback;
    DWORD tick, tickold;

	unsigned char *lpMemoryByte;

    cap = (COMPRESS_ARGS_PTR)lpParam;
    checksum = 0;
    time0 = 0;
    time1 = 0;
    time_cb = 0;
    ct_callback = 0;

    j = 0;

	lpMemoryByte = (unsigned char *)cap->lpMemory;
    
    for(i = 0; i < cap->dwMemSize; i += 4) {
        tickold = GetTickCount();

        checksum += get32(&lpMemoryByte[i]);
        j += 4;

        if(j >= cap->dwCBTime) {
            tick = GetTickCount();
            time0 += (tick - tickold);
            time1 += (tick - tickold);

            //checksum &= 0xffffffff00000000;
            j = 0;

            if(cap->fxcProc != NULL) {
                tickold = GetTickCount();

                ct_callback++;
                cap->fxcProc(cap->lParam, cap->dwMemSize, ct_callback*cap->dwCBTime, cap->dwCBTime);

                tick = GetTickCount();
                time0 += (tick - tickold);
                time_cb += (tick - tickold);
            }
        }
    }

    if(cap->fxcProc != NULL) {
        tickold = GetTickCount();

        ct_callback++;
        cap->fxcProc(cap->lParam, cap->dwMemSize, ct_callback*cap->dwCBTime, cap->dwCBTime);

        tick = GetTickCount();
        time0 += (tick - tickold);
        time_cb += (tick - tickold);
    }

    checksum0 = (DWORD) checksum;
    checksum1 = checksum0 + cap->dwMemSize;

    cap->csp->checksum0 = checksum0;
    cap->csp->checksum1 = checksum1;
    cap->csp->filesize = cap->dwMemSize;
    cap->csp->ct_callback = ct_callback;
    cap->csp->time0 = time0;
    cap->csp->time1 = time1;
    cap->csp->time_cb = time_cb;

    SendMessage(cap->hWnd, WM_NOTIFY, (WPARAM)1, (LPARAM)CTRL_FINISH);

    return 0;
}

// 线程回调程序: ProcFxProgress

// 参数:
// LPARAM lParam: 线程传来的参数
// DWORD dwMaxium: 内存容量
// DWORD dwCurrent: 已处理大小
// DWORD dwCBTime: 处理频率

// 返回值:
// int: 保证返回0.
int ProcFxProgress(LPARAM lParam, DWORD dwMaxium, DWORD dwCurrent, DWORD dwCBTime)
{
    int dx;
    HWND hDlg;

    dx = dwCurrent / dwCBTime;
    if(dwCurrent % dwCBTime!= 0) {
        dx++;
    }

    hDlg = (HWND) lParam;
    SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETPOS, (WPARAM)dx, NULL);

    return 0;
}
