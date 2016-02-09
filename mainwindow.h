#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStatusBar>
#include <QLabel>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include <QStatusBar>
#include <QDebug>
#include <QHostInfo>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QApplication>
#include <QPushButton>
#include <QSignalMapper>
#include <QTcpSocket>
#include <QMap>
#include <QTableWidget>
#include <QVector>

#include "broadcast.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    #define conT  1000
    #define PORT    23
    #define WRR   3000
    #define WBR   3000

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openMain();
    void slotSendActivation(int a);
    Broadcast *broad = new Broadcast(this);
    QTimer *timer;
    qint8 DI = 0;
    QSettings *ini = new QSettings(qApp->applicationDirPath()+"/config.ini",QSettings::IniFormat);
    QString arr[10];
    QByteArray cmd1 = "1\n";
    QByteArray cmd2 = "y\n";
    QByteArray cmd3 = "q\n";
    QFont fnt; 
    QUdpSocket *networkInterface;
    QUdpSocket *tcpALL;
    QStringList items;
    qint8 colNI = 0;
    QString errorMessage = "Хьюстон, у нас проблемы. Проверьте соединение с устройством!";

    //----------------------------------------------------------------
private slots:
    void on_findButton_clicked();
    void processPendingDatagrams();
    void writeInfoToStruct(QString &host);
    void saveDeviceInfoInFile();
    void loadDeviceFromFile();
    void addTableItem();
    void loadPreference();     
    void addConnect(int a,int b);
    void slotError(QAbstractSocket::SocketError);
    void errorState(){qDebug()<< "Хьюстон, у нас проблемы";}
    void getNetworkInterface();
    void on_saveDevBX_toggled(bool checked);
    void on_passStartBox_toggled(bool checked);
    void on_lineEditBox_returnPressed();
    void on_lineEditBox_textChanged();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void slotCheckBox();
    void slotConnectTableStatus(int a);
    void slotBadConnectTableStatus(int a);
    void slotDisconnectTableStatus(int a);
    void slotNoConnect(int a);
    void slotHHeader(int a);

    qint8 getItemCheckBox(int i){
        int a = 0;
        CheckBoxList.indexOf(i);
        if(i != -1)
        {
            a = CheckBoxList.at(i);
        }
        return a;
    }

// Проверка на повторы-----------------------------------------------------
    bool checkDevPresent(QString *ip, QString *devName){
        bool ans;
        for(int i = 0; i <= DI; i++ )
            {
                if((ip == devStruct[i].ipAddres)&&(devName == devStruct[i].nameDevice))
                 {
                    ans = true;
                    qDebug()<< "найдены копии!" << devStruct[i].nameDevice << "Номер устройства" << i;
                    break;
                 }
                else
                 {
                    ans = false;
                    qDebug()<< "Не совпадает!"<< i;
                 }
            }
        return ans;
    }
//--------------------------------------------------------------------------
    void on_ActiveAll_clicked();

public slots:

signals:

private:

    Ui::MainWindow *ui;
    void createTable();
    void renewTable();
    QFile infoDevice;
    QByteArray Discovery = "Discovery: Who is out there?\0\n";
    QByteArray answerDatagram;
    QString writeEnd = "Устройства найдены";
    QSettings *dList;
    QVector<int> CheckBoxList;
    bool checkStateHeader = false;


    struct devInfo
    {
        QString ipAddres;
        QString nameDevice;
        QString macAddres;
        QString passDevice;
        qint8   numberDevice;
        bool    state = false;
        bool    checkBox = false;

    };
    devInfo devStruct[10];
};

#endif // MAINWINDOW_H
