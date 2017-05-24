#include "playlistwidget.h"
#include<QVBoxLayout>
#include<QPalette>
#include<QMessageBox>
PlayListWidget::PlayListWidget(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(200);
    setAutoFillBackground(true);
    QPalette text_palette(palette());
    text_palette.setColor(QPalette::WindowText, QColor(150,150,150));
    setPalette(text_palette);
    QFont &text_font = const_cast<QFont &>(font());
    text_font.setWeight(QFont::Bold);

    OnlineMusicList=new QListWidget;
    MyMusicList=new QListWidget;
    OnlineLabel=new QLabel(tr("在  线  音  乐    "));
    MymusicLabel=new QLabel(tr("我  的  音  乐    "));

    CreateAction();

    QVBoxLayout *lay=new QVBoxLayout(this);
    lay->setSpacing(10);
    lay->setContentsMargins(20,10,0,0);
    lay->addWidget(OnlineLabel,0,Qt::AlignHCenter);
    lay->addWidget(OnlineMusicList);
    lay->addWidget(MymusicLabel,0,Qt::AlignHCenter);
    lay->addWidget(MyMusicList);

    //连接点击listwidget的点击信号
   connect(OnlineMusicList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(OnlineListChange(QListWidgetItem *)));
   connect(MyMusicList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(MyMuscChange(QListWidgetItem *)));


}

PlayListWidget::~PlayListWidget()
{

}
void PlayListWidget::CreateAction()
{
    musicAction=new QListWidgetItem(QIcon(":/item/music"),tr("音乐馆"));
    MVAction=new QListWidgetItem(QIcon(":/item/video"),tr("MV"));
    radioAction=new QListWidgetItem(QIcon(":/item/heart"),tr("电台"));
    starAction=new QListWidgetItem(QIcon(":/item/star"),tr("梦想的声音"));
    OnlineMusicList->addItem(musicAction);
    OnlineMusicList->addItem(MVAction);
    OnlineMusicList->addItem(radioAction);
    OnlineMusicList->addItem(starAction);
    OnlineMusicList->setStyleSheet(GetQss(":/qss/list"));

    loveAction=new QListWidgetItem(QIcon(":/item/love"),tr("我喜欢"));
    listAciton=new QListWidgetItem(QIcon(":/item/menu"),tr("试听列表"));
    historyAction=new QListWidgetItem(QIcon(":/item/history"),tr("播放历史"));
    PCAction=new QListWidgetItem(QIcon(":/item/computer"),tr("本地音乐"));
    mobileAction=new QListWidgetItem(QIcon(":/item/mobile"),tr("传歌到设备"));
    MyMusicList->addItem(loveAction);
    MyMusicList->addItem(listAciton);
    MyMusicList->addItem(historyAction);
    MyMusicList->addItem(PCAction);
    MyMusicList->addItem(mobileAction);

    MyMusicList->setStyleSheet(GetQss(":/qss/list"));
}
//当选择某一项时。由于有两个list不能做到互斥。所以用Reset函数先全部不选中。在将对应的一个项选中
void PlayListWidget::OnlineListChange(QListWidgetItem *pItem)
{
   Reset();
   if(pItem==musicAction)
    {
        musicAction->setSelected(true);
        emit musicHall();
   }
   if(pItem==MVAction)
   {
       MVAction->setSelected(true);
       emit MV();
   }
   if(pItem==radioAction)
   {
       radioAction->setSelected(true);
        emit radio();
   }
   if(pItem==starAction)
   {
       starAction->setSelected(true);
       emit dreamVoice();
   }

}
void PlayListWidget::MyMuscChange(QListWidgetItem *pItem)
{
    Reset();
    if(pItem==loveAction)
    {
        loveAction->setSelected(true);
         emit love();
    }
    if(pItem==listAciton)
    {
        listAciton->setSelected(true);
         emit trylisten();
    }
    if(pItem==historyAction)
    {
        historyAction->setSelected(true);
         emit history();
    }
    if(pItem==PCAction)
    {
        PCAction->setSelected(true);
         emit localMusic();
    }
    if(pItem==mobileAction)
    {
        mobileAction->setSelected(true);
        emit mobile();
    }
}
void PlayListWidget::Reset()
{
   QListWidgetItem *list[]={musicAction,MVAction,radioAction,starAction,loveAction,listAciton,historyAction,PCAction,mobileAction};
   for(int i=0;i<9;++i)
       list[i]->setSelected(false);
}

