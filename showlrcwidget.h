#ifndef SHOWLRCWIDGET_H
#define SHOWLRCWIDGET_H

#include <QWidget>
#include<QNetworkRequest>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QMessageBox>
#include<QEventLoop>
#include<QLabel>
#include<QTextEdit>
#include<QRegExp>
#include<QMap>
#include<QTimer>
#include"mypushbutton.h"
#include<QTextBlock >
class ShowLrcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowLrcWidget(QWidget *parent = 0);
    ~ShowLrcWidget();
    void Mes(QString str)
    {
        QMessageBox::information(this,"search",str);
    }
    QString lrcUrl,picUrl;
    QTimer *timer;
    bool isFirstTime;
    void StartTimer();
    void onFind(QString);
    int pos;
    void ShowLrc();
    QPixmap getWebPix(QString);
    void resolve_lrc();
    void convertImage();
    void SetBackGround();
private:
    QLabel *label;
    QTextEdit *textEdit;
    MyPushButton *downBtn;
    QByteArray lrc;
    QMap<qint64,QString> lrc_map;
    QTextCursor cursor;

signals:
    void HideLrc();
public slots:
    void onTimeout();
};

#endif // SHOWLRCWIDGET_H
