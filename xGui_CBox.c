

#define CBOX _T("CBox")
#define CBOX_WS WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX
#define CBOX_EX 0
#define xGui_Add_CBox(xSup, l, t, w, h, Name) \
        xGui_Add_CBox_Ex(xSup, l, t, w, h, Name, CBOX_WS, CBOX_EX)


struct xGui *
xGui_Add_CBox_Ex(struct xGui * xSup, int l, int t, int w, int h, TCHAR * Name, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, CBOX, 0, WC_BUTTON, Name,
            l, t, w, h, WS, EX);
    return this;
}


int
CBox_GetCheck(struct xGui * this)
{
    if (_tcscmp(this->Flag, CBOX) == 0) {
        switch(Button_GetCheck(this->hwnd)) {
            case BST_UNCHECKED:
                return 0;
            case BST_CHECKED:
                return 1;
            case BST_INDETERMINATE:
                return 2;
        }
    }
    return -1;
}
