#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include<QListWidget>
#include<QLabel>
#include<QListWidgetItem>
#include<QFile>
class PlayListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayListWidget(QWidget *parent = 0);
    ~PlayListWidget();
    QString GetQss(const QString &path)
    {
        QString content;
        QFile qss(path);
        qss.open(QFile::ReadOnly);
        content=qss.readAll();
        qss.close();
        return content;
    }
    void CreateAction();
    void Reset();
private:
    QListWidget *OnlineMusicList,*MyMusicList;
    QLabel *OnlineLabel,*MymusicLabel;
    QListWidgetItem *musicAction,*MVAction,*radioAction,*starAction;
    QListWidgetItem *loveAction,*listAciton,*historyAction,*PCAction,*mobileAction;
signals:
    void musicHall();
    void MV();
    void radio();
    void dreamVoice();

    void love();
    void trylisten();
    void history();
    void localMusic();
    void mobile();
public slots:
    void OnlineListChange(QListWidgetItem *);
    void MyMuscChange(QListWidgetItem *);
};

#endif // PLAYLISTWIDGET_H
