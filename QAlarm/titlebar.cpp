#include "titlebar.h"


TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(30);
    setWindowFlag(Qt::FramelessWindowHint);
    miconlabe = new QLabel(this);
    miconlabe->setFixedSize(20,20);
    miconlabe->setScaledContents(true);
    QHBoxLayout * TitleBarLayout = new QHBoxLayout();
    TitleBarLayout->addWidget(miconlabe);

    setLayout(TitleBarLayout);
}

void TitleBar::SetTitleBarIcon(const QString &iconpath){
    QPixmap map(iconpath);
    miconlabe->setPixmap(map);
}

TitleBar::~TitleBar()
{

}
