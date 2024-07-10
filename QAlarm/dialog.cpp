#include "dialog.h"
#include <iostream>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QString version = "闹钟";
    qtcss = new qss();
    styleSheet = qtcss->loadStyleSheet(":/css/clock.qss");
    //当前窗口设置样式表
    this->setStyleSheet(styleSheet);
    this->setFixedSize(1024,580);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowIcon(QPixmap(":images/clock.ico"));
    setWindowTitle("定时器");
    lablel1 = new QLabel();
    lablel2 = new QLabel();
    lablel3 = new QLabel();
    version_lable = new QLabel();
    qtimeedit = new QTimeEdit();
    lineedit =new QTextEdit();
    button = new QPushButton();
    hidebutton = new QPushButton();
    forever_button =  new QPushButton();
    textbrowser = new QTextEdit();

    button->setText(tr("定时"));
    hidebutton->setText(tr("停止音乐"));
    lablel1->setText(tr("设置时间"));
    lablel2->setText(tr("内容"));
    lablel3->setText(tr("定时列表"));
    version_lable->setText(version);
    version_lable->setObjectName("version");
    forever_button->setObjectName("fbutton");
    forever_button->setText(tr("持久"));
    button->setObjectName("sbutton");
    lineedit->setText(tr("输入内容"));
    textbrowser->setReadOnly(true);
    qtimeedit->setDisplayFormat("hh:mm:ss");
    mplayer = new QMediaPlayer();
    path =  QDir::currentPath();
    path += "/mp3/clock.mp3";
    mplayer->setMedia(QUrl::fromLocalFile(path));

    exit_button = new QPushButton(this);
    exit_button->setGeometry(980,1,40,40);
    exit_button->setStyleSheet("border-image:url(:/images/exit.png)");
    connect(exit_button,&QPushButton::clicked,[=](){
            exit(1);//退出程序
        });

    //布局
    mainlayout = new QGridLayout(this);
    hblayout = new QHBoxLayout();
    mainlayout->addWidget(version_lable,0,0);
    mainlayout->addWidget(lablel1,1,1);
    mainlayout->addWidget(qtimeedit,1,0);
    mainlayout->addWidget(lineedit,3,0);
    mainlayout->addWidget(textbrowser,3,1);
    mainlayout->addWidget(lablel2,2,0);
    mainlayout->addWidget(lablel3,2,1);
    mainlayout->addWidget(hidebutton,7,0);
    hblayout->addWidget(button);
    hblayout->addWidget(forever_button);
    mainlayout->addLayout(hblayout,7,1);

    connect(button,SIGNAL(clicked()),this,SLOT(setclock()));
    connect(forever_button,SIGNAL(clicked()),this,SLOT(set_forever_clock()));
    connect(hidebutton,SIGNAL(clicked()),this,SLOT(HideClock()));

    //设置ini文件
    settings =  new QSettings("clock.ini", QSettings::IniFormat);
    settings->setIniCodec("utf-8");
    //遍历INI文件.
    Traverse_ini("clock");
    Traverse_ini("forver");

    time_clock = new QTimer();
    connect(time_clock,SIGNAL(timeout()),this,SLOT(showclock()));
    time_clock->start(1000);


}
/*
*
*   重写close事件。
*
*/
void Dialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->showMinimized();
}
/*
*
*   根据组名遍历ini文件
*
*/
void Dialog::Traverse_ini(QString  gname)
{
    settings->beginGroup(gname);
    QStringList keys = settings->childKeys();
    settings->endGroup();
    QStringList::const_iterator ci;
    QTime key;
    QString value;
    for (ci = keys.constBegin(); ci != keys.constEnd();++ci)
    {
        key = QTime::fromString(*ci,"hh:mm:ss");
        value = this->readini(key.toString(),gname);
        writehash(key,value);
    }
}

/*
*
*  ini文件设置forver组
*
*/
void Dialog::set_forever_clock()
{
    settime = qtimeedit->time();
    writini(settime.toString(),lineedit->toPlainText(),"forver");
    writehash(settime,lineedit->toPlainText());
}

/*
*
*  slots 隐藏窗口并停止音乐
*
*/
void Dialog::HideClock()
{
    mplayer->stop();
    if(clockhash.isEmpty()){
       QMessageBox::information(this,tr("设置"),tr("没有设置定时\n 请先设置定时！"));
    }
}

/*
*
*  把时间和内容写入HASH表和定时列表
*
*/
void Dialog::writehash(QTime times,QString strs)
{
    clockhash.insert(times,strs);
    textbrowser->append("<font color='red' size='5px'>"\
                        +times.toString()+"</font> <font color='blue' size='5px'>"\
                        +strs+"</font>");
}
/*
*
*  根据组名GNAME 把KEY,VALUE 写入INI文件
*
*/
void Dialog::writini(QString KEY,QString VALUE,QString GNAME)
{
    settings->beginGroup(GNAME);
    settings->setValue(KEY,VALUE);
    settings->endGroup();
    settings->sync();
}

/*
*
*  根据组名GNAME 读取INI文件中的KEY
*
*/
QString Dialog::readini(QString KEY,QString GNAME)
{
    QString str;
    settings->beginGroup(GNAME);
    str = settings->value(KEY).toString();
    settings->endGroup();
    return str;
}

/*
*
*  根据组名GNAME 移除INI文件中的KEY
*
*/
void Dialog::removeini(QString KEY,QString GNAME)
{
    settings->beginGroup(GNAME);
    settings->remove(KEY);
    settings->remove(KEY);
    settings->endGroup();
    settings->sync();
}

/*
*
*  把时间和定时内容写入
*
*/
void Dialog::setclock()
{
    settime = qtimeedit->time();
    writini(settime.toString(),lineedit->toPlainText(),"clock");
    writehash(settime,lineedit->toPlainText());
}

/*
*
*  定时任触发后，移除HASH、INI、和任务列表中的任务。
*
*/
void Dialog::showclock()
{
     QTime tkey;
     QString tvalue;
    for(i=clockhash.begin();i!=clockhash.end();++i)
    {
        if(qtime->currentTime()>i.key())
        {
            tkey = i.key();
            tvalue = i.value();
            lineedit->setText(tvalue);
            qtimeedit->setTime(tkey);
            this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            mplayer->play();
            removeini(tkey.toString(),"clock");
            clockhash.remove(tkey);
            textbrowser->setText(tr(""));
            QString tstr;
            for(j=clockhash.begin();j!=clockhash.end();++j)
            {
                    textbrowser->append("<font color='red' size='5px'>"\
                                        +j.key().toString()+"</font> <font color='blue' size='5px'>"\
                                        +j.value()+"</font>");
            }
            break;
        }
    }


}
Dialog::~Dialog()
{
    settings->endGroup();

}
