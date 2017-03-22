#include "snakewin.h"

snakewin::snakewin(QWidget *parent):QFrame(parent)
{
    view=NULL;
    labellist=NULL;
    paint=new QPainter;
    pen=paint->pen();
    pen.setWidth(1);
    pen.setColor(Qt::black);
    paint->setPen(pen);
    road=":/new/prefix1/";
}
snakewin::~snakewin()
{
    delete paint;
    p=labellist;
    delete body;
    delete eng;
    while (p!=NULL)
    {
        labellist=labellist->next;
        delete p->a;
        delete p;
        p=labellist;
    }
}

void snakewin::paintEvent(QPaintEvent *)
{
    if (begingame)
        paintbegin();
    paintview();

}

void snakewin::paintview()
{

    int k;
    if(view!=NULL)
    {
      p=labellist;
      while (p!=NULL)
      {
          labellist=labellist->next;
          delete p->a;
          delete p;
          p=labellist;
      }
      paint->begin(this);
      paint->drawLine(0,0,0,500);
      paint->drawLine(0,0,500,0);
      paint->drawLine(0,500,500,500);
      paint->drawLine(500,500,500,0);
      paint->end();
      for (int i=0;i<=25;i++)     //画方格
      {
          paint->drawLine(i*20,0,i*20,500);
          paint->drawLine(0,i*20,500,i*20);
      }
      for (int i=1;i<=25;i++)
          for (int j=1;j<=25;j++)
          {
              if (view[i][j]!=0)  //画蛇
              {
                  if (view[i][j]==lsnake) //是蛇头
                  {
                      p=new llist;
                      p->a=new QLabel(this);
                      p->a->setText("🐸");
                      p->a->move((j-1)*20+1,(i-1)*20+1);
                      p->a->show();
                      p->next=labellist;
                      labellist=p;
                  }
                  else if ((view[i][j]<100)&&(view[i][j]>=1)) //是蛇身
                  {
                      p=new llist;
                      p->a=new QLabel(this);
                      p->a->resize(20,20);
                      p->a->setText("🌕");
                      p->a->move((j-1)*20+1,(i-1)*20+1);
                      p->a->show();
                      p->next=labellist;
                      labellist=p;
                  }
                  else        // 是要吃的单词
                  {
                      k=view[i][j]-100;
                      p=new llist;
                      p->a=new QLabel(this);  //设置要吃的单词
                      p->a->setPixmap(QPixmap(":/new/prefix1/snakeimg/"+(QString)word[k]+".png"));
                      p->a->move((j-1)*20,(i-1)*20);
                      p->a->show();
                      p->next=labellist;
                      labellist=p;
                  }

              }
          }
    }

}

void snakewin::paintbegin()  //画游戏开始的label;
{
    if (beginlabel)
    {
        beginlabel->setText(QString::number(begintime));
    }
    else
    {
        beginlabel=new QLabel(this);
        beginlabel->setFont(QFont("Kefa",400,QFont::Bold));
        beginlabel->move(180,50);
        beginlabel->resize(320,300);
        beginlabel->show();
    }
}

void snakewin::getview(int **x,int l)
{
    view=x;
    lsnake=l;
}

void snakewin::getword(QString s)
{
    word=s.toLower();
}

void snakewin::setbegin(int x, bool b)
{
    begintime=x;
    if ((b==false)&&(begingame==true))
    {
        if (beginlabel!=NULL)
        {
            delete beginlabel;
            beginlabel=NULL;
        }
    }
    begingame=b;
}
