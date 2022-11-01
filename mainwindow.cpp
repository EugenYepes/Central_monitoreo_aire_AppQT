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

    Resize_Image();

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
    AirDataDAO airDAO;
    AirData *airData = new AirData();
    AirData airData2(1.45, 1.65, 1.62, 20.32);
    airDAO.deleteAllData();
    // airDAO.selectDB(airData, 1);
    // airDAO.insertDB(airData2);
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    Resize_Image();
}

void MainWindow::Resize_Image(void)
{
    /*
    QPixmap pix("D:/UTN/EnCurso/Info_2_linux/TPO/AppQt/CentralMonitoreo/FondoPrueba.bmp");

    QSize  Var = ui->tabWidget->size();

    std::cout << "Width: " << Var.width() << " Height: " << Var.height() <<  std::endl;

    ui->label_Historical->setPixmap(pix.scaled(Var, Qt::IgnoreAspectRatio));
    */
}

void MainWindow::on_pushButton_clicked()
{
    Resize_Image();
}
