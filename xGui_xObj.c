

struct xGui *
xGui_Add_Ex(struct xGui * xSup, TCHAR * Flag, int xLen, TCHAR * CName, TCHAR * WName,
        int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * xNew, * xOwn, * xTop;

    xOwn = xSup;
    while (xOwn->bPan == 0) {
        xOwn = xOwn->xSup;
    }

    if (!xSup->bPan) {
        RECT rc = { 0, 0, 0, 0 };
        GetWindowRect(xSup->hwnd, &rc);
        MapWindowPoints(NULL, xOwn->hwnd, (LPPOINT) &rc, 2);
        l += rc.left;
        t += rc.top;
    }

    xNew = calloc(1, sizeof(struct xGui) + xLen);
    xNew->Flag = Flag;
    xNew->xSup = xSup;
    xTop = xSup->xTop;
    xNew->xTop = xTop;
    xNew->xOwn = xOwn;

    xNew->hwnd = CreateWindowEx(EX, CName, WName, WS | WS_VISIBLE,
            l, t, w, h,
            xOwn->hwnd, NULL, xTop_Inst(xSup), xNew);

    if (_tcscmp(xNew->Flag, _T("WPan")) != 0) {
        xNew->defproc = (WNDPROC) GetWindowLongPtr(xNew->hwnd, GWL_WNDPROC);
        SetWindowLongPtr(xNew->hwnd, GWL_USERDATA, (LONG) xNew);
        SetWindowLongPtr(xNew->hwnd, GWL_WNDPROC, (LONG) xGui_Proc);
    }

    SendMessage(xNew->hwnd, WM_SETFONT, (WPARAM) xTop_Font(xNew), TRUE);

    return xNew;
}


int
xGui_Show(struct xGui * this)
{
    return ShowWindow(this->hwnd, SW_SHOW);
}


int
xGui_Hide(struct xGui * this)
{
    return ShowWindow(this->hwnd, SW_HIDE);
}


int
xGui_Set_Able(struct xGui * this, int bVal)
{
    return EnableWindow(this->hwnd, bVal);
}


int
xGui_Set_Text(struct xGui * this, const TCHAR * Text)
{
    SetWindowText(this->hwnd, Text);
    if (IsWindowVisible(this->hwnd) && _tcscmp(this->Flag, XGUI) != 0) {
        ShowWindow(this->hwnd, SW_HIDE);
        ShowWindow(this->hwnd, SW_SHOW);
    }
    return 1;
}

/*
int
xGui_Set_Font(struct xGui * this, int Color, int Style, int Size, const TCHAR * Face)
{
    if (this->Method_Set_Font) {
        return this->Method_Set_Font(this, Color, Style, Size, Face);
    }
    return 0;
}
*/
/*
int
xGui_On_Click(struct xGui * this, void * Func)
{
    if (this->Method_On_Click) {
        return this->Method_On_Click(this, Func);
    }
    return 0;
}
*/

int
xGui_On_Close(struct xGui * this, void * Func)
{
    if (_tcscmp(this->Flag, XGUI) == 0) {
        xGui_xPtr(struct xWnd, this)->On_Close = Func;
        return 1;
    }
    return 0;
}


int
xGui_Del(struct xGui * this)
{
    if (this) {
        return DestroyWindow(this->hwnd);
    }
    return 0;
}


void
xGui_Close(struct xGui * this)
{
    SendMessage(this->hwnd, WM_CLOSE, 0, 0);
}


int
xGui_UpdateBkBr(struct xGui * this)
{
    HWND hwnd;
    HDC hdc, hdcMem;
    RECT rc;
    HBITMAP hBmpNew;
    HBITMAP hBmpOld;

    hwnd = this->hwnd;
    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);
    GetWindowRect(hwnd, &rc);
    hBmpNew = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    hBmpOld = (HBITMAP) SelectObject(hdcMem, hBmpNew);
    SendMessage(hwnd, WM_PRINT, (WPARAM) hdcMem, (LPARAM)(PRF_ERASEBKGND | PRF_CLIENT));

    if (this->hbrBkgnd) {
        DeleteObject(this->hbrBkgnd);
    }
    this->hbrBkgnd = CreatePatternBrush(hBmpNew);

    SelectObject(hdcMem, hBmpOld);
    DeleteObject(hBmpNew);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);

    return 1;
}


int
xGui_UpdateBkDc(struct xGui * this)
{
    HDC hdc, hdcMem;
    RECT rc;
    HBITMAP hBmpNew;
    HBITMAP hBmpOld;
    HWND hwnd;

    hwnd = this->hwnd;
    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);
    GetWindowRect(hwnd, &rc);
    hBmpNew = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    hBmpOld = (HBITMAP) SelectObject(hdcMem, hBmpNew);
    SendMessage(hwnd, WM_PRINT, (WPARAM) hdcMem, (LPARAM)(PRF_ERASEBKGND | PRF_CLIENT));

    if (this->hdcBkgnd) {
        DeleteObject(SelectObject(this->hdcBkgnd, this->hBmpOld));
        DeleteDC(this->hdcBkgnd);
    }
    this->hdcBkgnd = hdcMem;
    this->hBmpOld = hBmpOld;

    ReleaseDC(hwnd, hdc);

    return 1;
}
