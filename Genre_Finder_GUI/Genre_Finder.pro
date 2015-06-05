TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    GenreFinder.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    GenreFinder.h \
    GUI_Object.h \
    libzplay.h \
    dirent.h \
    Genre.h \
    AudioData.h

LIBS += -L"$$_PRO_FILE_PWD_/libs/" -llibzplay

RC_FILE = appRC.rc #application icon
