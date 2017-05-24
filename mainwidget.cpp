#include "mainwidget.h"
#include<QVBoxLayout>
#include<QMessageBox>
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    ,bPlaying(false)
    ,volume(50)
    ,DownLoadPath("./")
{
    resize(800, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    InitWidget();
    InitLayout();
    setWindowOpacity(0.96);
    player=new QMediaPlayer;
   // player->setMedia(QUrl("http://yinyueshiting.baidu.com/data2/music/42783748/42783748.mp3?xcode=e03a1574360225dae61345ed80ef9cd2"));
   // player->play();

    connect(playlistWidget,SIGNAL(love()),this,SLOT(ChangeMylove()));
    connect(playlistWidget,SIGNAL(trylisten()),this,SLOT(ChangeTryListen()));
    connect(playlistWidget,SIGNAL(history()),this,SLOT(ChangePlayHistory()));
    connect(playlistWidget,SIGNAL(localMusic()),this,SLOT(ChangeLocalMusic()));

    connect(localmusicWidget,SIGNAL(ChangeSong(QString)),this,SLOT(ChangeSong(QString)));//本地歌曲点击了添加后播放新的歌
    connect(controlWidget,SIGNAL(ShowVolume()),this,SLOT(ShowVolume()));//当下方控制处点击了音量按钮发射ShowVolume信号
    connect(controlWidget,SIGNAL(PlayPause()),this,SLOT(PlayPause()));
    connect(controlWidget,SIGNAL(ProgressUpdate(int)),this,SLOT(ChangeProgress(int)));
    connect(controlWidget,SIGNAL(ShowLrc()),this,SLOT(showLrc()));
    connect(controlWidget,SIGNAL(DownLoad()),this,SLOT(DownLoad()));

    connect(titleWidget,SIGNAL(Search(QString)),this,SLOT(onSearch(QString)));

    connect(searchWidget,SIGNAL(ChangeSong(QString)),this,SLOT(ChangeSong(QString)));//在线音乐的切换歌曲信号
    connect(showlrcWidget,SIGNAL(HideLrc()),this,SLOT(HideLrc()));
}

MainWidget::~MainWidget()
{

}
void MainWidget::InitWidget()
{
    titleWidget=new TitleBar;
    controlWidget=new ControlWidget;
    playlistWidget=new PlayListWidget;
    trylistenWiidget=new TryListenWidget;
    myloveWidget=new MyLoveWidget;
    playhistoryWidget=new PlayHistoryWidget;
    localmusicWidget=new LocalMusicWidget;
    searchWidget=new SearchWidget;
    showlrcWidget=new ShowLrcWidget;
    stackWidget=new QStackedWidget;
    stackWidget->addWidget(trylistenWiidget);
    stackWidget->addWidget(myloveWidget);
    stackWidget->addWidget(playhistoryWidget);
    stackWidget->addWidget(localmusicWidget);
    stackWidget->addWidget(searchWidget);
    stackWidget->addWidget(showlrcWidget);

    volumeBar=new QSlider();
    volumeBar->setObjectName("volume");
    volumeBar->setOrientation(Qt::Vertical);
    volumeBar->setStyleSheet(controlWidget->GetQss(":/qss/style"));
    volumeBar->setRange(0,150);
    volumeBar->setValue(50);
    volumeBar->hide();
    connect(volumeBar,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged(int)));
}
void MainWidget::InitLayout()
{
    QVBoxLayout *main_layout=new QVBoxLayout(this);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(titleWidget,0,Qt::AlignTop);
    QHBoxLayout *hLay=new QHBoxLayout;//水平布局包括左边的列表以及右边的显示项
    hLay->setSpacing(0);hLay->setContentsMargins(0,0,0,0);
    hLay->addWidget(playlistWidget);
    hLay->addWidget(stackWidget);
    main_layout->addLayout(hLay);
    main_layout->addWidget(controlWidget,0,Qt::AlignBottom);

}
QPixmap MainWidget::getWebPix(QString picUrl)
{
    //=============显示网络图片到label==========================
        QUrl pic_url(picUrl);
        QNetworkAccessManager manager;
        QEventLoop loop;
        QNetworkReply *reply = manager.get(QNetworkRequest(pic_url));
        //请求结束并下载完成后，退出子事件循环
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        //开启子事件循环
        loop.exec();
        QByteArray jpegData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(jpegData);
        return pixmap;

}
void MainWidget::ChangeSong(QString str)
{
  //  QMessageBox::information(this,"",str);
    //传递格式为url(本机路径或网络路径)|类型(Local或Web)|标题|时间(xx:xx格式)|pic地址|lrc地址
    paramList=str.split('|');
    QString url=paramList.at(0);
    QString type=paramList.at(1);
    QString songInfo=paramList.at(2);
    QString time =paramList.at(3);
    picUrl=paramList.at(4);
    lrcUrl=paramList.at(5);
    controlWidget->songNameLabel->setText(songInfo);
    controlWidget->playBtn->setPicName(":/playControl/pause");//更新按钮
    controlWidget->RestartProgressBar();//进度条重置
    if(picUrl!="default")
        controlWidget->albumnLabel->setPixmap(getWebPix(picUrl)); // 在QLabel显示图片
    else
        controlWidget->albumnLabel->setPixmap(QPixmap(tr(":/logo")));

    bool ok;
    int h=time.split(':').at(0).toInt(&ok);
    int m=time.split(':').at(1).toInt(&ok);
    controlWidget->progressBar->setMaximum(h*60+m);//设置进度条最大值
    controlWidget->update();//刷新
    //歌词部分也开始显示了
    showlrcWidget->StartTimer();
    //播放
    if(url=="Local")
        player->setMedia(QUrl::fromLocalFile(url));
    else
        player->setMedia(QUrl(url));
    player->play();
    bPlaying=true;

}
void MainWidget::volumeChanged(int i)
{
    int pos=volumeBar->value();
    player->setVolume(pos);
}
void MainWidget::PlayPause()
{
    if(bPlaying)
    {
        player->pause();
        showlrcWidget->timer->stop();
        bPlaying=false;
    }
    else
    {
        player->play();
        showlrcWidget->timer->start();
        bPlaying=true;
    }
}
void MainWidget::ChangeProgress(int pos)
{
    player->setPosition(pos*1000);
    showlrcWidget->pos=pos;
}
void MainWidget::setStyle(const QString &style) {
       QFile qss(style);
       qss.open(QFile::ReadOnly);
       qApp->setStyleSheet(qss.readAll());
       qss.close();
   }
void MainWidget::ShowVolume()
{
    QPoint pt=pos();
    volumeBar->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    volumeBar->show();
    volumeBar->setGeometry(pt.x()+915,pt.y()+425,50,120);
}
void MainWidget::DownLoad()
{
   // QStringList strList=str.split('|');
    QString music_url=paramList.at(0);
    QString type=paramList.at(1);
    if(type=="Local")
        QMessageBox::information(this,"提示","当前播放的是本地歌曲");
    else
    {
        QUrl url(music_url);
        QNetworkRequest request;
        QNetworkAccessManager * manager = new QNetworkAccessManager();
        request.setUrl(url);
        manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownload(QNetworkReply*)));//数据传输完成信号
    }
}
void MainWidget::onDownload(QNetworkReply *reply)
{
    QByteArray content=reply->readAll();
    QString title=paramList.at(2);
    QFile f(DownLoadPath+title+".mp3");
    f.open(QFile::WriteOnly);
    f.write(content);
    f.close();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton)
    {
        volumeBar->hide();
    }
}
void MainWidget::ChangeMylove()
{
    stackWidget->setCurrentWidget(myloveWidget);
}

void MainWidget::ChangeTryListen()
{
     stackWidget->setCurrentWidget(trylistenWiidget);
}

void MainWidget::ChangePlayHistory()
{
     stackWidget->setCurrentWidget(playhistoryWidget);
}

void MainWidget::ChangeLocalMusic()
{
     stackWidget->setCurrentWidget(localmusicWidget);
}
void MainWidget::onSearch(QString str)
{
    stackWidget->setCurrentWidget(searchWidget);
    searchWidget->searchContent=str;
    searchWidget->Search();
}
void MainWidget::showLrc()
{
    prevWidgetIndex=stackWidget->currentIndex();//得到当前显示的窗口的序号

    showlrcWidget->lrcUrl=lrcUrl;
    showlrcWidget->picUrl=picUrl;
    showlrcWidget->ShowLrc();
    stackWidget->setCurrentWidget(showlrcWidget);

}
void MainWidget::HideLrc()
{
    stackWidget->setCurrentIndex(prevWidgetIndex);
}
