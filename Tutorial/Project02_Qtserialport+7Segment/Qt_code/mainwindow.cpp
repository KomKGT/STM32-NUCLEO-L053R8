#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stm32_is_available = false;
    stm32_port_name = "";
    stm32 = new QSerialPort;
    /*Check*/
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
            qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
            if(serialPortInfo.hasVendorIdentifier())
            {
                qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
            }
            qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
            if(serialPortInfo.hasProductIdentifier())
            {
                qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
            }
    }
    /*Check*/
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
            if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
            {
                if(serialPortInfo.vendorIdentifier() == stm32_nucleo_vendor_id)
                {
                    if(serialPortInfo.productIdentifier() == stm32_nucleo_product_id)
                    {
                        stm32_port_name = serialPortInfo.portName();
                        stm32_is_available = true;
                    }
                }
             }
     }
    if(stm32_is_available)
    {
        stm32->setPortName(stm32_port_name);
        stm32->open(QSerialPort::WriteOnly);
        stm32->setBaudRate(QSerialPort::Baud9600);
        stm32->setDataBits(QSerialPort::Data8);
        stm32->setParity(QSerialPort::NoParity);
        stm32->setStopBits(QSerialPort::OneStop);
        stm32->setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
        QMessageBox::warning(this,"Port error","Couldn't find stm32");
    }

}

MainWindow::~MainWindow()
{
    if(stm32->isOpen())
    {
        stm32->close();
    }
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    MainWindow::updatestm(QString("%1").arg(value)+'\r');
    qDebug()<< value;
}

void MainWindow::updatestm(QString command)
{
    if(stm32->isWritable())
    {
        stm32->write(command.toStdString().c_str());
        qDebug()<<command;
    }
    else
    {
        qDebug()<<"Couldn't write to serial";
    }
}
