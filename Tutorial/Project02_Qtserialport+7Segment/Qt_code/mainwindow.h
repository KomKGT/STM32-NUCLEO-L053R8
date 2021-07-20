#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void updatestm(QString);
private:
    Ui::MainWindow *ui;
    QSerialPort *stm32;

    static const quint16 stm32_nucleo_vendor_id = 1155; //stm32
    static const quint16 stm32_nucleo_product_id = 14155;//stm32

    QString stm32_port_name;
    bool stm32_is_available;
};

#endif // MAINWINDOW_H
