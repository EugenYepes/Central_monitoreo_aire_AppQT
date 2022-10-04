#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPixmap>
#include <iostream>

#define LO 0.0
#define HI 100.0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("E:/UTN/EnCurso/Info_2_linux/TPO/AppQt/CentralMonitoreo/FondoPrueba.bmp");
    ui->label_Picture->setPixmap(pix.scaled(1000, 1000, Qt::KeepAspectRatio));
    /*se puede hacer sin poner una ruta absoluta?
     como hacer para que la imagen ocupe toda la pantalla*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_update_clicked()
{
    /*-set an event when reciev a data array from serial port
      -parse data and fill an object with data
      -diplayed the data
      -read current data object*/
    float r2 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    ui->lcdNumber_Oxigeno->display(r2 + 1);
    ui->lcdNumber_Monoxido->display(r2 + 2);
    ui->lcdNumber_Explosividad->display(r2 + 3);
    ui->lcdNumber_Temperatura->display(r2 + 4);
}


void MainWindow::on_pushButton_connectDB_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("");

    if(db.open()){
        std::cout<<"conexion exitosa: "<<db.hostName().toLocal8Bit().data()<<std::endl;
    }
}

