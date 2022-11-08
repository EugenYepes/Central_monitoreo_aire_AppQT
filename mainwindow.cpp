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

    showBaudRates();
    showPorts();
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
    Communic communic;
    communic.readMessageSerial(ui->comboBox_serialBaudRate->currentText(), ui->comboBox_serialPorts->currentText());
}

