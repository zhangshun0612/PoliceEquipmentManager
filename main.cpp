#include "policeequipmentmanager.h"
#include <QApplication>
#include <QTranslator>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QElapsedTimer>


int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    QSystemSemaphore sema("PoliceEquipManagerSem", 1, QSystemSemaphore::Open);
    sema.acquire();
    QSharedMemory mem("PoliceEquipManagerSingleFlag");
    if(!mem.create(1))
    {
        QMessageBox::information(0, QObject::tr("Error"), QObject::tr("An instance is Running"));

        sema.release();
        return 0;
    }

    sema.release();

    QTranslator translator;
    translator.load("lang_cn");
    a.installTranslator(&translator);

    QSplashScreen splash(QPixmap(":/image/src/splash.bmp"));
    splash.setDisabled(true);
    splash.show();

    QElapsedTimer timer;
    timer.start();

    while(timer.elapsed() < 3000){
        QApplication::processEvents();
    }


    PoliceEquipmentManager w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
