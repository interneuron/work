#include "mynet.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MyNet::MyNet(QWidget *parent) : QWidget(parent)
{

//    MainWindow obj;
//    obj.ui->statusLabel->setText("!!!!!!!");
    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(30303, QUdpSocket::ShareAddress);
    connect(udpSocket,SIGNAL(readyRead()), SLOT (processPendingDatagrams()));

}

MyNet::~MyNet()
{

}

void MyNet::broadcastDatagram(){

    QByteArray datagram = "Discovery: Who is out there?\0\n";
    udpSocket->writeDatagram(datagram.data(), datagram.size(),QHostAddress::Broadcast, 30303);
}

void MyNet::processPendingDatagrams()
{

    QHostAddress  host;
    quint16       port;
    QString       tempHOST;

    while (udpSocket->hasPendingDatagrams())

    {
        answerDatagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(answerDatagram.data(), answerDatagram.size(),&host, &port);
        tempHOST = host.toString();
    }

   if (answerDatagram!= Discovery){

     MyNet::writeInfoToStruct(answerDatagram, tempHOST);
     MyNet::saveDeviceInfoInFile();
    }
}

void MyNet::writeInfoToStruct(QByteArray &arr, QString &host){
    int k;
    int passBegin,passEnd;
    QByteArray answerDatagram = arr;
    QString m = "\n", mChar;
    if (deviceItem == 0){
        devStruct[deviceItem].numberDevice = deviceItem;
    }
    else{
        deviceItem+=1;
        devStruct[deviceItem].numberDevice = deviceItem;
    }

    devStruct[deviceItem].ipAddres = host;

    qDebug() << "IP адрес устройства "+devStruct[deviceItem].ipAddres;

    for (k = 0; m!= mChar; k++)

        {
            mChar = answerDatagram[k];
            if(mChar == "("){
                passBegin = k+1;
            }
            if(mChar == ")"){
                passEnd = k;
            }
            devStruct[deviceItem].nameDevice += answerDatagram[k];
//            qDebug() << devStruct.nameDevice;
        }
    devStruct[deviceItem].nameDevice = devStruct[deviceItem].nameDevice.simplified();


    for (passBegin=passBegin; passBegin<passEnd; passBegin++){

       devStruct[deviceItem].passDevice += answerDatagram[passBegin];
    }

 //   qDebug()<< devStruct[deviceItem].passDevice;
    mChar.clear();

    for (k=k; m!=mChar; k++){

        mChar = answerDatagram[k];
        devStruct[deviceItem].macAddres += answerDatagram[k];
 //       qDebug() << devStruct[deviceItem].macAddres;

    }
    devStruct[deviceItem].macAddres = devStruct[deviceItem].macAddres.simplified();
    mChar.clear();
    qDebug() << "Имя устройства "+devStruct[deviceItem].ipAddres+" "+devStruct[deviceItem].nameDevice+" "+ devStruct[deviceItem].macAddres;

}

void MyNet::saveDeviceInfoInFile()
{
    infoDevice.setFileName("UserDeviceData.dat");
    if(infoDevice.exists()){
        qDebug()<< "File is Exist";
    }
    else{
        infoDevice.open(QIODevice::WriteOnly);
        infoDevice.close();
        qDebug()<< "File is Create";
    }
    if(infoDevice.open(QIODevice::WriteOnly)){

        infoDevice.open(QIODevice::WriteOnly);
        QTextStream data (&infoDevice);
        data<<"NumberDevice:"<<devStruct[deviceItem].numberDevice<<" : ";
        data<<"IP Addres:"<< devStruct[deviceItem].ipAddres<<" : ";
        data<<"DeviceName:"<< devStruct[deviceItem].nameDevice <<" : ";
        data<<"Mac Addres:"<< devStruct[deviceItem].macAddres;
        infoDevice.close();

        qDebug()<< "File is Exist";
    }
    else{
        qDebug("Error");
    }


    MainWindow *mw = new MainWindow();
    mw->addTableItems(devStruct[deviceItem].ipAddres);

    qDebug() << mw->objectName();


    qDebug()<< devStruct[deviceItem].ipAddres;
}


