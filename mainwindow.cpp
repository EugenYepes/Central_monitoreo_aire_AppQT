#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#define LO 0.0
#define HI 100.0


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create chart
    QChart *chart = new QChart();
    chart->createDefaultAxes();
    chart->setTitle("Air data historical");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // show chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame_chart);

    // line 1
    QLineSeries *series1 = new QLineSeries();
    for (int i = 0; i < 20; i++) {
        series1->append(i, (i +1)*2);
        *series1 << QPointF(i, (i +1)*2);
    }
    series1->setName("Sulfur Dioxide");
    chart->addSeries(series1);

    //line 2
    QLineSeries *series2 = new QLineSeries();
    for (int i = 0; i < 20; i++) {
        series2->append(i, (i +1)*(-2));
        *series2 << QPointF(i, (i +1)*(-2));

    }
    series2->setName("Carbon Monoxide");
    chart->addSeries(series2);
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
    // airDAO.deleteAllData();
    airDAO.selectDB(airData, 21);
    airDAO.insertDB(airData2);
}
