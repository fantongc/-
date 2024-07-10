#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QGridLayout>
#include <QDateTimeEdit>
#include <QTimeEdit>
#include <QMediaPlayer>
#include <QtMultimediaWidgets>
#include <QDir>
#include "qss.h"
#include <QSettings>
#include <QMessageBox>
#include <QTextBrowser>
#include <QStringList>
#include <iostream>
#include <QHBoxLayout>
#include <QCloseEvent>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    QLabel *lablel1,*lablel2,*lablel3,*version_lable;
    QTextEdit *lineedit;
    QPushButton *button;
    QPushButton *forever_button;
    QPushButton *hidebutton;
    QPushButton *exit_button;
    QGridLayout *mainlayout;
    QHBoxLayout *hblayout;
    QDateTimeEdit *datatime;
    QTimeEdit *qtimeedit;
    QTime *qtime;
    QTime settime;
    QTimer *time_clock;
    QMediaPlayer *mplayer;
    QString path;
    QString styleSheet;
    qss *qtcss;
    QSettings  *settings;
    QTime readtime;
    QString key = "time%1";
    QHash <QTime,QString> clockhash;
    QTextEdit *textbrowser;
    QHash<QTime,QString>::iterator i;
    QHash<QTime,QString>::iterator j;
    void writehash(QTime times,QString strs);
    void writini(QString qkeys,QString qvalues,QString gname);
    QString readini(QString qkeys,QString gname);
    void removeini(QString qkeys,QString gname);
    void Traverse_ini(QString  gname);
    void closeEvent(QCloseEvent *event);

private slots:
    void setclock();
    void showclock();
    void HideClock();
    void set_forever_clock();

};

#endif // DIALOG_H
