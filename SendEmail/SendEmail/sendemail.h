#pragma once

#include <QtWidgets/QDialog>
#include "ui_sendemail.h"
#include "qpop.h"

class SendEmail : public QDialog
{
    Q_OBJECT

public:
    SendEmail(QWidget *parent = Q_NULLPTR);
    QString             m_email;
    QString             m_password;
    QString             m_inServer;
    int                 m_inPort;
    ENCRYPTION_TYPE     m_inEncryptionType;
    QString             m_outServer;
    int                 m_outPort;
    ENCRYPTION_TYPE     m_outEncryptionType;
    QPop                m_pop;
    void                GetNeededInfor(BOOL IO);
    void                setTestInfor();

private:
    Ui::SendEmailClass ui;
private slots:
    void on_CheckConnection_clicked();
};
