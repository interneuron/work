#ifndef MYNET_H
#define MYNET_H

#include <QWidget>
#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include <QStatusBar>
#include <QDebug>
#include <QLabel>
#include <QHostInfo>
#include <QFile>
#include <QTextStream>

class MyNet : public QWidget
{
    Q_OBJECT

public:
    explicit MyNet(QWidget *parent = 0);
    ~MyNet();
    qint8 deviceItem = 0;
    QString msg = "Test";
private slots:

    void processPendingDatagrams();
    void writeInfoToStruct(QByteArray &arr, QString &host);
    void saveDeviceInfoInFile();

public slots:

    void broadcastDatagram();

private:
    QFile infoDevice;
    QUdpSocket *udpSocket;
    QTimer *timer;
    QByteArray Discovery = "Discovery: Who is out there?\0\n";
    QByteArray answerDatagram;
    QString writeEnd = "Устройства найдены";
// Структура для хранения информации об устройствах
    struct devInfo
    {
        QString ipAddres;
        QString nameDevice;
        QString macAddres;
        QString passDevice;
        qint8   numberDevice;

    };
    devInfo devStruct[20];
//--------------------------------
signals:
void sendStringMy(int i);



};




#endif // MYNET_H
