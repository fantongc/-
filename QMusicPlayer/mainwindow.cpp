/*
 * 作者：化作尘
 * csdn：化作尘
 * 哔哩哔哩：化作尘my
 * 邮箱：2809786963@qq.com
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSlider>
#include "config.h"
#include "mypushbutton.h"
#include <QTimer>
#include <QListWidget>
#include <QProcess>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QTextCodec>
//播放列表
QFileInfoList song_info_list;
//当前播放
int curr_num;
//定时器
QTimer *timer,*timer1,*timer2,*vol_timer,*lab_timer;
//音量显示开关
bool vol_button_flag;

bool pause_flag;//播放暂停状态 1播放 0暂停
int now_palytime,last_playtime,time_length,now_progress,last_progress;//当前播放时间 进度

//进程
QProcess * mypro;
//进度条
QSlider* video_Slider,*volumeslider;
//按钮
myPushButton * prev_button,* pause_button,* next_button,*exit_button,* vol_button,*file_button;
//播放列表
QListWidget *songList;
//歌词显示标签
QVector<QLabel *> labelArr;
QLabel *LabelMainWindowBackground;
QLabel *labelDuration;
QLabel *labelSongsTitle;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1024,580);//设置界面固定大小
    setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("MainWindow{border-image: url(:/pic/music_menu.jpg);}");

    timerInit();//定时器初始化
    PushButton_init();//按钮初始化
    QSlider_init();//进度条初始化
    QListWidget_init();//播放列表初始化
    labelInit();//歌词标签初始化

    scanSongs();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mypro;
}

//定时器初始化
void MainWindow::timerInit()
{
    //开启定时
    timer = new QTimer(this);
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    timer->start(400);
    connect(timer,&QTimer::timeout,[=](){
    if(mypro->state() == QProcess::Running)
    {
        timer->stop();
        timer1->start(100);
        mypro->write("get_percent_pos\n");
    }

    });
    connect(timer1,&QTimer::timeout,[=](){
    if(mypro->state() == QProcess::Running)
    {
        timer1->stop();
        timer2->start(100);
        mypro->write("get_time_pos\n");
    }

    });
    connect(timer2,&QTimer::timeout,[=](){
    if(mypro->state() == QProcess::Running)
    {
        timer2->stop();
        timer->start(100);
        mypro->write("get_time_length\n");
    }

    });


    QLabel *lab = new QLabel(this);
    lab->setGeometry(170,440,50,20);
    QFont    f("宋体",10,50);
    lab->setFont(f);
    lab->setStyleSheet("color:white;");

    mypro = new QProcess(this);//进程分配内存
    connect(mypro,&QProcess::readyReadStandardOutput,[=](){
    QString msg = mypro->readAll();

    if(msg.indexOf("ANS_TIME_POSITION",0)!=-1)//获取播放时间
    {
        last_playtime = now_palytime;//保存上一次的时间
        msg = msg.section('=', 1, 1).trimmed();
        now_palytime = msg.toDouble();
        QString text = QString("%1%2:%3%4/%5%6:%7%8")
                .arg(now_palytime/60/10)
                .arg(now_palytime/60%10)
                .arg(now_palytime%60/10)
                .arg(now_palytime%60%10)
                .arg(time_length/60/10)
                .arg(time_length/60%10)
                .arg(time_length%60/10)
                .arg(time_length%60%10);
        if(now_palytime!=0)
        labelDuration->setText(text);
    }
    else if(msg.indexOf("ANS_LENGTH",0)!=-1)//获取时间
    {
        msg = msg.section('=', 1, 1).trimmed();
        time_length = msg.toDouble();
    }

    else if(msg.indexOf("ANS_PERCENT_POSITION=",0)!=-1)//获取进度
    {
        last_progress = now_progress;
        msg = msg.section('=', 1, 1).trimmed();
        now_progress = msg.toInt();
        if(now_progress!=0)
            video_Slider->setValue(now_progress);
        if(last_progress != now_progress)
        {
            QPixmap pix;
            pause_flag = 1;
            pix.load(":/pic/pause.png");
            pause_button->setIcon(pix);//重新设置图标
        }
    }

    });
}

//按钮初始化
void MainWindow::PushButton_init()
{
    //上一曲按钮
    prev_button=new myPushButton(":/pic/prev.png","",40,40);
    QTimer * time1= new QTimer(this);
    prev_button->setParent(this);
    prev_button->move(270,530);

    //上一曲按钮按下处理
    connect(prev_button,&myPushButton::clicked,[=](){
        time1->start(500);
        connect(time1,&QTimer::timeout,[=](){
            if(curr_num>0)curr_num--;
            else curr_num=song_info_list.size()-1;
            time1->stop();
            mypro->close();
            QString cmd = QString("mplayer -quiet -slave %1").arg(song_info_list[curr_num].filePath());
            labelSongsTitle->setText(song_info_list[curr_num].fileName());
            mypro->start(cmd);
            //lrcArr_init();
            });
        });


    //暂停继续按钮
    pause_button=new myPushButton(":/pic/play.png","",40,40);
    pause_button->setParent(this);
    pause_button->move(330,530);

    //暂停继续按钮下处理
    connect(pause_button,&myPushButton::clicked,[=](){
        pause_flag = !pause_flag;//播放状态改变

        time1->start(500);
        connect(time1,&QTimer::timeout,[=](){
            QPixmap pix;
            if(pause_flag==1){//开始播放
                system("killall -CONT mplayer");
                pix.load(":/pic/pause.png");
            }
            else {
                system("killall -STOP mplayer");
                pix.load(":/pic/play.png");

            }
            pause_button->setIcon(pix);//重新设置图标
            time1->stop();
            });
        });


    //下一曲按钮
    next_button=new myPushButton(":/pic/next.png","",40,40);
    next_button->setParent(this);
    next_button->move(390,530);

    //下一曲按钮下处理
    connect(next_button,&myPushButton::clicked,[=](){
        time1->start(500);
        connect(time1,&QTimer::timeout,[=](){
            if(curr_num<song_info_list.size()-1)curr_num++;
            else curr_num = 0;
            time1->stop();
            mypro->close();
            QString cmd = QString("mplayer -quiet -slave %1").arg(song_info_list[curr_num].filePath());
            labelSongsTitle->setText(song_info_list[curr_num].fileName());
            mypro->start(cmd);
            lrcArr_init();
            });
        });

    //返回退出按钮
    exit_button=new myPushButton(":/pic/exit.png","",40,40);
    exit_button->setParent(this);
    exit_button->move(980,0);

    //返回退出按钮下处理
    connect(exit_button,&myPushButton::clicked,[=](){
    time1->start(500);
    connect(time1,&QTimer::timeout,[=](){
        mypro->close();
        exit(1);//退出程序
        time1->stop(); });
    });

    //音量按钮
    vol_button=new myPushButton(":/pic/vol.png","",40,40);
    vol_button->setParent(this);
    vol_button->move(870,500);

    vol_timer = new QTimer(this);//音量按钮隐藏定时
    connect(vol_timer,&QTimer::timeout,[=](){
        volumeslider->setVisible(false);
        vol_button_flag = 0;
        });

    QTimer *vol_timer1 = new QTimer(this);
    //音量按钮下处理
    connect(vol_button,&myPushButton::clicked,[=](){
    vol_timer1->start(500);
    vol_button_flag = !vol_button_flag;

    connect(vol_timer1,&QTimer::timeout,[=](){
        vol_timer1->stop();
        volumeslider->raise();//设置到最前面
        volumeslider->setVisible(vol_button_flag);
        if(vol_button_flag)vol_timer->start(10000);

         });
    });


    //选择文件按钮
    file_button=new myPushButton(":/pic/openfile.png","",30,25);
    file_button->setParent(this);
    file_button->move(200,0);



    //文件按钮下处理
    QTimer *file_timer = new QTimer(this);
    connect(file_button,&myPushButton::clicked,[=](){
    file_timer->start(500);
    connect(file_timer,&QTimer::timeout,[=](){
    file_timer->stop();
    QStringList msg = QFileDialog::getOpenFileNames(
                                         this,
                                        "Select one or more files to open",
//                                      "/home",
                                        "Images (*.mp3 *.wav)");
    for(int i=0; i<msg.size(); i++)
    {
        song_info_list.append(QFileInfo(msg[i]));
        songList->addItem(QFileInfo(msg[i]).fileName());
    }
    });

    });
}

//进度条初始化
void MainWindow::QSlider_init()
{
    //播放进度条
    video_Slider = new QSlider(Qt::Horizontal, this);     //设置为水平滑动条
    video_Slider->setGeometry(270,500,600,40);//200,430,500,20设置位置与大小
    video_Slider->setFocusPolicy(Qt::NoFocus);                       //去除虚线边框
    video_Slider->setStyleSheet(MEDIASLIDER_STYLE);                  //样式表设置
    //video_Slider->setRange(0,0);
    video_Slider->installEventFilter(this);                          //安装事件过滤器
    video_Slider->setValue(0);                                             //设置进度为0
    connect(video_Slider,&QSlider::sliderReleased,[=](){

      int pos = video_Slider->value()-now_progress;
      pos = pos*(now_palytime*100/now_progress)/100;
      QString msg = QString("seek %1\n").arg(pos);
      mypro->write(msg.toUtf8().data());
    });


    //音量条
    volumeslider = new QSlider(Qt::Vertical, this);                 //设置为垂直滑动条
    volumeslider->setGeometry(870,350,40,150);//设置位置与大小
    volumeslider->setFocusPolicy(Qt::NoFocus);                          //去除虚线边框
    volumeslider->setStyleSheet(VOLUMESLIDER_STYLE);                    //样式表设置
    volumeslider->setVisible(false);                                    //开机先隐藏
    volumeslider->installEventFilter(this);                             //安装事件过滤器
    volumeslider->setValue(100);           //设置媒体默认音量
    volumeslider->raise();//设置到最前面
    connect(volumeslider,&QSlider::valueChanged,[=](int vlaue){
    vol_timer->stop();
    vol_timer->start(10000);//重设时间
    QString msg = QString("volume %1 1\n").arg(vlaue);
    mypro->write(msg.toUtf8().data());
    });
}


//播放列表初始化
void MainWindow::QListWidget_init()
{

    songList = new QListWidget(this);
    songList->clear();                                                  //清空列表
    songList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);       //关闭水平滚动条
    songList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     //关闭垂直滚动条
    songList->setFrameShape(QListWidget::NoFrame);                      //去除边框
    songList->setGeometry(0,20,250,580);//设置歌曲列表的位置与大小

    QFont font;
    font.setPointSize(16);
    songList->setFont(font);                                            //设置字体
    songList->setStyleSheet(SONGSLIST_STYLE);

    QListWidgetItem *item = NULL;

    scanSongs();

    for (int i = 0; i < song_info_list.size(); i++) {
        songList->addItem(song_info_list[i].fileName());
        item = songList->item(i);
        item->setForeground(Qt::white);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),
                                (float)30/480*this->geometry().height())
                          );//250，480
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    songList->setVisible(true);

    connect(songList,&QListWidget::itemClicked,[=](QListWidgetItem *item){
        mypro->close();
        for(int i=0; i<song_info_list.size(); i++)
        {
            if(song_info_list[i].fileName().indexOf(item->text(),0)!=-1)
            {
                curr_num = i;
                labelSongsTitle->setText(song_info_list[curr_num].fileName());
            }

        }

        QString cmd = QString("mplayer -quiet -slave %1").arg(song_info_list[curr_num].filePath());

        mypro->start(cmd);
        lrcArr_init();
    });

}



//歌词标签初始化
void MainWindow::labelInit()
{
    QFont font;

    LabelMainWindowBackground = new QLabel(this);           //用于覆盖桌面写真背景，防止背景过亮
    labelDuration = new QLabel(this);                       //播放进度标签
    font.setPointSize(16);
    labelDuration->setFont(font);
    labelDuration->setGeometry((float)570/800*this->geometry().width(),
                               (float)430/480*this->geometry().height(),
                               (float)120/800*this->geometry().width(),
                               (float)20/480*this->geometry().height());//570,440,120,20
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);          //设置对齐方式


    labelSongsTitle = new QLabel(this);                     //歌曲标题（标签）
    labelSongsTitle->setGeometry(280,490,350,25);//212,416,350,25
    labelSongsTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelSongsTitle->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelSongsTitle->setText("音乐 聆听美好！");
    font.setPointSize(16);
    labelSongsTitle->setFont(font);

    //初始化歌词标签文本
    labelArr.clear();
    for (int i = 0; i < 7; i++) {
        QLabel *label = new QLabel(this);
        labelArr.append(label);
    }

    font.setPointSize(14);
    for (int i = 0; i < 7; i++) {
        labelArr[i]->setFont(font);
        labelArr[i]->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
        labelArr[i]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    labelArr[3]->setStyleSheet("QLabel{color:#faff02;}QLabel{background:transparent}");

    labelArr[3]->setText("音乐 聆听美好！");                  //设置歌词文本初始显示内容

    for (int i = 0; i < 7; i++){
        labelArr[i]->setGeometry((float)250/800*this->geometry().width(),
                                 (float)(50 + i*40)/480*this->geometry().height(),
                                 (float)550/800*this->geometry().width(),
                                 (float)30/480*this->geometry().height());//250, (50 + i*40), 550, 30
    }

    lab_timer = new QTimer(this);
    lab_timer->start(200);
    connect(lab_timer,&QTimer::timeout,this,&MainWindow::lrcArrDisplay);
}



void MainWindow::scanSongs()
{
    QDir dir1(QCoreApplication::applicationDirPath()+ "/music");
    //绝对路径转换，如果不转换则linux底层的播放器会找不到文件
    QDir dir2(dir1.absolutePath());
    if (dir2.exists()) {                                               // 如果目录存在
        QStringList filter;                                            // 定义过滤器
        filter << "*.mp3";                                             // 包含所有.mp3后缀的文件
        song_info_list = dir2.entryInfoList(filter, QDir::Files);      // 获取该目录下的所有文件
    }

    else  qDebug() << "没有文件" ;

}


//歌词结构体
typedef struct LrcParseFormat {
    QString time;
    QString content;
}LrcFormat;


QVector<LrcFormat> lrcArr;                              //lrc歌词数组


//歌词更新
void MainWindow::lrcArrDisplay()
{
    if(last_playtime == now_palytime)
    {
        return ;
    }
    int second  = now_palytime%60;
    int minute = now_palytime/60;

    QString strLabel;
    strLabel.clear();

    if (minute >= 10)
        strLabel = QString::number(minute, 10);
    else
        strLabel = "0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);

    //歌词显示更新
    int count = lrcArr.count();
    for (int i = 0; i < count; i++) {

        if (lrcArr[i].time == strLabel) {

            labelArr[3]->setText(lrcArr[i].content);
            if (i-1 >= 0)
                labelArr[2]->setText(lrcArr[i-1].content);
            else
                labelArr[2]->clear();

            if (i-2 >= 0)
                labelArr[1]->setText(lrcArr[i-2].content);
            else
                labelArr[1]->clear();

            if (i-3 >= 0)
                labelArr[0]->setText(lrcArr[i-3].content);
            else
                labelArr[0]->clear();

            if (i+1 < count)
                labelArr[4]->setText(lrcArr[i+1].content);
            else
                labelArr[4]->clear();

            if (i+2 < count)
                labelArr[5]->setText(lrcArr[i+2].content);
            else
                labelArr[5]->clear();

            if (i+3 < count)
                labelArr[6]->setText(lrcArr[i+3].content);
            else
                labelArr[6]->clear();
        }
    }
}


void MainWindow::lrcArr_init()
{


    //labelSongsTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);//注意这里不要放错地方了

    lrcArr.clear();                                                     // 释放上一首歌曲的歌词所占内存
    for (int i = 0; i < 7; i++){                                        // 清理歌词显示label
        labelArr[i]->clear();
    }
    QString lrcFile = labelSongsTitle->text();//获取当前播放文件名
    lrcFile.remove(QString("mp3"), Qt::CaseInsensitive);//删除
    lrcFile  = lrcFile+"lrc";

    QFile file(lrcFile);

    if (!file.exists())                                                 //判断文件是否存在
        return;                                                         //不存在则返回

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))              //尝试打开文件
        return;

    QTextCodec *code = QTextCodec::codecForName("UTF-8");               //设置读取文件的编码方式，这里注意了，歌词文本要是”UTF-8“的，否则会乱码
    QTextStream in(&file);
    in.setCodec(code);
    in.readLine();
    in.readLine();                                                      //读取文件，跳过前面两行，所以这里是读取两次
    QString line;
    LrcFormat info;

    while (!in.atEnd()) {
        line = in.readLine();
        info.time = line.mid(1,5);
        info.content = line.mid(10);
        lrcArr.append(info);

        qDebug() << info.time << info.content;//debug
    }
    file.close();                                                        //读取完成之后 关闭文件
    int count = lrcArr.count();                                          //显示最前面的几句歌词
    if (count > 0)
        labelArr[4]->setText(lrcArr[0].content);
    if (count > 1)
        labelArr[5]->setText(lrcArr[1].content);
    if (count > 2)
        labelArr[6]->setText(lrcArr[2].content);

}

