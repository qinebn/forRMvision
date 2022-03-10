#ifndef DAHENG_H
#define DAHENG_H

#include "cstdio"
#include "GxIAPI.h"
#include "DxImageProc.h"

class daheng
{
public:
    daheng();
    int start(int64_t nWidth, int64_t nHeight);
    void aProcGetImage(double dExposureValue, size_t *change);
    GX_STATUS change_roi(GX_STATUS emStatus, size_t mode);
    //void ProcGetImage(void* pParam);
    //void ProcGetImage(GX_DEV_HANDLE g_hDevice);

public:
    size_t get_count;
};

#endif // DAHENG_H
