// Ghdgtdgu Studio Vo. 4 - CheckerSum
// by Ghdgtdgu, July 2021

// easteregg.cpp
// 彩蛋

#include "checkersum.h"

WNDPROC function1;

// 设置彩蛋函数: InstEasterEgg

// 参数:
// (void)

// 返回值:
// VRESULT. 保证返回ERR_SUCCED.

VRESULT InstEasterEgg(void)
{
    WNDCLASSEX stWndClassEx1;
	
	RtlZeroMemory(&stWndClassEx1, sizeof(WNDCLASSEX));
	stWndClassEx1.cbSize = sizeof(WNDCLASSEX);
	
	GetClassInfoEx(NULL, L"edit", &stWndClassEx1);
	
	function1 = stWndClassEx1.lpfnWndProc;
	
    stWndClassEx1.style |= CS_DBLCLKS;
	stWndClassEx1.lpszClassName = L"edit_00xx";
	stWndClassEx1.hInstance = GetModuleHandle(NULL);
	stWndClassEx1.lpfnWndProc = (WNDPROC) ProcEasterEgg;
	RegisterClassEx(&stWndClassEx1);
	
	return ERR_SUCCED;
}

// 彩蛋回调函数: ProcEasterEgg (保留)

// 参数:
// HWND hWnd: 代表窗口的句柄.
// UINT uMessage: 本次消息.
// WPARAM wParam: 参数1.
// LPARAM lParam: 参数2.

// 返回值:
// BOOL: 返回CallWindowProc的返回值.
BOOL ProcEasterEgg(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static int vest = 0;

    switch(uMessage) {
        case WM_LBUTTONDBLCLK:
			vest++;

            if(vest == 7) {
                MessageBox(NULL, L"Easter Egg!", L"CheckerSum", MB_OK);
            }
            break;

		case WM_CHAR:
			return TRUE;
    }

    return CallWindowProc(function1, hWnd, uMessage, wParam, lParam);
}