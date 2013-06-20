

#define SICO _T("SIco")


#define SICO_WS WS_CHILD | SS_ICON
#define SICO_EX 0


#define xGui_Add_SIco(xSup, l, t, iRes) \
        xGui_Add_SIco_Ex(xSup, l, t, 0, 0, iRes, SICO_WS, SICO_EX)


struct xGui *
xGui_Add_SIco_Ex(struct xGui * xSup, int l, int t, int w, int h, int iRes, DWORD WS, DWORD EX)
{
    struct xGui * xNew;
    xNew = xGui_Add_Ex(xSup, SICO, 0, WC_STATIC, _T(""),
            l, t, w, h, WS, EX);

    if (xNew) {
        HANDLE hIco;
        hIco = LoadImage(xTop_Inst(xSup), MAKEINTRESOURCE(iRes),
                IMAGE_ICON, 0, 0, LR_SHARED);
        SendMessage(xNew->hwnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM) hIco);
    }

    return xNew;
}
