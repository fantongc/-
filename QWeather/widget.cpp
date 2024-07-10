#include "widget.h"
#include <QGridLayout>
#include <QBoxLayout>
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QFont>
#include <QTime>
#include <QGridLayout>
#include <QSpacerItem>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    requst_city = new QNetworkAccessManager;
    requst_weather = new QNetworkAccessManager;

    this->setFixedSize(1024,580);
//  this->setStyleSheet("QWidget { background-color: rgba(250, 25, 25, 10%); }");
    setWindowFlag(Qt::FramelessWindowHint);

    w1=new QWidget;
    w2=new QWidget;
    w3=new QWidget;
    w4=new QWidget;
    w5=new QWidget;
    w6=new QWidget;
    w7=new QWidget;
    w8=new QWidget;

    psearch = new QToolButton;
    psearch->setIconSize(QSize(40, 50));
    psearch->setStyleSheet("background-color: rgba(0, 0, 0, 30)");//第四个值设置透明程度
    psearch->setStyleSheet("border-image:url(:/img/搜索.png)");

    lsearch = new QLineEdit;
    lsearch->setPlaceholderText("搜索城市     ");
    lsearch->setAlignment(Qt::AlignRight); //右对齐
    lsearch->setMaximumWidth(130);
    lsearch->setMinimumHeight(40);
    lsearch->setFont(QFont());
    lsearch->setFont(QFont("SimHei",15,15));
    lsearch->setStyleSheet("background-color: rgba(0, 0, 0, 100%);""color:white");



    lab_city = new QLabel;
    lab_city->setFont(QFont("SimHei",30,70));
    lab_city->setStyleSheet("color:rgb(181,225,249)");

    plocal = new QToolButton;
    plocal->setIconSize(QSize(40, 50));
    plocal->setStyleSheet("background-color: rgba(0, 0, 0, 100%)");//第四个值设置透明程度
    plocal->setStyleSheet("border-image:url(:/img/位置.png)");

    lab_date[0] = new QLabel;
    lab_date[0]->setFont(QFont("SimHei",20,70));
    lab_date[0]->setStyleSheet("color:rgb(181,225,249)");
    w3->setFixedSize(940,70);
    w1->setFixedSize(940,300);
    w2->setFixedSize(154,130);
    w4->setFixedSize(154,130);
    w5->setFixedSize(154,130);
    w6->setFixedSize(154,130);
    w7->setFixedSize(154,130);
    w8->setFixedSize(154,130);

    lab_weather_type_pic[0] = new QLabel;
    lab_weather_type_pic[0]->setMaximumSize(200,200);
    lab_weather_type_pic[0]->setScaledContents(true);
    lab_wendu = new QLabel;
    lab_wendu->setFont(QFont("SimHei",50,50));
    lab_wendu->setStyleSheet("color:rgb(255,255,255)");
    lab_temp_range[0] = new QLabel;
    lab_temp_range[0]->setFont(QFont("SimHei",30,30));
    lab_temp_range[0]->setStyleSheet("color:rgb(255,255,255)");
    lab_weather_type = new QLabel;
    lab_weather_type->setFont(QFont("SimHei",20,20));
    lab_weather_type->setStyleSheet("color:rgb(255,255,255)");

    QVBoxLayout *w1_vbox_1 = new QVBoxLayout;
    w1_vbox_1->addStretch(1);
    w1_vbox_1->addWidget(lab_wendu);
    w1_vbox_1->addWidget(lab_weather_type);
    w1_vbox_1->addWidget(lab_temp_range[0]);
    w1_vbox_1->addStretch(1);

    lab_wind = new QLabel;
    lab_wind->setFont(QFont("SimHei",20,20));
    lab_wind->setStyleSheet("color:rgb(255,255,255)");
    lab_airD = new QLabel;
    lab_airD->setFont(QFont("SimHei",20,20));
    lab_airD->setStyleSheet("color:rgb(255,255,255)");
    lab_airQ = new QLabel;
    lab_airQ->setFont(QFont("SimHei",20,20));
    lab_airQ->setStyleSheet("color:rgb(255,255,255)");
    lab_pm = new QLabel;
    lab_pm->setFont(QFont("SimHei",20,20));
    lab_pm->setStyleSheet("color:rgb(255,255,255)");
    lab_hum = new QLabel;
    lab_hum->setFont(QFont("SimHei",20,20));
    lab_hum->setStyleSheet("color:rgb(255,255,255)");
    QVBoxLayout *w1_vbox_2 = new QVBoxLayout;
    w1_vbox_2->addStretch(1);
    w1_vbox_2->addWidget(lab_wind);
    w1_vbox_2->addWidget(lab_airQ);
    w1_vbox_2->addWidget(lab_airD);
    w1_vbox_2->addWidget(lab_pm);
    w1_vbox_2->addWidget(lab_hum);
    w1_vbox_2->addStretch(1);

    QHBoxLayout *w1_hbox = new QHBoxLayout;
    w1_hbox->addWidget(lab_weather_type_pic[0]);
    w1_hbox->addItem(w1_vbox_1);
    w1_hbox->addItem(w1_vbox_2);
    w1->setLayout(w1_hbox);

    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addWidget(plocal);
    hbox1->addWidget(lab_city);
    hbox1->addWidget(lab_date[0]);
    hbox1->addWidget(lsearch);
    hbox1->addWidget(psearch);
    w3->setLayout(hbox1);

    for (int i=1;i<7;i++) {
        lab_weather_type_pic[i] = new QLabel;
        lab_weather_type_pic[i]->setMaximumSize(70,70);
        lab_weather_type_pic[i]->setScaledContents(true);
        lab_temp_range[i] = new QLabel;
        lab_temp_range[i]->setFont(QFont("SimHei",15,15));
        lab_temp_range[i]->setStyleSheet("color:rgb(181,225,249)");
        lab_date[i] = new QLabel;
        lab_date[i]->setStyleSheet("color:rgb(181,225,249)");
        lab_date[i]->setFont(QFont("SimHei",15,15));
    }

    QVBoxLayout *w2_vbox = new QVBoxLayout;
    w2_vbox->addWidget(lab_weather_type_pic[1]);
    w2_vbox->addWidget(lab_temp_range[1]);
    w2_vbox->addWidget(lab_date[1]);
    w2->setLayout(w2_vbox);

    QVBoxLayout *w4_vbox = new QVBoxLayout;
    w4_vbox->addWidget(lab_weather_type_pic[2]);
    w4_vbox->addWidget(lab_temp_range[2]);
    w4_vbox->addWidget(lab_date[2]);
    w4->setLayout(w4_vbox);

    QVBoxLayout *w5_vbox = new QVBoxLayout;
    w5_vbox->addWidget(lab_weather_type_pic[3]);
    w5_vbox->addWidget(lab_temp_range[3]);
    w5_vbox->addWidget(lab_date[3]);
    w5->setLayout(w5_vbox);

    QVBoxLayout *w6_vbox = new QVBoxLayout;
    w6_vbox->addWidget(lab_weather_type_pic[4]);
    w6_vbox->addWidget(lab_temp_range[4]);
    w6_vbox->addWidget(lab_date[4]);
    w6->setLayout(w6_vbox);

    QVBoxLayout *w7_vbox = new QVBoxLayout;
    w7_vbox->addWidget(lab_weather_type_pic[5]);
    w7_vbox->addWidget(lab_temp_range[5]);
    w7_vbox->addWidget(lab_date[5]);
    w7->setLayout(w7_vbox);

    QVBoxLayout *w8_vbox = new QVBoxLayout;
    w8_vbox->addWidget(lab_weather_type_pic[6]);
    w8_vbox->addWidget(lab_temp_range[6]);
    w8_vbox->addWidget(lab_date[6]);
    w8->setLayout(w8_vbox);


    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(w2);
    hbox->addWidget(w4);
    hbox->addWidget(w5);
    hbox->addWidget(w6);
    hbox->addWidget(w7);
    hbox->addWidget(w8);
    this->setWindowTitle("PainEvent");
    QVBoxLayout *main_vbox = new QVBoxLayout;
    main_vbox->addWidget(w3);
    main_vbox->addWidget(w1);
    main_vbox->addItem(hbox);
    setLayout(main_vbox);

    exit_button = new QPushButton(this);
    exit_button->setGeometry(980,1,40,40);
    exit_button->setStyleSheet("border-image:url(:/img/exit.png)");
    connect(exit_button,&QPushButton::clicked,[=](){
            exit(1);//退出程序
        });

    get_city();
    timer_1 = new QTimer;
    connect(timer_1,SIGNAL(timeout()),this,SLOT(data_updata()));
    timer_1->start(1000);
    timer_2 = new QTimer;
    connect(timer_2,SIGNAL(timeout()),this,SLOT(get_city()));
    timer_2->start(300000);
    connect(psearch,SIGNAL(clicked()),this,SLOT(search_city()));
    connect(plocal, &QToolButton::clicked, [&](){ get_city(); });
    connect(requst_city, SIGNAL(finished(QNetworkReply*)), this, SLOT(json_city(QNetworkReply*)));
    connect(requst_weather, SIGNAL(finished(QNetworkReply*)), this, SLOT(json_weather(QNetworkReply*)));

}

Widget::~Widget()
{
}

void Widget::data_updata()
{
       lab_city->setText(locat_city);
       lab_date[0]->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd dddd"));
       lab_weather_type_pic[0]->setPixmap(QPixmap(QString(":/img/%1.png").arg(weather_type[0])));
       lab_wendu->setText(QString::number(temp[0])+"℃");
       lab_weather_type->setText(weather_type[0]);
       lab_temp_range[0]->setText(temp_range[0]);

       lab_wind->setText("风    向:"+wind_type[0]);
       lab_airD->setText("空气指数:"+airData[0]);
       lab_airQ->setText("空气质量:"+airQuality[0]);
       lab_pm->setText("PM2.5   :"+QString::number(pm[0]));
       lab_hum->setText("湿    度:"+humidity[0]);

       for (int i=1;i<7;i++) {
           lab_weather_type_pic[i]->setPixmap(QPixmap(QString(":/img/%1.png").arg(weather_type[i])));
           lab_temp_range[i]->setText(temp_range[i]);
           lab_date[i]->setText(date[i]);
       }
}

void Widget::search_city()
{
    locat_city = lsearch->text();
    get_weather(locat_city);
    data_updata();
}


void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/img/back2.jpg"),QRect());
}

void Widget::get_city(void)
{
    //发送http请求
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.map.baidu.com/location/ip?&ak= &coor=bd09ll"));
    requst_city->get(request);


}

void Widget::json_city(QNetworkReply *reply)
{
    QString all = reply->readAll();
    //unicode 转化为汉字

    QString filename = all;
    do
    {
        int idx = filename.indexOf("\\u");
        QString strHex = filename.mid(idx, 6);
        strHex = strHex.replace("\\u", QString());
        int nHex = strHex.toInt(0, 16);
        filename.replace(idx, 6, QChar(nHex));
    } while (filename.indexOf("\\u") != -1);
//   qDebug()<<filename;
//    te->setText(filename);
    //json解析 详细步骤
    QJsonDocument  Document;
    QJsonParseError json_error;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&json_error);//解析json对象
    QJsonObject object = json_recv.object();
    if(object.contains("content"))
    {
       QJsonValue value = object.value("content");
       if(value.isObject())
       {
           QJsonObject object_1 = value.toObject();
           if(object_1.contains("address_detail"))
           {
               QJsonValue value_1 = object_1.value("address_detail");
               if(value_1.isObject())
               {
                   QJsonObject object_2 = value_1.toObject();
                   locat_city =object_2.value("city").toString();
//                   qDebug()<< "city "<<locat_city;
               }
            }
       }
    }
    //调用天气请求
    locat_city = "南宁市";
    get_weather(locat_city);

}
void Widget::get_weather(QString city)
{

    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=%1&needMoreData=true&pageNo=1&pageSize=7").arg(city)));
    requst_weather->get(request);
}
void Widget::json_weather(QNetworkReply *reply)
{
    QString all = reply->readAll();
//    qDebug()<<all;
    QJsonDocument  Document;
    QJsonParseError json_error;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&json_error);//解析json对象
    QJsonObject object = json_recv.object();
        if(object.contains("data"))
        {
           QJsonValue value = object.value("data");
           if(value.isObject())
           {
               QJsonObject object_1 = value.toObject();
               if(object_1.contains("list"))
               {
                   QJsonValue value_1 = object_1.value("list");
                   if(value_1.isArray())
                   {
                        for(int i=0; i<7; i++)
                        {
                            QJsonObject weather = value_1.toArray().at(i).toObject();
                            QString strdate = weather.value("date").toString();
                            date[i] = strdate.right(5);
                            weather_type[i] = weather.value("weather").toString();
                            temp[i] = weather.value("temp").toInt();
                            humidity[i] = weather.value("humidity").toString();
                            pm[i] = weather.value("pm25").toInt();
                            int low = weather.value("low").toInt();
                            int high = weather.value("high").toInt();
                            temp_range[i] = QString::number(low)+"~~"+QString::number(high)+"℃";
                            airData[i] = weather.value("airData").toString();
                            airQuality[i] = weather.value("airQuality").toString();
                            wind_type[i] = weather.value("wind").toString();
                        }
                   }
                }
           }
        }
}







