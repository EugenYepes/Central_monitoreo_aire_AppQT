#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include "test.h"
#include "logs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    HAL_LOG_INFO("log ", "con ", "argumentos", "variables");
//    if (0 != Test::testCommunic()) {
//        std::cout << "ERROR TEST COMMUNICATIONS NOT PASSED" << std::endl;
//    }
    return a.exec();;
}
