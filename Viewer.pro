#-------------------------------------------------
# This Code Skeleton uses Qt 3D for 3D rendering #
#-------------------------------------------------
QT += core gui widgets 3dcore 3drender 3dinput 3dlogic 3dextras

TARGET = Viewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES  += main.cpp renderview.cpp

HEADERS  += renderview.h

FORMS    += renderview.ui
