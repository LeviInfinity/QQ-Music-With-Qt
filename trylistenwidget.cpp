#include "trylistenwidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QHeaderView>
#include<QScrollBar>
TryListenWidget::TryListenWidget(QWidget *parent) : QWidget(parent)
{
    InitWidget();

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
    btnLay->addWidget(searchEdit);
    main_lay->addLayout(btnLay);
    main_lay->addWidget(tableWidget);

    setStyle(":/qss/center");
}

TryListenWidget::~TryListenWidget()
{

}
void TryListenWidget::InitWidget()
{
    textLabel=new QLabel("试听列表");//标题
    playAllBtn=new QPushButton(QIcon(":/center/play"),"播放全部");
    downLoad = new QPushButton(QIcon(":/center/download"),"下载");
    searchEdit=new QLineEdit;
    searchEdit->setFixedHeight(28);
    searchEdit->setPlaceholderText("在试听列表中搜索");
    searchAction=new QAction(QIcon(":/title/search"),tr("搜索"),this);
    searchEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    tableWidget = new QTableWidget(20,4);
    QStringList header;
    header<<"歌曲"<<"歌手"<<"专辑"<<"时长";
    tableWidget->setHorizontalHeaderLabels(header);//插入表头
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setItem(0,0,new QTableWidgetItem("半城烟沙"));
    tableWidget->setItem(0,1,new QTableWidgetItem("许嵩"));
    tableWidget->setItem(0,2,new QTableWidgetItem("半城烟沙"));
    tableWidget->setItem(0,3,new QTableWidgetItem("04：52"));
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
void TryListenWidget::setStyle(const QString &style)
{
       QFile qss(style);
       qss.open(QFile::ReadOnly);
       setStyleSheet(qss.readAll());
       qss.close();
}
