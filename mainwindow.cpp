#include <QTableWidget>
#include <QTcpSocket>
#include <QComboBox>
#include <QFont>
#include <QTime>
#include <QString>
#include <QMessageBox>
#include <QTextCodec>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "broadcast.h"
#include "passbox.h"
#include "changepass.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //    setFixedSize(768,335);
    ui->setupUi(this);
    ui->wronglblBox->setHidden(true);
    timer = new QTimer(this);
    ui->ActiveAll->setDefault(true);
    ui->ActiveAll->setDisabled(true);

    connect(ui->passBoxBut,SIGNAL(clicked()),SLOT(on_lineEditBox_returnPressed()));

    //getNetworkInterface();
    createTable();

}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug()<< "MainWindows - was Deleted";
}

void MainWindow::openMain()
{
    setFixedSize(770,335);
    show();
    statusBar()->showMessage("Добро пожаловать!");
    loadPreference();
    //this->getNetworkInterface();
    //   createTable();
}

void MainWindow::on_findButton_clicked()
{
    items.clear();
    getNetworkInterface();
    colNI = (items.count())-1;
    for (int i = 0;i<=colNI; i++)
    {
        QString s = QString::number(i);
        QUdpSocket *sock = this->findChild<QUdpSocket*>(s);
        sock->writeDatagram(Discovery.data(),Discovery.size(),QHostAddress::Broadcast, 30303);
    }
    ui->statusBar->showMessage("Поиск устройств...");
}

void MainWindow::processPendingDatagrams()
{
    QObject *object = QObject::sender();
    QUdpSocket *socketUDP = static_cast<QUdpSocket *>(object);

    QHostAddress  host;
    quint16       port = 0;
    QString       tempHOST = "";
    QString       devName = "";

    while (socketUDP->hasPendingDatagrams())
    {
        answerDatagram.resize(socketUDP->pendingDatagramSize());
        socketUDP->readDatagram(answerDatagram.data(), answerDatagram.size(),&host, &port);
        tempHOST = host.toString();

        if (answerDatagram!= Discovery)
        {
            devName = broad->Broadcast::getDeviceInfo(answerDatagram);

            if(!checkDevPresent(&tempHOST,&devName))
            {
                writeInfoToStruct(tempHOST);
                addTableItem();
            }
            else
            {
                devName.clear();
                ui->statusBar->showMessage("Устройства найдены!");
            }
        }
    }
}

void MainWindow::writeInfoToStruct(QString &host){
    if (DI == 0)
    {
        DI = 1;
        devStruct[DI].numberDevice = DI;
    }
    else
    {
        DI += 1;
        devStruct[DI].numberDevice = DI;
    }
    devStruct[DI].ipAddres = host;
    devStruct[DI].macAddres = broad->Broadcast::getDeviceMac();
    devStruct[DI].nameDevice = broad->Broadcast::getDeviceName();
    devStruct[DI].passDevice = broad->Broadcast::getDevicePass();
}
void MainWindow::saveDeviceInfoInFile()
{

    PassBox *pb = new PassBox;
    infoDevice.setFileName(".\\UserDeviceData.dat");

    if (infoDevice.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        infoDevice.close();
    }
    if(infoDevice.open(QIODevice::Append)){

        //  infoDevice.open(QIODevice::Append);
        for(int i = 1; i<=DI; i++)
        {
            QTextStream data (&infoDevice);
            data<< devStruct[i].numberDevice<<":";
            data<< devStruct[i].ipAddres<<":";
            data<< devStruct[i].nameDevice <<":";
            data<< pb->encodeStr(devStruct[i].passDevice)<<":";
            data<< devStruct[i].macAddres <<":\r";
        }
    }
    infoDevice.close();
    pb->deleteLater();
    pb->close();
}

void MainWindow::loadDeviceFromFile()
{
    int j=1,b = 0;

    infoDevice.setFileName(".\\UserDeviceData.dat");
    if(infoDevice.open(QIODevice::ReadOnly |QIODevice::Text))
    {

        PassBox *pb = new PassBox(this);

        while(!infoDevice.atEnd())
        {
            //читаем строку
            QString str = infoDevice.readLine();
            //Делим строку на слова разделенные пробелом
            QStringList lst = str.split(":");
            b = lst.count()-1;
            // выводим первых три слова
            for(int i=0; i<b; i++)
            {
                DI++;
                devStruct[DI].numberDevice = DI;
                devStruct[DI].ipAddres = lst.at(j);
                j++;
                devStruct[DI].nameDevice = lst.at(j);
                j++;
                devStruct[DI].passDevice = pb->decodeStr(lst.at(j));
                pb->deleteLater();
                pb->close();
                j++;
                devStruct[DI].macAddres = lst.at(j);
                j = j+2;
                i = j;
            }
            qDebug() << lst.at(0) << lst.at(1)<<lst.at(2);
        }
    }
    else
    {
        qDebug()<< "don't open file";

    }
    infoDevice.close();
    addTableItem();
}
void MainWindow::createTable()
{
    int i,k,m;
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setRowCount(10);
    k = ui->tableWidget->rowCount();
    m = ui->tableWidget->columnCount();
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),SLOT(addConnect(int, int)));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotHHeader(int)));
    int ct = 1;
    for(i = 0; i < k; i++)
    {
        ui->tableWidget->setRowHeight(i,20);
        for(int j=0; j < m; j++)
        {
            QTableWidgetItem *itm = new QTableWidgetItem(tr(""));
            ui->tableWidget->setItem(i,j,itm);
            itm->setTextAlignment(Qt::AlignCenter);
            itm->tableWidget()->setColumnWidth(j,120);
            itm->tableWidget()->setColumnWidth(0,15);
            ui->tableWidget->setRowHidden(i,true);
            ui->tableWidget->horizontalHeader()->setResizeMode( j, QHeaderView::Fixed );
            if(j == 1)
            {
                itm->tableWidget()->setColumnWidth(j,110);
            }
            if((j!=5)&&(j!= 6))
            {
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
            }
            if(j == 5)
            {
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsDropEnabled);
                itm->tableWidget()->setColumnWidth(j,115);
            }
            if(j == 6)
            {
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::gray);
                ui->tableWidget->item(i,j)->setTextColor(Qt::white);
                fnt.setPointSize(9);
                fnt.setBold(true);
                fnt.setFamily("Arial");
                ui->tableWidget->item(i,j)->setFont(fnt);
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
            }
            if(j == 0)
            {
                QCheckBox *cb = new QCheckBox(this);
                QString oName = QString::number(i);
                cb->setObjectName(oName);
                ui->tableWidget->setCellWidget(i,j,cb);

                connect(cb,SIGNAL(stateChanged(int)),this,SLOT(slotCheckBox()));
            }
        }
        ct++;
    }
}
void MainWindow::addTableItem()
{
    int i;
    ui->statusBar->showMessage("Устройства найдены!");
    for (i=1; i<=DI;i++)
    {
        ui->tableWidget->setRowHidden(i,false);

        arr[1]=devStruct[i].ipAddres;
        arr[2]=devStruct[i].nameDevice;
        arr[3]=devStruct[i].macAddres;
        arr[4]= "Найдено";
        arr[5]= "Активировать";
        arr[6]= "Подключить";

        for(int j=1; j<ui->tableWidget->columnCount();j++)
        {
            ui->tableWidget->item(i,j)->setText(arr[j]);
        }
    }
}

void MainWindow::renewTable()
{
    int i,k;
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setRowCount(10);
    k = ui->tableWidget->rowCount();
    int ct = 1;
    for(i = 0; i < k; i++)
    {
        ui->tableWidget->setRowHeight(i,20);
        for(int j=0; j<ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem *itm = new QTableWidgetItem(tr(""));
            ui->tableWidget->setItem(i,j,itm);
            itm->setTextAlignment(Qt::AlignCenter);
            itm->tableWidget()->setColumnWidth(j,120);
            itm->tableWidget()->setColumnWidth(0,15);
            ui->tableWidget->setRowHidden(i,true);
            ui->tableWidget->horizontalHeader()->setResizeMode( j, QHeaderView::Fixed );
            if((j!= 5)&&(j!=6)){

                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
            }
            if(j==5)
            {
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsDropEnabled);
            }
            if(j==6)
            {
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::gray);
                ui->tableWidget->item(i,j)->setTextColor(Qt::white);
                fnt.setPointSize(10);
                fnt.setBold(true);
                fnt.setFamily("Arial");
                ui->tableWidget->item(i,j)->setFont(fnt);
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
            }
            if(j == 0)
            {
                QCheckBox *cb = new QCheckBox(this);
                QString oName = QString::number(i);
                cb->setObjectName(oName);
                ui->tableWidget->setCellWidget(i,j,cb);

                connect(cb,SIGNAL(toggled(bool)),this,SLOT(slotCheckBox()));
            }
        }
        ct++;
    }

}

void MainWindow::loadPreference()
{   
    if(ini->value("CheckBox/saveDevCB").toBool())
    {
        ui->saveDevBX->setCheckState(Qt::Checked);
        loadDeviceFromFile();
    }
    if(ini->value("CheckBox/passStartBox").toBool())
    {
        ui->passStartBox->setCheckState(Qt::Checked);
    }

}
void MainWindow::addConnect(int a, int b)
{
    qDebug()<<a<<b;
    QString statusLinc = ui->tableWidget->item(a,4)->text();

    if(((statusLinc == "Найдено")&&(b == 6))||((statusLinc == "Нет связи")&&(b == 6)))
    {
        QString password = devStruct[a].passDevice + "\n";
        QString warCon = "Подключение к "+ ui->tableWidget->item(a,2)->text();
        QString s = QString::number(a);

        QByteArray pass(password.toUtf8());
        QByteArray ip(devStruct[a].ipAddres.toUtf8());

        QTcpSocket *tcpALL = new QTcpSocket(this);
        tcpALL->setObjectName(s);
        connect(tcpALL,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(slotError(QAbstractSocket::SocketError)));
        tcpALL->connectToHost(ip,PORT);

        if (tcpALL->waitForConnected(conT))
        {
            qWarning() << "Соединяем";
            if(!tcpALL->waitForReadyRead(WRR))
            {
                qWarning() << "false";
            }
            if(tcpALL->write(pass.data(),pass.size()))
            {
                QByteArray answer = tcpALL->readAll();
                if(answer!= "")
                {

                    slotConnectTableStatus(a);
                    answer = tcpALL->readAll();
                    devStruct[a].state = true;
                    qDebug()<<"Connected! "<<devStruct[a].ipAddres;
                }
            }
        }
        else
        {
            slotBadConnectTableStatus(a);
            int button = QMessageBox::warning(this,warCon,QString::fromLocal8Bit("Не удалось подключиться. Проверьте соединение с устройством!"), QMessageBox::Ok);
            if (button == QMessageBox::Ok){
                ui->statusBar->clearMessage();
            }
            qWarning() << "No connect";
        }
    }
    else if((ui->tableWidget->item(a,4)->text()=="Активно")&&(b == 6))
    {
        QString s = QString::number(a);
        QTcpSocket *sock = this->findChild<QTcpSocket*>(s);
        sock->disconnectFromHost();
        if (sock->state() == QAbstractSocket::UnconnectedState || sock->waitForDisconnected(1000))
        {
            slotDisconnectTableStatus(a);
            devStruct[a].state = false;
            sock->deleteLater();
            qDebug()<<"Отключили";
        }
    }
    else if((ui->tableWidget->item(a,4)->text()=="Активно")&&(b == 5))
    {
        QString s = QString::number(a);
        QTcpSocket *sock = this->findChild<QTcpSocket*>(s);
        if(sock->state()== QAbstractSocket::ConnectedState)
        {
            QByteArray answer;
            QString warring = "Активация "+ ui->tableWidget->item(a,2)->text();
            QString warring_2 = "Вы уверены что хотите провести активацию?";
            int button = QMessageBox::question(this,warring,warring_2,QMessageBox::Yes | QMessageBox::No);
            answer = sock->readAll();
            answer.clear();
            if (button == QMessageBox::Yes)
            {
                if(sock->write(cmd1.data(),cmd1.size()))
                {
                    if(!sock->waitForReadyRead(WRR))
                    {
                        qWarning() << "false";

                    }
                    sock->waitForBytesWritten(WBR);
                    answer = sock->readAll();
                    if(answer != "")
                    {
                        answer.clear();
                        if(sock->write(cmd2.data(),cmd2.size()))
                        {
                            if(sock->state() == QAbstractSocket::ConnectedState)
                            {
                                sock->waitForReadyRead(WRR);
                                sock->waitForBytesWritten(WBR);
                                answer = sock->readAll();
                                if(answer != "")
                                {
                                    ui->statusBar->showMessage("Устройство успешно активировано!");
                                    int button = QMessageBox::information(this,warring,QString::fromLocal8Bit("Устройство успешно активировано!"), QMessageBox::Ok);
                                    if(button == QMessageBox::Ok)
                                    {
                                        ui->statusBar->clearMessage();
                                    }
                                    qDebug()<< "2 cmd is OK";
                                }
                                else
                                {
                                    devStruct[a].state = false;
                                    sock->disconnectFromHost();
                                    slotDisconnectTableStatus(a);
                                    ui->statusBar->showMessage("Активация устройства не выполнена!");
                                    int button = QMessageBox::warning(this,warring,QString::fromLocal8Bit("Активация устройства не выполнена!"), QMessageBox::Ok);
                                    if (button == QMessageBox::Ok){
                                        ui->statusBar->clearMessage();
                                    }
                                    qDebug()<< "2 cmd is bad";
                                }
                            }
                        }

                    }
                    else
                    {
                        devStruct[a].state = false;
                        sock->disconnectFromHost();
                        slotNoConnect(a);
                        ui->statusBar->showMessage("Активация устройства не выполнена!");
                        int button = QMessageBox::warning(this,warring,QString::fromLocal8Bit("Активация устройства не выполнена!"), QMessageBox::Ok);
                        if (button == QMessageBox::Ok){
                            ui->statusBar->clearMessage();
                        }
                        qDebug()<< "1 cmd is bad";
                    }
                }
            }
        }// if(tcpALL->state()== QAbstractSocket::ConnectedState)
    }//else if
}// MainWindow::addConnect

void MainWindow::on_saveDevBX_toggled(bool checked)
{
    QString val = QString::number(checked);
    ini->setValue("CheckBox/saveDevCB",val);
}
void MainWindow::on_passStartBox_toggled(bool checked)
{
    QString val = QString::number(checked);
    ini->setValue("CheckBox/passStartBox",val);
    // qDebug()<< "проверка";
}


void MainWindow::on_lineEditBox_returnPressed()
{
    PassBox *pb = new PassBox(this);
    changePass *cp = new changePass(this);
    QString tempPWD = pb->getPassword_all();
    if((ui->lineEditBox->text()) == tempPWD)
    {
        ui->lineEditBox->clear();
        cp->show();
        cp->exec();
    }
    else
    {
        ui->lineEditBox->clear();
        ui->wronglblBox->setHidden(false);
        cp->deleteLater();
    }
}

void MainWindow::on_lineEditBox_textChanged()
{
    ui->wronglblBox->setHidden(true);
}


void MainWindow::on_pushButton_2_clicked()
{

    for(int i=1;i<=DI;i++){
        if(devStruct[i].state == true){
            QString s = QString::number(i);
            QTcpSocket *sock = this->findChild<QTcpSocket*>(s);
            sock->disconnectFromHost();
            sock->deleteLater();
        }
    }
    for(int i=0; i<ui->tableWidget->columnCount();i++){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowHidden(i,true);
        devStruct[i].ipAddres.clear();
        devStruct[i].macAddres.clear();
        devStruct[i].nameDevice.clear();
        devStruct[i].numberDevice = 0;
        devStruct[i].passDevice.clear();
        devStruct[i].state = false;
    }
    DI = 0;
    CheckBoxList.clear();
    ui->ActiveAll->setDisabled(true);
    items.clear();
    renewTable();
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    qDebug()<< err;
    QObject *object = QObject::sender();
    QTcpSocket *socket = static_cast<QTcpSocket *>(object);
    QString strError = "Error: " +
            (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
             err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
             err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
             QString(socket->errorString()));
    qDebug()<< strError;
}

void MainWindow::getNetworkInterface() // Ищем включенные интерфейсы
{
    int i = 0;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        //Ищем интерфесы с айпишниками
        if (interface.flags().testFlag(QNetworkInterface::IsRunning))
            foreach (QNetworkAddressEntry entry, interface.addressEntries())
            {
                if ( interface.hardwareAddress() != "00:00:00:00:00:00"&& entry.ip().toString().contains(".")&& entry.ip().toString()!="127.0.0.1")
                {
                    //Добавили найденные интерфейсы в список
                    items << entry.ip().toString();
                    QString s = QString::number(i);
                    QUdpSocket *networkInterface = new QUdpSocket(this);
                    networkInterface->setObjectName(s);
                    networkInterface->bind(QHostAddress(items.at(i)),30303,QUdpSocket::ShareAddress);
                    connect(networkInterface,SIGNAL(readyRead()), SLOT (processPendingDatagrams()));
                    i++;
                }
            }
    }
}


void MainWindow::on_ActiveAll_clicked()
{
    QTcpSocket tcp;
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/IMG/images.png"));
    QString badDevice;

    if(!CheckBoxList.isEmpty())
    {
        int c = CheckBoxList.count()-1;
        bool badConnectedDevice = false;
        qDebug()<< "CheckBox Count: "<<c;

        for(int i = 0; i < c; i++)
        {
            int a = getItemCheckBox(i);
            if(devStruct[a].state == false)
            {
                QString password = devStruct[a].passDevice + "\n";
                QByteArray pass(password.toUtf8());
                QByteArray ip(devStruct[a].ipAddres.toUtf8());

                QTcpSocket *tcpALL = new QTcpSocket(this);
                QString s = QString::number(a);
                tcpALL->setObjectName(s);
                connect(tcpALL,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(slotError(QAbstractSocket::SocketError)));
                tcpALL->connectToHost(ip,23);
                if (tcpALL->waitForConnected(conT))
                {
                    devStruct[a].state = true;
                    if(tcpALL->waitForReadyRead(WRR))
                    {
                        qWarning() << "false";
                    }
                    if(tcpALL->write(pass.data(),pass.size()))
                    {
                        QByteArray answer = tcpALL->readAll();
                        if(answer!= "")
                        {
                            slotConnectTableStatus(a);
                            answer = tcpALL->readAll();
                            devStruct[a].state = true;
                            qDebug()<<"Connected! "<<devStruct[a].ipAddres;
                        }
                        qWarning() << "Отправляем пароль";
                    }
                    qDebug()<<"Connected:"<< a << devStruct[a].state << DI;
                }
                else
                {
                    QString str = "Устройство: "+devStruct[a].ipAddres +"-"+ devStruct[a].nameDevice + "-не отвечает\n";
                    badDevice += str;
                    badConnectedDevice = true;
                    devStruct[a].checkBox = false;
                }
            }

        }
        if(badConnectedDevice == true)
        {
            msgBox.setDetailedText(badDevice);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle("Хьюстон, у нас проблемы!");
            msgBox.setText(tr("Не удалось подключиться к некоторым устройствам.\n"
                              "Проверьте подключены ли устройства!"));
            int m = CheckBoxList.count()-1;
            int j = 0;
            for (int i = 0;i<m; i++)
            {
                if(devStruct[CheckBoxList.at(i)].state == true)
                {
                    j++;
                }
            }
            if(j>1)
            {
                msgBox.setInformativeText(tr("Желаете продолжить активацию устройств?"));
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

            }
            else
            {
                msgBox.setInformativeText(tr("Нет подключенных устройств для активации"));
                msgBox.setStandardButtons(QMessageBox::Cancel);
            }
            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Ok:
                // Save was clicked
                badConnectedDevice = false;
                msgBox.setText(tr(""));

                break;
            case QMessageBox::Cancel:
                badConnectedDevice = true;
                badDevice.clear();
                qDebug()<< "Cancel";
                break;
            default:
                // should never be reached
                break;
            }
            badDevice.clear();
//            badConnectedDevice = false;
        }
        if(badConnectedDevice == false)
        {
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setDetailedText("");
            msgBox.setWindowTitle("Подтверждение Активации");
            msgBox.setText(tr("Устройства подключены и готовы к активации!"));
            msgBox.setInformativeText(tr("Подтвердите активацию устройств."));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int ret = msgBox.exec();
            bool badActiveDevice = false;
            switch (ret)
            {
            case QMessageBox::Ok:
            {
                for(int a = 1; a <= DI; a++)
                {
                    if(devStruct[a].checkBox)
                    {
                        QByteArray answer;

                        QString s = QString::number(a);
                        QTcpSocket *sock = this->findChild<QTcpSocket*>(s);

                        if(sock->write(cmd1.data(),cmd1.size()))
                        {
                            sock->waitForBytesWritten(WBR);
                            answer = sock->readAll();
                            if(answer != "")
                            {
                                answer.clear();
                                if(sock->write(cmd2.data(),cmd2.size()))
                                {
                                    if(sock->state() == QAbstractSocket::ConnectedState)
                                    {
                                        sock->waitForReadyRead(WRR);
                                        sock->waitForBytesWritten(WBR);
                                        answer = sock->readAll();
                                        if(answer == "")
                                        {
                                            QString str = "Устройство: "+devStruct[a].ipAddres +"-"+ devStruct[a].nameDevice + "- не удалось активировать\n";
                                            badDevice += str;
                                            badActiveDevice = true;
                                        }
                                    }
                                }

                            }
                            else
                            {
                                devStruct[a].state = false;
                                sock->disconnectFromHost();
                                slotNoConnect(a);
                                QString str = "Устройство: "+devStruct[a].ipAddres +"-"+ devStruct[a].nameDevice;
                                badDevice += str;
                                badActiveDevice = true;
                            }
                        }
                    }
                }
                if(!badActiveDevice)
                {

                    msgBox.setDetailedText(badDevice);
                    msgBox.setText(tr("Активация устройств выполнена успешно!"));
                    msgBox.setInformativeText(tr(""));
                    msgBox.setIcon(QMessageBox::Information);
                    msgBox.setWindowTitle("Активация выполнена!");
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.exec();
                }
                else
                {
                    msgBox.setDetailedText(badDevice);
                    msgBox.setIcon(QMessageBox::Information);
                    msgBox.setWindowTitle("Активация выполнена!");
                    msgBox.setText(tr("Не удалось выполнить активацию некоторых устройств.\n"));
                    msgBox.setInformativeText(tr(""));
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.exec();
                }

                badActiveDevice = false;
                badConnectedDevice = false;
                badDevice.clear();
              qDebug()<< "ok";
            }
                break;
            case QMessageBox::Cancel:
            {
                badActiveDevice = false;
                badConnectedDevice = false;
                badDevice.clear();
            }
                break;
            default:
                // should never be reached
                break;
            }

        }
        qDebug()<< "end ";


    }
    else
    {

    }
}
void MainWindow::slotConnectTableStatus(int a)
{
    ui->tableWidget->item(a,6)->setText("Отключить");
    ui->tableWidget->item(a,4)->setText("Активно");
    ui->tableWidget->item(a,5)->setFlags(Qt::ItemIsEnabled);
    ui->tableWidget->item(a,5)->setBackgroundColor("#e27d5e");
    ui->tableWidget->item(a,5)->setTextColor(Qt::white);
}

void MainWindow::slotBadConnectTableStatus(int a)
{
    ui->statusBar->showMessage("Не удалось подключиться. Проверьте соединение с устройством!");
    ui->tableWidget->item(a,4)->setText("Нет связи");
}

void MainWindow::slotDisconnectTableStatus(int a)
{
    ui->tableWidget->item(a,4)->setText("Найдено");
    ui->tableWidget->item(a,5)->setTextColor(Qt::black);
    ui->tableWidget->item(a,6)->setText("Подключить");
    ui->tableWidget->item(a,5)->setBackgroundColor(Qt::white);
    ui->tableWidget->item(a,5)->setTextColor(Qt::black);
    ui->tableWidget->item(a,5)->setFlags(Qt::ItemIsDropEnabled);
}

void MainWindow::slotNoConnect(int a)
{
    ui->tableWidget->item(a,4)->setText("Нет связи");
    ui->tableWidget->item(a,4)->setTextColor(Qt::black);
    ui->tableWidget->item(a,6)->setText("Подключить");
    ui->tableWidget->item(a,5)->setBackgroundColor(Qt::white);
    ui->tableWidget->item(a,5)->setTextColor(Qt::black);
    ui->tableWidget->item(a,5)->setFlags(Qt::ItemIsDropEnabled);
}

void MainWindow::slotHHeader(int a) // Меняем состояние QcheckBox
{
    if((a == 0)&&!checkStateHeader)
    {
        for(int i = 1; i<=DI; i++)
        {
           QString s = QString::number(i);
           QCheckBox *cb = this->findChild<QCheckBox*>(s);
           cb->setCheckState(Qt::Checked);
           qDebug()<<cb->objectName();

        }
     checkStateHeader = true;
    }
    else if((a == 0)&&checkStateHeader)
    {
        for(int i = 1; i<=DI; i++)
        {
           QString s = QString::number(i);
           QCheckBox *cb = this->findChild<QCheckBox*>(s);
           cb->setCheckState(Qt::Unchecked);
           qDebug()<<cb->objectName();

        }
     checkStateHeader = false;
    }
}
void MainWindow::on_pushButton_clicked()
{
    saveDeviceInfoInFile();
}

void MainWindow::slotCheckBox()
{
    QObject *object = QObject::sender();
    QCheckBox *cbox = static_cast<QCheckBox *>(object);
    QString str;


    if(cbox->isChecked())
    {
        str = cbox->objectName();
        int i = str.toInt();
        if(CheckBoxList.isEmpty())
        {
            CheckBoxList.resize(1);
        }
        if(CheckBoxList.count() == 2)
        {
            ui->ActiveAll->setDisabled(false);
        }
        CheckBoxList.insert(CheckBoxList.last(),i);
        devStruct[i].checkBox = true;
        qDebug()<< str;
    }
    else
    {
        str = cbox->objectName();
        int s = str.toInt();
        int i = CheckBoxList.indexOf(s);
        if (i != -1){
            CheckBoxList.remove(i);
        }
        if(CheckBoxList.count() < 3){
            ui->ActiveAll->setDisabled(true);
        }
    }

    qDebug()<< "Чекбокс сработал " << str;
}
