QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

 LIBS += -lgxiapi \
 /usr/local/lib/libopencv_highgui.so\
 /usr/local/lib/libopencv_core.so\
 /usr/local/lib/libopencv_imgproc.so\
 /usr/local/lib/libopencv_imgcodecs.so\
 /usr/local/lib/libopencv_shape.so\
 /usr/local/lib/libopencv_videoio.so\

LIBS += `pkg-config --libs opencv` \
    /usr/lib/x86_64-linux-gnu/libpthread.so

TARGET = GxViewer
TEMPLATE = app

unix:!mac:QMAKE_LFLAGS += -L/usr/lib -L./ -Wl,--rpath=.:/usr/lib

INCLUDEPATH+=/usr/local/include\
 /usr/local/include/opencv\
 /usr/local/include/opencv2\
/home/aji/桌面/all_aji7.23/AJI-daheng/GxViewer\
/home/aji/桌面/all_aji7.23/AJI-daheng/armor\
/home/aji/桌面/all_aji7.23/AJI-daheng/串口\
/home/aji/桌面/all_aji7.23/AJI-daheng/角度解算\
/home/aji/桌面/all_aji7.23/AJI-daheng/预测\
/home/aji/桌面/all_aji7.23/AJI-daheng/风车\
/home/aji/桌面/all_aji7.23/AJI-daheng/tuoluo\
/home/aji/桌面/all_aji7.23/AJI-daheng/滤波\
/home/aji/桌面/all_aji7.23/AJI-daheng/sentry\
/home/aji/桌面/all_aji7.23/AJI-daheng/predict

SOURCES += \
        GxViewer/daheng.cpp \
        Threads.cpp \
        armor/armor.cpp \
        armor/lastarmor.cpp \
        armor/light.cpp \
        main.cpp \
        串口/serial.cpp \
        角度解算/AngleSolver.cpp \
        预测/ArmorPredict.cpp \
        预测/StoreDate.cpp \
        预测/armorpredictrun.cpp \
        预测/distancepredict.cpp \
        预测/shootTuoLuo.cpp \
        预测/staticpredict.cpp \
        风车/Rotate.cpp \
        风车/findCenter.cpp \
        风车/findbuff.cpp \
        风车/hitpredict.cpp \
        风车/pretreat.cpp \
        风车/run3.cpp \
        sentry/sentry.cpp \
    tuoluo/tuoluo.cpp \
    滤波/kalman.cpp \
    predict/predict.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    GxViewer/DxImageProc.h \
    GxViewer/GxIAPI.h \
    GxViewer/daheng.h \
    Threads.h \
    armor/armor.h \
    armor/lastarmor.h \
    armor/light.h \
    base.h \
    串口/serial.h \
    角度解算/AngleSolver.h \
    角度解算/Gernal.h \
    预测/ArmorPredict.h \
    预测/StoreDate.h \
    预测/armorpredictrun.h \
    预测/distancepredict.h \
    预测/shootTuoLuo.h \
    预测/staticpredict.h \
    风车/Rotate.h \
    风车/buffbase.h \
    风车/findCenter.h \
    风车/findbuff.h \
    风车/hitpredict.h \
    风车/pretreat.h \
    tuoluo/tuoluo.h \
    sentry/sentry.h \
    滤波/kalman.h \
    predict/predict.h
