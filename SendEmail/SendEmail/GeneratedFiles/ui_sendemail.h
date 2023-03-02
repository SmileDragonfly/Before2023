/********************************************************************************
** Form generated from reading UI file 'sendemail.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDEMAIL_H
#define UI_SENDEMAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SendEmailClass
{
public:
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QLineEdit *password;
    QLabel *emailText;
    QLabel *pass;
    QLineEdit *email;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QLabel *label_3;
    QLineEdit *inServer;
    QLabel *label_4;
    QLineEdit *inPort;
    QLabel *label_5;
    QComboBox *inEncryptionType;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QLabel *label_10;
    QLineEdit *outServer;
    QLabel *label_9;
    QLineEdit *outPort;
    QLabel *label_7;
    QComboBox *outEncryptionType;
    QWidget *widget1;
    QGridLayout *gridLayout_5;
    QPushButton *CheckConnection;
    QLabel *checkStatus;

    void setupUi(QDialog *SendEmailClass)
    {
        if (SendEmailClass->objectName().isEmpty())
            SendEmailClass->setObjectName(QString::fromUtf8("SendEmailClass"));
        SendEmailClass->resize(588, 334);
        widget = new QWidget(SendEmailClass);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(60, 10, 451, 248));
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        password = new QLineEdit(widget);
        password->setObjectName(QString::fromUtf8("password"));

        gridLayout->addWidget(password, 1, 1, 1, 1);

        emailText = new QLabel(widget);
        emailText->setObjectName(QString::fromUtf8("emailText"));

        gridLayout->addWidget(emailText, 0, 0, 1, 1);

        pass = new QLabel(widget);
        pass->setObjectName(QString::fromUtf8("pass"));

        gridLayout->addWidget(pass, 1, 0, 1, 1);

        email = new QLineEdit(widget);
        email->setObjectName(QString::fromUtf8("email"));

        gridLayout->addWidget(email, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        inServer = new QLineEdit(widget);
        inServer->setObjectName(QString::fromUtf8("inServer"));

        gridLayout_2->addWidget(inServer, 1, 1, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        inPort = new QLineEdit(widget);
        inPort->setObjectName(QString::fromUtf8("inPort"));

        gridLayout_2->addWidget(inPort, 2, 1, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        inEncryptionType = new QComboBox(widget);
        inEncryptionType->setObjectName(QString::fromUtf8("inEncryptionType"));

        gridLayout_2->addWidget(inEncryptionType, 3, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 1, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);

        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 1, 0, 1, 1);

        outServer = new QLineEdit(widget);
        outServer->setObjectName(QString::fromUtf8("outServer"));

        gridLayout_3->addWidget(outServer, 1, 1, 1, 1);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 2, 0, 1, 1);

        outPort = new QLineEdit(widget);
        outPort->setObjectName(QString::fromUtf8("outPort"));

        gridLayout_3->addWidget(outPort, 2, 1, 1, 1);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 3, 0, 1, 1);

        outEncryptionType = new QComboBox(widget);
        outEncryptionType->setObjectName(QString::fromUtf8("outEncryptionType"));

        gridLayout_3->addWidget(outEncryptionType, 3, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 2, 0, 1, 1);

        widget1 = new QWidget(SendEmailClass);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(60, 280, 146, 25));
        gridLayout_5 = new QGridLayout(widget1);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        CheckConnection = new QPushButton(widget1);
        CheckConnection->setObjectName(QString::fromUtf8("CheckConnection"));

        gridLayout_5->addWidget(CheckConnection, 0, 0, 1, 1);

        checkStatus = new QLabel(widget1);
        checkStatus->setObjectName(QString::fromUtf8("checkStatus"));

        gridLayout_5->addWidget(checkStatus, 0, 1, 1, 1);


        retranslateUi(SendEmailClass);

        QMetaObject::connectSlotsByName(SendEmailClass);
    } // setupUi

    void retranslateUi(QDialog *SendEmailClass)
    {
        SendEmailClass->setWindowTitle(QApplication::translate("SendEmailClass", "SendEmail", nullptr));
        emailText->setText(QApplication::translate("SendEmailClass", "Email", nullptr));
        pass->setText(QApplication::translate("SendEmailClass", "Pass", nullptr));
        label_6->setText(QApplication::translate("SendEmailClass", "InComing", nullptr));
        label_3->setText(QApplication::translate("SendEmailClass", "Server", nullptr));
        label_4->setText(QApplication::translate("SendEmailClass", "Port", nullptr));
        label_5->setText(QApplication::translate("SendEmailClass", "Encryption Type", nullptr));
        label_8->setText(QApplication::translate("SendEmailClass", "OutGoing", nullptr));
        label_10->setText(QApplication::translate("SendEmailClass", "Server", nullptr));
        label_9->setText(QApplication::translate("SendEmailClass", "Port", nullptr));
        label_7->setText(QApplication::translate("SendEmailClass", "Encryption Type", nullptr));
        CheckConnection->setText(QApplication::translate("SendEmailClass", "CheckConnection", nullptr));
        checkStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SendEmailClass: public Ui_SendEmailClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDEMAIL_H
