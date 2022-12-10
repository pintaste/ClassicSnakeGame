#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 600);
    w.setWindowTitle(QString("Classic Snake Game"));
    w.show();
    return a.exec();
}
