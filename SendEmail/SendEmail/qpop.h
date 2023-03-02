#pragma once

#include <QtNetwork/qsslsocket.h>

class QPop : public QSslSocket
{
    Q_OBJECT

public:
    QPop(QObject *parent = NULL);
    ~QPop();
    BOOL testConnection(QString server, int port, ENCRYPTION_TYPE encryptionType);
private slots:
    void onConnected();                                     // signal connected()
    void onErrorReceive(QAbstractSocket::SocketError);      // signal error()
    void onHostFound();                                     // signal hostfound()
    void onStateChanged(QAbstractSocket::SocketState);
};
