//-------------------------------------------------------------
/**
\file      GxSingleCamColor.cpp
\brief     Sample to show how to acquire color image continuously and save ppm images
\version   1.0.1901.9311
\date      2019.01.31
*/
//-------------------------------------------------------------
#include "daheng.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#define ACQ_BUFFER_NUM          5               ///< Acquisition Buffer Qty.
#define ACQ_TRANSFER_SIZE       (64 * 1024)     ///< Size of data transfer block
#define ACQ_TRANSFER_NUMBER_URB 64              ///< Qty. of data transfer block
#define FILE_NAME_LEN           50              ///< Save image file name length

#define PIXFMT_CVT_FAIL             -1             ///< PixelFormatConvert fail
#define PIXFMT_CVT_SUCCESS          0              ///< PixelFormatConvert success

//Show error message
#define GX_VERIFY(emStatus) \
    if (emStatus != GX_STATUS_SUCCESS)     \
    {                                      \
        GetErrorString(emStatus);          \
        return emStatus;                   \
    }

//Show error message, close device and lib
#define GX_VERIFY_EXIT(emStatus) \
    if (emStatus != GX_STATUS_SUCCESS)     \
    {                                      \
        GetErrorString(emStatus);          \
        GXCloseDevice(g_hDevice);          \
        g_hDevice = NULL;                  \
        GXCloseLib();                      \
        printf("<App Exit!>\n");           \
        return emStatus;                   \
    }


daheng::daheng()
{
    get_count = 0;
}

GX_DEV_HANDLE g_hDevice = NULL;                     ///< Device handle
bool g_bColorFilter = false;                        ///< Color filter support flag
int64_t g_i64ColorFilter = GX_COLOR_FILTER_NONE;    ///< Color filter of device
bool g_bAcquisitionFlag = false;                    ///< Thread running flag
bool g_bSavePPMImage = false;                       ///< Save raw image flag
pthread_t g_nAcquisitonThreadID = 0;                ///< Thread ID of Acquisition thread

unsigned char* g_pRGBImageBuf = NULL;               ///< Memory for RAW8toRGB24
unsigned char* g_pRaw8Image = NULL;                 ///< Memory for RAW16toRAW8

int64_t g_nPayloadSize = 0;                         ///< Payload size

//Allocate the memory for pixel format transform
void PreForAcquisition();

//Release the memory allocated
void UnPreForAcquisition();

//Convert frame date to suitable pixel format
int PixelFormatConvert(PGX_FRAME_BUFFER);

//Save one frame to PPM image file
void SavePPMFile(uint32_t, uint32_t);

//Acquisition thread function
void *ProcGetImage(void*);

void aProcGetImage();

//Get description of error
void GetErrorString(GX_STATUS);

//Get Mat image
void getMat(uint32_t ui32Width, uint32_t ui32Height);

int daheng::start(int64_t nWidth, int64_t nHeight)
{
    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("Sample to show how to acquire color image continuously and save ppm image.\n");
    printf("version: 1.0.1901.9311\n");
    printf("-------------------------------------------------------------\n");
    printf("\n");
    printf("Initializing......");
    printf("\n\n");

    GX_STATUS emStatus = GX_STATUS_SUCCESS;

    uint32_t ui32DeviceNum = 0;

    //Initialize libary
    emStatus = GXInitLib();
    if(emStatus != GX_STATUS_SUCCESS)
    {
        GetErrorString(emStatus);
        return emStatus;
    }

    //Get device enumerated number
    emStatus = GXUpdateDeviceList(&ui32DeviceNum, 1000);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        GetErrorString(emStatus);
        GXCloseLib();
        return emStatus;
    }

    //If no device found, app exit
    if(ui32DeviceNum <= 0)
    {
        printf("<No device found>\n");
        GXCloseLib();
        return emStatus;
    }

    //Open first device enumerated
    emStatus = GXOpenDeviceByIndex(1, &g_hDevice);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        GetErrorString(emStatus);
        GXCloseLib();
        return emStatus;
    }

    //Get Device Info
    printf("***********************************************\n");
    //Get libary version
    printf("<Libary Version : %s>\n", GXGetLibVersion());
    size_t nSize = 0;
    //Get string length of Vendor name
    emStatus = GXGetStringLength(g_hDevice, GX_STRING_DEVICE_VENDOR_NAME, &nSize);
    GX_VERIFY_EXIT(emStatus);
    //Alloc memory for Vendor name
    char *pszVendorName = new char[nSize];
    //Get Vendor name
    emStatus = GXGetString(g_hDevice, GX_STRING_DEVICE_VENDOR_NAME, pszVendorName, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        delete[] pszVendorName;
        pszVendorName = NULL;
        GX_VERIFY_EXIT(emStatus);
    }

    printf("<Vendor Name : %s>\n", pszVendorName);
    //Release memory for Vendor name
    delete[] pszVendorName;
    pszVendorName = NULL;

    //Get string length of Model name
    emStatus = GXGetStringLength(g_hDevice, GX_STRING_DEVICE_MODEL_NAME, &nSize);
    GX_VERIFY_EXIT(emStatus);
    //Alloc memory for Model name
    char *pszModelName = new char[nSize];
    //Get Model name
    emStatus = GXGetString(g_hDevice, GX_STRING_DEVICE_MODEL_NAME, pszModelName, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        delete[] pszModelName;
        pszModelName = NULL;
        GX_VERIFY_EXIT(emStatus);
    }

    printf("<Model Name : %s>\n", pszModelName);
    //Release memory for Model name
    delete[] pszModelName;
    pszModelName = NULL;

    //Get string length of Serial number
    emStatus = GXGetStringLength(g_hDevice, GX_STRING_DEVICE_SERIAL_NUMBER, &nSize);
    GX_VERIFY_EXIT(emStatus);
    //Alloc memory for Serial number
    char *pszSerialNumber = new char[nSize];
    //Get Serial Number
    emStatus = GXGetString(g_hDevice, GX_STRING_DEVICE_SERIAL_NUMBER, pszSerialNumber, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        delete[] pszSerialNumber;
        pszSerialNumber = NULL;
        GX_VERIFY_EXIT(emStatus);
    }

    printf("<Serial Number : %s>\n", pszSerialNumber);
    //Release memory for Serial number
    delete[] pszSerialNumber;
    pszSerialNumber = NULL;

    //Get string length of Device version
    emStatus = GXGetStringLength(g_hDevice, GX_STRING_DEVICE_VERSION, &nSize);
    GX_VERIFY_EXIT(emStatus);
    char *pszDeviceVersion = new char[nSize];
    //Get Device Version
    emStatus = GXGetString(g_hDevice, GX_STRING_DEVICE_VERSION, pszDeviceVersion, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        delete[] pszDeviceVersion;
        pszDeviceVersion = NULL;
        GX_VERIFY_EXIT(emStatus);
    }

    printf("<Device Version : %s>\n", pszDeviceVersion);
    //Release memory for Device version
    delete[] pszDeviceVersion;
    pszDeviceVersion = NULL;
    printf("***********************************************\n");

    //Get the type of Bayer conversion. whether is a color camera.
    emStatus = GXIsImplemented(g_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &g_bColorFilter);
    GX_VERIFY_EXIT(emStatus);

    //This app only support color cameras
    if (!g_bColorFilter)
    {
        printf("<This app only support color cameras! App Exit!>\n");
        GXCloseDevice(g_hDevice);
        g_hDevice = NULL;
        GXCloseLib();
        return 0;
    }
    else
    {
        emStatus = GXGetEnum(g_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &g_i64ColorFilter);
        GX_VERIFY_EXIT(emStatus);
    }

    emStatus = GXGetInt(g_hDevice, GX_INT_PAYLOAD_SIZE, &g_nPayloadSize);
    GX_VERIFY(emStatus);

    printf("\n");
    printf("Press [a] or [A] and then press [Enter] to start acquisition\n");
    printf("Press [s] or [S] and then press [Enter] to save one ppm image\n");
    printf("Press [x] or [X] and then press [Enter] to Exit the Program\n");
    printf("\n");

    //beinning and sum
//   int64_t nBinningH = 2;
//    int64_t nBinningV = 2;
//    int64_t nDecimationH= 2;
//    int64_t nDecimationV= 2;
//    emStatus = GXSetEnum(g_hDevice,GX_ENUM_BINNING_HORIZONTAL_MODE, GX_BINNING_HORIZONTAL_MODE_SUM);
//    emStatus = GXSetEnum(g_hDevice,GX_ENUM_BINNING_VERTICAL_MODE, GX_BINNING_VERTICAL_MODE_SUM);
//    emStatus = GXSetInt(g_hDevice, GX_INT_BINNING_HORIZONTAL, nBinningH);
//    emStatus = GXSetInt(g_hDevice, GX_INT_BINNING_VERTICAL, nBinningV);
//    emStatus = GXSetInt(g_hDevice, GX_INT_DECIMATION_HORIZONTAL, nDecimationH);
//    emStatus = GXSetInt(g_hDevice, GX_INT_DECIMATION_VERTICAL, nDecimationV);

    //roi
//    int64_t nWidth= 640;
//    int64_t nHeight= 480;
//    int64_t nOffsetX = 0;
//    int64_t nOffsetY = 0;
//    emStatus = GXSetInt(g_hDevice, GX_INT_WIDTH, nWidth);
//    emStatus = GXSetInt(g_hDevice, GX_INT_HEIGHT, nHeight);
//    emStatus = GXSetInt(g_hDevice, GX_INT_OFFSET_X, nOffsetX);
//    emStatus = GXSetInt(g_hDevice, GX_INT_OFFSET_Y, nOffsetY);

    //Set acquisition mode
    emStatus = GXSetEnum(g_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
    GX_VERIFY_EXIT(emStatus);

    //Set trigger mode
    emStatus = GXSetEnum(g_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
    GX_VERIFY_EXIT(emStatus);

    //Set buffer quantity of acquisition queue
    uint64_t nBufferNum = ACQ_BUFFER_NUM;
    emStatus = GXSetAcqusitionBufferNumber(g_hDevice, nBufferNum);
    GX_VERIFY_EXIT(emStatus);

    bool bStreamTransferSize = false;
    emStatus = GXIsImplemented(g_hDevice, GX_DS_INT_STREAM_TRANSFER_SIZE, &bStreamTransferSize);
    GX_VERIFY_EXIT(emStatus);

    if(bStreamTransferSize)
    {
        //Set size of data transfer block
        emStatus = GXSetInt(g_hDevice, GX_DS_INT_STREAM_TRANSFER_SIZE, ACQ_TRANSFER_SIZE);
        GX_VERIFY_EXIT(emStatus);
    }

    bool bStreamTransferNumberUrb = false;
    emStatus = GXIsImplemented(g_hDevice, GX_DS_INT_STREAM_TRANSFER_NUMBER_URB, &bStreamTransferNumberUrb);
    GX_VERIFY_EXIT(emStatus);

    if(bStreamTransferNumberUrb)
    {
        //Set qty. of data transfer block
        emStatus = GXSetInt(g_hDevice, GX_DS_INT_STREAM_TRANSFER_NUMBER_URB, ACQ_TRANSFER_NUMBER_URB);
        GX_VERIFY_EXIT(emStatus);
    }

    //曝光
    double dExposureValue = 4000.0;
    emStatus = GXSetFloat(g_hDevice, GX_FLOAT_EXPOSURE_TIME, dExposureValue);


// image width and height
 /*   int64_t nBinningH = 2;
    int64_t nBinningV = 2;
    int64_t nDecimationH= 2;
    int64_t nDecimationV= 2;
    emStatus = GXSetEnum(g_hDevice,GX_ENUM_BINNING_HORIZONTAL_MODE,GX_BINNING_HORIZONTAL_MODE_SUM);
    emStatus = GXSetEnum(g_hDevice,GX_ENUM_BINNING_VERTICAL_MODE,GX_BINNING_VERTICAL_MODE_SUM);
    emStatus = GXSetInt(g_hDevice, GX_INT_BINNING_HORIZONTAL, nBinningH);
    emStatus = GXSetInt(g_hDevice, GX_INT_BINNING_VERTICAL, nBinningV);
    emStatus = GXSetInt(g_hDevice, GX_INT_DECIMATION_HORIZONTAL, nDecimationH);
    emStatus = GXSetInt(g_hDevice, GX_INT_DECIMATION_VERTICAL, nDecimationV);
*/
    //Set Balance White Mode : Continuous   
    /*int i=0;
    for(i=0;i<100;i++)
    {
    emStatus = GXSetEnum(g_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_ONCE);
    GX_VERIFY_EXIT(emStatus);
    }*/

    //GX_VERIFY(emStatus);-

    //Allocate the memory for pixel format transform
    PreForAcquisition();

    //Device start acquisition
    emStatus = GXStreamOn(g_hDevice);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        //Release the memory allocated
        UnPreForAcquisition();
        GX_VERIFY_EXIT(emStatus);
    }

    //Start acquisition thread, if thread create failed, exit this app
    /*int nRet = pthread_create(&g_nAcquisitonThreadID, NULL, ProcGetImage, NULL);
    if(nRet != 0)
    {
        //Release the memory allocated
        UnPreForAcquisition();

        GXCloseDevice(g_hDevice);
        g_hDevice = NULL;
        GXCloseLib();

        printf("<Failed to create the acquisition thread, App Exit!>\n");
        exit(nRet);
    }*/
    //aProcGetImage();
    return 0;




    printf("Press [s] or [S] and then press [Enter] to save one ppm image\n");
    printf("Press [x] or [X] and then press [Enter] to Exit the Program\n");
    //Main loop
    bool bRun = true;
    while(bRun == true)
    {
        char chKey = getchar();
        //press 's' and [Enter] to save image;
        //press 'x' and [Enter] to exit.
        switch(chKey)
        {
            //Save PPM Image
            case 'S':
            case 's':
                g_bSavePPMImage = true;
                break;
            //Exit app
            case 'X':
            case 'x':
                bRun = false;
                break;
            default:
                break;
        }
    }

    //Stop Acquisition thread
    g_bAcquisitionFlag = false;
    pthread_join(g_nAcquisitonThreadID, NULL);

    //Device stop acquisition
    emStatus = GXStreamOff(g_hDevice);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        //Release the memory allocated
        UnPreForAcquisition();
        GX_VERIFY_EXIT(emStatus);
    }

    //Release the resources and stop acquisition thread
    UnPreForAcquisition();

    //Close device
    emStatus = GXCloseDevice(g_hDevice);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        GetErrorString(emStatus);
        g_hDevice = NULL;
        GXCloseLib();
        return emStatus;
    }

    //Release libary
    emStatus = GXCloseLib();
    if(emStatus != GX_STATUS_SUCCESS)
    {
        GetErrorString(emStatus);
        return emStatus;
    }

    printf("<App exit!>\n");
    return 0;
}

//-------------------------------------------------
/**
\brief Save PPM image
\param ui32Width[in]       image width
\param ui32Height[in]      image height
\return void
*/
//-------------------------------------------------
void SavePPMFile(uint32_t ui32Width, uint32_t ui32Height)
{
    char szName[FILE_NAME_LEN] = {0};

    static int nRawFileIndex = 0;
    FILE* phImageFile = NULL;
    sprintf(szName, "%d.ppm", nRawFileIndex++);
    phImageFile = fopen(szName, "wb");
    if (phImageFile == NULL)
    {
        printf("Save %s failed!\n", szName);
        return;
    }

    if(g_pRGBImageBuf != NULL)
    {
        //Save color image
        fprintf(phImageFile, "P6\n" "%u %u 255\n", ui32Width, ui32Height);
        fwrite(g_pRGBImageBuf, 1, g_nPayloadSize * 3, phImageFile);
        fclose(phImageFile);
        phImageFile = NULL;
        printf("Save %s successed!\n", szName);
    }
    else
    {
        printf("Save %s failed!\n", szName);
    }
}

//-------------------------------------------------
/**
\brief Convert frame date to suitable pixel format
\param pParam[in]           pFrameBuffer       FrameData from camera
\return void
*/
//-------------------------------------------------
int PixelFormatConvert(PGX_FRAME_BUFFER pFrameBuffer)
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    VxInt32 emDXStatus = DX_OK;

    // Convert RAW8 or RAW16 image to RGB24 image
    switch (pFrameBuffer->nPixelFormat)
    {
        case GX_PIXEL_FORMAT_BAYER_GR8:
        case GX_PIXEL_FORMAT_BAYER_RG8:
        case GX_PIXEL_FORMAT_BAYER_GB8:
        case GX_PIXEL_FORMAT_BAYER_BG8:
        {
            // Convert to the RGB image
            emDXStatus = DxRaw8toRGB24((unsigned char*)pFrameBuffer->pImgBuf, g_pRGBImageBuf, pFrameBuffer->nWidth, pFrameBuffer->nHeight,
                              RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(g_i64ColorFilter), false);
            if (emDXStatus != DX_OK)
            {
                printf("DxRaw8toRGB24 Failed, Error Code: %d\n", emDXStatus);
                return PIXFMT_CVT_FAIL;
            }
            break;
        }
        case GX_PIXEL_FORMAT_BAYER_GR10:
        case GX_PIXEL_FORMAT_BAYER_RG10:
        case GX_PIXEL_FORMAT_BAYER_GB10:
        case GX_PIXEL_FORMAT_BAYER_BG10:
        case GX_PIXEL_FORMAT_BAYER_GR12:
        case GX_PIXEL_FORMAT_BAYER_RG12:
        case GX_PIXEL_FORMAT_BAYER_GB12:
        case GX_PIXEL_FORMAT_BAYER_BG12:
        {
            // Convert to the Raw8 image
            emDXStatus = DxRaw16toRaw8((unsigned char*)pFrameBuffer->pImgBuf, g_pRaw8Image, pFrameBuffer->nWidth, pFrameBuffer->nHeight, DX_BIT_2_9);
            if (emDXStatus != DX_OK)
            {
                printf("DxRaw16toRaw8 Failed, Error Code: %d\n", emDXStatus);
                return PIXFMT_CVT_FAIL;
            }
            // Convert to the RGB24 image
            emDXStatus = DxRaw8toRGB24((unsigned char*)g_pRaw8Image, g_pRGBImageBuf, pFrameBuffer->nWidth, pFrameBuffer->nHeight,
                              RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(g_i64ColorFilter), false);
            if (emDXStatus != DX_OK)
            {
                printf("DxRaw8toRGB24 Failed, Error Code: %d\n", emDXStatus);
                return PIXFMT_CVT_FAIL;
            }
            break;
        }
        default:
        {
            printf("Error : PixelFormat of this camera is not supported\n");
            return PIXFMT_CVT_FAIL;
        }
    }
    return PIXFMT_CVT_SUCCESS;
}

//-------------------------------------------------
/**
\brief Allocate the memory for pixel format transform
\return void
*/
//-------------------------------------------------
void PreForAcquisition()
{
    g_pRGBImageBuf = new unsigned char[g_nPayloadSize * 3];
    g_pRaw8Image = new unsigned char[g_nPayloadSize];

    return;
}

//-------------------------------------------------
/**
\brief Release the memory allocated
\return void
*/
//-------------------------------------------------
void UnPreForAcquisition()
{
    //Release resources
    if (g_pRaw8Image != NULL)
    {
        delete[] g_pRaw8Image;
        g_pRaw8Image = NULL;
    }
    if (g_pRGBImageBuf != NULL)
    {
        delete[] g_pRGBImageBuf;
        g_pRGBImageBuf = NULL;
    }

    return;
}

//-------------------------------------------------
/**
\brief Acquisition thread function
\param pParam       thread param, not used in this app
\return void*
*/
//-------------------------------------------------
void daheng::aProcGetImage(double dExposureValue, size_t *change)
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    //Thread running flag setup
    g_bAcquisitionFlag = true;
    PGX_FRAME_BUFFER pFrameBuffer = NULL;

    time_t lInit;
    time_t lEnd;
    uint32_t ui32FrameCount = 0;
    uint32_t ui32AcqFrameRate = 0;

    //while(g_bAcquisitionFlag)
    {
        if(!ui32FrameCount)
        {
            time(&lInit);
        }

        // Get a frame from Queue
        emStatus = GXDQBuf(g_hDevice, &pFrameBuffer, 1000);

        if(emStatus != GX_STATUS_SUCCESS)
        {
            if (emStatus == GX_STATUS_TIMEOUT)
            {
                printf("no GX_STATUS_TIMEOUT");
            }
            else
            {
                GetErrorString(emStatus);
                printf("no GetErrorString");
            }
        }

        if(pFrameBuffer->nStatus != GX_FRAME_STATUS_SUCCESS)
        {
            printf("<Abnormal Acquisition: Exception code: %d>\n", pFrameBuffer->nStatus);
        }
        else
        {
            ui32FrameCount++;
            time (&lEnd);
            // Print acquisition info each second.
            if (lEnd - lInit >= 1)
            {
                printf("<Successful acquisition: FrameCount: %u Width: %d Height: %d FrameID: %llu>\n",
                    ui32FrameCount, pFrameBuffer->nWidth, pFrameBuffer->nHeight, pFrameBuffer->nFrameID);
                ui32FrameCount = 0;
            }

            if (1)
            {
                int nRet = PixelFormatConvert(pFrameBuffer);
                if (nRet == PIXFMT_CVT_SUCCESS)
                {
                    cv::Mat src;
                    //SavePPMFile(pFrameBuffer->nWidth, pFrameBuffer->nHeight);        //保存图像
                    getMat(pFrameBuffer->nWidth, pFrameBuffer->nHeight);             //获得Mat图像
                    //cv::namedWindow("src", CV_WINDOW_NORMAL);
                    //cv::imshow("src",src);
                   /* src = cv::imread("pic.ppm");
                    cv::imshow("camera",src);
                    cv::waitKey(10);*/
                    if(*change)
                    {
                        emStatus = GXSetFloat(g_hDevice, GX_FLOAT_EXPOSURE_TIME, dExposureValue);
                        printf("asdqawedawdawqd\n");
                        printf("%d", *change);
                        *change = 0;
                    }
                    //if(1)       start(640,480);
                }
                else
                {
                    printf("PixelFormat Convert failed!\n");
                }
                g_bSavePPMImage = false;
            }
        }

        emStatus = GXQBuf(g_hDevice, pFrameBuffer);
        if(emStatus != GX_STATUS_SUCCESS)
        {
            GetErrorString(emStatus);
            //break;
        }
    }
    printf("<Acquisition thread Exit!>\n");

    return ;
}

//----------------------------------------------------------------------------------
/**
\brief  Get description of input error code
\param  emErrorStatus  error code

\return void
*/
//----------------------------------------------------------------------------------
void GetErrorString(GX_STATUS emErrorStatus)
{
    char *error_info = NULL;
    size_t size = 0;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

    // Get length of error description
    emStatus = GXGetLastError(&emErrorStatus, NULL, &size);
    if(emStatus != GX_STATUS_SUCCESS)
    {
        printf("<Error when calling GXGetLastError>\n");
        return;
    }

    // Alloc error resources
    error_info = new char[size];
    if (error_info == NULL)
    {
        printf("<Failed to allocate memory>\n");
        return ;
    }

    // Get error description
    emStatus = GXGetLastError(&emErrorStatus, error_info, &size);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        printf("<Error when calling GXGetLastError>\n");
    }
    else
    {
        printf("%s\n", (char*)error_info);
    }

    // Realease error resources
    if (error_info != NULL)
    {
        delete []error_info;
        error_info = NULL;
    }
}


void getMat(uint32_t ui32Width, uint32_t ui32Height)
{

        //src.create(ui32Height ,ui32Width ,CV_8UC3);
        //cv::namedWindow("src", CV_WINDOW_AUTOSIZE);
        //memcpy(src.data, g_pRGBImageBuf, ui32Width*ui32Height*3);
        //memcpy(src.data, g_pRGBImageBuf, g_nPayloadSize * 3);
        //cv::imshow("src",src);
#ifdef SHOW_TIME
        double t= (double) cvGetTickCount();
#endif
        FILE* phImageFile = NULL;
        phImageFile = fopen("pic.ppm", "wb");
        fprintf(phImageFile, "P6\n" "%u %u 255\n", ui32Width, ui32Height);
        fwrite(g_pRGBImageBuf, 1, g_nPayloadSize * 3, phImageFile);
        fclose(phImageFile);
#ifdef SHOW_TIME
        t=(double)cvGetTickCount() -t;
        t=t/(cvGetTickFrequency()*1000);
        printf("getMat Use time is: %gms\n", t);
#endif
        //imwrite("./test1.ppm",src);
        //printf("width:%d\n",src.cols);
        //cv::waitKey(100);

    return ;
}

GX_STATUS daheng::change_roi(GX_STATUS emStatus, size_t mode)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    GX_OPEN_PARAM stOpenParam;
    uint32_t nDeviceNum = 0;
    //枚 举 设 备 列 表
    status = GXUpdateDeviceList(&nDeviceNum, 1000);
    if ((status!= GX_STATUS_SUCCESS)||(nDeviceNum<= 0))
    {
         return 0;
    }
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode = GX_OPEN_INDEX;
    stOpenParam.pszContent = "1";
    status = GXOpenDevice(&stOpenParam, &g_hDevice);
    emStatus = status;
   //emStatus = GXOpenDeviceByIndex(1, &g_hDevice);
   emStatus = GXStreamOn(g_hDevice);
    int64_t nWidth= 640;
    int64_t nHeight= 480;
    int64_t nOffsetX = 0;
    int64_t nOffsetY = 0;
    if(0 == mode)
    {
        nWidth= 640;
        nHeight= 480;
        nOffsetX = 0;
        nOffsetY = 0;
    }
    else
    {
        nWidth= 1280;
        nHeight= 1024;
        nOffsetX = 0;
        nOffsetY = 0;
    }
    emStatus = GXSetInt(g_hDevice, GX_INT_WIDTH, nWidth);
    emStatus = GXSetInt(g_hDevice, GX_INT_HEIGHT, nHeight);
    emStatus = GXSetInt(g_hDevice, GX_INT_OFFSET_X, nOffsetX);
    emStatus = GXSetInt(g_hDevice, GX_INT_OFFSET_Y, nOffsetY);
    GX_VERIFY_EXIT(emStatus);
    return emStatus;
}
