#-------------------------------------------------
#
# Project created by QtCreator 2010-08-02T22:13:12
#
#-------------------------------------------------

TARGET = nes-emulator
TEMPLATE = lib

# Remove Qt libraries
QT =

TOP = ../..

# Build static library
#CONFIG += staticlib

# Remove crap we don't need!
CONFIG -= rtti exceptions

INCLUDEPATH += \
   . \
   ./common \
   ./emulator \
   $$TOP/common

SOURCES += \
   $$TOP/common/cbreakpointinfo.cpp \
   emulator/cnesrommapper068.cpp \
   emulator/cnesrommapper065.cpp \
   emulator/cnesrommapper011.cpp \
   emulator/cnesrommapper010.cpp \
   emulator/cnesrommapper009.cpp \
   emulator/cnesrommapper007.cpp \
   emulator/cnesrommapper005.cpp \
   emulator/cnesrommapper004.cpp \
   emulator/cnesrommapper003.cpp \
   emulator/cnesrommapper002.cpp \
   emulator/cnesrommapper001.cpp \
   emulator/cnesrom.cpp \
   emulator/cnesppu.cpp \
   emulator/cnesmappers.cpp \
   emulator/cnesio.cpp \
   emulator/cnesapu.cpp \
   emulator/cnes.cpp \
   emulator/cnes6502.cpp \
   common/cnessystempalette.cpp \
   nes_emulator_core.cpp \
   emulator/cmarker.cpp \
   emulator/cjoypadlogger.cpp \
   emulator/ccodedatalogger.cpp \
   emulator/ctracer.cpp \
   emulator/cnesbreakpointinfo.cpp \
   emulator/cnesios.cpp \
   emulator/cnesrommapper033.cpp \
   emulator/cnesrommapper069.cpp \
   emulator/cnesrommapper075.cpp \
   emulator/cnesrommapper034.cpp

HEADERS +=\
   emulator/cnesrommapper068.h \
   emulator/cnesrommapper065.h \
   emulator/cnesrommapper011.h \
   emulator/cnesrommapper010.h \
   emulator/cnesrommapper009.h \
   emulator/cnesrommapper007.h \
   emulator/cnesrommapper005.h \
   emulator/cnesrommapper004.h \
   emulator/cnesrommapper003.h \
   emulator/cnesrommapper002.h \
   emulator/cnesrommapper001.h \
   emulator/cnesrom.h \
   emulator/cnesppu.h \
   emulator/cnesmappers.h \
   emulator/cnesio.h \
   emulator/cnesapu.h \
   emulator/cnes.h \
   emulator/cnes6502.h \
   nes_emulator_core.h \
   common/cnessystempalette.h \
   emulator/cmarker.h \
   emulator/cjoypadlogger.h \
   emulator/ccodedatalogger.h \
   emulator/ctracer.h \
   emulator/cnesios.h \
   emulator/cnesrommapper033.h \
   emulator/cnesrommapper069.h \
   emulator/cnesrommapper075.h \
   emulator/cnesbreakpointinfo.h \
   emulator/cnesrommapper034.h