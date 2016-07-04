TEMPLATE = app

QT += widgets

TARGET = CQSeahaven

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
main.cpp \
CQSeahaven.cpp \
CQSeahavenCanvas.cpp \
CSeahaven.cpp \
CSeahavenMove.cpp \
CSeahavenPile.cpp \
CSeahavenStack.cpp \
CSeahavenWorkArea.cpp \
CCard.cpp \
CCardDeck.cpp \
CCardPlayer.cpp \

HEADERS += \
CQSeahaven.h \
CQSeahavenCanvas.h \
CSeahaven.h \
CSeahavenMove.h \
CSeahavenPile.h \
CSeahavenStack.h \
CSeahavenWorkArea.h \
CCardDeck.h \
CCard.h \
CCardPlayer.h \

FORMS += \
CQSeahaven.ui

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFont/include \
../../CConfig/include \
../../CFile/include \
../../COS/include \
../../CRegExp/include \
../../CStrUtil/include \
../../CMath/include \
../../CUtil/include \
.

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CRegExp/lib \
-L../../CStrUtil/lib \
-L../../CMath/lib \
-L../../CUtil/lib \
-lCQUtil -lCImageLib -lCFont -lCConfig -lCFileUtil \
-lCFile -lCOS -lCUtil -lCRegExp -lCStrUtil \
-ljpeg -ltre -lpng
