#include "serialtool.h"

SerialTool::SerialTool()
{
    global_port = new QSerialPort();
}

QStringList SerialTool::getPortNameList() {  // 这个是用来获取可用串口的函数
    QStringList m_serialPortName;  // 先定义一个QStringList用来存储串口名字
    // 简化版的for循环,foreach(x1,x2):其中每次循环x1都是从x2中取出来的,相当于一个遍历的效果
    // availablePorts返回所有可用的串口,但他是一个QSerialPortInfo对象,我们要做的是在foreach循环里面把他转换成QStringList列表
    // 为什么要转换?答:因为availablePorts对象里面的成员变量太多了,所以把他转换成相对简洁的列表
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        m_serialPortName << info.portName();  // 把每次遍历取到的值写入QStringList中
        qDebug() << "serialportname:" << info.portName();
    }
    return m_serialPortName;
}

void SerialTool::open_port() {
    QStringList portNameList = getPortNameList();  // 获取串口列表
    qDebug() << "Debug::serialTool"<< portNameList;
    global_port->setPortName(portNameList[0]);  // 打开第一个,一般来说只有这一个,连接多个串口的时候可能存在选择错误设备的缺陷,最好设置成固定的地址(这个自己查一下)
    global_port->setBaudRate(QSerialPort::Baud115200);  // 固定波特率
    if(global_port->isOpen())// 打开前如果串口还打开着 先给他关闭了
    {
        global_port->clear();
        global_port->close();
    }
    if(!global_port->open(QIODevice::ReadWrite)) //用ReadWrite 的模式尝试打开串口
    {
        qDebug()<<"打开失败!";
        return;
    }
    qDebug()<<"串口打开成功!";
}

void SerialTool::close_port() {
    global_port->clear();
    global_port->close();
}

QByteArray SerialTool::receive_data() {
    QByteArray array=global_port->readAll();
 //   qDebug()<<array;
    static QByteArray PasteData;
    static QByteArray prePasteData;
    static QByteArray sumData;  // sumData用来做缓存的功能
    static bool arrayContainsGNGGA = false;


    // 第一种:数据帧包含GNGGA,说明是GPS数据帧,先保存到缓存
    if (array.contains("$GNGGA")) {
        sumData.append(array.mid(array.indexOf("$GNGGA")));
        arrayContainsGNGGA = true;
    }
    // 第二种:第一种来的数据不完整,一般是经度数据存在在第一种来的帧的下一帧,
    // 此时把第二帧数据存到缓存sumData中,拼接起来,再做截取处理
    if (array.contains("E")&arrayContainsGNGGA) {
        sumData.append(array);
        arrayContainsGNGGA = false;
        PasteData = sumData;
        sumData.clear();
    }

    // 经过第一种和第二种出来的数据还有一点杂乱,这里进一步过滤
    // 主要是过滤出存在下述三种的帧数据,但是又不存在/r/n这样的换行符数据,
    // 符合要求的就做返回值
    if (PasteData.contains("$GNGGA")&PasteData.contains("N")&PasteData.contains("E")&(!PasteData.contains("\r\n"))) {
        int indexOfGNGGA = PasteData.indexOf("$GNGGA");
        int indexOfN = PasteData.indexOf("N");
        int indexOfE = PasteData.indexOf("E");
        int len = indexOfE - indexOfGNGGA + 1;
        PasteData = PasteData.mid(indexOfGNGGA, len);
        prePasteData = PasteData;
//        qDebug() << PasteData.mid(indexOfGNGGA, len);
        return PasteData;
    } else {
        return prePasteData;
    }
}
