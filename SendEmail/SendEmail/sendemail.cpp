#include "stdafx.h"
#include "sendemail.h"

SendEmail::SendEmail(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QStringList encryptionTypeList;
    encryptionTypeList.push_back("None");
    encryptionTypeList.push_back("SSL");
    encryptionTypeList.push_back("TLS");
    ui.inEncryptionType->addItems(encryptionTypeList);
    ui.outEncryptionType->addItems(encryptionTypeList);
    setTestInfor();
}

void SendEmail::on_CheckConnection_clicked()
{
    // Get email and pass, infor
    if (ui.email->text() != "")
    {
        m_email = ui.email->text();
    }
    else
    {
        QMessageBox mess;
        mess.setDefaultButton(QMessageBox::Ok);
        mess.setText("Fill email");
        mess.exec();
    }
    if (ui.password->text() != "")
    {
        m_password = ui.password->text();
    }
    else
    {
        QMessageBox mess;
        mess.setDefaultButton(QMessageBox::Ok);
        mess.setText("Fill password");
        mess.exec();
    }
    GetNeededInfor(TRUE);
    GetNeededInfor(FALSE);

    // Check Connection
    //if (!QSslSocket::supportsSsl())
    //{
    //    QMessageBox::information(0, "Secure Socket Client",
    //        "System does not support OpenSSL.");
    //    return;
    //}
    m_pop.testConnection(m_outServer, m_outPort, m_inEncryptionType);
}

void SendEmail::GetNeededInfor(BOOL IO)
{
    QLineEdit* server;
    QLineEdit* port;
    QComboBox* encryptionType;
    if (IO == TRUE)
    {
        server = ui.inServer;
        port = ui.inPort;
        encryptionType = ui.inEncryptionType;
    }
    else
    {
        server = ui.outServer;
        port = ui.outPort;
        encryptionType = ui.outEncryptionType;
    }
    if (server->text() != "")
    {
        if (IO == TRUE)
        {
            m_inServer = server->text();
        }
        else
        {
            m_outServer = server->text();
        }
    }
    else
    {
        QMessageBox mess;
        mess.setDefaultButton(QMessageBox::Ok);
        mess.setText("Fill server");
        mess.exec();
        return;
    }
    if (port->text() != "")
    {
        if (IO == TRUE)
        {
            m_inPort = port->text().toInt();
        }
        else
        {
            m_outPort = port->text().toInt();
        }
    }
    else
    {
        QMessageBox mess;
        mess.setDefaultButton(QMessageBox::Ok);
        mess.setText("Fill port");
        mess.exec();
        return;
    }
    if (encryptionType->currentText() != "")
    {
        if (IO == TRUE)
        {
            m_inEncryptionType = (ENCRYPTION_TYPE)encryptionType->currentText().toInt();
        }
        else
        {
            m_outEncryptionType = (ENCRYPTION_TYPE)encryptionType->currentText().toInt();
        }
    }
    else
    {
        QMessageBox mess;
        mess.setDefaultButton(QMessageBox::Ok);
        mess.setText("Fill encryptionType");
        mess.exec();
        return;
    }
}

void SendEmail::setTestInfor()
{
    ui.email->setText("testnkt@ifisolution.vn");
    ui.password->setText("ifi@WSxc");
    ui.inServer->setText("imap.gmail.com");
    ui.inPort->setText("993");
    ui.inEncryptionType->setCurrentIndex(0);
    ui.outServer->setText("smtp.gmail.com");
    ui.outPort->setText("587");
    ui.outEncryptionType->setCurrentIndex(0);
}
