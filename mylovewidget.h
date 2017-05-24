#ifndef MYLOVEWIDGET_H
#define MYLOVEWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QFile>
#include<QAction>
class MyLoveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyLoveWidget(QWidget *parent = 0);
    ~MyLoveWidget();
    void InitWidget();
    void setStyle(const QString &style);
public:
    QPushButton *playAllBtn,*downLoad;
    QLineEdit *searchEdit;
    QAction *searchAction;
    QLabel *textLabel;
    QTableWidget *tableWidget;
    QLabel *picLabel;
signals:

public slots:
};

#endif // MYLOVEWIDGET_H
