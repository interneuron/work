#include "broadcast.h"
#include "QDebug"

Broadcast::Broadcast(QObject *parent) : QObject(parent)
{

}

Broadcast::~Broadcast()
{
    qDebug()<< "Broadcast - was Deleted";
}


