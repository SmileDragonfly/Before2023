#include "stdafx.h"
#include "qpop.h"

QPop::QPop(QObject *parent)
    : QSslSocket(parent)
{
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(hostFound()), this, SLOT(onHostFound()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onErrorReceive(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

QPop::~QPop()
{
}

BOOL QPop::testConnection(QString server, int port, ENCRYPTION_TYPE encryptionType)
{
    //connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    //connect(this, SIGNAL(hostFound()), this, SLOT(onHostFound()));
    //connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onErrorReceive(QAbstractSocket::SocketError)));
    //connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    //connectToHostEncrypted(server, port);
    connectToHost(server, port);
    return 0;
}

void QPop::onConnected()
{
    qDebug() << "onConnected :Connected";
}

void QPop::onErrorReceive(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error: " << socketError;
}

void QPop::onHostFound()
{
    qDebug() << "onHostFound: Founded";
}

void QPop::onStateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "State Change: " << socketState;
}
