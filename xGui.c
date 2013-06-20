

#include "xGui.h"
#include "xRes.c"

#include "xGui_xDbg.c"
#include "xGui_Proc.c"
#include "xGui_xObj.c"

#define XGUI_WS \
    WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE// | WS_CLIPCHILDREN
#define XGUI_EX WS_EX_CONTROLPARENT
#define xGui_New(l, t, w, h, Name) \
        xGui_New_Ex(l, t, w, h, Name, XGUI_WS, XGUI_EX)


void
xGui_Init()
{
    if (xG_xGui_Init) {
        return;
    }

    do {
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_STANDARD_CLASSES | ICC_UPDOWN_CLASS | ICC_LISTVIEW_CLASSES | ICC_USEREX_CLASSES | ICC_TAB_CLASSES;
        InitCommonControlsEx(&icex);
    } while (0);

    do {
        WNDCLASSEX wcex;
        ZeroMemory(&wcex, sizeof(wcex));
        wcex.cbSize = sizeof(wcex);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = xGui_Proc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(100));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = XGUI;
        wcex.hIconSm = NULL;
        RegisterClassEx(&wcex);
    } while (0);

    xG_xGui_Init = TRUE;
}


int
xGui_Def_On_Close(struct xGui * this)
{
    xGui_Del(this);
    PostQuitMessage(0);
    return 1;
}


int
xGui_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
        {
            RECT rc;
            int w, h;
            GetWindowRect(this->hwnd, &rc);
            w = rc.right - rc.left;
            h = rc.bottom - rc.top;
            if (xGui_xPtr(struct xWnd, this)->l == -1) {
                rc.left = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
            }
            if (xGui_xPtr(struct xWnd, this)->t == -1) {
                rc.top = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
            }
            SetWindowPos(this->hwnd, NULL, rc.left, rc.top, w, h, SWP_SHOWWINDOW);
            break;
        }
        case WM_ACTIVATE:
        {
            xG_Curr_Hwnd = this->hwnd;
            break;
        }
        case WM_CLOSE:
        {
            if (xGui_xPtr(struct xWnd, this)->On_Close &&
                xGui_xPtr(struct xWnd, this)->On_Close(this)) {
                return this->lResult;
            }
            break;
        }
        default:
            break;
    } /* switch */

    return 0;
}


struct xGui *
xGui_New_Ex(int l, int t, int w, int h, TCHAR * Name, DWORD WS, DWORD EX)
{
    struct xGui * this;
    struct xWnd * xPtr;
    LOGFONT lf = { 0 };
    RECT awr = { 0, 0, w, h };

    xGui_Init();

    AdjustWindowRectEx(&awr, WS, FALSE, EX);
    this = calloc(1, sizeof(struct xGui) + sizeof(struct xWnd));
    this->Flag = XGUI;
    this->xTop = this;
    this->xOwn = this;

    xPtr = xGui_xPtr(struct xWnd, this);
    xPtr->l = l;
    xPtr->t = t;
    xPtr->hInst = GetModuleHandle(NULL);
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    lf.lfHeight = -12;
    xPtr->hFont = CreateFontIndirect(&lf);
    //xPtr->On_Close = xGui_Def_On_Close;

    this->Own_On_Proc = xGui_On_Proc;

    this->hwnd = CreateWindowEx(EX, XGUI, Name, WS,
            l, t, awr.right - awr.left, awr.bottom - awr.top,
            HWND_DESKTOP, NULL, xPtr->hInst, this);
    SendMessage(this->hwnd, WM_SETFONT, (WPARAM) xPtr->hFont, TRUE);

    this->bPan = 1;
    xGui_UpdateBkBr(this);
    xGui_UpdateBkDc(this);

    return this;
}


int
xGui_Msg_Loop(void)
{
    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        if (xG_Curr_Hwnd == NULL || !IsDialogMessage(xG_Curr_Hwnd, &Msg)) {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }
    return Msg.wParam;
}


void
xGui_Quit(void)
{
    PostQuitMessage(0);
}


#include "xGui_MBox.c"
#include "xGui_WPan.c"
#include "xGui_STxt.c"
#include "xGui_SIco.c"
#include "xGui_SImg.c"
#include "xGui_Butn.c"
#include "xGui_GBox.c"
#include "xGui_CBox.c"
#include "xGui_RBtn.c"
#include "xGui_Edit.c"
#include "xGui_LBox.c"
#include "xGui_LsVw.c"
#include "xGui_TabC.c"
#include "xGui_SciL.c"
