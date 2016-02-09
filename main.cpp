/*

QMAKE_LFLAGS += -static -static-libgcc


*/
#include <QApplication>
#include <QTableWidgetItem>
#include <qdebug.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QCryptographicHash>
#include <QSettings>
#include <QTextCodec>

#include "mainwindow.h"
#include "passbox.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    QSettings *ini = new QSettings(qApp->applicationDirPath()+"/config.ini",QSettings::IniFormat);
    if(!ini->value("CheckBox/passStartBox").toBool()){
        PassBox *p = new PassBox;
        // p->setAttribute(Qt::WA_DeleteOnClose,true);
        p->setFixedSize(315,107);
        p->show();
    }
    else{
        MainWindow *wdw = new MainWindow;
        // wdw->setAttribute(Qt::WA_DeleteOnClose,true);
        wdw->openMain();
    }
    ini->deleteLater();
    return a.exec();
}
