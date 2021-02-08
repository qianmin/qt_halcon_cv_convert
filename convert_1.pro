QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#########QT_HALOCN
macx {
  QMAKE_CXXFLAGS += -F/Library/Frameworks
  QMAKE_LFLAGS   += -F/Library/Frameworks
  LIBS           += -framework HALCONCpp
}
else {
  #defines
  win32:DEFINES += WIN32

  #includes
  INCLUDEPATH   += "$$(HALCONROOT)/include"
  INCLUDEPATH   += "$$(HALCONROOT)/include/halconcpp"

  #libs
  QMAKE_LIBDIR  += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
  unix:LIBS     += -lhalconcpp -lhalcon -lXext -lX11 -ldl -lpthread
  win32:LIBS    += "$$(HALCONROOT)/lib/$$(HALCONARCH)/halconcpp.lib" \
                   "$$(HALCONROOT)/lib/$$(HALCONARCH)/halcon.lib"
}
###opencv(MSVC opencv4.15.1)
# 手动添加工程包含（include）目录：
INCLUDEPATH+= C:\software\opencv\opencv4151\opencv\build\include
INCLUDEPATH+= C:\software\opencv\opencv4151\opencv\build\include\opencv 	// 可不添加
INCLUDEPATH+= C:\software\opencv\opencv4151\opencv\build\include\opencv2	// 可不添加
# 手动添加工程库（lib）目录：
LIBS += -LC:/software/opencv/opencv4151/opencv/build/x64/vc15/lib/ -lopencv_world451d





SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
