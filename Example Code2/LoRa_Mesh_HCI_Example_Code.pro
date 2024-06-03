QT -= gui
QT += core serialport

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

SOURCES += \
        RadioHub/RadioHub.cpp \
        RadioHub/ServiceAccessPoints/LoRaMeshRouter/LoRaMeshRouter.cpp \
        RadioHub/ServiceAccessPoints/ServiceAccessPoint.cpp \
        RadioHub/ServiceAccessPoints/DeviceManagement/DeviceManagement.cpp \
        RadioHub/ServiceAccessPoints/Trace/Trace.cpp \
        RadioHub/Utils/CRC16.cpp \
        RadioHub/Utils/SerialMessage.cpp \
        RadioHub/Utils/SlipDecoder.cpp \
        RadioHub/Utils/SlipEncoder.cpp \
        Utils/Console.cpp \
        LoRa_Mesh_DemoApp.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    RadioHub/RadioHub.h \
    RadioHub/ServiceAccessPoints/LoRaMeshRouter/LoRaMeshRouter.h \
    RadioHub/ServiceAccessPoints/ServiceAccessPoint.h \
    RadioHub/ServiceAccessPoints/DeviceManagement/DeviceManagement.h \
    RadioHub/ServiceAccessPoints/Trace/Trace.h \
    RadioHub/Utils/CRC16.h \
    RadioHub/Utils/SerialMessage.h \
    RadioHub/Utils/SlipDecoder.h \
    RadioHub/Utils/SlipEncoder.h \
    Utils/Console.h \
    LoRa_Mesh_DemoApp.h
