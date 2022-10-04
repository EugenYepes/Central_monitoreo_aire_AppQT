#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    if (0 != Test::testCommunic()) {
        std::cout << "ERROR TEST COMMUNICATIONS NOT PASSED" << std::endl;
    }
    return a.exec();
}
