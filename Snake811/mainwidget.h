#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QKeyEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QSound>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE
//0.蛇的表示
//  QList

//1.按键处理事件
//2.使用定时器
//  2.1关联信号
//  2.2启动定时器
//  2.3实现函数
//3.渲染
typedef enum Direct{
    DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN
}dir_t;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void addTop();
    void addDown();
    void addRight();
    void addLeft();
    void deleteNode();

    void addNewReword();
    bool checkContact();

private slots:
    void updateSnake();

private:
    Ui::MainWidget *ui;
    int moveFlag = DIR_UP;
    bool gameStart = false;

    //定时器
    QTimer *timer; int time = 100;

    //蛇
    QList <QRectF> snake;

    //小方块的宽高
    int nodeWidth = 20;
    int nodeHeight = 20;

    QRectF rewardNode;

    QSound *sound, *eatsound;
};
#endif // MAINWIDGET_H
