QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

CONFIG += c11 warn_off

include(qtgifimage/src/gifimage/qtgifimage.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    myopenglspace.cpp \
    parser.c \
    setting.cpp \
    viewer.cpp

HEADERS += \
    myopenglspace.h \
    parser.h \
    setting.h \
    viewer.h

FORMS += \
    setting.ui \
    viewer.ui

unix:!macx: LIBS += -LlOpenGL -LOpenGL
win32 {
    LIBS += -lopengl32
    RC_ICONS = res\images\appico.ico
}
macx: ICON = res/images/appico.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
