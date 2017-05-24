#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include<QPushButton>
#include <QPainter>
#include <QMouseEvent>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget *parent = 0);
    ~MyPushButton();

    void loadPixmap(QString pic_name);
    void enterEvent(QEvent *);//鼠标悬浮
    void leaveEvent(QEvent *); //鼠标离开
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void setPicName(QString);
    QString getPicName();
private:
    enum ButtonStatus{NORMAL, ENTER, PRESS}; //枚举按钮的几种状态 (正常、悬浮、按下)
    ButtonStatus status;
    QPixmap pixmap;
    bool mouse_press; //按钮左键是否按下
    QString pic_name;

};

#endif // MYPUSHBUTTON_H
