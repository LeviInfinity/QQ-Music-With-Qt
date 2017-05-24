#ifndef PLAYHISTORY_H
#define PLAYHISTORY_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QFile>
#include<QAction>
class PlayHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayHistoryWidget(QWidget *parent = 0);
    ~PlayHistoryWidget();
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

#endif // PLAYHISTORY_H
