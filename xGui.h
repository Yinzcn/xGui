

#include <tchar.h>


#ifndef WINVER
#define WINVER 0x0501
#endif

#include <windows.h>
#include <process.h>

#include <windowsx.h>

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif
#include <commctrl.h>
#include <richedit.h>


#define XGUI _T("xWnd")


#define X_FONTSTYLE_N 0x01
#define X_FONTSTYLE_B 0x02
#define X_FONTSTYLE_I 0x04
#define X_FONTSTYLE_U 0x08


struct xGui;


struct xGui
{
    HWND hwnd;
    struct xGui * xTop; /* 最顶窗口 */
    struct xGui * xOwn; /* 从属窗口 */
    struct xGui * xSup; /* 上级窗口 */
    TCHAR * Flag; /* 用于类别标识 */
    WNDPROC defproc;
    LRESULT lResult;
    HBRUSH hbrBkgnd; /* [ for WM_CTLCOLORxxx */
    HBITMAP hBmpOld; /* ] */
    HDC hdcBkgnd;
    int bPan;

    int (* Own_On_Command)(struct xGui * this, UINT uMsg, WPARAM, LPARAM);
    int (* Own_On_Notify)(struct xGui * this, UINT uMsg, WPARAM, LPARAM);
    int (* Own_On_Color)(struct xGui * this, UINT uMsg, WPARAM, LPARAM);
    int (* Own_On_Proc)(struct xGui * this, UINT uMsg, WPARAM, LPARAM);

    void (* On_Destroy)(struct xGui * this);
    void (* On_NCDestroy)(struct xGui * this);

    void * udat;
};


struct xWnd {
    int l;
    int t;
    HFONT hFont;
    HINSTANCE hInst;
    int (* On_Close)(struct xGui *);
};


#define xGui_xPtr(Type, xObj) ((Type *)(xObj + 1))
#define xTop_Inst(xObj) (xGui_xPtr(struct xWnd, xObj->xTop))->hInst
#define xTop_Font(xObj) (xGui_xPtr(struct xWnd, xObj->xTop))->hFont


LRESULT CALLBACK
xGui_Proc(HWND, UINT, WPARAM, LPARAM);


HWND xG_Curr_Hwnd = NULL;
BOOL xG_xGui_Init = FALSE;
