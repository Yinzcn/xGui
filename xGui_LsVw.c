

#define LSVW _T("LsVw")
#define LSVW_WS WS_CHILD | WS_TABSTOP | LBS_EXTENDEDSEL | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY
#define LSVW_EX WS_EX_CLIENTEDGE


#define xGui_Add_LsVw(xSup, l, t, w, h) \
        xGui_Add_LsVw_Ex(xSup, l, t, w, h, LSVW_WS, LSVW_EX) \


struct LsVw {
    int (* On_DBClick)(struct xGui *, int);
    int nCol; /* 列数 */
    int iCol; /* 当前列指针 */
    int iRow; /* 当前行指针 */
};


int
LsVw_Own_On_Notify(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (((LPNMHDR) lParam)->code == NM_DBLCLK) {
        LPNMITEMACTIVATE nmia = (LPNMITEMACTIVATE) lParam;
        if (xGui_xPtr(struct LsVw, this)->On_DBClick) {
            xGui_xPtr(struct LsVw, this)->On_DBClick(this, nmia->iItem);
        }
    }
    return 0;
}


struct xGui *
xGui_Add_LsVw_Ex(struct xGui * xSup,
        int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, LSVW, sizeof(struct LsVw), WC_LISTVIEW, _T(""),
            l, t, w, h, WS, EX);
    this->Own_On_Notify = LsVw_Own_On_Notify;
    ListView_SetExtendedListViewStyle(this->hwnd,
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT);
    return this;
}


int
LsVw_Get_iCount(struct xGui * this)
{
    return SendMessage(this->hwnd, LVM_GETITEMCOUNT, 0, 0);
}


int
LsVw_Append_Val(struct xGui * this, TCHAR * text)
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        struct LsVw * xPtr = xGui_xPtr(struct LsVw, this);
        LVITEM LvItem = { 0 };
        LvItem.mask = LVIF_TEXT;
        if (xPtr->iCol == 0) {
            LvItem.iItem = LsVw_Get_iCount(this);
            LvItem.iSubItem = 0;
            LvItem.pszText = text;
            ListView_InsertItem(this->hwnd, &LvItem);
        } else {
            LvItem.iItem = LsVw_Get_iCount(this) - 1;
            LvItem.iSubItem = xPtr->iCol;
            LvItem.pszText = text;
            ListView_SetItem(this->hwnd, &LvItem);
        }
        xPtr->iCol++;
        xPtr->iCol = xPtr->iCol % xPtr->nCol;
        return 1;
    }
    return 0;
}


int
LsVw_Add_Column(struct xGui * this, TCHAR * title, int width)
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        LV_COLUMN col;
        col.mask    = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        col.fmt     = LVCFMT_LEFT;
        col.pszText = title;
        col.cx      = width;
        ListView_InsertColumn(this->hwnd, xGui_xPtr(struct LsVw, this)->nCol++, &col);
        return 1;
    }
    return 0;
}


int
LsVw_Add_Record(struct xGui * this, ...)
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        int n = xGui_xPtr(struct LsVw, this)->nCol;
        if (n) {
            LVITEM LvItem;
            va_list va;
            va_start(va, this);
            LvItem.mask = LVIF_TEXT;
            LvItem.iItem = LsVw_Get_iCount(this);
            LvItem.iSubItem = 0;
            LvItem.pszText = va_arg(va, TCHAR *);
            ListView_InsertItem(this->hwnd, &LvItem);
            while (--n) {
                LvItem.iSubItem++;
                LvItem.pszText = va_arg(va, TCHAR *);
                ListView_SetItem(this->hwnd, &LvItem);
            }
            va_end(va);
            return 1;
        }
    }
    return 0;
}


int
LsVw_Set_Record(struct xGui * this, int iRow, ...)
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        int n = xGui_xPtr(struct LsVw, this)->nCol;
        if (n) {
            LVITEM LvItem;
            va_list va;
            va_start(va, iRow);
            LvItem.mask = LVIF_TEXT;
            LvItem.iItem = iRow;
            LvItem.iSubItem = 0;
            LvItem.pszText = va_arg(va, TCHAR *);
            ListView_InsertItem(this->hwnd, &LvItem);
            while (--n) {
                LvItem.iSubItem++;
                LvItem.pszText = va_arg(va, TCHAR *);
                ListView_SetItem(this->hwnd, &LvItem);
            }
            va_end(va);
            return 1;
        }
    }
    return 0;
}


int
LsVw_Del_Record(struct xGui * this, int iRow)
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        ListView_DeleteItem(this->hwnd, iRow);
        return 1;
    }
    return 0;
}


int
LsVw_On_DBClick(struct xGui * this, int (* Func)(struct xGui *, int))
{
    if (_tcscmp(this->Flag, LSVW) == 0) {
        xGui_xPtr(struct LsVw, this)->On_DBClick = Func;
        return 1;
    }
    return 0;
}
