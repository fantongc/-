#include "menubutton.h"
#include <QDebug>

MenuButton::MenuButton(QString normal_path,QString press_path,int pixwidth,int pixheight)
{
    this->normal_path=normal_path;
    this->press_path=press_path;

    QPixmap pix;
    bool ret = pix.load(this->normal_path);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return ;

    }
    //设置图片固定大小
    this->setFixedSize(pixwidth,pixheight);
    //设置不规则图片样式
    this->setStyleSheet( "QPushButton{border:0px;}" );
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(pixwidth,pixheight));

    this->setFocusPolicy(Qt::NoFocus);     //去除虚线边框

    animation = new QPropertyAnimation(this,"geometry");

}

void MenuButton::zoom1()
{
    //设置动画时间间隔
    animation->setDuration(200);
    //设置起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

void MenuButton::zoom2()
{
    //设置动画时间间隔
    animation->setDuration(200);
    //设置起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置结束位置
    animation->setEndValue(QRect(this->x(),this->y()-10,this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::InElastic);
    //执行动画
    animation->start();
}
