

/*
    Load jpg / gif image from Resource
*/


#include <olectl.h>


HBITMAP
xRes_Image_Load(UINT Res_ID)
{
    HBITMAP hBmp = NULL;
    HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(Res_ID), _T("BINARY"));
    if (hRsrc) {
        HGLOBAL hData = LoadResource(NULL, hRsrc);
        if (hData) {
            const IID IID_IPicture = {0x7BF80980, 0xBF32, 0x101A, {0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB}};
            LPSTREAM pStream = NULL;
            LPPICTURE pPicture = NULL;
            DWORD ResSize;
            LPVOID lockedData;
            HGLOBAL globalData;

            ResSize = SizeofResource(NULL, hRsrc);
            globalData = GlobalAlloc(0, ResSize);
            lockedData = GlobalLock(globalData);
            memcpy(lockedData, hData, ResSize);
            GlobalUnlock(globalData);

            CreateStreamOnHGlobal(globalData, 0, &pStream);

            OleLoadPicture(pStream, ResSize, 1, (REFIID) &IID_IPicture, (LPVOID *) &pPicture);

            if (pPicture) {
                HBITMAP hImage = NULL;
                pPicture->lpVtbl->get_Handle(pPicture, (OLE_HANDLE *) &hImage);
                hBmp = (HBITMAP) CopyImage(hImage, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
                pPicture->lpVtbl->Release(pPicture);
            }

            pStream->lpVtbl->Release(pStream);
            GlobalFree(globalData);
        }
    }
    return hBmp;
}


BOOL
xRes_Image_Free(HBITMAP hBmp)
{
    return DeleteObject(hBmp);
}
