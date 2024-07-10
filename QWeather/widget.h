#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QToolButton>
#include <QHostAddress>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
//网络请求
#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//json解析
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPaintEvent>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



private:
//    QPainter *painter;
    QString locat_city;

    QWidget *w1;
    QWidget *w2;
    QWidget *w3;
    QWidget *w4;
    QWidget *w5;
    QWidget *w6;
    QWidget *w7;
    QWidget *w8;

    QPushButton *exit_button;

    QLabel *lab_city;

    QLabel *lab_date[7];
    QLabel *lab_weather_type;
    QLabel *lab_weather_type_pic[7];
    QLabel *lab_wendu;
    QLabel *lab_temp_range[7];
//    QLabel *lab_date[7];
    QLabel *lab_wind;
    QLabel *lab_airD;
    QLabel *lab_airQ;
    QLabel *lab_pm;
    QLabel *lab_hum;

    QDateTime current_date_time;

    QLineEdit *lsearch;
    QToolButton *psearch;
    QToolButton *plocal;

    QNetworkAccessManager *requst_city;
    QNetworkAccessManager *requst_weather;
    QNetworkAccessManager m_networkGetWeather;

    QString date[7];
    QString weather_type[7] ;
    int temp[7];
    QString humidity[7];
    int pm[7];
    QString temp_range[7];
    QString airData[7];
    QString airQuality[7];
    QString wind_type[7];

    QTimer *timer_1;
    QTimer *timer_2;
    void get_weather(QString );
    void paintEvent(QPaintEvent *event);

 //   QWidget *window = nullptr;

public slots:
     void get_city(void);
     void json_city(QNetworkReply *reply);
     void json_weather(QNetworkReply*);
     void data_updata();
     void search_city();

//     bool eventFilter(QObject *,QEvent *);
 //    void keyPressEvent(QKeyEvent * event);

signals:

};
#endif // WIDGET_H
