

#ifndef BCM_FIRST

typedef struct {
    HIMAGELIST  himl;
    RECT        margin;
    UINT        uAlign;
} BUTTON_IMAGELIST, * PBUTTON_IMAGELIST;

#define BCM_FIRST 0x1600

#define BUTTON_IMAGELIST_ALIGN_LEFT     0
#define BUTTON_IMAGELIST_ALIGN_RIGHT    1
#define BUTTON_IMAGELIST_ALIGN_TOP      2
#define BUTTON_IMAGELIST_ALIGN_BOTTOM   3
#define BUTTON_IMAGELIST_ALIGN_CENTER   4

#define BCM_SETIMAGELIST (BCM_FIRST + 0x0002)

#define Button_SetImageList(hwnd, pbuttonImagelist) \
    (BOOL)SNDMSG((hwnd), BCM_SETIMAGELIST, 0, (LPARAM)(pbuttonImagelist))

#endif


#define BUTN _T("Butn")
#define BUTN_WS WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON
#define BUTN_EX 0
#define xGui_Add_Butn(xSup, l, t, w, h, Name) \
        xGui_Add_Butn_Ex(xSup, l, t, w, h, Name, BUTN_WS, BUTN_EX)


struct Butn {
    int (* On_Click)(struct xGui *);
    HIMAGELIST himgl;
};


int
Butn_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCDESTROY) {
        ImageList_Destroy(xGui_xPtr(struct Butn, this)->himgl);
    }
    return 0;
}


int
Butn_On_Command(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (xGui_xPtr(struct Butn, this)->On_Click) {
        xGui_xPtr(struct Butn, this)->On_Click(this);
        return 1;
    }
    return 0;
}


int
Butn_Set_Icon(struct xGui * this, int iRes)
{
    if (_tcscmp(this->Flag, BUTN) == 0) {
        struct Butn * xPtr = xGui_xPtr(struct Butn, this);
        HANDLE hIco;
        HIMAGELIST himgl;
        BUTTON_IMAGELIST bimgl;

        hIco = LoadImage(xTop_Inst(this), MAKEINTRESOURCE(iRes), IMAGE_ICON, 16, 16, LR_SHARED);
        himgl = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR32, 1, 1);
        ImageList_SetBkColor(himgl, CLR_NONE);
        //ImageList_AddIcon(himgl, hIco);
        ImageList_ReplaceIcon(himgl, -1, hIco);
        bimgl.uAlign = BUTTON_IMAGELIST_ALIGN_TOP;
        bimgl.margin.left = 0;
        bimgl.margin.top = 10;
        bimgl.margin.right = 0;
        bimgl.margin.bottom = 0;
        bimgl.himl = himgl;
        Button_SetImageList(this->hwnd, &bimgl);
        ImageList_Destroy(xPtr->himgl);
        xPtr->himgl = himgl;
        return 1;
    }
    return 0;
}


struct xGui *
xGui_Add_Butn_Ex(struct xGui * xSup, int l, int t, int w, int h, TCHAR * Name, DWORD WS, DWORD EX)
{
    struct xGui * this;

    this = xGui_Add_Ex(xSup, BUTN, sizeof(struct Butn), WC_BUTTON, Name,
            l, t, w, h, WS, EX);
    this->Own_On_Proc = Butn_On_Proc;
    this->Own_On_Command = Butn_On_Command;

    return this;
}


int
Butn_On_Click(struct xGui * this, void * Func)
{
    if (_tcscmp(this->Flag, BUTN) == 0) {
        xGui_xPtr(struct Butn, this)->On_Click = Func;
        return 1;
    }
    return 0;
}
