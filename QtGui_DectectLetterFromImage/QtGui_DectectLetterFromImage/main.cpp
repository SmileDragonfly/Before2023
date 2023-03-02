#include "stdafx.h"
#include "qtgui_dectectletterfromimage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // QtGui_DectectLetterFromImage w; 
    // Change dialog ui: add minimize and maximize button
    Qt::WindowFlags f = Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint;
    QtGui_DectectLetterFromImage w(Q_NULLPTR, f);
    w.show();
    return a.exec();
}
