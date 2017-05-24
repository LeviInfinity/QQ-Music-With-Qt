#ifndef TRYLISTENWIDGET_H
#define TRYLISTENWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QFile>
#include<QAction>
class TryListenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TryListenWidget(QWidget *parent = 0);
    ~TryListenWidget();
    void InitWidget();
    void setStyle(const QString &style);
public:
    QPushButton *playAllBtn,*downLoad;
    QLineEdit *searchEdit;
    QAction *searchAction;
    QLabel *textLabel;
    QTableWidget *tableWidget;

signals:

public slots:
};

#endif // TryListenWidget_H
