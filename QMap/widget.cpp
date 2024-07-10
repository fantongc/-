#include "widget.h"
#include "ui_widget.h"
#include "QHBoxLayout"
#include "QWebEngineView"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("地图");
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    QWebEngineView *webView = new QWebEngineView(this);
    pLayout->addWidget(webView);
    webView->load(QUrl("http://map.baidu.com/"));
}



Widget::~Widget()
{
    delete ui;
}




