#include "localmusicwidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QHeaderView>
#include<QScrollBar>
#include<QFileDialog>
#include<QMessageBox>
LocalMusicWidget::LocalMusicWidget(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    player=new QMediaPlayer;
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(DurationChanged()));//为得到总时间

    QVBoxLayout *main_lay=new QVBoxLayout(this);
    main_lay->setSpacing(10);
    main_lay->setContentsMargins(20,20,20,0);
    main_lay->addWidget(textLabel);
    QHBoxLayout *btnLay=new QHBoxLayout;
    btnLay->setSpacing(15);
    btnLay->setContentsMargins(0,0,0,0);
    btnLay->addWidget(playAllBtn);
    btnLay->addWidget(add);
    btnLay->addStretch();
    btnLay->addWidget(searchEdit);
    main_lay->addLayout(btnLay);
    main_lay->addWidget(tableWidget);

    connect(add,SIGNAL(clicked()),this,SLOT(AddSong()));//点击添加按钮
    connect(tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(DoubleClickedTable(QTableWidgetItem*)));
    setStyle(":/qss/center");
}

LocalMusicWidget::~LocalMusicWidget()
{

}
void LocalMusicWidget::InitWidget()
{
    textLabel=new QLabel("本地歌曲");//标题
    playAllBtn=new QPushButton(QIcon(":/center/play"),"播放全部");
    add = new QPushButton(QIcon(":/center/add"),"添加");
    searchEdit=new QLineEdit;
    searchEdit->setFixedHeight(28);
    searchEdit->setPlaceholderText("在本地列表中搜索");
    searchAction=new QAction(QIcon(":/title/search"),tr("搜索"),this);
    searchEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    tableWidget = new QTableWidget(0,4);
    QStringList header;
    header<<"歌曲"<<"歌手"<<"专辑"<<"时长";
    tableWidget->setHorizontalHeaderLabels(header);//插入表头
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中一行
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    tableWidget->setColumnWidth(0,300);
    tableWidget->setColumnWidth(1,180);
    tableWidget->setColumnWidth(2,180);
    tableWidget->setColumnWidth(3,100);

    //tableWidget->setRowHeight(0,60);
    tableWidget->setStyleSheet("QTableWidget{\
                                    font-size:13px; \
                                    font-weight:300;\
                                    color: rgb(60,60,60);\
                                    gridline-color: white;\
                                    alternate-background: #3A3A3C;\
                                    selection-background-color: rgb(210,210,210);\
                                    selection-color:rgb(60,60,60);\
                                    border: 2px groove gray;\
                                    border-radius: 0px;\
                                    border:1px solid rgb(220,220,220);}\
                               }");
    tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:rgb(230,230,230); width:12px;}\
                                                    QScrollBar::handle:vertical {\
                                                    background-color:rgb(200,200,200);\
                                                    border-radius: 6px;\
                                                    min-height: 5px;\
                                                    }\
                                                     ");
}
void LocalMusicWidget::setStyle(const QString &style)
{
       QFile qss(style);
       qss.open(QFile::ReadOnly);
       setStyleSheet(qss.readAll());
       qss.close();
}
void LocalMusicWidget::DurationChanged()//获取时间
{//由于时间必须要在durationChanged信号中才能获取。所以干脆先获得时间，再获取其他信息
   qint64 t=player->duration();
   if(t<10000)//小于10s的过滤掉
       return;
   int h=(t/1024+3)/60;
   int m=(t/1024+3)%60;
   m_Time.sprintf("%02d:%02d",h,m);//格式化时间
   //QMessageBox::information(this,"",m_Time);
   if(GetAndAddInfo())
   {
       QString seprator="|";
       QString type="Local";
       QString param=path + seprator + type + seprator + m_Title + " - " + m_Artist + seprator + m_Time+seprator+"default"+seprator+"NULL";
      //向主窗口widget发送切换歌曲的信号   //传递格式为url(本机路径或网络路径)|类型(Local或Web)|标题|时间(xx:xx格式)|pic地址|lrc地址
       emit ChangeSong(param);
   }
}
void LocalMusicWidget::AddSong()
{
      path=QFileDialog::getOpenFileName(this,"添加","/","Mp3 files(*.mp3)\nAll files(*.*)");
      player->setMedia(QUrl::fromLocalFile(path));//为得到时间所用
      url_list.append(path);
}

bool LocalMusicWidget::GetAndAddInfo()
{
   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    FILE * fp;
    unsigned char mp3tag[128] = { 0 };
    MP3INFO mp3info;
    fp = fopen(path.toLocal8Bit().data(), "rb");
    if (NULL == fp)
    {
        QMessageBox::information(this,"提示","打开文件失败");
        return false;
    }
    fseek(fp, -128, SEEK_END);
    fread(&mp3tag, 1, 128, fp);
    if (!((mp3tag[0] == 'T' || mp3tag[0] == 't')
        && (mp3tag[1] == 'A' || mp3tag[1] == 'a')
        && (mp3tag[2] == 'G' || mp3tag[0] == 'g')))
    {
        printf("mp3 file is error!!");
        fclose(fp);
        QMessageBox::information(this,"提示","解析当前文件失败");
        return false;
    }
    //获取Mp3信息
    memcpy((void *)mp3info.Identify, mp3tag, 3); //获得tag
    memcpy((void *)mp3info.Title, mp3tag + 3, 30); //获得歌名
    memcpy((void *)mp3info.Artist, mp3tag + 33, 30); //获得作者
    memcpy((void *)mp3info.Album, mp3tag + 63, 30); //获得唱片名
    memcpy((void *)mp3info.Year, mp3tag + 93, 4); //获得年
    memcpy((void *)mp3info.Comment, mp3tag + 97, 28); //获得注释
    memcpy((void *)&mp3info.reserved, mp3tag + 125, 1); //获得保留
    memcpy((void *)&mp3info.reserved2, mp3tag + 126, 1);
    memcpy((void *)&mp3info.reserved3, mp3tag + 127, 1);
    fclose(fp);
    //此处我们只需要歌曲名，歌首，专辑
    m_Title=QString::fromLocal8Bit((char*)mp3info.Title);
    m_Artist=QString::fromLocal8Bit((char*)mp3info.Artist);
    m_Album = QString::fromLocal8Bit((char*)mp3info.Album);

    int row=tableWidget->rowCount();//获取总行数
    tableWidget->setRowCount(row+1);//添加一行
    tableWidget->setItem(row,0,new QTableWidgetItem(m_Title));//添加标题
    tableWidget->setItem(row,1,new QTableWidgetItem(m_Artist));//添加歌手名
    tableWidget->setItem(row,2,new QTableWidgetItem(m_Album));//添加专辑名
    tableWidget->setItem(row,3,new QTableWidgetItem(m_Time));//添加时间
    //QMessageBox::information(this,"ADD info",m_Time);
    return true;
}
void LocalMusicWidget::DoubleClickedTable(QTableWidgetItem* item)
{
    QString s=item->text();
    int row = tableWidget->currentRow();//获得当前行号
    QString url=url_list.at(row);//得到绝对地址

    QString title= tableWidget->selectedItems().at(0)->text();
    QString artist= tableWidget->selectedItems().at(1)->text();
    QString time= tableWidget->selectedItems().at(3)->text();
    //发送更换播放信号
    QString seprator="|";
    QString type="Local";
     //传递格式为url(本机路径或网络路径)|类型(Local或Web)|标题|时间(xx:xx格式)|pic地址|lrc地址
    QString param=url+ seprator +type+seprator+title+" - "+artist+seprator+time+seprator+"default"+seprator+"NULL";
    emit ChangeSong(param);

    //QMessageBox::information(this,"",param);
}
