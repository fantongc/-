#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QProcess>
#include <QTimer>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLCDNumber *m_pLCD;
    QProcess *my_pro;
    QTimer *time1;

    QPushButton *exit_button;

    /* 布局初始化 */
    void layoutInit();

private slots:
    //void onTimeOut();
};
#endif // MAINWINDOW_H
