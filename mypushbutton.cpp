#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent):QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
}

MyPushButton::~MyPushButton()
{

}
void MyPushButton::setPicName(QString pic_name)
{
    this->pic_name = pic_name;
    setFixedSize(QPixmap(pic_name).size());
}
QString MyPushButton::getPicName()
{
    return pic_name;
}
void MyPushButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}

void MyPushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}
void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        {
            mouse_press = true;
            status = PRESS;
            update();
        }
}
void MyPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouse_press)
        {
            mouse_press = false;
            status = ENTER;
            update();
            emit clicked();//此处表示mouseReleaseEvent执行后发出clicked信号
        }
}
void MyPushButton::paintEvent(QPaintEvent *)
{
        QPainter painter(this);
        QPixmap pixmap;
        switch(status)
        {
        case NORMAL:
            {
                pixmap.load(pic_name);
                break;
            }
        case ENTER:
            {
                pixmap.load(pic_name+"_hover" );
               /* QPen pen(Qt::NoBrush, 1);
                painter.setPen(pen);
                QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
                linear.setColorAt(0, QColor(230, 230, 230, 0));
                linear.setColorAt(0.5, QColor(230, 230, 230, 100));
                linear.setColorAt(1, QColor(230, 230, 230, 150));
                painter.setBrush(linear);
                painter.drawRect(rect());*/
                break;
            }
        case PRESS:
            {
                 pixmap.load(pic_name+"_click");
                break;
            }
        default:
            pixmap.load(pic_name);
        }

        painter.drawPixmap(rect(), pixmap);
}


