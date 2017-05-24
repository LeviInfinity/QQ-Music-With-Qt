#include "controlwidget.h"
#include<QHBoxLayout>
#include<QPalette>
ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
  ,m_proPos(0)
{
    setFixedHeight(55);
    QPalette pal(palette());
    pal.setColor(QPalette::Background,QColor(60,60,60));
    setPalette(pal);
    setAutoFillBackground(true);
    QPalette text_palette(palette());
    text_palette.setColor(QPalette::WindowText, QColor(220, 220, 220));
    setPalette(text_palette);

    leftBtn=new MyPushButton;
    playBtn=new MyPushButton;
    rightBtn=new MyPushButton;
    downloadBtn=new MyPushButton;
    volumeBtn=new MyPushButton;
    loopBtn=new MyPushButton;
    menuBtn=new MyPushButton;
    lrcBtn=new MyPushButton;

    leftBtn->setPicName(":/playControl/left");
    playBtn->setPicName(":/playControl/play");
    rightBtn->setPicName(":/playControl/right");
    downloadBtn->setPicName(":/playControl/download");
    volumeBtn->setPicName(":/playControl/volume");
    loopBtn->setPicName(":/playControl/loop");
    menuBtn->setPicName(":/playControl/menu");
    lrcBtn->setPicName(":/playControl/lrc");

    loveBtn=new QCheckBox;
    loveBtn->setStyleSheet(GetQss(":/qss/check"));

    albumnLabel=new QLabel;
    albumnLabel->setFixedSize(40, 40);
    albumnLabel->setScaledContents(true);//设置自动缩放
    albumnLabel->setPixmap(QPixmap(tr(":/logo")));
    textTimeLabel=new QLabel("00:00 / 00:00");
    songNameLabel=new QLabel("QQ音乐，听我想听的歌",this);
    songNameLabel->setGeometry(230,6,250,20);

    progressBar=new QSlider;//播放进度的进度条
    progressBar->setObjectName("progress");
    progressBar->setValue(0);
    progressBar->setFixedWidth(450);
    progressBar->setOrientation(Qt::Horizontal);
    progressBar->setStyleSheet(GetQss(":/qss/style"));
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateProgress()));

    QHBoxLayout *lay=new QHBoxLayout(this);
    lay->setSpacing(15);
    lay->setContentsMargins(20,10,20,0);
    lay->addWidget(leftBtn);
    lay->addWidget(playBtn);
    lay->addWidget(rightBtn);
    lay->addStretch();
    lay->addWidget(albumnLabel);
    lay->addWidget(progressBar);
    lay->addWidget(textTimeLabel);
    lay->addStretch();

    lay->addSpacing(30);
    lay->addWidget(loveBtn);
    lay->addWidget(downloadBtn);
    lay->addWidget(volumeBtn);
    lay->addWidget(lrcBtn);
    lay->addWidget(loopBtn);
    lay->addWidget(menuBtn);

    connect(volumeBtn,SIGNAL(clicked()),this,SIGNAL(ShowVolume()));
    connect(playBtn,SIGNAL(clicked()),this,SLOT(PlayBtnClicked()));

    connect(progressBar,SIGNAL(sliderReleased()),this,SLOT(ProgressChanged()));
    connect(lrcBtn,SIGNAL(clicked()),this,SIGNAL(ShowLrc()));
    connect(downloadBtn,SIGNAL(clicked()),this,SIGNAL(DownLoad()));
}

ControlWidget::~ControlWidget()
{

}
void ControlWidget::PlayBtnClicked()
{
    if(playBtn->getPicName()==":/playControl/play")
    {
        playBtn->setPicName(":/playControl/pause");
        timer->start();
    }
    else
    {
        playBtn->setPicName(":/playControl/play");
        timer->stop();
     }
    emit PlayPause();
}
void ControlWidget::RestartProgressBar()
{
       m_proPos=0;
       progressBar->setValue(0);
       timer->start(1000);
}
void ControlWidget::UpdateProgress()
{
       m_proPos++;
       if(m_proPos >= progressBar->maximum())
       {
           m_proPos=0;
           progressBar->setValue(0);
           textTimeLabel->setText("00:00 / 00:00");
           songNameLabel->setText(tr("QQ音乐，听我想听的歌"));
           playBtn->setPicName(":/playControl/play");
           update();
           timer->stop();
           return;
       }
       progressBar->setValue(m_proPos);
       int h=m_proPos/60;
       int m=m_proPos%60;
       QString str;
       int max= progressBar->maximum();
       str.sprintf("%02d:%02d / %02d:%02d",h,m,max/60,max%60);
       textTimeLabel->setText(str);
}
void ControlWidget::ProgressChanged()
{
    int pos=progressBar->value();
    m_proPos=pos;
    emit ProgressUpdate(pos);
}
