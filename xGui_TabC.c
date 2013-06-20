

#define TABC _T("TabC")
#define TABC_WS WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN
#define TABC_EX WS_EX_CONTROLPARENT
#define xGui_Add_TabC(xSup, l, t, w, h) \
        xGui_Add_TabC_Ex(xSup, l, t, w, h, TABC_WS, TABC_EX)


struct TabC {
    int nTab;
    int iCur;
};


int
TabC_Own_On_Notify(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    struct TabC * xPtr;
    if (((LPNMHDR) lParam)->code == TCN_SELCHANGE) {
        TCITEM tcitem;
        tcitem.mask = TCIF_PARAM;
        xPtr = xGui_xPtr(struct TabC, this);

        TabCtrl_GetItem(this->hwnd, xPtr->iCur, &tcitem);
        xGui_Hide((void *) tcitem.lParam);

        xPtr->iCur = TabCtrl_GetCurSel(this->hwnd);

        TabCtrl_GetItem(this->hwnd, xPtr->iCur, &tcitem);
        xGui_Show((void *) tcitem.lParam);
    }
    return 0;
}


struct xGui *
xGui_Add_TabC_Ex(struct xGui * xSup, int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, TABC, sizeof(struct TabC), WC_TABCONTROL, _T(""),
            l, t, w, h, WS, EX);
    this->Own_On_Notify = TabC_Own_On_Notify;
    this->bPan = 1;
    xGui_UpdateBkBr(this);
    xGui_UpdateBkDc(this);
    return this;
}


struct xGui *
TabC_Add_Page(struct xGui * this, TCHAR * name)
{
    if (_tcscmp(this->Flag, TABC) == 0) {
        struct TabC * xPtr = xGui_xPtr(struct TabC, this);
        struct xGui * Page;
        TCITEM tcitem;
        RECT rc;
        RECT rctab;

        tcitem.mask = TCIF_TEXT;/* | TCIF_IMAGE;
        tcitem.iImage = -1;*/
        tcitem.pszText = name;
        if (TabCtrl_InsertItem(this->hwnd, xPtr->nTab, &tcitem) == -1) {
            DestroyWindow(this->hwnd);
            return NULL;
        }
        GetClientRect(this->hwnd, (void *) &rc);
        rctab = rc;
        TabCtrl_AdjustRect(this->hwnd, FALSE, (void *) &rc);
        rc.top = rc.top % 4 ? rc.top / 4 * 4 + 4 : rc.top;
        Page = xGui_Add_WPan(this,
                rc.left, rc.top, rc.right - (rc.left - rctab.left), rc.bottom - (rc.top - rctab.top));
        tcitem.mask = TCIF_PARAM;
        tcitem.lParam = (LPARAM) Page;
        TabCtrl_SetItem(this->hwnd, xPtr->nTab, &tcitem);

        if (xPtr->nTab) {
            xGui_Hide(Page);
        } else {
            xGui_Show(Page);
        }
        xPtr->nTab++;

        return Page;
    }
    return 0;
}


int
TabC_SetCurrSel(struct xGui * this, int iSel)
{
    if (_tcscmp(this->Flag, TABC) == 0) {
        TabCtrl_SetCurFocus(this->hwnd, iSel);
        return 1;
    }
    return 0;
}
