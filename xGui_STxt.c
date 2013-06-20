

#define STXT _T("STxt")
#define STXT_WS WS_CHILD// | WS_CLIPSIBLINGS
#define STXT_EX 0
#define xGui_Add_STxt(xSup, l, t, w, h, Text) \
        xGui_Add_STxt_Ex(xSup, l, t, w, h, Text, STXT_WS, STXT_EX)


struct STxt {
    HFONT hFont;
    COLORREF color;
};


int
STxt_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCDESTROY) {
        DeleteObject(xGui_xPtr(struct STxt, this)->hFont);
    }
    return 0;
}


int
STxt_On_Color(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SetBkMode((HDC) wParam, TRANSPARENT);
    SetTextColor((HDC) wParam, xGui_xPtr(struct STxt, this)->color);
    this->lResult = (LRESULT) GetStockObject(NULL_BRUSH);
    return 1;
}


struct xGui *
xGui_Add_STxt_Ex(struct xGui * xSup, int l, int t, int w, int h, TCHAR * Text, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, STXT, sizeof(struct STxt), WC_STATIC, Text,
            l, t, w, h, WS, EX);
    this->Own_On_Proc = STxt_On_Proc;
    this->Own_On_Color = STxt_On_Color;
    return this;
}


int
STxt_Set_Font(struct xGui * this, const TCHAR * Face, int Size, int Style)
{
    if (_tcscmp(this->Flag, STXT) == 0) {
        struct STxt * xPtr = xGui_xPtr(struct STxt, this);
        LOGFONT lf;
        GetObject(xTop_Font(this), sizeof(LOGFONT), &lf);
        if (Size) {
            HDC hDC = GetDC(this->hwnd);
            lf.lfHeight = -MulDiv(Size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
            ReleaseDC(this->hwnd, hDC);
        }
        lf.lfWidth = 0;
        lf.lfWeight = (Style & X_FONTSTYLE_B) ? FW_BOLD : FW_NORMAL;
        lf.lfItalic = (Style & X_FONTSTYLE_I) ? TRUE : FALSE;
        lf.lfUnderline = (Style & X_FONTSTYLE_U) ? TRUE : FALSE;
        lf.lfCharSet = DEFAULT_CHARSET;
        if (Face && _tcslen(Face)) {
            _tcsncpy(lf.lfFaceName, Face, sizeof(TCHAR) * LF_FACESIZE - 1);
        }
        if (xPtr->hFont) {
            DeleteObject(xPtr->hFont);
        }
        xPtr->hFont = CreateFontIndirect(&lf);
        SendMessage(this->hwnd, WM_SETFONT, (WPARAM) xPtr->hFont, TRUE);
        if (IsWindowVisible(this->hwnd)) {
            ShowWindow(this->hwnd, SW_HIDE);
            ShowWindow(this->hwnd, SW_SHOW);
        }
        return 1;
    }
    return 0;
}


int
STxt_Set_Color(struct xGui * this, int Color)
{
    if (_tcscmp(this->Flag, STXT) == 0) {
        xGui_xPtr(struct STxt, this)->color =
                (Color >> 16 & 0x0000FF) | (Color & 0x00FF00) | (Color << 16 & 0xFF0000);
        return 1;
    }
    return 0;
}
