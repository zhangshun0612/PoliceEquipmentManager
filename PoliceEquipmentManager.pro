#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T20:59:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PoliceEquipmentManager
TEMPLATE = app


SOURCES += main.cpp\
        policeequipmentmanager.cpp \
    policestaff.cpp \
    firearmclass.cpp \
    firearmclassregisterdialog.cpp \
    databasemanager.cpp \
    policestaffregesterdialog.cpp \
    firearm.cpp \
    firearmregisterdialog.cpp \
    policestaffchoosedialog.cpp \
    chooselineedit.cpp \
    calendardateedit.cpp \
    firearmchoosedialog.cpp \
    firearmmanagerregisterdialog.cpp \
    recordinquirydialog.cpp \
    vehicle.cpp \
    vehicleregisterdialog.cpp \
    vehiclechoosedialog.cpp \
    logindialog.cpp \
    passwordchangedialog.cpp \
    otherequipment.cpp \
    otherequipmentregisterdialog.cpp \
    otherequipmentchoosedialog.cpp

HEADERS  += policeequipmentmanager.h \
    policestaff.h \
    firearmclass.h \
    firearmclassregisterdialog.h \
    databasemanager.h \
    policestaffregesterdialog.h \
    firearm.h \
    firearmregisterdialog.h \
    policestaffchoosedialog.h \
    chooselineedit.h \
    calendardateedit.h \
    firearmchoosedialog.h \
    firearmmanagerregisterdialog.h \
    recordinquirydialog.h \
    vehicle.h \
    vehicleregisterdialog.h \
    vehiclechoosedialog.h \
    logindialog.h \
    passwordchangedialog.h \
    otherequipment.h \
    otherequipmentregisterdialog.h \
    otherequipmentchoosedialog.h

FORMS    += policeequipmentmanager.ui \
    firearmclassregisterdialog.ui \
    policestaffregesterdialog.ui \
    firearmregisterdialog.ui \
    policestaffchoosedialog.ui \
    firearmchoosedialog.ui \
    firearmmanagerregisterdialog.ui \
    recordinquirydialog.ui \
    vehicleregisterdialog.ui \
    vehiclechoosedialog.ui \
    logindialog.ui \
    passwordchangedialog.ui \
    otherequipmentregisterdialog.ui \
    otherequipmentchoosedialog.ui

TRANSLATIONS += lang_cn.ts

OTHER_FILES += \
    myapp.rc

RC_FILE = myapp.rc

RESOURCES += \
    src.qrc
