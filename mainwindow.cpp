#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    backgroundsound=new QSound(":/new/prefix1/background.wav");
    eatsound=new QSound(":/new/prefix1/eat.wav");
    deadsound=new QSound(":/new/prefix1/dead.wav");
    thread=new calthread;
    testTimer = new QTimer(this);
    testTimer->start(1000);
    connect(thread,SIGNAL(sendview(int**,int)),this,SLOT(updatepic(int**,int)));
    connect(thread,SIGNAL(sendword(QString,QString)),this,SLOT(updateword(QString,QString)));
    connect(thread,SIGNAL(senddead(int)),this,SLOT(deadinf(int)));
    connect(thread,SIGNAL(showjifen(int)),this,SLOT(upjifen(int)));
    connect(thread,SIGNAL(showlevel(int)),this,SLOT(uplevel(int)));
    connect(thread,SIGNAL(sendlife(int)),this,SLOT(updatelife(int)));
    connect(thread,SIGNAL(playeat()),this,SLOT(playeat()));
    connect(thread,SIGNAL(playdead()),this,SLOT(playdead()));
    connect(testTimer,SIGNAL(timeout()),this,SLOT(setmusic()));
    ui->setupUi(this);
    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(autoswitch(bool)));
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(speedgame(QString)));
    connect(thread,SIGNAL(sendbegin(int,bool)),ui->frame,SLOT(setbegin(int,bool)));
    thread->rebegin();
    ui->groupBox->setFont(QFont("Kefa",15,QFont::Bold));   //设置积分显示字体
    ui->groupBox_2->setFont(QFont("Kefa",15,QFont::Bold));
    ui->groupBox_3->setFont(QFont("Kefa",15,QFont::Bold));
    ui->groupBox_4->setFont(QFont("Kefa",15,QFont::Bold));
    ui->pushButton->setFont(QFont("Kefa",16,QFont::Bold));
    ui->pushButton_2->setFont(QFont("Kefa",16,QFont::Bold));
    ui->label->setFont(QFont("Kefa",24,QFont::Bold));   //设置积分显示字体
    ui->label_2->setFont(QFont("Kefa",48,QFont::Bold));   //设置积分显示字体
    ui->label_3->setFont(QFont("Kefa",98,QFont::Bold));   //设置关卡显示字体
    ui->label_4->setFont(QFont("Kefa",48,QFont::Bold));   //设置生命显示字体
    ui->comboBox->addItem("普通");
    ui->comboBox->addItem("慢");
    ui->comboBox->addItem("快");
    ui->comboBox->addItem("较快");
    pixmap = QPixmap(":/new/prefix1/snakeimg/background.jpg").scaled(size());
    palettee=palette();
    palettee.setBrush(QPalette::Window,QBrush(pixmap)); //设置图片
    setPalette(palettee);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(pe);
    ui->pushButton->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
    ui->pushButton_2->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete deadsound;
    delete eatsound;
}

void MainWindow::keyPressEvent(QKeyEvent  *event)   //捕获键盘事件
{

    if((event->key()==Qt::Key_Up)||(event->key()==Qt::Key_W) )    //上
    {
        thread->setdir(true,false,false,false);
    }
    if((event->key()==Qt::Key_Down)||(event->key()==Qt::Key_S))   //下
    {
        thread->setdir(false,true,false,false);
    }
    if((event->key()==Qt::Key_Left)||(event->key()==Qt::Key_A))  //左
    {
       thread->setdir(false,false,true,false);
    }
    if((event->key()==Qt::Key_Right)||(event->key()==Qt::Key_D))   //右
    {
       thread->setdir(false,false,false,true);
    }
    if(event->key()==Qt::Key_X)   //自动寻路快捷键开关
    {
       if (ui->checkBox->isChecked())
       {
           ui->checkBox->setChecked(false);
           thread->getauto(false);
       }
       else
       {
           ui->checkBox->setChecked(true);
           thread->getauto(true);
       }
    }
    if (event->key()==Qt::Key_Enter)
        on_pushButton_clicked();
    if (event->key()==Qt::Key_Space)
        on_pushButton_2_clicked();
    if (event->key()==Qt::Key_Comma)
        thread->beslow();
    if (event->key()==Qt::Key_Period)
        thread->befast();
}
void MainWindow::autoswitch(bool b)
{
    thread->getauto(b);
}

void MainWindow::updatepic(int **x,int l)   //更新画图
{
    ui->frame->getview(x,l);
    ui->frame->update(0,0,500,500);
}

void MainWindow::updateword(QString s1, QString s2) //更新单词
{
    ui->frame->getword(s1); //设置单词
    ui->label->setText("当前单词:  "+s1+" "+s2); //设置单词意思
}

void MainWindow::upjifen(int x)
{
    ui->label_2->setText(QString::number(x));
}

void MainWindow::uplevel(int x)
{
    ui->label_3->setText(QString::number(x));
}

void MainWindow::updatelife(int x)
{
    ui->label_4->setText(QString::number(x));
}

void MainWindow::deadinf(int k)     //🐍死了信息
{
    backgroundsound->stop();
    if (k>0)
    {
        QString s;
        s="🐍死啦\r";
        s=s+"你总共获得"+ui->label_2->text()+"分\n";
        s=s+"是否从当前关卡继续游戏?";
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "Question", s,QMessageBox::Yes | QMessageBox::No);
        if( replay == QMessageBox::Yes)
        {
            thread->exit();     //初始化线程
            thread->relife();
            thread->terminate();
            thread->start();
            backgroundsound->play();
        }
        else
        {
            thread->exit();     //初始化线程
            thread->rebegin();
            thread->terminate();
            ui->pushButton->setText("Begin");
        }
    }
    else
    {
        QString s;
        s="🐍死啦\r";
        s=s+"你总共获得"+ui->label_2->text()+"分\n";
        s=s+"你已经不能从当前关卡继续游戏";
        QMessageBox::information(this, "Question", s,QMessageBox::Yes);
        thread->exit();     //初始化线程
        thread->rebegin();
        thread->terminate();
        ui->pushButton->setText("Begin");
    }
}

void MainWindow::speedgame(QString s)
{
    if (s=="快")
        thread->settime(100);
    if (s=="慢")
        thread->settime(1000);
    if (s=="较快")
        thread->settime(300);
    if (s=="普通")
        thread->settime(500);
}

void MainWindow::on_pushButton_clicked()    //控制游戏的开始、结束
{
    if (thread->isRunning())
    {
        backgroundsound->stop();
        thread->exit();
        thread->rebegin();
        thread->terminate();
        ui->pushButton->setText("Start");
        ui->pushButton_2->setText("Begin");
    }
    else
    {
        backgroundsound->play();
        QString s;
        s=ui->comboBox->currentText();
        if (s=="快")
            thread->settime(100);
        if (s=="慢")
            thread->settime(1000);
        if (s=="较快")
            thread->settime(300);
        if (s=="普通")
            thread->settime(500);
        thread->start();
        ui->pushButton->setText("Exit");
        ui->pushButton_2->setText("Stop");
   }

}

void MainWindow::playeat()
{
    eatsound->play();
}

void MainWindow::playdead()
{
    deadsound->play();
}


void MainWindow::on_pushButton_2_clicked()
{
    if (thread->isRunning())
    {
        backgroundsound->stop();
        thread->exit();
        thread->terminate();
        ui->pushButton->setText("Start");
        ui->pushButton_2->setText("Begin");
    }
    else
    {
        backgroundsound->play();
        thread->start();
        ui->pushButton->setText("Exit");
        ui->pushButton_2->setText("Stop");
   }

}

void MainWindow::setmusic()
{
    if (thread->isRunning())
        if (backgroundsound->isFinished())
            backgroundsound->play();
    testTimer->start(1000);
}
