TEMPLATE = app

QT += qml quick
CONFIG += c++11
#QMAKE_CXX += -g

LIBS += -F/System/Library/Frameworks
SOURCES += main.cpp \
    isingmodelsystem.cpp \
    random.cpp \
    graph.cpp \
    linegraph.cpp \
    figure.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    isingmodelsystem.h \
    random.h \
    random2.h \
    graph.h \
    linegraph.h \
    figure.h

