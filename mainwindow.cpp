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

    setLabelText();

    for (int i = 0; i<30; i++) {
        AirData airData(0.1 * i, 0.2 * i, 0.3 * i, 0.4 * i);
        g_airDataDAO->insertDB(airData);
    }
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
    updateDB();
}

void MainWindow::updateDiplayData()
{
    ///<@todo uncoment this line
    int lastID;
    if (g_airDataDAO->getLastID(&lastID) != SUCCESS) {
        LOG_MSG("Last id not found");
        return;
    }
    LOG_MSG("update display data");

    AirData airData;
    g_airDataDAO->selectDB(&airData, lastID);

    ui->lcdNumber_sulfDioxide->display(airData.getSulfDioxide());
    ui->lcdNumber_carbonMonoxide->display(airData.getCarbonMonoxide());
    ui->lcdNumber_lel->display(airData.getLowerExplosiveLimit());
    ui->lcdNumber_temperature->display(airData.getTemperature());
}


void MainWindow::showDataChart(void)
{
    LOG_MSG("update charts data");
    //get data from data base
    AirData airData[AMOUNT_MEASURMENTS];    
    int dbId;
    int quantityOfReadData = AMOUNT_MEASURMENTS;

    if (g_airDataDAO->getLastID(&dbId) != SUCCESS) {
        LOG_MSG("Last id not found");
        return;
    }

    if (g_airDataDAO->selectBetweenIntervalDB(airData, &quantityOfReadData, dbId, dbId - AMOUNT_MEASURMENTS + 1) != 0) {
        LOG_MSG("ERROR data dont found");
        return;
    }

    // create series
    // Sulfur Dioxide
    QLineSeries *seriesSulfurDioxide = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesSulfurDioxide->append(i, airData[i].getSulfDioxide());
        *seriesSulfurDioxide << QPointF(i, airData[i].getSulfDioxide());
    }
    seriesSulfurDioxide->setName("Sulfur Dioxide");
    seriesSulfurDioxide->setColor(QColor::fromRgb(0x5DFF00));

    // Carbon Monoxide
    QLineSeries *seriesCarbonMonoxide = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesCarbonMonoxide->append(i, airData[i].getCarbonMonoxide());
        *seriesCarbonMonoxide << QPointF(i, airData[i].getCarbonMonoxide());
    }
    seriesCarbonMonoxide->setName("Carbon Monoxide");
    seriesCarbonMonoxide->setColor(QColor::fromRgb(0xFFC000));


    // Lower Explosive Limit
    QLineSeries *seriesLEL = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesLEL->append(i, airData[i].getLowerExplosiveLimit());
        *seriesLEL << QPointF(i, airData[i].getLowerExplosiveLimit());
    }
    seriesLEL->setName("Lower Explosive Limit");
    seriesLEL->setColor(QColor::fromRgb(0x0000FF));


    // Temperature
    QLineSeries *seriesTemperature = new QLineSeries();
    for (int i = 0; i < quantityOfReadData; i++) {
        seriesTemperature->append(i, airData[i].getTemperature());
        *seriesTemperature << QPointF(i, airData[i].getTemperature());
    }
    seriesTemperature->setName("Temperature");
    seriesTemperature->setColor(QColor::fromRgb(0xFF0000));


    // create chart
    QChart *chart = new QChart();
    chart->setTitle("<P><b><i><font color='#000000' font_size=5> Air Data Historical </font></i></b></P></br>");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // add series
    chart->addSeries(seriesSulfurDioxide);
    chart->addSeries(seriesCarbonMonoxide);
    chart->addSeries(seriesLEL);
    chart->addSeries(seriesTemperature);

    chart->createDefaultAxes();

    // show chart
    QChartView *chartViewClean = new QChartView();
    chartViewClean->setRenderHint(QPainter::Antialiasing);
    chartViewClean->setParent(ui->horizontalFrame_chart);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame_chart);
}

void MainWindow::updateDB(void)
{
    if(false == Communic::getReadValue()) {
        if (g_airDataDAO->insertDB(communic->getAirData()) == 0)
            Communic::setReadValue(true);
    }
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

void MainWindow::setLabelText(void)
{
    QString openTags = "<P><b><i><font color='#5DFF00' font_size=4>";
    QString closeTags = "</font></i></b></P></br>";
    QString labelText = "Dioxido de azufre (SO2)";
    ui->label_SulfDioxide->setText(openTags + labelText + closeTags);

    openTags = "<P><b><i><font color='#FFC000' font_size=4>";
    labelText = "Monox de carbono (CO)";
    ui->label_CarbonMonoxide->setText(openTags + labelText + closeTags);

    openTags = "<P><b><i><font color='#0000FF' font_size=4>";
    labelText = "Indice de explosividad (%LEL)";
    ui->label_lel->setText(openTags + labelText + closeTags);

    openTags = "<P><b><i><font color='#FF0000' font_size=4>";
    labelText = "Temperatura";
    ui->label_temperature->setText(openTags + labelText + closeTags);
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
}


void MainWindow::on_pushButto_deleteAllDB_clicked()
{
    ///<@todo read a confirmation
    ///<@todo add an administrator user
    g_airDataDAO->deleteAllData();
}

