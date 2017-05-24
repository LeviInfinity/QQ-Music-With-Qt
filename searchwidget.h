#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QFile>
#include<QNetworkRequest>
#include<QNetworkAccessManager>
#include<QNetworkReply>

#include<QAction>
#include<QLineEdit>
/*#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>*/
#include <QtScript>
#include<QEventLoop>
#include<QMessageBox>
#include<QTextEdit>
#include<QBitmap>
typedef struct
{
    QString albumn;
    QString duration;
    QString music_url;
    QString lrc_url;
    QString pic_url;
    QString title;
    QString artist;
    QString artist_id;
}MUSICINFO;
class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();
    void InitWidget();
    void setStyle(const QString &style);
    void Mes(QString str)
    {
        QMessageBox::information(this,"search",str);
    }
public:
    QPushButton *playAllBtn,*downLoad;
    QLabel *textLabel;
    QTableWidget *tableWidget;
   // QLabel *picLabel;
    QString searchContent;
    void Clear();
    void Search();
    QByteArray ContactHttp(QString);
    void GetIdList(QByteArray);
    void GetFurtherInfo(QString id);
    void readFurtherJson(QByteArray);
    void GetArtistInfo(QByteArray);
    QPixmap getWebPix(QString picUrl);
    MUSICINFO musicList[100];
    QString idList[100];
    int nCount1,nCount2;

    void AddToTableWidget();
    QString GetQss(const QString &path)
    {
        QString content;
        QFile qss(path);
        qss.open(QFile::ReadOnly);
        content=qss.readAll();
        qss.close();
        return content;
    }

    QWidget *loadingWidget;
    void ShowLoadingWdiget();
    QLabel *loadingLabel;
    QTimer *loadingTimer;
    int loadingPicIndex;
private:
    bool bSearching;
signals:
    void ChangeSong(QString);
public slots:
      void DoubleClickedTable(QTableWidgetItem*);
      void ShowArtistInfo();
      void TimerLoading();
};

#endif // SEARCHWIDGET_H
