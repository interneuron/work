#ifndef BROADCAST_H
#define BROADCAST_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTextCodec>
//#include <QDebug>

class Broadcast : public QObject

{
    Q_OBJECT
public:
    explicit Broadcast(QObject *parent = 0);
    ~Broadcast();

private:
    struct devInfo
    {
        QString nameDevice;
        QString macAddres;
        QString passDevice;
    };
    devInfo tempDevStruct;
    void clearStruct()
    {

        tempDevStruct.macAddres.clear();
        tempDevStruct.nameDevice.clear();
        tempDevStruct.passDevice.clear();
    }
 QTextCodec* codec;
signals:

public slots:
    QString getDeviceInfo(QByteArray &arr) //Подумать над оптимизацией!!!
    {
        QTextCodec *codec = QTextCodec::codecForName("cp1251");
        QString string = codec->toUnicode(arr);

        int k = 0;
        int passBegin = 0;
        int passEnd = 0;

        QString m = "\n", mChar;

        tempDevStruct.macAddres.clear();
        tempDevStruct.nameDevice.clear();
        tempDevStruct.passDevice.clear();

        for (k = 0; m!= mChar; k++)

            {
                mChar = string[k];
                if(mChar == "("){
                    passBegin = k+1;
                }
                if(mChar == ")"){
                    passEnd = k;
                }
                tempDevStruct.nameDevice += string[k];
            }

        tempDevStruct.nameDevice = tempDevStruct.nameDevice.simplified();

        for (passBegin = passBegin; passBegin<passEnd; passBegin++){

           tempDevStruct.passDevice += string[passBegin];
        }

        mChar.clear();

        for (k=k; m!=mChar; k++){

            mChar = string[k];
            tempDevStruct.macAddres += string[k];

        }
        tempDevStruct.macAddres = tempDevStruct.macAddres.simplified();
        mChar.clear();

        return tempDevStruct.nameDevice;
    }

    QString getDeviceName(){

        return tempDevStruct.nameDevice;
    }

    QString getDeviceMac(){
        return tempDevStruct.macAddres;
    }

    QString getDevicePass(){

        return tempDevStruct.passDevice;
    }

};

#endif // BROADCAST_H
