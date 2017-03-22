#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <snakewin.h>
#include <calthread.h>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QSound>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);   //开始游戏按钮
    void keyPressEvent(QKeyEvent  *event);  //获取键盘事件

    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updatepic(int **x, int l);  //接受来自计算线程的信号并给frame绘图
    void updateword(QString s1,QString s2);     //更新frame的单词信息
    void updatelife(int x);
    void deadinf(int k);    //贪吃蛇死了，弹出提示
    void upjifen(int x);    //更新积分显示
    void uplevel(int x);
    void autoswitch(bool b);
    void speedgame(QString s);
    void playeat();
    void playdead();
    void setmusic();

    void on_pushButton_2_clicked();

private:
    calthread *thread;
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    QSound *eatsound,*deadsound;
    QSound *backgroundsound;
    QTimer *testTimer;
    QPixmap pixmap;     //背景图
    QPalette palettee;   //背景palette
};

#endif // MAINWINDOW_H
