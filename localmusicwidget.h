#ifndef LOCALMUSICWIDGET_H
#define LOCALMUSICWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QFile>
#include<QAction>

#include<QMediaPlayer>
#include<QList>
typedef struct _MP3INFO //MP3信息的结构
{
    char Identify[3]; //TAG三个字母
    //这里可以用来鉴别是不是文件信息内容
    char Title[31];   //歌曲名，30个字节
    char Artist[31];  //歌手名，30个字节
    char Album[31];   //所属唱片，30个字节
    char Year[5];	  //年，4个字节
    char Comment[29]; //注释，28个字节
    unsigned char reserved;  //保留位，	1个字节
    unsigned char reserved2; //保留位，1个字节
    unsigned char reserved3; //保留位，1个字节
} MP3INFO;

class LocalMusicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LocalMusicWidget(QWidget *parent = 0);
    ~LocalMusicWidget();
    void InitWidget();
    void setStyle(const QString &style);
    bool GetAndAddInfo();
public:
    QPushButton *playAllBtn,*add;
    QLineEdit *searchEdit;
    QAction *searchAction;
    QLabel *textLabel;
    QTableWidget *tableWidget;
private:
    QString  m_Title,m_Artist,m_Album,m_Time;
   // QMediaPlayer *player;
    QMediaPlayer *player;
    QString path;//本地歌曲地址
    QList<QString>url_list;
signals:
    void ChangeSong(QString);
public slots:
    void AddSong();
    void DurationChanged();
    void DoubleClickedTable(QTableWidgetItem*);
};

#endif // LOCALMUSICWIDGET_H
