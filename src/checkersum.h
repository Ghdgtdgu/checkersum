// Ghdgtdgu Studio Vo.4 - CheckerSum
// by Ghdgtdgu, July 2021

// checkersum.h
// 头文件. 是程序的核心.

#include    <windows.h>
#include    "resources.h"      //为了能够使用资源ID

// 通用控件(comctl32.dll)
#include    <commctrl.h>
#pragma     comment (lib, "comctl32.lib")

// 通用对话框(comdlg32.dll)
#include       <commdlg.h>
#pragma     comment (lib, "comdlg32.lib")

#define     VERSION_BUILD           12      // 12=Finial Build!!!!!

// 类型性质的定义
typedef     int     VRESULT;
typedef     int     (*FXCPROC) (LPARAM, DWORD, DWORD, DWORD);

// 一般的定义
#define     MAX_STRSIZE         400
#define     MAX_PATHSIZE        512
#define     MAX_TINYSIZE        40
#define     MAX_LARGESIZE       2048

#define     CFG_KEYLOST         9999        // 该数字键值并不存在或者缺失

#define     CTRL_FINISH         1000
#define     CTRL_EASTEREGG      1234        // 彩蛋

#define     CBT_COMMON          32
#define		CBT_4K				2048

// 错误定义
#define     ERR_COUNT           5
#define     ERR_SUCCED          0       // 程序成功运行
#define     ERR_UNKNOWN         1       // 出现未知错误
#define     ERR_POINTERR        2       // 指针错误
#define     ERR_FILERW          3       // 文件读写错误
#define     ERR_FILESIZE        4       // 文件大小超过2GB

// PROGRAM_KERNEL中的dwFlag的位标志定义
#define     FLG_PROGRESS        0x00000001  // 启用进度条

// PROGRAM_KERNEL中的dwStatus的数值定义
#define     STU_BOOT            1       // 初始化完毕
#define     STU_SELECTED        2       // 已经选择了文件, 但没有开始运算
#define     STU_COMPRESSING     3       // 已经选择了文件, 并且正在运算
#define     STU_COMPRESSED      4       // 已经选择了文件, 并且已经运算完毕

// errmsg.cpp中的ErrMsgCallBack函数参数dwFlag的数值定义
#define     EMCB_EXIT           0x00000001  // 出现严重错误. 必须退出程序.
#define     EMCB_IMVISIBLE      0x00000002  // 不显示错误窗口

// 校验和结果结构体 (struct tagCOMPRESS_STUB:COMPRESS_STUB:*COMPRESS_STUB_PTR)
// 建议缩写: cs, *csp

// unsigned int checksum0: 没有加上文件大小的校验和
// unsigned int checksum1: 加上了文件大小的校验和
// unsigned int filesize: 文件大小
// int ct_callback: 调用回调函数的次数
// int time0: 计算耗用的总时间 (单位:ms, 下同)
// int time1: 计算过程耗用的总时间, 不包括调用回调函数
// int time_cb: 调用回调函数耗用的总时间
struct tagCOMPRESS_STUB {
    unsigned int checksum0, checksum1, filesize;
    int ct_callback;
    int time0, time1, time_cb;
} typedef COMPRESS_STUB, *COMPRESS_STUB_PTR;

// 程序中央结构体 (struct tagPROGRAM_KERNEL:PROGRAM_KERNEL:*PROGRAM_KERNEL_PTR)
// 建议缩写: pk, *pkp

// HINSTANCE hInstance: 程序exe句柄
// HANDLE hFile, hFileMap: 临时句柄, 需要及时回收
// LPVOID lpMemory: 需要及时回收
// int dwFlag: 程序的设定属性, 定义见上
// int dwStatu: 程序此时的状态, 定义见上
// unsigned short szFileName[]: 用于存放选择的文件名
// unsigned short szFilePath[]: 用于存放选择的文件路径
// unsigned short szProfilePath[]; 用于存放配置文件路径名称
// COMPRESS_STUB cs: 用于存放运算结果 
struct tagPROGRAM_KERNEL {
    HINSTANCE hInstance;
    HANDLE hFile, hFileMap;
    LPVOID lpMemory;

    int dwFlag, dwStatu;
    unsigned short szFileName[MAX_STRSIZE], szFilePath[MAX_PATHSIZE];
    unsigned short szProfilePath[MAX_PATHSIZE];

    COMPRESS_STUB cs;
} typedef PROGRAM_KERNEL, *PROGRAM_KERNEL_PTR;

//函数区 (ALL:23/25)
// winmain.cpp (2)
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);
BOOL CALLBACK ProcWinMain(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

// install.cpp (4)
VRESULT InstProgramKernel(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT InstUI(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT RefreshUI(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT InstFileName(PROGRAM_KERNEL_PTR pkp, HWND hWnd);

// errmsg.cpp (2)
VRESULT ErrMsgCallBack(VRESULT vrsl, HWND hWnd, int dwFlag);
VRESULT ErrMsgShow(VRESULT vrsl, HWND hWnd, int dwExit);

// easteregg.cpp (2)
VRESULT InstEasterEgg(void);
BOOL ProcEasterEgg(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

// config.cpp (2)
VRESULT ConfigApply(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT ConfigSave(PROGRAM_KERNEL_PTR pkp, HWND hWnd);

// filefx.cpp (5)
VRESULT ChkFileSize(LPCWSTR lpszFilePath, HWND hWnd);
VRESULT InstFileFx(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT Proto_FxCheckSum(HWND hWnd, LPVOID lpMemory, DWORD dwMemSize, COMPRESS_STUB_PTR csp, FXCPROC fxcProc, DWORD dwCBTime, LPARAM lParam);
DWORD WINAPI FxCheckSum(LPVOID lpParam);
int ProcFxProgress(LPARAM lParam, DWORD dwMaxium, DWORD dwCurrent, DWORD dwCBTime);

// misc.cpp (3)
VRESULT CheckedControl(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT AboutMsg(HWND hWnd);
int get32(unsigned char *buffer);

// logview.cpp(3)
BOOL WINAPI LogViewProc(HWND hLogView, UINT uMessage, WPARAM wParam, LPARAM lParam);
VRESULT LogViewSetText(HWND hWnd, PROGRAM_KERNEL_PTR pkp);
VRESULT LogViewSaveFile(HWND hWnd, PROGRAM_KERNEL_PTR pkp);

#ifdef  __CHECK__
// check.cpp(2)
VRESULT ShowPK(PROGRAM_KERNEL_PTR pkp, HWND hWnd);
VRESULT ShowCS(COMPRESS_STUB_PTR csp, HWND hWnd);
#endif