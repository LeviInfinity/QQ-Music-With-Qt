#include "showlrcwidget.h"
#include<QHBoxLayout>
#include<QScrollBar>
#include"cv.h"
#include"highgui.h"
ShowLrcWidget::ShowLrcWidget(QWidget *parent) : QWidget(parent)
{
    label=new QLabel;
    textEdit=new QTextEdit;
    downBtn=new MyPushButton;
    downBtn->setPicName(tr(":/playControl/down"));
    timer=new QTimer(this);
    textEdit->setReadOnly(true);
    textEdit->setAlignment(Qt::AlignCenter );
    textEdit->setStyleSheet("border:0px;color:rgb(230,230,230); font: 75 16pt \"华文行楷\";font-size:18px;font-weight:300;background:rgba(100,100,100,128);");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{background:rgb(180,180,180); width:12px;}\
                                                   QScrollBar::handle:vertical {\
                                                   background-color:rgb(150,220,150);\
                                                   border-radius: 6px;\
                                                   min-height: 5px;\
                                                   }\
                                                    ");
    QHBoxLayout *layout=new QHBoxLayout(this);
    layout->setContentsMargins(0,6,6,6);
     layout->addWidget(downBtn,0,Qt::AlignTop);
    layout->addSpacing(20);
    layout->addWidget(label,0,Qt::AlignVCenter);
    layout->addSpacing(20);
    layout->addWidget(textEdit);

    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    connect(downBtn,SIGNAL(clicked()),this,SIGNAL(HideLrc()));
}

ShowLrcWidget::~ShowLrcWidget()
{

}
void ShowLrcWidget::ShowLrc()
{
    QUrl url(lrcUrl);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();
    lrc = reply->readAll();

    resolve_lrc();//解析lrc
    isFirstTime=true;
    QPixmap pixmap=getWebPix(picUrl);
    QPixmap scaledPixmap = pixmap.scaled(400,400);
    label->setPixmap(scaledPixmap);

}
void ShowLrcWidget::resolve_lrc()
{
    textEdit->setText("");
    QStringList lines=QString(lrc).split('\n');
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    foreach(QString oneline, lines)
    {
            QString temp = oneline;
            temp.replace(rx, "");//用空字符串替换正则表达式中所匹配的地方,这样就获得了歌词文本
            textEdit->append(temp);//此时添加的是不带时间标签的
            // 然后依次获取当前行中的所有时间标签，并分别与歌词文本存入QMap中
            //indexIn()为返回第一个匹配的位置，如果返回为-1，则表示没有匹配成功
            //正常情况下pos后面应该对应的是歌词文件
            int pos = rx.indexIn(oneline, 0);
            while (pos != -1)
            { //表示匹配成功
                 QString cap = rx.cap(0);//返回第0个表达式匹配的内容
                // 将时间标签转换为时间数值，以毫秒为单位
                 QRegExp regexp;
                 regexp.setPattern("\\d{2}(?=:)");
                 regexp.indexIn(cap);
                 int minute = regexp.cap(0).toInt();
                 regexp.setPattern("\\d{2}(?=\\.)");
                 regexp.indexIn(cap);
                 int second = regexp.cap(0).toInt();
                 regexp.setPattern("\\d{2}(?=\\])");
                 regexp.indexIn(cap);
                 int millisecond = regexp.cap(0).toInt();
                 qint64 totalTime = minute * 60000 + second * 1000;
                // 插入到lrc_map中
                 lrc_map.insert(totalTime, temp);
                 pos += rx.matchedLength();
                 pos = rx.indexIn(oneline, pos);//匹配全部
            }
        }
    //由于是一行一行的添加，所以光标初始是在末尾。所以此处先把光标移到起始位置
    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);
    textEdit->setTextCursor(cursor);
}
QPixmap ShowLrcWidget::getWebPix(QString picUrl)
{

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
        pixmap.save("./source.jpg");//将得到的封面保存以用来下面的convert函数进行处理，处理完成后再setbackground设为背景

        convertImage();
        SetBackGround();
        return pixmap;

}

void ShowLrcWidget::SetBackGround()
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("./res.png")));
    this->setPalette(palette);
}
void ShowLrcWidget::convertImage()//转换为高斯模糊图像
{
    IplImage *img = cvLoadImage("./source.jpg");

    IplImage *t = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvSmooth(img, t, CV_GAUSSIAN, 161, 161);
    cvWaitKey(500);
    cvSaveImage("./res.png", t);
    cvReleaseImage(&t);
    cvReleaseImage(&img);
}
void ShowLrcWidget::StartTimer()
{
    pos=0;
    timer->start(1000);
}
void ShowLrcWidget::onTimeout()
{
     pos++;
     static int cursorPos=1;
     QMap<qint64,QString>::iterator it;//遍历map
     for (it = lrc_map.begin();it != lrc_map.end(); ++it )
     {
            if(it.key()==pos*1000)
            {
                QString str=it.value();              
                onFind(str);

               cursor = textEdit->textCursor();
                //下面是为了保证当前唱的这句歌词保持在中央位置。方法。由于歌词显示一共24行，所以初始是从1开始，当到12行时，也就是中间位置
                //需要开始一行行的往下移动。所以在12行时，移动到末尾，也就是24行处，之后就每次移动一行就可以了
                if(cursorPos!=-1)
               {
                    cursorPos++;
                    if(cursorPos>=12)
                    {
                            cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,12);//参数3是移动的次数
                            textEdit->setTextCursor(cursor);
                            cursorPos=-1;
                            break;
                    }
               }
               cursor.movePosition(QTextCursor::Down);
               textEdit->setTextCursor(cursor);
               break;

            }
     }
}
void ShowLrcWidget::onFind(QString searchString)
{
    QTextDocument *document = textEdit->document();
    bool found = false;
    if (isFirstTime == false)
        document->undo();
    if (searchString == "")
        return;
    else {
        QTextCursor highlightCursor(document);
         QTextCursor cursor(document);
        //***************开始***************
        cursor.beginEditBlock();
        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setForeground(QColor(100,250,100));
        while (!highlightCursor.isNull() && !highlightCursor.atEnd())
        {
            highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);
            if (!highlightCursor.isNull())
            {
                found = true;
                highlightCursor.mergeCharFormat(colorFormat);
                break;
            }
        }
        cursor.endEditBlock();
        isFirstTime = false;
    }

}
