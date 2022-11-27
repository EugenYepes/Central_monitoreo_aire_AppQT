#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QSerialPortInfo>
#include <QTimer>

//threads includes
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "airdatadao.h"
#include "communic.h"
#include "logs.h"


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

#define AMOUNT_MEASURMENTS 20

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

private slots:
    /**
     * @brief updateWrap
     * wrapper to update screens
     */
    void updateWrap(void);

    void on_pushButton_serialStart_clicked();

    void on_pushButton_serialClose_clicked();

    void on_pushButto_deleteAllDB_clicked();

private:
    Ui::MainWindow *ui;
    struct
    {
        BaudRateType    BaudRate;
        QString         Nombre;
    }BaudRates[6];

    /**
     * @brief updateDiplayData
     * update the data displayed on the display
     * call this function every time whe reciev new data, or update
     */
    void updateDiplayData(void);

    /**
     * @brief showDataChart
     * show the data contained obtained from data base
     */
    void showDataChart(void);

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
