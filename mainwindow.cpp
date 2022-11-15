#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showBaudRates();
    showPorts();
    QTimer timer;
    connect(&timer, SIGNAL(tiemout()), this, SLOT(updateDiplayData()));
    timer.start();
}


MainWindow::~MainWindow()
{
    std::cout << "closing app" << std::endl;
    //close thread
    if(communic->isConnected) {
        communic->closeCommunicSerialThread();
        delete communic;
    } else {
        std::cout << "trhead is not open, cant close the thread" << std::endl;
    }
    delete ui;
}

void MainWindow::updateDiplayData()
{
    if (communic->isConnected) {
        std::cout << "update display data" << std::endl;
        ui->lcdNumber_sulfDioxide->display(communic->getAirData().getSulfDioxide());
        ui->lcdNumber_carbonMonoxide->display(communic->getAirData().getCarbonMonoxide());
        ui->lcdNumber_lel->display(communic->getAirData().getLowerExplosiveLimit());
        ui->lcdNumber_temperature->display(communic->getAirData().getTemperature());
    }
}


int MainWindow::showDataChart(AirData *airData, int quantityData)
{
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
    return 0;
}

void MainWindow::showBaudRates(void)
{
    BaudRates[0].BaudRate = BAUD9600;
    BaudRates[0].Nombre = "9600";
    BaudRates[1].BaudRate = BAUD19200;
    BaudRates[1].Nombre = "19200";
    BaudRates[2].BaudRate = BAUD38400;
    BaudRates[2].Nombre = "38400";
    BaudRates[3].BaudRate = BAUD57600;
    BaudRates[3].Nombre = "57600";
    BaudRates[4].BaudRate = BAUD115200;
    BaudRates[4].Nombre = "115200";
    BaudRates[5].BaudRate = BAUD115200;
    BaudRates[5].Nombre = "115200";

    for(int i = 0; i < 6; i++)
        ui->comboBox_serialBaudRate->addItem(BaudRates[i].Nombre);
}

void MainWindow::showPorts(void)
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->comboBox_serialPorts->addItem(info.portName());
}

void MainWindow::on_pushButton_serialStart_clicked()
{
    communic = new Communic(ui->comboBox_serialBaudRate->currentText(), ui->comboBox_serialPorts->currentText());
    communic->createCommunicSerialThread();
}


void MainWindow::on_pushButton_serialClose_clicked()
{
    if(communic->isConnected) {
        communic->closeCommunicSerialThread();
        delete communic;
    } else {
        std::cout << "trhead is not open, cant close the thread" << std::endl;
    }
//    pthread_join(serialThread, NULL);
}

