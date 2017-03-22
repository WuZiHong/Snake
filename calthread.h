#ifndef CALTHREAD_H
#define CALTHREAD_H
#include <QThread>
#include <QPoint>
#include <fstream>
#include <string>
//#include <iostream>
#include <QDir>
struct hashpc   //蛇形态判重
{
    unsigned long x;  //存储当前蛇的形态数组
    hashpc *next;
    hashpc()
    {
        next=NULL;
        x=0;
    }
};
struct bfsl
{
    unsigned long k;  //蛇形态缩小成的k
    int pre;
    int dir;
    int x,y;    //蛇头位置
};
class calthread:public QThread
{
    Q_OBJECT
public:
    calthread();
    void run();
    void launch();  //手动运行控制
    void randword();    //随机化单词
    void settime(int x);
    void review(int **x,int l)    //发送当前要更新的图
    {
        emit sendview(x,l);
    }
    ~calthread();
    void sendwords(QString s1,QString s2)   //新的单词、意思
    {
        emit sendword(s1,s2);
    }
    void rebegin(); //线程再次被启动
    void relife();  //继续从当前关卡开始
    //自动寻路代码段
    void recordsnake(int **a, int x, int y, unsigned long *k);  //记录当前蛇的形态到k
    bool isexisted(int x,int y,unsigned int k);   //判断蛇头为x，y位置的蛇身形态k是否存在
    void addhash(int x,int y,unsigned int k);   //将形态为k，头在x，y的地方的数组加入判重表
    void reput(int **a,int x,int y,unsigned int k);   //x,y:蛇头位置  k:蛇身形态  函数重现数组
    void gosnake(int **a,int x1,int y1,int x,int y);    //蛇从x，y走到x1，y1后数组变动
    void cleanroad(int **a,int x,int y);       //清除蛇走的下一步
    void bfs();                                 //宽搜关键代码段
    void printload(bfsl *o,int k);       //获取蛇走路径
    void getauto(bool b);   //是否进行自动寻路
    void redsnake(int x, int y,int k);      //蛇长度重置
    void befast()
    {
        if (mtime>80)
        mtime=mtime*0.8;
    }
    void beslow()
    {
        if (mtime<5000)
        mtime=mtime*1.0/0.8;
    }

public slots:
    void setdir(bool a,bool b,bool c,bool d);   //设置往哪边走
signals:
    void sendbegin(int x,bool b);
    void showjifen(int x);  //发送积分+1信号
    void showlevel(int x);  //发送等级+1信号
    void senddead(int x);    //发送蛇死了信号
    void sendview(int **x,int l);   //将头结点、数组信息发送至frame
    void sendword(QString s1,QString s2);
    void sendlife(int x);
    void playeat(); //播放吃的声音
    void playdead();//播放死的声音
private:
    QPoint *head;    //x1,y1当前蛇头位置
    QPoint *prehead;    //当前过程初始蛇头位置
    int **view;   //显示的矩阵
    int **preview;  //保存的矩阵
    int lsnake,wordnum;     //蛇长 单词个数
    bool left,right,up,down;    //上下左右方向
    bool dead;  //是不是死了
    bool firststart;    //第一次运行
    QString word[5000],mean[5000];
    int lword[15];  //单词长度
    int level;      //设置关卡等级
    int zmnum;  //字母的个数
    int eat;    //当前要吃第几个字母
    int currentword;    //当前单词在word数组中的编号
    int jifen;  //计算分数
    int prejifen; //当前关卡初试分数
    int mtime;
    hashpc *hashl[26][26];    //判重,hash[i][j]代表蛇头的位置
    int autoroad[100];  //自动寻路时走的数组
    int autoi,autosn;          //当前走到第几步
    bool autogo;    //自动寻路开启开关
    bfsl o[2000000];
    int lifenum;    //生命个数
    bool isgo[27][27];
};

#endif // CALTHREAD_H
