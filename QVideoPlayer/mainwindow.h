#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSlider>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void MediaInit();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    ~MainWindow();

private slots:
    void on_actionimport_file_triggered();

    void on_Begin_clicked();

    void onItemDoubleClicked(QListWidgetItem * item);

    void showListWidgetMenuSlot(QPoint);

    void deleteAllData();

    void GetDuration(qint64);

    void upDateSlider(qint64);

    void slot_PlayError(QMediaPlayer::Error);

    void on_back_clicked();

    void on_advance_clicked();

    void ChangeVoice(int);

    void MoveVideo(int);

    void on_fullScreen_clicked();

    void on_volumeData_clicked();

    void on_action_h_Help_triggered();

    void keyPressEvent(QKeyEvent *event);

    void on_Speed_currentIndexChanged(const QString &arg1);

    void scanVideoFiles();


private:
    Ui::MainWindow *ui;
    QMediaPlayer *Player;
    QVideoWidget *videoWidget;
    QListWidget *DataList;
    QMenu *Menu;
    QAction *deleteAll;
    QString totalFormatTime;
    QString currentFormatTime;
    int back_volume = 0;
    int flag_back_volume = 0;
};

#endif // MAINWINDOW_H
