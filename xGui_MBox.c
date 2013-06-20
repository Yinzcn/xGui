

#define xGui_MsgBox(xSup, capt, text) \
    xGui_MsgBox_Ex(xSup, capt, text, MB_OK | MB_ICONINFORMATION)


int
xGui_MsgBox_Ex(struct xGui * xSup, TCHAR * Capt, TCHAR * Text, UINT Type)
{
    return MessageBox(xSup ? xSup->hwnd : NULL, Text, Capt, Type);
}
