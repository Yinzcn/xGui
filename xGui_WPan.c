

#define WPAN _T("WPan")
#define WPAN_WS WS_CHILD | WS_TABSTOP
#define WPAN_EX WS_EX_CONTROLPARENT
#define xGui_Add_WPan(xSup, l, t, w, h) \
        xGui_Add_WPan_Ex(xSup, l, t, w, h, WPAN_WS, WPAN_EX)


int
WPan_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_ERASEBKGND) {
        RECT rc;
        GetWindowRect(this->hwnd, &rc);
        MapWindowPoints(NULL, this->xOwn->hwnd, (LPPOINT)(&rc), 2);
        BitBlt((HDC) wParam, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                this->xOwn->hdcBkgnd, rc.left, rc.top, SRCCOPY);
        this->lResult = 1;
        return 1;
    }
    return 0;
}


struct xGui *
xGui_Add_WPan_Ex(struct xGui * xSup, int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, WPAN, sizeof(struct xWnd), XGUI, _T(""),
            l, t, w, h, WS, EX);
    this->Own_On_Proc = WPan_On_Proc;

    RECT gwr = { 0, 0, 0, 0 };
    RECT awr = { 0, 0, w, h };
    AdjustWindowRectEx(&awr, WS, FALSE, EX);
    POINT cts = { 0, 0 };
    GetWindowRect(this->hwnd, &gwr);
    ClientToScreen(this->hwnd, &cts);
    MoveWindow(this->hwnd,
            l + (cts.x - gwr.left), t + (cts.y - gwr.top),
            awr.right - awr.left, awr.bottom - awr.top,
            TRUE);
    this->bPan = 1;
    xGui_UpdateBkBr(this);
    xGui_UpdateBkDc(this);
    return this;
}
