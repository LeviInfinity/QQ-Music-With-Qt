#include "searchwidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QHeaderView>
#include<QScrollBar>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{
    bSearching=false;
    InitWidget();
    //request=new QNetworkRequest;
    nCount1=nCount2=0;
    loadingPicIndex=1;

    QVBoxLayout *main_lay=new QVBoxLayout(this);
    main_lay->setSpacing(10);
    main_lay->setContentsMargins(20,20,20,0);
    main_lay->addWidget(textLabel);
    QHBoxLayout *btnLay=new QHBoxLayout;
    btnLay->setSpacing(15);
    btnLay->setContentsMargins(0,0,0,0);
    btnLay->addWidget(playAllBtn);
    btnLay->addWidget(downLoad);
    btnLay->addStretch();
    main_lay->addLayout(btnLay);
    main_lay->addWidget(tableWidget);

    connect(tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(DoubleClickedTable(QTableWidgetItem*)));
    setStyle(":/qss/center");
}

SearchWidget::~SearchWidget()
{

}
void SearchWidget::InitWidget()
{
    textLabel=new QLabel("搜索结果");//标题
    playAllBtn=new QPushButton(QIcon(":/center/play"),"播放全部");
    downLoad = new QPushButton(QIcon(":/center/download"),"下载");

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
void SearchWidget::setStyle(const QString &style)
{
       QFile qss(style);
       qss.open(QFile::ReadOnly);
       setStyleSheet(qss.readAll());
       qss.close();
}
QByteArray SearchWidget::ContactHttp(QString szUrl)
{
    QUrl url(szUrl);
    QNetworkRequest request;
    QNetworkAccessManager * manager = new QNetworkAccessManager();
    request.setUrl(url);
    QNetworkReply *reply =  manager->get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();
    return reply->readAll();
}
void SearchWidget::ShowLoadingWdiget()//显示加载中界面
{
    loadingWidget=new QWidget(this);
    QPixmap pix;
    pix.load(":/center/loading1");
    loadingWidget->resize(pix.size());
    loadingLabel=new QLabel(loadingWidget);
    loadingLabel->setPixmap(QPixmap(":/center/loading1"));
    loadingWidget->setMask(QBitmap(pix.mask()));
    loadingTimer=new QTimer(loadingWidget);
    connect(loadingTimer,SIGNAL(timeout()),this,SLOT(TimerLoading()));


    QPoint pt=pos();
    loadingWidget->move(pt.x()+400,pt.y()+280);
    loadingWidget->show();
    loadingTimer->start(200);
   // loadingLabel->setGeometry(pt.x()+400,pt.y()+200,pix.width(),pix.height());

}
void SearchWidget::TimerLoading()//加载中动画的显示
{
    loadingPicIndex++;
    if(loadingPicIndex>8)
        loadingPicIndex=1;
    QString szPic;
    szPic.sprintf(":/center/loading%d",loadingPicIndex);
    loadingWidget->setMask(QBitmap(QPixmap(szPic).mask()));
    loadingLabel->setPixmap(QPixmap(szPic));

}
void SearchWidget::Clear()//清除列表
{
    tableWidget->setRowCount(0);
    tableWidget->clearContents();
    nCount1=nCount2=0;
}
void SearchWidget::Search()
{
     if(bSearching==false)//防止连续按搜索造成crash
     {
         bSearching=true;
     }
     else
         return;
     Clear();
     ShowLoadingWdiget();
     QString url = "http://tingapi.ting.baidu.com/v1/restserver/ting?from=android&version=5.6.5.0&method=baidu.ting.search.merge&format=json&query="+searchContent+"&page_no=1&page_size=50&type=-1&data_source=0&use_cluster=1";
     //QUrl url("http://tingapi.ting.baidu.com/v1/restserver/ting?from=android&version=5.6.5.0&method=baidu.ting.search.catalogSug&format=json&query="+searchContent);
     QByteArray content=ContactHttp(url);
     GetIdList(content);
   //  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRead(QNetworkReply*)));//数据传输完成信号

}
void SearchWidget::GetIdList(QByteArray byteArray)//此处仅为了获取id信息
{
   QScriptEngine engine;
   QScriptValue sc =  engine.evaluate("value=" + byteArray);
   QScriptValue res=sc.property("result");
   QScriptValue songinfo=res.property("song_info");
   QScriptValue songlist=songinfo.property("song_list");
  /* if (sc.property("song").isArray())   //解析数组
   {
      QScriptValueIterator it(sc.property("song"));
      it.next();//迭代器第一个为空
      while(it.hasNext())
      {
         QString id=it.value().property("songid").toString();//得到歌曲id后进一步获取信息

         idList[nCount1]=id;
         it.next();
         nCount1++;
       }
    }*/
    if(songlist.isArray())
    {
        QScriptValueIterator it(songlist);
        it.next();//迭代器第一个为空
        while(it.hasNext())
        {
           QString id=it.value().property("song_id").toString();//得到歌曲id后进一步获取信息

           idList[nCount1]=id;
           it.next();
           nCount1++;
         }
    }
//得到id数组后把每个id包含的信息解析出来并保存到信息的结构体数组中
   for(int i=0;i<nCount1;++i)
     GetFurtherInfo(idList[i]);
 //当所有信息全部解析完成，添加到列表中
 //且两个信息相同时再调用
   if(nCount1==nCount2)
   {
       AddToTableWidget();
       bSearching=false;
   }
}
void SearchWidget::GetFurtherInfo(QString id)//此处是为了根据id获取到专辑名以及时间
{
   QString url = "http://tingapi.ting.baidu.com/v1/restserver/ting?from=android&version=5.6.5.0&method=baidu.ting.song.play&songid="+id;
   QByteArray content=ContactHttp(url);
   readFurtherJson(content);
}

void SearchWidget::readFurtherJson(QByteArray byteArray)//解析其余信息
{
  QScriptEngine engine;
  QScriptValue sc =  engine.evaluate("value=" + byteArray);
  QScriptValue s1 =  sc.property("songinfo");//s1为songinfo的嵌套json部分

  musicList[nCount2].title=s1.property("title").toString();//标题名
  musicList[nCount2].artist=s1.property("author").toString();//歌手名
  musicList[nCount2].albumn=s1.property("album_title").toString();//专辑名
  musicList[nCount2].lrc_url=s1.property("lrclink").toString();//歌词地址
  musicList[nCount2].artist_id=s1.property("ting_uid").toString();//歌手id
  if(s1.property("pic_huge").toString().isEmpty())//封面图地址
        musicList[nCount2].pic_url=s1.property("pic_big").toString();
  else
      musicList[nCount2].pic_url=s1.property("pic_huge").toString();
  QScriptValue s2=sc.property("bitrate");
  musicList[nCount2].music_url=s2.property("file_link").toString();//歌曲播放下载地址
  musicList[nCount2].duration=s2.property("file_duration").toString();//歌曲长度

  nCount2++;
}

void SearchWidget::AddToTableWidget()
{
   for(int i=0;i<nCount1;i++)
    {
         int row=tableWidget->rowCount();//获取总行数
         tableWidget->setRowCount(row+1);//添加一行
         tableWidget->setItem(row,0,new QTableWidgetItem(musicList[i].title));//添加标题
         tableWidget->setItem(row,1,new QTableWidgetItem(musicList[i].artist));
         tableWidget->setItem(row,2,new QTableWidgetItem(musicList[i].albumn));
         int dur=musicList[i].duration.toInt();
         QString szDur;
         szDur.sprintf("%02d:%02d",dur/60,dur%60);
         tableWidget->setItem(row,3,new QTableWidgetItem(szDur));

        //下面为在歌手处添加显示歌手信息图标，所以加了一个QLineEdit控件，并添加图标
        QLineEdit *edit=new QLineEdit();
        //edit->setAttribute();
        edit->setStyleSheet("border:0px;background:transparent;");
        edit->setReadOnly(true);
        QAction* artistInfoAction=new QAction(QIcon(":/center/information"),QString::number(i),this);

        edit->addAction(artistInfoAction,QLineEdit::TrailingPosition);
        tableWidget->setCellWidget(row,1,edit);
        connect(artistInfoAction,SIGNAL(triggered()),this,SLOT(ShowArtistInfo()));
    }
   //添加完成后删除loadingwidget
   delete loadingLabel;
   loadingTimer->stop();
   delete loadingTimer;
   loadingPicIndex=1;
   delete loadingWidget;
}
void SearchWidget::ShowArtistInfo()
{
    QAction*ac = dynamic_cast<QAction*>(sender());//判断是从哪个action发送来的
    QString szNum= ac->text();
    int index=szNum.toInt();
    QString url = "http://tingapi.ting.baidu.com/v1/restserver/ting?from=android&version=5.6.5.0&method=baidu.ting.artist.getInfo&tinguid="+musicList[index].artist_id;
    QByteArray content=ContactHttp(url);
    GetArtistInfo(content);
}
void SearchWidget::GetArtistInfo(QByteArray byteArray)
{//解析json中歌手信息
    QScriptEngine engine;
    QScriptValue sc =  engine.evaluate("value=" + byteArray);
    QString picUrl=sc.property("avatar_s500").toString();
    QString name=sc.property("name").toString();
    QString stature=sc.property("stature").toString();
    QString weight=sc.property("weight").toString();
    QString birth=sc.property("birth").toString();
    QString xingzuo=sc.property("constellation").toString();
    QString country=sc.property("country").toString();
    QString intro=sc.property("intro").toString();

//将歌手信息在一个新的widget中展现出来
    QWidget *infoWidget=new QWidget;
    infoWidget->setWindowFlags(Qt::FramelessWindowHint);
    //infoWidget->resize(600,400);
    infoWidget->setStyleSheet(GetQss("introWidget.qss"));

    QLabel *name_label=new QLabel("姓名:"+name);
    QLabel *stature_label=new QLabel("身高:"+stature);
    QLabel *weight_label=new QLabel("体重:"+weight);
    QLabel *birth_label=new QLabel("生日:"+birth);
    QLabel *xingzuo_label=new QLabel("星座:"+xingzuo);
    QLabel *country_label=new QLabel("国家:"+country);
    QPushButton *quitBtn=new QPushButton("关闭");
    QTextEdit *intro_textEdit=new QTextEdit;
    intro_textEdit->setText(intro);
    intro_textEdit->setReadOnly(true);

    QLabel*picLabel=new QLabel;
    QPixmap pix=getWebPix(picUrl);
    pix=pix.scaled(300,300);
    picLabel->setPixmap(pix);
    QHBoxLayout *mainLay=new QHBoxLayout(infoWidget);
    mainLay->setContentsMargins(15,15,15,15);
    mainLay->setSpacing(30);

    QVBoxLayout *vLay=new QVBoxLayout;
    vLay->setContentsMargins(0,0,0,0);
    vLay->setSpacing(30);
    vLay->addWidget(name_label);
    vLay->addWidget(stature_label);
    vLay->addWidget(weight_label);
    vLay->addWidget(birth_label);
    vLay->addWidget(xingzuo_label);
    vLay->addWidget(country_label);
    vLay->addStretch();
    vLay->addWidget(quitBtn);
    connect(quitBtn,SIGNAL(clicked()),infoWidget,SLOT(close()));

    mainLay->addWidget(picLabel,0,Qt::AlignTop);
    mainLay->addLayout(vLay);
    mainLay->addWidget(intro_textEdit);
    intro_textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{background:rgb(180,180,180); width:12px;}\
                                                   QScrollBar::handle:vertical {\
                                                   background-color:rgb(150,220,150);\
                                                   border-radius: 6px;\
                                                   min-height: 5px;\
                                                   }");
    infoWidget->show();
}
QPixmap SearchWidget::getWebPix(QString picUrl)
{
        QByteArray jpegData = ContactHttp(picUrl);
        QPixmap pixmap;
        pixmap.loadFromData(jpegData);

        return pixmap;
}
void SearchWidget::DoubleClickedTable(QTableWidgetItem* item)
{
    int row = tableWidget->currentRow();//获得当前行号

    QString title= tableWidget->selectedItems().at(0)->text();
    QString artist= tableWidget->selectedItems().at(1)->text();
    QString time= tableWidget->selectedItems().at(3)->text();

    int i=0;
    for(;i<nCount1;i++)
        if(title==musicList[i].title)
            break;
    QString url=musicList[i].music_url;
    QString picUrl=musicList[i].pic_url;
    //发送更换播放信号
    QString seprator="|";
    QString type="Web";
    QString param=url+ seprator +type+seprator+title+" - "+artist+seprator+time+seprator+picUrl+seprator+musicList[i].lrc_url;
    emit ChangeSong(param);

    //QMessageBox::information(this,"",param);
}
