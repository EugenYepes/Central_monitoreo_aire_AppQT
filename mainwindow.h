#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QSerialPortInfo>

//threads includes
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "airdatadao.h"
#include "communic.h"


enum BaudRateType {
    BAUD110 = 110,
    BAUD300 = 300,
    BAUD600 = 600,
    BAUD1200 = 1200,
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD57600 = 57600,
    BAUD115200 = 115200
};

struct QextPortInfo {
    QString portName;   ///< Port name.
    QString physName;   ///< Physical name.
    QString friendName; ///< Friendly name.
    QString enumName;   ///< Enumerator name.
    int vendorID;       ///< Vendor ID.
    int productID;      ///< Product ID
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Communic *communic;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief showDataChart
     * show the data contained in airData
     * @param airData       data to be show
     * @param quantityData  number of register of data
     * @return
     */
    int showDataChart(AirData *airData, int quantityData);
private slots:
    /**
     * @brief updateDiplayData
     * update the data displayed on the display
     * call this function every time whe reciev new data, or update
     */
    void updateDiplayData();

    void on_pushButton_serialStart_clicked();

    void on_pushButton_serialClose_clicked();

private:
    Ui::MainWindow *ui;
    struct
    {
        BaudRateType    BaudRate;
        QString         Nombre;
    }BaudRates[6];

    /**
     * @brief showBaudRates
     * show oll posibles baud rates in a combo box
     */
    void showBaudRates(void);

    /**
     * @brief showPorts
     * show all available ports
     */
    void showPorts(void);

};

#endif // MAINWINDOW_H
