#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent);
    // 这里parent没有给默认值NULL，保证在创建MyTitleBar对象时父指针必须得赋值;且赋值不为NULL;
    ~TitleBar();
    void SetTitleBarIcon(const QString &iconpath);
private:
    QLabel *miconlabe;
};

#endif // TITLEBAR_H
