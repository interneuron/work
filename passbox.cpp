#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "passbox.h"
#include "mainwindow.h"
#include "ui_passbox.h"

PassBox::PassBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassBox)
{
    ui->setupUi(this);
    ui->ok->setDefault(true);
    ui->cancel->setDefault(true);
    ui->ok->setEnabled(false);
    ui->wrongPass->clear();
    getPassword();

    connect(ui->ok,SIGNAL(clicked()),SLOT(on_lineEdit_returnPressed()));

}

PassBox::~PassBox()
{
    delete ui;
    qDebug()<< "PassBox - was Deleted";
}


void PassBox::on_lineEdit_textChanged()
{
        ui->ok->setEnabled(true);
}

void PassBox::on_lineEdit_returnPressed()
{

   if (ui->lineEdit->text()== pwd){

       MainWindow *mw = new MainWindow;
       mw->openMain();
       close();

    }
     else {
            ui->wrongPass->setStyleSheet("color:red");
            ui->wrongPass->setText("Пароль введен неверно");
            ui->lineEdit->clear();

          }
 }

void PassBox::on_cancel_clicked()
{
    PassBox::close();
}


//--------------------------------------------------------------
QString PassBox::encodeStr(const QString& str)
{
    QByteArray arr(str.toUtf8());
    for(int i =0; i<arr.size(); i++)
        arr[i] = arr[i] ^ key;

    return QString::fromLatin1(arr.toBase64());
}

QString PassBox::decodeStr(const QString &str)
{
    QByteArray arr = QByteArray::fromBase64(str.toLatin1());

    for(int i =0; i<arr.size(); i++)
        arr[i] =arr[i] ^ key;

    return QString::fromUtf8(arr);
}

void PassBox::setNewPassword(QString &newpass)
{
    pwd = encodeStr(newpass);
    mw->ini->setValue("Main/password",pwd);
}


