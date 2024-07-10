#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化
    this->MediaInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化函数
void MainWindow::MediaInit() {

    this->setWindowTitle("视频播放器");
    this->setWindowIcon(QPixmap(":/Icon/Icon/tortoiseTitleIcon.png"));
    this->setStyleSheet("MainWindow{border-image: url(:/Icon/Icon/OIP-C.jfif);}");
    setWindowFlag(Qt::FramelessWindowHint);
//    this->showFullScreen();
    // 初始化变量
    Player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    DataList = new QListWidget(this);
    Menu = new QMenu(this);
    deleteAll = new QAction("删除全部", this);

    QPushButton *exit_button = new QPushButton(this);

    exit_button->setGeometry(980,1,40,40);
    exit_button->setStyleSheet("border-image:url(:/Icon/Icon/exit.png)");
    connect(exit_button,&QPushButton::clicked,[=](){
            exit(1);//退出程序
        });

    // 功能性设置
    DataList->setContextMenuPolicy(Qt::CustomContextMenu);
    Menu->addAction(deleteAll);
    ui->videoSlider->setSingleStep(0);
    ui->Begin->setDisabled(true);
    ui->back->setDisabled(true);
    ui->advance->setDisabled(true);
    this->setAcceptDrops(true);

    scanVideoFiles();

    // ui布局
    this->DataList->setStyleSheet("QListView {	font: 25 9pt Microsoft YaHei;border: 15px solid white;border-radius: 10px;}"
                                  "QListView::item {height: 60px;}"
                                  "QListView::item:hover {background-color: transparent;padding: 10px;border-left: 3px solid rgb(130, 130, 130);}"
                                  "QListView::item:selected { background-color: transparent;color: black;padding: 10px;border-left: 3px solid black;}");

    ui->horizontalLayout->addWidget(videoWidget,10);
    ui->horizontalLayout->addWidget(DataList,0);
    ui->fullScreen->setIcon(QPixmap(":/Icon/Icon/magnify.png"));
    ui->fullScreen->setIconSize(QSize(20,20));
    ui->Begin->setIcon(QPixmap(":/Icon/Icon/begin.png"));
    ui->Begin->setIconSize(QSize(20,20));
    ui->back->setIcon(QPixmap(":/Icon/Icon/back.png"));
    ui->back->setIconSize(QSize(20,20));
    ui->advance->setIcon(QPixmap(":/Icon/Icon/advance.png"));
    ui->advance->setIconSize(QSize(20,20));
    ui->volumeData->setIcon(QPixmap(":/Icon/Icon/volumeHigh.png"));
    ui->volumeData->setIconSize(QSize(20,20));



    // 信号
    connect(DataList, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(DataList, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(showListWidgetMenuSlot(QPoint)));
    connect(deleteAll,SIGNAL(triggered()),this,SLOT(deleteAllData()));
    connect(Player, SIGNAL(durationChanged(qint64)), this, SLOT(GetDuration(qint64)));
    connect(Player, SIGNAL(positionChanged(qint64)), this, SLOT(upDateSlider(qint64)));
    connect(Player, SIGNAL(error(QMediaPlayer::Error)),this,SLOT(slot_PlayError(QMediaPlayer::Error)));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(ChangeVoice(int)));
    connect(ui->videoSlider, SIGNAL(sliderMoved(int)), this, SLOT(MoveVideo(int)));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QString name;
    QList<QUrl> urls;
    QList<QUrl>::Iterator i;
    urls = event->mimeData()->urls();
    for(i = urls.begin(); i != urls.end(); ++i) {
        name = i->path();
        this->DataList->addItem(name);
    }

    Player->setMedia(QUrl::fromLocalFile(name));
    Player->setVideoOutput(videoWidget);
    Player->play();
}

// 错误提示
void MainWindow::slot_PlayError(QMediaPlayer::Error error) {
    QString text;
    if(QMediaPlayer::NoError==error)
    {
        text="正常解码!";
    }
    else if(QMediaPlayer::ResourceError==error)
    {
        text="媒体资源无法解析。";
    }
    else if(QMediaPlayer::FormatError==error)
    {
        text="不支持该媒体资源的格式，没有解码器!";
    }
    else if(QMediaPlayer::NetworkError==error)
    {
        text="发生网络错误。";
    }
    else if(QMediaPlayer::AccessDeniedError==error)
    {
        text="没有适当的权限来播放媒体资源。";
    }
    else if(QMediaPlayer::ServiceMissingError==error)
    {
        text="没有找到有效的播放服务，播放无法继续。";
    }

    QMessageBox::critical(this,"播放器提示",text,QMessageBox::Ok,QMessageBox::Ok);
}

// 更新进度条
void MainWindow::upDateSlider(qint64 position) {

    ui->Begin->setDisabled(false);
    ui->back->setDisabled(false);
    ui->advance->setDisabled(false);

    if(ui->videoSlider->isSliderDown()) {
        return;
    }

    double num_p = position;
    double num_d = Player->duration();

    ui->videoSlider->setSliderPosition(100 * num_p / num_d);

    QTime currentTime(0, 0, 0, 0);
    currentTime = currentTime.addMSecs(position);
    currentFormatTime = currentTime.toString("mm:ss");
    ui->BeginTime->setText(currentFormatTime);
}

// 获取视频时间信息
void MainWindow::GetDuration(qint64 duration) {
    QTime totalTime = QTime(0, 0, 0, 0);
    totalTime = totalTime.addMSecs(duration);
    totalFormatTime = totalTime.toString("mm:ss");
    ui->EndTime->setText(totalFormatTime);
}

void MainWindow::showListWidgetMenuSlot(QPoint) {
    Menu->exec(QCursor::pos());
}

// 清空列表
void MainWindow::deleteAllData() {
    DataList->clear();
    Player->stop();
    ui->Begin->setIcon(QPixmap(":/Icon/Icon/begin.png"));
    ui->Begin->setIconSize(QSize(20,20));
}

// 导入
void MainWindow::on_actionimport_file_triggered()
{
    deleteAllData();

    QString strCurrentPath = QDir::homePath();
    QString stdDlgTitle = "文件选取";
    QString strFilter = "MP4 File(*.mp4);;All File(*.*);;MKV File(*.mkv)";
    QString strAllFiles = QFileDialog::getOpenFileName(this, stdDlgTitle,
                                                       strCurrentPath,strFilter);

    if(strAllFiles.isEmpty()) {
        QMessageBox::information(this,"错误","打开视频文件失败，请重新检查",QMessageBox::Yes|QMessageBox::No);
        return;
    }

    QFileInfo fileInfos(strAllFiles);
    qDebug() << "File info: " << fileInfos.fileName() << ", path: " << fileInfos.filePath();

    DataList->addItem(fileInfos.filePath());
    Player->setMedia(QUrl::fromLocalFile(strAllFiles));
    Player->setVideoOutput(videoWidget);
    Player->play();
}

void MainWindow::scanVideoFiles()
{
    QDir dir(QCoreApplication::applicationDirPath()
                 + "/myVideo");
        QDir dirbsolutePath(dir.absolutePath());
         if (dirbsolutePath.exists()) {
             QStringList filter;
             filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi";
             QFileInfoList files = dirbsolutePath.entryInfoList(filter, QDir::Files);
             for (int i = 0; i < files.count(); i++) {

                          QString fileName = QString::fromUtf8(files.at(i)
                                                           .fileName()
                                                           .toUtf8()
                                                           .data());
                          QString filePath = QString::fromUtf8(files.at(i)
                                                           .filePath()
                                                           .toUtf8()
                                                           .data());
                          DataList->addItem(filePath);
                         }

         }
}



// 双击
void MainWindow::onItemDoubleClicked(QListWidgetItem * item) {
    QString itemStr = item->text();

    Player->setMedia(QUrl::fromLocalFile(itemStr));
    Player->setVideoOutput(videoWidget);
    Player->play();
}

// 开始/暂停
void MainWindow::on_Begin_clicked()
{
    if(Player->state() ==  QMediaPlayer::PlayingState) {
        ui->Begin->setIcon(QPixmap(":/Icon/Icon/begin.png"));
        Player->pause();
    } else {
        ui->Begin->setIcon(QPixmap(":/Icon/Icon/pause.png"));
        Player->play();
    }
}

// 回退
void MainWindow::on_back_clicked()
{
    qint64 currentpos = Player->position();
    Player->setPosition(currentpos - 10000);//在原来的基础上前进10s
}

// 前进
void MainWindow::on_advance_clicked()
{
    qint64 currentpos = Player->position();
    Player->setPosition(currentpos + 10000);//在原来的基础上前进10s
}

// 全屏
void MainWindow::on_fullScreen_clicked()
{
    if(this->isFullScreen()) {
        this->showNormal();
        ui->fullScreen->setIcon(QPixmap(":/Icon/Icon/magnify.png"));
    } else {
        this->showFullScreen();
        ui->fullScreen->setIcon(QPixmap(":/Icon/Icon/lessen.png"));
    }
}

// 倍速
void MainWindow::on_Speed_currentIndexChanged(const QString &arg1)
{
    Player->setPlaybackRate(arg1.toDouble());
}



// 一键静音
void MainWindow::on_volumeData_clicked()
{
    if(++flag_back_volume % 2 != 0) {
        back_volume = ui->volumeValue->text().toInt();
    }

    if(ui->volumeValue->text().toInt() != 0) {
        ui->volumeSlider->setValue(0);
        Player->setVolume(0);
        ui->volumeValue->setText(QString::number(0));
        ui->volumeData->setIcon(QPixmap(":/Icon/Icon/volumeCross.png"));
    } else {
        ui->volumeSlider->setValue(back_volume);
        Player->setVolume(back_volume);
        ui->volumeValue->setText(QString::number(back_volume));
        ui->volumeData->setIcon(QPixmap(":/Icon/Icon/volumeHigh.png"));
    }

}

// 更新音量
void MainWindow::ChangeVoice(int volumeData) {
    Player->setVolume(volumeData);
    ui->volumeValue->setText(QString::number(volumeData));
    if(volumeData == 0) {
        ui->volumeData->setIcon(QPixmap(":/Icon/Icon/volumeCross.png"));
    } else {
        ui->volumeData->setIcon(QPixmap(":/Icon/Icon/volumeHigh.png"));
    }
}

//更新视频进度
void MainWindow::MoveVideo(int videoData) {
    double num_d = Player->duration();
    Player->setPosition(videoData * 1000 * (num_d / 100000));

}


// 捕获键盘事件
void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << event->key();
    switch (event->key()) {
    case Qt::Key_Escape:
        if(!this->isFullScreen()) {
            QMessageBox:: StandardButton result= QMessageBox::information(this, "information", "是否要关闭窗口？",QMessageBox::Yes|QMessageBox::No);
            if(result == QMessageBox::Yes) {
                this->close();
            }
        } else {
            on_fullScreen_clicked();
        }
        break;
    case Qt::Key_Space:
        on_Begin_clicked();
        break;
    default:
        break;
    }
}

void MainWindow::on_action_h_Help_triggered()
{
//    QDesktopServices::openUrl(QUrl(QString("https://blog.csdn.net/LKHzzzzz?spm=1000.2115.3001.5343")));
}
