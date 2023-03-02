#include "stdafx.h"
#include "sendemail.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendEmail w;
    w.show();
    return a.exec();
}
