#ifndef PASSBOX_H
#define PASSBOX_H

#include <QWidget>
#include <string>
#include <QString>

#include "mainwindow.h"

namespace Ui {
class PassBox;
}

class PassBox : public QWidget
{
    Q_OBJECT

public:
//    void UserData();
    explicit PassBox(QWidget *parent = 0);
    ~PassBox();
    QString encodeStr(const QString &str);
    QString decodeStr(const QString &str);
// переменные


private:
// методы
    Ui::PassBox *ui;
    MainWindow *mw = new MainWindow(this);

// переменные
    QString pwd; //шифрованный 12345 - CgkIDw4=
    quint32 key = 073; //шифр
    QString pwdDef = "12345";


public slots:
    void setNewPassword(QString &newpass);
    void getPassword()
    {

        pwd = mw->ini->value("Main/password").toString();
        if(pwd == ""){setNewPassword(pwdDef);}
        pwd = decodeStr(pwd);
//        mw->deleteLater();
    }
    QString getPassword_all()
    {
        pwd = mw->ini->value("Main/password").toString();
        pwd = decodeStr(pwd);
        return pwd;
//        mw->deleteLater();
    }
private slots:
    void on_lineEdit_textChanged();
    void on_lineEdit_returnPressed();
    void on_cancel_clicked();

signals:


protected:


};

#endif // PASSBOX_H
