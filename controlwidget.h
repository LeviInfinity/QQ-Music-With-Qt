#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include"mypushbutton.h"
#include<QSlider>
#include<QLabel>
#include<QFile>
#include<QCheckBox>
#include<QTimer>
class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();
    QString GetQss(const QString &path)
    {
        QString content;
        QFile qss(path);
        qss.open(QFile::ReadOnly);
        content=qss.readAll();
        qss.close();
        return content;
    }
    void RestartProgressBar();
    int m_proPos;
    QTimer *timer;

public:
    MyPushButton *leftBtn,*playBtn,*rightBtn,*downloadBtn,*playModeBtn,*volumeBtn,*loopBtn,*menuBtn,*lrcBtn;
    QCheckBox *loveBtn;
    QSlider *progressBar;
    QLabel *albumnLabel;
    QLabel *textTimeLabel,*songNameLabel;

signals:
    void ShowVolume();
    void PlayPause();
    void ProgressUpdate(int);
    void ShowLrc();
    void DownLoad();
public slots:
    void PlayBtnClicked();
    void UpdateProgress();
    void ProgressChanged();

};

#endif // CONTROLWIDGET_H
