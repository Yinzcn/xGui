

#include "Scintilla.h"
#include "SciLexer.h"


#define SCIL _T("SciL")
#define SCIL_WS WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN
#define SCIL_EX WS_EX_CLIENTEDGE
#define xGui_Add_SciL(xSup, l, t, w, h) \
        xGui_Add_SciL_Ex(xSup, l, t, w, h, EDIT_WS, EDIT_EX)


struct SciL {
    TCHAR * Text;
    int (* SciF)(void *, int, int, int);
	void * SciP;
    int (* OnStyleNeeded)(struct xGui * this, struct SCNotification * scn);
};


int
SciL_UpdateLineNumberWidth(struct xGui * this)
{
    char buff[32];
    int (* SciF)(void *, int, int, int);
    void * SciP;
    SciF = xGui_xPtr(struct SciL, this)->SciF;
    SciP = xGui_xPtr(struct SciL, this)->SciP;
    snprintf(buff, sizeof(buff), "_%d",
            SciF(SciP, SCI_GETLINECOUNT, 0, 0));
    SciF(SciP, SCI_SETMARGINWIDTHN,
            0, 4 + SciF(SciP, SCI_TEXTWIDTH,
                    STYLE_LINENUMBER, (int) buff));
    return 0;
}


int
SciL_On_Notify(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    NMHDR * nm = (NMHDR *) lParam;
    if (nm->code == SCN_UPDATEUI) {
        SciL_UpdateLineNumberWidth(this);
    } else
    if (nm->code == SCN_STYLENEEDED && xGui_xPtr(struct SciL, this)->OnStyleNeeded) {
        xGui_xPtr(struct SciL, this)->OnStyleNeeded(this, (struct SCNotification *) lParam);
    }
    return 0;
}


struct xGui *
xGui_Add_SciL_Ex(struct xGui * xSup, int l, int t, int w, int h, DWORD WS, DWORD EX)
{
    struct xGui * this;
    struct SciL * xPtr;
    if (LoadLibrary(_T("SciLexer.DLL")) == NULL) {
        MessageBox(NULL,
            _T("The Scintilla DLL could not be loaded."),
            _T("Error loading Scintilla"),
            MB_OK | MB_ICONERROR);
        return NULL;
    }
    this = xGui_Add_Ex(xSup, SCIL, sizeof(struct SciL), _T("Scintilla"), _T(""),
            l, t, w, h, WS, EX);
    
	xPtr = xGui_xPtr(struct SciL, this);
    xPtr->SciF = (void *) SendMessage(this->hwnd, SCI_GETDIRECTFUNCTION, 0, 0);
	xPtr->SciP = (void *) SendMessage(this->hwnd, SCI_GETDIRECTPOINTER, 0, 0);
    xPtr->SciF(xPtr->SciP, SCI_SETCODEPAGE, CP_ACP, 0); /* CodePage */
    xPtr->SciF(xPtr->SciP, SCI_SETWRAPMODE, SC_WRAP_WORD, 0); /* 自动换行 */
    xPtr->SciF(xPtr->SciP, SCI_SETCARETLINEVISIBLE, 1, 0); /* 高亮当前行 */
    xPtr->SciF(xPtr->SciP, SCI_SETCARETLINEBACK, 0xFFE8E8, 0); /* 当前行背景 */
    xPtr->SciF(xPtr->SciP, SCI_SETTABWIDTH, 4, 0); /* TAB 宽度 */
    xPtr->SciF(xPtr->SciP, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER); /* 0 号页边设为显示行号 */
    xPtr->SciF(xPtr->SciP, SCI_SETMARGINWIDTHN, 0, 1); /* 0 号页边设为默认宽度 */
    xPtr->SciF(xPtr->SciP, SCI_SETMARGINWIDTHN, 1, 12); /* 1 号页边设为0宽度 */
    xPtr->SciF(xPtr->SciP, SCI_STYLESETFORE, STYLE_LINENUMBER, 0x787878); /* 行号前景颜色 */
    xPtr->SciF(xPtr->SciP, SCI_STYLESETBACK, STYLE_LINENUMBER, 0xE8E8E8); /* 行号背景颜色 */
    this->Own_On_Notify = SciL_On_Notify;

    return this;
}


int
SciL_Func(struct xGui * this, int SciMsg, int wParam, int lParam)
{
    if (_tcscmp(this->Flag, SCIL) == 0) {
        struct SciL * xPtr = xGui_xPtr(struct SciL, this);
        return xPtr->SciF(xPtr->SciP, SciMsg, wParam, lParam);
    }
    return 0;
}
#define SciL_Send(this, M, w, l) SciL_Func(this, M, (int) w, (int) l)


TCHAR *
SciL_Get_Text(struct xGui * this)
{
    if (_tcscmp(this->Flag, SCIL) == 0) {
        struct SciL * xPtr = xGui_xPtr(struct SciL, this);
        int nBuf;
        if (xPtr->Text) {
            free(xPtr->Text);
        }
        nBuf = Edit_GetTextLength(this->hwnd) + 1;
        xPtr->Text = calloc(1, nBuf);
        Edit_GetText(this->hwnd, xPtr->Text, nBuf);
        return xPtr->Text;
    }
    return NULL;
}
