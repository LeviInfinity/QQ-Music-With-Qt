#include "titlebar.h"
#include<QHBoxLayout>
#include<QPalette>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(55);//设置标题栏高度
    QPalette pal(palette());
    pal.setColor(QPalette::Background,QColor(0,204,102));
    setPalette(pal);
    setAutoFillBackground(true);//颜色要生效必须设置这句话
    QPalette text_palette(palette());
    text_palette.setColor(QPalette::WindowText, QColor(255, 250, 250));
    setPalette(text_palette);
    //设置文本粗体
    QFont &text_font = const_cast<QFont &>(font());
    text_font.setWeight(QFont::Bold);
    //创建图标以及标题
    iconLabel=new QLabel;
    iconLabel->setFixedSize(30, 30);//设置图标大小
    iconLabel->setScaledContents(true);//设置自动缩放
    iconLabel->setPixmap(QPixmap(tr(":/logo")));
    textLabel=new QLabel(tr("QQ音乐"));

    minBtn=new MyPushButton;
    closeBtn=new MyPushButton;
   // minBtn2,*menuBtn,*skinBtn,*vipBtn,*mailBtn;
    minBtn->setPicName(tr(":/title/min"));
    closeBtn->setPicName(tr(":/title/close"));
    minBtn2=new MyPushButton;
    minBtn2->setPicName(tr(":/title/min_arrow"));
    menuBtn=new MyPushButton;
    menuBtn->setPicName(tr(":/title/menu"));
    skinBtn=new MyPushButton;
    skinBtn->setPicName(tr(":/title/skin"));
    mailBtn=new MyPushButton;
    mailBtn->setPicName(tr(":/title/mail"));
    vipBtn=new MyPushButton;
    vipBtn->setPicName(tr(":/title/vip"));
    cloudBtn=new MyPushButton;
    cloudBtn->setPicName(tr(":/title/cloud"));

    searchEdit=new QLineEdit;
    searchEdit->setStyleSheet("border-radius:14px;border:1px solid rgb(220,220,200);height:28px;width:100px;padding-left:4px;font-style:bold;");
    searchEdit->setPlaceholderText("输入你想搜索的歌曲");
    searchAction=new QAction(QIcon(":/title/search"),tr("搜索"),this);
    searchEdit->addAction(searchAction,QLineEdit::TrailingPosition);
    connect(searchAction,SIGNAL(triggered()),this,SLOT(onSearch()));
    connect(searchEdit,SIGNAL(returnPressed()),this,SLOT(onSearch()));//回车时响应搜索
    //水平布局依次添加Widget
    QHBoxLayout *hLayout=new QHBoxLayout(this);
    hLayout->addWidget(iconLabel);
    hLayout->setSpacing(1);
    hLayout->addSpacing(5);
    hLayout->addWidget(textLabel);
    hLayout->addSpacing(100);
    hLayout->addWidget(cloudBtn);
    hLayout->addSpacing(10);
    hLayout->addWidget(searchEdit);
    hLayout->addSpacing(160);
    hLayout->addWidget(vipBtn);
    hLayout->addWidget(mailBtn);
    hLayout->addWidget(skinBtn);
    hLayout->addWidget(menuBtn);
    hLayout->addSpacing(15);
    hLayout->addWidget(minBtn2);
    hLayout->addWidget(minBtn);
    hLayout->addWidget(closeBtn);
    hLayout->setContentsMargins(5, 0, 10, 0);

    connect(minBtn,SIGNAL(clicked()),this,SLOT(onClicked()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(onClicked()));

}

TitleBar::~TitleBar()
{

}
void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == minBtn)
        {
            pWindow->showMinimized();
        }
       /* else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }*/
        else if (pButton == closeBtn)
        {
            pWindow->close();

        }
    }
}
void TitleBar::mousePressEvent(QMouseEvent *event)//移动窗口
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#else
#endif
}
void TitleBar::onSearch()
{
    QString str=searchEdit->text();
    emit Search(str);
}
