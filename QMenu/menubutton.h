#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include<QPushButton>
#include<QPropertyAnimation>
#include<QString>
#include<QEvent>
#include<QMouseEvent>

#include <QObject>
#include <QWidget>

/*
 * 作者：混分巨兽龙某某
 * csdn：混分巨兽龙某某
 * 邮箱：1178305328@qq.com
 * 嵌入式技术交流群：958820627
 */

/* 创建了MenuButton的类，并且该类继承QPushButton */
class MenuButton : public QPushButton
{
    Q_OBJECT

public:
    MenuButton(QString normal_path,QString press_path="",int pixwidth=10,int pixheight=10);
    void zoom1();
    void zoom2();

private:
    QString normal_path;
    QString press_path;
    QPropertyAnimation* animation;

protected:

signals:

public slots:
};

#endif // MENUBUTTON_H
