TEMPLATE = app

TARGET = 

DEPENDPATH += .

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
CQUtil.cpp \
CQImage.cpp \
CQFont.cpp \

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
CQUtil.h \
CQImage.h \
CQFont.h \

FORMS += \
CQSeahaven.ui

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
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
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CRegExp/lib \
-L../../CStrUtil/lib \
-L../../CMath/lib \
-L../../CUtil/lib \
-lCImageLib -lCFont -lCConfig -lCFile -lCOS -lCRegExp -lCStrUtil \
-ljpeg -ltre
