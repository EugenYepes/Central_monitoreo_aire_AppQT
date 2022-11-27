#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

QTimer timer;
AirDataDAO *g_airDataDAO;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showBaudRates();
    showPorts();
    g_airDataDAO = new AirDataDAO();

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateWrap()));
    timer.start(1000);
}


MainWindow::~MainWindow()
{
    LOG_MSG("closing app");
    //close thread
    if(communic->isConnected) {
        communic->closeCommunicSerialThread();
        delete communic;
    } else {
        LOG_MSG("trhead is not open, cant close the thread");
    }
    delete ui;
}

void MainWindow::updateWrap()
{
    updateDiplayData();
    showDataChart();
}

void MainWindow::updateDiplayData()
{
    ///<@todo uncoment this line
//    if (communic->isConnected) {
        LOG_MSG("update display data");

        AirData airData;
        g_airDataDAO->selectDB(&airData, g_airDataDAO->getLastID());

        ui->lcdNumber_sulfDioxide->display(airData.getSulfDioxide());
        ui->lcdNumber_carbonMonoxide->display(airData.getCarbonMonoxide());
        ui->lcdNumber_lel->display(airData.getLowerExplosiveLimit());
        ui->lcdNumber_temperature->display(airData.getTemperature());
//    }
}


void MainWindow::showDataChart(void)
{
    LOG_MSG("update charts data");
    //get data from data base
    AirData airData[AMOUNT_MEASURMENTS];    
    int dbId = g_airDataDAO->getLastID();
    int quantityOfReadData = AMOUNT_MEASURMENTS;

    if (g_airDataDAO->selectBetweenIntervalDB(airData, &quantityOfReadData, dbId, dbId - AMOUNT_MEASURMENTS + 1) != 0) {
        return;
    }

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

    // Sulfur Dioxide
    QLineSeries *seriesSulfurDioxide = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesSulfurDioxide->append(i, airData[i].getSulfDioxide());
        *seriesSulfurDioxide << QPointF(i, airData[i].getSulfDioxide());
    }
    seriesSulfurDioxide->setName("Sulfur Dioxide");
    chart->addSeries(seriesSulfurDioxide);

    // Carbon Monoxide
    QLineSeries *seriesCarbonMonoxide = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesCarbonMonoxide->append(i, airData[i].getCarbonMonoxide());
        *seriesCarbonMonoxide << QPointF(i, airData[i].getCarbonMonoxide());
    }
    seriesCarbonMonoxide->setName("Carbon Monoxide");
    chart->addSeries(seriesCarbonMonoxide);

    // Lower Explosive Limit
    QLineSeries *seriesLEL = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesLEL->append(i, airData[i].getLowerExplosiveLimit());
        *seriesLEL << QPointF(i, airData[i].getLowerExplosiveLimit());
    }
    seriesLEL->setName("Lower Explosive Limit");
    chart->addSeries(seriesLEL);

    // Temperature
    QLineSeries *seriesTemperature = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesTemperature->append(i, airData[i].getTemperature());
        *seriesTemperature << QPointF(i, airData[i].getTemperature());
    }
    seriesTemperature->setName("Temperature");
    chart->addSeries(seriesTemperature);
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
        LOG_MSG("trhead is not open, cant close the thread");
    }
//    pthread_join(serialThread, NULL);
}


void MainWindow::on_pushButto_deleteAllDB_clicked()
{
    ///<@todo read a confirmation
    ///<@todo add an administrator user
    g_airDataDAO->deleteAllData();
}

