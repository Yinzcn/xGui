

#define EDIT _T("Edit")
#define EDIT_WS WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL
#define EDIT_EX WS_EX_CLIENTEDGE
#define xGui_Add_Edit(xSup, l, t, w, h) \
        xGui_Add_Edit_Ex(xSup, l, t, w, h, EDIT_WS, EDIT_EX)


struct Edit {
    TCHAR * Text;
};


int
Edit_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCDESTROY) {
        free(xGui_xPtr(struct Edit, this)->Text);
    }
    return 0;
}


struct xGui *
xGui_Add_Edit_Ex(struct xGui * xSup, int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, EDIT, 0, WC_EDIT, _T(""),
            l, t, w, h, WS, EX);
    this->Own_On_Proc = Edit_On_Proc;
    return this;
}


TCHAR *
Edit_Get_Text(struct xGui * this)
{
    if (_tcscmp(this->Flag, EDIT) == 0) {
        struct Edit * xPtr = xGui_xPtr(struct Edit, this);
        int nBuf;
        if (xPtr->Text) {
            free(xPtr->Text);
        }
        nBuf = Edit_GetTextLength(this->hwnd) + 2;
        xPtr->Text = calloc(1, nBuf);
        Edit_GetText(this->hwnd, xPtr->Text, nBuf);
        return xPtr->Text;
    }
    return NULL;
}
