// Ghdgtdgu Studio Vo.4 - CheckerSum
// by Ghdgtdgu, July 2021

// winmain.cpp
// 程序的入口, 一切的起点, 顶点.

#include    "checkersum.h"

// 程序入口函数(保留): WinMain

// 参数:
// HINSTANCE hInstance: 程序句柄
// HINSTANCE hPrevInstance: 保留.
// LPSTR lpszCmdLine: 命令行参数
// int nCmdShow: 保留.

// 返回值:
// int: 程序默认返回值.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

    InitCommonControls();
	InstEasterEgg();
    DialogBoxParam(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)ProcWinMain, NULL);
	
    return ERR_SUCCED;
}

// 窗口回调函数(保留): ProcWinMain

// 参数:
// HWND hWnd: 代表窗口的句柄.
// UINT uMessage: 本次消息.
// WPARAM wParam: 参数1.
// LPARAM lParam: 参数2.

// 返回值:
// BOOL: 如果消息在程序内部被捕获, 则返回FALSE; 否则返回TRUE.
BOOL CALLBACK ProcWinMain(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static PROGRAM_KERNEL pk;

    switch(uMessage) {
        case WM_COMMAND:
            switch(wParam) {
                case IDM_ABOUT:
                    AboutMsg(hWnd);
                    break;

                case IDM_QUIT:
                    SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                    break;
					
				case IDM_RESET:
					MessageBox(hWnd, L"Setting options will now be reset.\nThe program is about to close.", L"Reset", MB_OK);
					DeleteFile(pk.szProfilePath);
					ExitProcess(ERR_SUCCED);
					break;

                case IDC_CHECKBOX1:
                    CheckedControl(&pk, hWnd);
                    break;

				case IDC_BUTTON1:
					InstFileName(&pk, hWnd);
					break;

                case IDC_BUTTON2:
                    InstFileFx(&pk, hWnd);
                    break;

                case IDC_BUTTON3:
                    DialogBoxParam(pk.hInstance, MAKEINTRESOURCE(DLG_LOGVIEW), hWnd, (DLGPROC)LogViewProc, (LPARAM)&pk);
                    break;

                #ifdef  __CHECK__
                case IDM_NULL:
                    ShowPK(&pk, hWnd);
                    ShowCS(&(pk.cs), hWnd);
                    break;
                #endif
            }
            break;

        case WM_NOTIFY:
            if(wParam == (WPARAM)1) {
                switch(lParam) {
                    case CTRL_FINISH:
                        UnmapViewOfFile(pk.lpMemory);
                        CloseHandle(pk.hFileMap);
                        CloseHandle(pk.hFile);

                        pk.dwStatu = STU_COMPRESSED;
                        RefreshUI(&pk, hWnd);

                        break;
                }
            }
            break;

        case WM_INITDIALOG:
            InstProgramKernel(&pk, hWnd);
			InstUI(&pk, hWnd);
            break;

        case WM_CLOSE:
            if(pk.dwStatu != STU_COMPRESSING) {
                ConfigSave(&pk, hWnd);
                EndDialog(hWnd, ERR_SUCCED);
            }
            
            break;

        default:
            return FALSE;
    }

    return TRUE;
}
