

TCHAR * x_nowstr();


LRESULT CALLBACK
xGui_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    struct xGui * this;

    if (uMsg == WM_NCCREATE) {
        this = ((LPCREATESTRUCT) lParam)->lpCreateParams;
        this->hwnd = hWnd;
        SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG) this);
        this->defproc = DefWindowProc;
    } else {
        this = (void *) GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (this == NULL) {
            _tprintf(_T("this=NULL : %s\n"), wMsgToStr(uMsg));
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    /*_tprintf("%s : %s : (%08x, %08x) %s\n",
            x_nowstr(), this->Flag, wParam, lParam, wMsgToStr(uMsg));*/
    if (this->Own_On_Proc &&
        this->Own_On_Proc(this, uMsg, wParam, lParam)) {
        return this->lResult;
    }

    switch (uMsg) {
        //case WM_CTLCOLORMSGBOX:
        //case WM_CTLCOLOREDIT:
        //case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORBTN:
        //case WM_CTLCOLORDLG:
        //case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        {
            struct xGui * xSub;
            xSub = (void *) GetWindowLongPtr((HWND) lParam, GWL_USERDATA);
            SetBkMode((HDC) wParam, TRANSPARENT);
            if (xSub &&
                xSub->Own_On_Color &&
                xSub->Own_On_Color(xSub, uMsg, wParam, lParam)) {
                return xSub->lResult;
            }
            RECT rc;
            GetWindowRect(xSub->hwnd, &rc);
            MapWindowPoints(NULL, this->hwnd, (LPPOINT)(&rc), 2);
            SetBrushOrgEx((HDC) wParam, -rc.left, -rc.top, NULL);
            return (LRESULT) this->hbrBkgnd;
        }
        case WM_COMMAND:
        {
            struct xGui * xSub;
            xSub = (void *) GetWindowLongPtr((HWND) lParam, GWL_USERDATA);
            if (xSub &&
                xSub->Own_On_Command &&
                xSub->Own_On_Command(xSub, uMsg, wParam, lParam)) {
                return 0;
            }
            break;
        }
        case WM_NOTIFY:
        {
            struct xGui * xSub;
            xSub = (void *) GetWindowLongPtr(((LPNMHDR) lParam)->hwndFrom, GWL_USERDATA);
            if (xSub &&
                xSub->Own_On_Notify &&
                xSub->Own_On_Notify(xSub, uMsg, wParam, lParam)) {
                return xSub->lResult;
            }
            break;
        }
        case WM_DESTROY:
        {
            if (this->On_Destroy) {
                this->On_Destroy(this);
            }
            break;
        }
        case WM_NCDESTROY:
        {
            WNDPROC wproc = this->defproc;
            if (this->On_NCDestroy) {
                this->On_NCDestroy(this);
            }
            if (this->hbrBkgnd) {
                DeleteObject(this->hbrBkgnd);
            }
            if (this->hdcBkgnd) {
                DeleteObject(SelectObject(this->hdcBkgnd, this->hBmpOld));
                DeleteDC(this->hdcBkgnd);
            }
            free(this);
            return CallWindowProc(wproc, hWnd, uMsg, wParam, lParam);
        }
    }

    return CallWindowProc(this->defproc, hWnd, uMsg, wParam, lParam);
}
