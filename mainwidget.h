#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include"titlebar.h"
#include"controlwidget.h"
#include"playlistwidget.h"
#include"trylistenwidget.h"
#include"mylovewidget.h"
#include"playhistorywidget.h"
#include"localmusicwidget.h"
#include"searchwidget.h"
#include"showlrcwidget.h"
#include<QFile>
#include<QApplication>
#include<QSlider>
#include<QMouseEvent>
#include<QStackedWidget>

#include<QMediaPlayer>
#include<QMediaPlaylist>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
     void setStyle(const QString &style);
     void mousePressEvent(QMouseEvent *);
     void InitWidget();
     void InitLayout();

     QPixmap getWebPix(QString);
private:
    TitleBar *titleWidget;//标题栏
    ControlWidget *controlWidget;//下方的控制
    PlayListWidget *playlistWidget;//左边的列表
    QSlider *volumeBar;//音量
    TryListenWidget *trylistenWiidget;//试听
    MyLoveWidget *myloveWidget;//我喜欢
    PlayHistoryWidget *playhistoryWidget;//播放历史
    LocalMusicWidget *localmusicWidget;//本地音乐
    SearchWidget *searchWidget;//搜索界面
    ShowLrcWidget *showlrcWidget;
    QStackedWidget *stackWidget;


    //QMediaPlaylist *playlist;//
    QMediaPlayer *player;
    bool bPlaying;
    int volume;
    QStringList paramList;
    QString lrcUrl,picUrl;//由于这两个地址需要传递给歌词widget，所以作为成员变量

    QString DownLoadPath;
    int prevWidgetIndex;//点击显示歌词时当前是那个界面，隐藏歌词界面后还显示这个界面
public slots:
    //controlWidget中的槽
    void ShowVolume();
    void ChangeSong(QString);//用户换歌的槽
    void PlayPause();
    void ChangeProgress(int);
    void showLrc();
    void DownLoad();

    //用于切换当前显示的widget槽
    void ChangeMylove();
    void ChangeTryListen();
    void ChangePlayHistory();
    void ChangeLocalMusic();

    //title中的槽
    void onSearch(QString);

    void volumeChanged(int);//调整音量
    void onDownload(QNetworkReply *reply);//下载歌曲
    void HideLrc();//隐藏歌词界面

};

#endif // MAINWIDGET_H
