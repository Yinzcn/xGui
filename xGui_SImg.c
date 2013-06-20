

#define SIMG _T("SImg")
#define SIMG_WS WS_CHILD | SS_BITMAP
#define SIMG_EX 0
#define xGui_Add_SImg(xSup, l, t, w, h, iRes) \
        xGui_Add_SImg_Ex(xSup, l, t, w, h, iRes, SIMG_WS, SIMG_EX)


struct SImg {
    HBITMAP hBmp;
};


int
SImg_On_Proc(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCDESTROY) {
        xRes_Image_Free(xGui_xPtr(struct SImg, this)->hBmp);
    }
    return 0;
}


struct xGui *
xGui_Add_SImg_Ex(struct xGui * xSup, int l, int t, int w, int h, int iRes, DWORD WS, DWORD EX)
{
    struct xGui * this;
    this = xGui_Add_Ex(xSup, SIMG, sizeof(struct SImg), WC_STATIC, _T(""),
            l, t, w, h, WS, EX);
    this->Own_On_Proc = SImg_On_Proc;

    if (this) {
        xGui_xPtr(struct SImg, this)->hBmp = xRes_Image_Load(iRes);
        SendMessage(this->hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) xGui_xPtr(struct SImg, this)->hBmp);
    }

    return this;
}
