#ifndef SERIALTOOL_H
#define SERIALTOOL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialTool
{
public:
    QSerialPort *global_port;

    SerialTool();
    QStringList getPortNameList();
    void open_port();
    void close_port();
    QByteArray receive_data();
};

#endif // SERIALTOOL_H
