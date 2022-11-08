#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts>
#include <QChartView>
#include <QLineSeries>

#include "airdatadao.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Resize_Image(void);


private slots:

    void on_pushButton_update_clicked();

    void on_pushButton_connectDB_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
