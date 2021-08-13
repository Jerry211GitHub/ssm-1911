#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QPainter>
#include <QDebug>



static qint64 TIMECount=0 ;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    setFixedSize(800,720);

    //使用定时器
    timer = new QTimer(this);

    //定时器的一个超时timeout信号
    //信号槽 （信号和对应的函数关联起来）
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSnake()));

    //初如化蛇身
    QRectF rect(200, 180, nodeWidth, nodeHeight);
    snake.append(rect);

    addTop();
    addTop();

    //初始化奖品
    addNewReword();

    sound = new QSound("./res/7895.wav", this);
    eatsound = new QSound("./res/7896.wav", this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

//控制方向
void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if(moveFlag != DIR_DOWN){
            moveFlag = DIR_UP;
        }
        break;
    case Qt::Key_Down:
        if(moveFlag != DIR_UP)
        {
            moveFlag = DIR_DOWN;
        }
        break;
    case Qt::Key_Right:
        if(moveFlag != DIR_LEFT)
        {
            moveFlag = DIR_RIGHT;
        }
        break;
    case Qt::Key_Left:
        if(moveFlag != DIR_RIGHT)
        {
            moveFlag = DIR_LEFT;
        }
        break;
    case Qt::Key_Space:
        if(gameStart == false)
        {
            gameStart = true;
            timer->start(time);
            sound->play();
            sound->setLoops(-1);  //无限循环
        }
        else
        {
            gameStart = false;
            timer->stop();
            sound->stop();
        }
        break;
    default:
        break;
    }
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen, pen2;
    QBrush brush, brush2;

    QPixmap pix;
    pix.load("./res/back.jpg");
    //qDebug() << pix.width();
    //painter.drawPixmap(0,0,pix.width(),pix.height(),pix);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画蛇准备工作
    pen.setColor(Qt::black);
    pen2.setColor(Qt::black);
    brush.setColor(Qt::darkMagenta);
    brush.setStyle(Qt::SolidPattern);

    brush2.setColor(Qt::darkYellow);
    brush2.setStyle(Qt::SolidPattern);

    //画蛇身
    painter.setPen(pen);
    painter.setBrush(brush);

    for(int i = 0; i< snake.length(); i++)
    {
        if(i == 0)
            painter.setBrush(brush2);
        else
            painter.setBrush(brush);

       // painter.drawRect(snake[i]);
        painter.drawEllipse(snake[i]);
    }

    QFont font("方正舒体",30, QFont::ExtraLight,false);
    painter.setFont(font);
    QString len = QString::number(snake.length());
    painter.drawText(310,35, len);

    QString timexx = QString::number((TIMECount*time)/1000);
    painter.drawText(120,35, timexx);

    //画蛇眼睛
    pen2.setWidth(5);painter.setPen(pen2);
    painter.drawPoint(snake[0].x()+10,snake[0].y() + 10);

    //画奖品
    pen.setColor(Qt::red);
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(rewardNode);


    if(checkContact())
    {
        pen.setColor(Qt::black);
        painter.setPen(pen);
        QFont font("方正舒体",30, QFont::ExtraLight,false);
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,
                         (this->height()-30)/2,
                         QString("GAME OVER!"));
        timer->stop();
    }

    QWidget::paintEvent(event);
}

void MainWidget::updateSnake()
{
    TIMECount ++;
    int count = 1;
    if(snake[0].intersects(rewardNode)){
        count ++;
        addNewReword();
        eatsound->play();
    }
    while(count--){
        switch (moveFlag)
        {
        case DIR_UP:
            //在顶部加一个小方块，底部减一个小方块
            addTop();
            break;
        case DIR_DOWN:
            addDown();
            break;
        case DIR_LEFT:
            addLeft();
            break;
        case DIR_RIGHT:
            addRight();
            break;
        default:
            break;
        }
    }

    deleteNode();
    update();
}

void MainWidget::deleteNode()
{
    snake.removeLast();
}

void MainWidget::addNewReword()
{
    rewardNode = QRectF(qrand()%(this->width()/20)*20,
                        qrand()%(this->height()/20)*20,
                        nodeWidth,nodeHeight);
}

bool MainWidget::checkContact()
{
    for (int i=0; i<snake.length();i++ ) {
        for (int j=i+1; j<snake.length(); j++ ) {
            if(snake[i] == snake[j])
            {
                return true;
            }
        }
    }
    return false;
}

//蛇向上移动
void MainWidget::addTop(){
    QPointF leftTop;
    QPointF rightBottom;

    if(snake[0].y() - nodeHeight < 0){
//        leftTop = QPointF(snake[0].x(), this->height()- nodeHeight);
//        rightBottom = QPointF(snake[0].x()+nodeWidth, this->height());

        leftTop = QPointF(snake[0].x() +nodeWidth, snake[0].y());
        rightBottom = QPointF(snake[0].x()+2*nodeWidth, nodeHeight);

        moveFlag = DIR_RIGHT;
        snake.insert(0,QRectF(leftTop, rightBottom));
    }else
    {

        leftTop = QPointF(snake[0].x(), snake[0].y()- nodeHeight);
        rightBottom = snake[0].topRight();
        snake.insert(0,QRectF(leftTop, rightBottom));
    }

}
//蛇向下移动
void MainWidget::addDown(){
    QPointF leftTop;
    QPointF rightBottom;


    if(snake[0].y() + 2*nodeHeight > this->height()){
//       leftTop = QPointF(snake[0].x(), 0);
//       rightBottom = QPointF(snake[0].x()+nodeWidth, nodeHeight);

        leftTop = QPointF((snake[0].x()-nodeWidth) > 0 ? (snake[0].x()-nodeWidth) : 0, snake[0].y());
        rightBottom = QPointF(snake[0].x(), snake[0].y());

        moveFlag = DIR_LEFT;
        snake.insert(0,QRectF(leftTop, rightBottom));
    }else
    {

        leftTop = snake[0].bottomLeft();
        rightBottom = snake[0].bottomRight()+ QPointF(0,nodeHeight);
        snake.insert(0,QRectF(leftTop, rightBottom));
    }
    //snake.insert(0,QRectF(leftTop, rightBottom));
}
//蛇向右移动
void MainWidget::addRight(){
    QPointF leftTop;
    QPointF rightBottom;
    if(snake[0].x() + nodeWidth > this->width())
    {
//        leftTop = QPointF(0, snake[0].y());
//        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);

        leftTop = QPointF(snake[0].x()-nodeWidth, snake[0].y()+nodeHeight);
        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);

        moveFlag = DIR_DOWN;
        snake.insert(0,QRectF(leftTop, rightBottom));
    }else
    {
        leftTop = snake[0].topRight();
        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);
        snake.insert(0,QRectF(leftTop, rightBottom));
    }

}
//蛇向左移动
void MainWidget::addLeft(){
    QPointF leftTop;
    QPointF rightBottom;
    if(snake[0].x() - nodeWidth < 0)
    {
        //        leftTop = QPointF(this->width()-nodeWidth, snake[0].y());
        //        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);

        leftTop = QPointF(snake[0].x(), snake[0].y()-nodeHeight);
        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);

        moveFlag = DIR_UP;
        snake.insert(0,QRectF(leftTop, rightBottom));
    }
    else
    {
        leftTop =snake[0].topLeft() - QPointF(nodeWidth,0);
        rightBottom = leftTop +QPointF(nodeWidth,nodeHeight);
        snake.insert(0,QRectF(leftTop, rightBottom));
    }



}
