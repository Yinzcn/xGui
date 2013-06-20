

#define LBOX _T("LBox")
#define LBOX_WS WS_CHILD | WS_TABSTOP | LBS_EXTENDEDSEL | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY// | WS_CLIPSIBLINGS
#define LBOX_EX WS_EX_CLIENTEDGE
#define xGui_Add_LBox(xSup, l, t, w, h) \
        xGui_Add_LBox_Ex(xSup, l, t, w, h, LBOX_WS, LBOX_EX)


struct xGui *
xGui_Add_LBox_Ex(struct xGui * xSup, int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * xNew;
    xNew = xGui_Add_Ex(xSup, LBOX, 0, WC_LISTBOX, _T(""),
            l, t, w, h, WS, EX);
    return xNew;
}


void
xGui_LBox_SetRedraw(struct xGui * this, int state)
{
    SendMessage(this->hwnd, WM_SETREDRAW, state, 0);
}


int
xGui_LBox_UpdateHScroll(struct xGui * this)
{
    if (_tcscmp(this->Flag, LBOX) == 0) {
        HWND hwnd;
        HDC hDC;
        int i;
        int n;
        int m = 0;
        int tlen;
        int blen = 1024;
        TCHAR * buff;
        SIZE tSize;
        hwnd = this->hwnd;
        hDC = GetDC(hwnd);
        SelectObject(hDC, (HGDIOBJ) SendMessage(hwnd, WM_GETFONT, 0, 0));
        n = SendMessage(hwnd, LB_GETCOUNT, 0, 0);
        buff = calloc(1, blen + 2);
        for (i = 0; i < n; i++)
        {
            tlen = SendMessage(hwnd, LB_GETTEXTLEN, (WPARAM) i, 0);
            if (tlen > blen) {
                blen = tlen;
                buff = realloc(buff, blen + 2);
            }
            SendMessage(hwnd, LB_GETTEXT, (WPARAM) i, (LPARAM) buff);
            GetTextExtentPoint32(hDC, buff, tlen, &tSize);
            if (tSize.cx > m) {
                m = tSize.cx;
            }
        }
        free(buff);
        SendMessage(hwnd, LB_SETHORIZONTALEXTENT, m + 5, 0);
        ReleaseDC(hwnd, hDC);
        SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
        return 1;
    }
    return 0;
}


int
xGui_LBox_AddString(struct xGui * xObj, TCHAR * Strs)
{
    SendMessage(xObj->hwnd, LB_ADDSTRING, 0, (LPARAM) Strs);
    return 1;
}
