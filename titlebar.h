#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include"mypushbutton.h"
#include<QLabel>
#include<QMouseEvent>
#include<QToolButton>
#include<QLineEdit>
#include<QAction>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0);
    ~TitleBar();
    void mousePressEvent(QMouseEvent *);

private:
    MyPushButton *minBtn,*closeBtn;
    MyPushButton*minBtn2,*menuBtn,*skinBtn,*vipBtn,*mailBtn,*cloudBtn;
    QLabel *iconLabel,*textLabel;
    QAction *searchAction;
    QLineEdit *searchEdit;
signals:
    void Search(QString);
public slots:
    void onClicked();
    void onSearch();
};

#endif // TITLEBAR_H
