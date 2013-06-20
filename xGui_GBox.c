

#define GBOX _T("GBox")
#define GBOX_WS WS_CHILD | WS_GROUP | BS_GROUPBOX// | WS_CLIPSIBLINGS
#define GBOX_EX 0
#define xGui_Add_GBox(xSup, Name, l, t, w, h) \
        xGui_Add_GBox_Ex(xSup, Name, l, t, w, h, GBOX_WS, GBOX_EX)


struct xGui *
xGui_Add_GBox_Ex(struct xGui * xSup, int l, int t, int w, int h, TCHAR * Name, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, GBOX, 0, WC_BUTTON, Name,
            l, t, w, h, WS, EX);
    return this;
}
