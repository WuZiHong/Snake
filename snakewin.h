#ifndef SNAKEWIN_H
#define SNAKEWIN_H
#include <QFrame>
#include <QPainter>
#include <QColor>
#include <QPoint>
#include <QLabel>

struct llist
{
    llist *next;
    QLabel *a;
    llist()
    {
        next=NULL;
        a=NULL;
    }
};

class snakewin:public QFrame
{
    Q_OBJECT
public:
    snakewin(QWidget *parent = 0);
    void getview(int **x,int l);  //获取画图的视窗
    void getword(QString s);
    ~snakewin();

public slots:
    void setbegin(int x,bool b);
private:

    void paintEvent(QPaintEvent *);
    void paintview();   //画游戏窗口
    void paintbegin();  //画游戏开始的label;
    QPainter *paint;    //建个画笔
    int **view; //画图规则数组
    int lsnake; //蛇长
    QString word;
    llist *labellist,*p;   //labellist数组
    QLabel *beginlabel;     //beginlabel 开始倒计时的label
    QColor color;   //设置画笔颜色
    QPen pen;
    bool begingame; //游戏开始倒计时
    int begintime;  //游戏开始倒计时的时间设置
    QImage *body,*head,*eng;
    QString road;

};

#endif // SNAKEWIN_H
