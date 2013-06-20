

#define RBTN _T("RBtn")
#define RBTN_WS WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON
#define RBTN_EX 0
#define xGui_Add_RBtn(xSup, Name, l, t, w, h) \
        xGui_Add_RBtn_Ex(xSup, Name, l, t, w, h, RBTN_WS, RBTN_EX)


struct RBtn {
    struct xGui * next;
};


int
RBtn_On_Command(struct xGui * this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    struct xGui * curr;
    Button_SetCheck(this->hwnd, BST_CHECKED);
    for (curr = xGui_xPtr(struct RBtn, this)->next;
        curr != this;
        curr = xGui_xPtr(struct RBtn, curr)->next) {
            Button_SetCheck(curr->hwnd, BST_UNCHECKED);
    }
    return 0;
}


struct xGui *
xGui_Add_RBtn_Ex(struct xGui * xSup, int l, int t, int w, int h, TCHAR * Name, DWORD WS, DWORD EX)
{
    struct xGui * this;

    this = xGui_Add_Ex(xSup, RBTN, sizeof(struct RBtn), WC_BUTTON, Name,
            l, t, w, h, WS, EX);
    this->Own_On_Command = RBtn_On_Command;

    if (_tcscmp(xSup->Flag, RBTN) != 0) {
        xGui_xPtr(struct RBtn, this)->next = this;
    } else {
        struct xGui * head, * last;
        for (head = xSup;
            _tcscmp(head->xSup->Flag, RBTN) == 0;
            head = head->xSup);
        for (last = xGui_xPtr(struct RBtn, head)->next;
            xGui_xPtr(struct RBtn, last)->next != head;
            last = xGui_xPtr(struct RBtn, last)->next);
        xGui_xPtr(struct RBtn, last)->next = this;
        xGui_xPtr(struct RBtn, this)->next = head;
    }

    return this;
}


int
RBtn_Set_iSel(struct xGui * this, int iSel)
{
    if (_tcscmp(this->Flag, RBTN) == 0) {
        int i = 0;
        struct xGui * head, * curr;
        for (head = (_tcscmp(this->xSup->Flag, RBTN) == 0) ? this->xSup : this;
            _tcscmp(head->xSup->Flag, RBTN) == 0;
            head = head->xSup);
        curr = head;
        while (1) {
            if (i == iSel) {
                Button_SetCheck(curr->hwnd, BST_CHECKED);
            } else {
                Button_SetCheck(curr->hwnd, BST_UNCHECKED);
            }
            i++;
            curr = xGui_xPtr(struct RBtn, curr)->next;
            if (curr == head) {
                break;
            }
        }
        return i;
    }
    return 0;
}


int
RBtn_Get_iSel(struct xGui * this)
{
    if (_tcscmp(this->Flag, RBTN) == 0) {
        int i = 0;
        struct xGui * head, * curr;
        for (head = (_tcscmp(this->xSup->Flag, RBTN) == 0) ? this->xSup : this;
            _tcscmp(head->xSup->Flag, RBTN) == 0;
            head = head->xSup);
        curr = head;
        while (1) {
            if (Button_GetCheck(curr->hwnd) == BST_CHECKED) {
                return i;
            }
            i++;
            curr = xGui_xPtr(struct RBtn, curr)->next;
            if (curr == head) {
                break;
            }
        }
    }
    return -1;
}
