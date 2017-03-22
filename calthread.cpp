#include "calthread.h"

calthread::calthread()
{
    view=new int *[27];     //初始化view二维数组
    preview=new int *[27];
    for (int i=0;i<=26;i++)
    {
        view[i]=new int [27];
        preview[i]=new int [27];
    }
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
            view[i][j]=0;
    for (int i=0;i<=26;i++)
    {
        view[0][i]=-1;
        view[26][i]=-1;
        view[i][0]=-1;
        view[i][26]=-1;
    }
    for (int i=5;i<10;i++)
        view[5][i]=i-4;
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
        {
            preview[i][j]=view[i][j];   //复制数组
            isgo[i][j]=true;
        };
    lsnake=5;
    wordnum=0;
    level=1;
    zmnum=0;
    eat=0;
    jifen=0;
    prejifen=0;
    autoi=-1;
    autosn=-1;
    head=new QPoint;
    prehead=new QPoint;
    head->setX(5);
    head->setY(9);
    prehead->setX(head->x());
    prehead->setY(head->y());
    left=false;
    right=true;
    up=false;
    down=false;
    firststart=true;
    autogo=false;
    QString path,file;       //获取程序运行文件路径
    QDir dir;
    /****************************
    *                           *
    *                           *
    *                           *
    *                           *
    *     绝对路径字符串（path ）  *
    *     需要修改               *
    *     **os x 下获取的path    *
    *     为应用程序内的路径       *
    *     最终得到文件路径file    *
    *     取路径中的2.txt文件     *
    *     即为贪吃蛇单词本         *
    *                           *
    *                           *
    *                           *
    *                           *
    *                           *
    ****************************/
    path=dir.currentPath();     //获取路径
    int k=path.indexOf("Snake.app");    //截断
    if (k!=-1)
        path.replace(k,path.length(),"");
    path=path+"2.txt";
    file=path;
    std::ifstream fin(file.toStdString());     //读入单词表
    int l;
    std::string s1,s2;
    while (fin>>s1)
    {
        getline(fin,s2);
        word[wordnum]=QString::fromStdString(s1);
        l=word[wordnum].length();
        lword[word[wordnum].length()]=wordnum;  //计算长度
        mean[wordnum]=QString::fromStdString(s2);
        wordnum++;
    }
    fin.close();
    for (int i=0;i<26;i++)      //判重表置空
            for (int j=0;j<26;j++)
                hashl[i][j]=NULL;
    lifenum=4;
}
calthread::~calthread()
{
    for (int i=0;i<=26;i++)
        delete [] view[i];
    delete []view;
    delete head;
}

void calthread::run()
{
    if (firststart)
    {
        sendlife(lifenum);
        randword();
        firststart=false;
        showlevel(level);   //初始化展示关卡
        showjifen(jifen);
    }
    for (int i=3;i>=1;i--)
    {
        sendbegin(i,true);
        sleep(1);
    }
    sendbegin(0,false);
    launch();
}

void calthread::settime(int x)
{
    mtime=x;
}

void calthread::setdir(bool a, bool b, bool c, bool d)
{
    if (a==true&&down==true)
        return;
    if (b==true&&up==true)
        return;
    if (c==true&&right==true)
        return;
    if (d==true&&left==true)
        return;
    up=a;
    down=b;
    left=c;
    right=d;
}

void calthread::launch()        //操纵蛇的上下左右移动
{
    int x,y;
    while (dead==false)
    {
        if (autogo==true)   //是自动寻路
        {
            if ((autoi==-1)||(autoi==autosn))   //自动寻路没有初始化
            {
                bfs();
                autosn=-1;
            }
            autosn++;
            if (autoi==autosn)
                autoi=-1;
            up=down=left=right=false;
            if (autoroad[autosn]==0)
                up=true;
            else if (autoroad[autosn]==1)
                down=true;
            else if (autoroad[autosn]==2)
                left=true;
            else if (autoroad[autosn]==3)
                right=true;
        }
        else
        {
            autoi=-1;
        }
        x=head->x();
        y=head->y();
        //使蛇向四个方向之一移动：
        if (up)
        {   if (view[x-1][y]==0)    //什么都没有，可以走
            {
                head->setX(x-1);
            }
            else if (view[x-1][y]==100+eat) //是要吃的单词
            {
                jifen++;
                eat++;
                head->setX(x-1);
                emit playeat();
            }
            else
                dead=true;  //碰到其他的都死
        }
        else if (down)
        {
            if (view[x+1][y]==0)
            {
                head->setX(x+1);
            }
            else if (view[x+1][y]==100+eat)
            {
                jifen++;
                eat++;
                head->setX(x+1);
                emit playeat();
            }
            else
                dead=true;
        }
        else if (left)
        {
            if (view[x][y-1]==0)
            {
                head->setY(y-1);
            }
            else if (view[x][y-1]==100+eat)
            {
                jifen++;
                eat++;
                head->setY(y-1);
                emit playeat();
            }
            else
                dead=true;
        }
        else if (right)
        {
            if (view[x][y+1]==0)
            {
                head->setY(y+1);
            }
            else if (view[x][y+1]==100+eat)
            {
                jifen++;
                eat++;
                head->setY(y+1);
                emit playeat();
            }
            else
                dead=true;
        }
        if (dead==false)    //4个方向移动，没死
        {
            if (eat==zmnum)
            {
                view[head->x()][head->y()]=view[x][y]+1;
                lsnake++;
                randword();
                //autogo=true;
            }
            else
            {
               gosnake(view,head->x(),head->y(),x,y);
            }
            review(view,lsnake);
            showjifen(jifen);
        }
        msleep(mtime);   //暂停0.5秒   //
    }
    if (dead==true) //蛇死了信息
    {
        emit playdead();
        emit senddead(lifenum);
    }
}

void calthread::redsnake(int x, int y,int k)
{
    if (k>0)
        k--;
    if (view[x-1][y]==view[x][y]-1)
    {
        redsnake(x-1,y,k);
        view[x][y]=k;
    }
    else if (view[x+1][y]==view[x][y]-1)
    {
        redsnake(x+1,y,k);
        view[x][y]=k;
    }
    else if (view[x][y-1]==view[x][y]-1)
    {
        redsnake(x,y-1,k);
        view[x][y]=k;
    }
    else if (view[x][y+1]==view[x][y]-1)
    {
        redsnake(x,y+1,k);
        view[x][y]=k;
    }
    else
        view[x][y]=0;
}

void calthread::randword()
{
    int x,y;
    eat=0;
    zmnum++;
    if (zmnum>10)
    {
        zmnum=1;
        level++;
        mtime=mtime*0.9;
        showlevel(level);
    }
    if (lsnake>14)
    {
        redsnake(head->x(),head->y(),6);
        lsnake=5;
    }

    currentword=lword[zmnum-1]+rand()%(lword[zmnum]-lword[zmnum-1]);
    for (int i=0;i<zmnum;i++)
    {
        x=rand()%25+1;
        y=rand()%25+1;
        while (view[x][y]!=0)
        {
            x=rand()%25+1;
            y=rand()%25+1;
        }
        view[x][y]=i+100;
    }
    sendwords(word[currentword],mean[currentword]);
    prehead->setX(head->x());
    prehead->setY(head->y());
    prejifen=jifen;
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
            preview[i][j]=view[i][j];   //复制数组

}
void calthread::rebegin()   //再次开始
{
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
            view[i][j]=0;
    for (int i=0;i<=26;i++)
    {
        view[0][i]=-1;
        view[26][i]=-1;
        view[i][0]=-1;
        view[i][26]=-1;
    }
    for (int i=5;i<10;i++)
        view[5][i]=i-4;
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
        {
            preview[i][j]=view[i][j];   //复制数组
            isgo[i][j]=true;
        };
    lsnake=5;
    wordnum=0;
    level=1;
    lifenum=4;
    zmnum=0;
    eat=0;
    jifen=0;
    head->setX(5);
    head->setY(9);
    left=false;
    right=true;
    up=false;
    down=false;
    dead=false;
    review(view,lsnake);
    sendlife(lifenum);
}
void calthread::relife()        //死了并从当前关卡开始
{
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
            view[i][j]=preview[i][j];   //复制数组
    head->setX(prehead->x());
    head->setY(prehead->y());
    eat=0;
    jifen=prejifen;
    dead=false;
    up=down=left=right=false;
    if (view[head->x()-1][head->y()]==0)
        up=true;
    else if (view[head->x()+1][head->y()]==0)
        down=true;
    else if (view[head->x()][head->y()-1]==0)
        left=true;
    else if (view[head->x()][head->y()+1]==0)
        right=true;
    lifenum--;
    sendlife(lifenum);
}

void calthread::recordsnake(int **a,int x,int y,unsigned long *k)    //记录蛇的形态到k
{
    if (a[x][y]!=1)
    {
        if (a[x-1][y]==a[x][y]-1)  //下一个蛇身在上面
        {
            recordsnake(a,x-1,y,k);
            *k=*k<<2;
        }
        else if (a[x+1][y]==a[x][y]-1)      //下一个蛇身在上面
        {
            recordsnake(a,x+1,y,k);
            *k=*k<<2;
            *k=*k+1;
        }
        else if (a[x][y-1]==a[x][y]-1)   //下一个蛇身在左边
        {
            recordsnake(a,x,y-1,k);
            *k=*k<<2;
            *k=*k+2;
        }
        else if (a[x][y+1]==a[x][y]-1)      //下一个蛇身在右面
        {
            recordsnake(a,x,y+1,k);
            *k=*k<<2;
            *k=*k+3;
        }
    }
}

bool calthread::isexisted(int x,int y,unsigned int k)   //判断蛇头为x，y位置的蛇身形态k是否存在
{
    hashpc *p;
    p=hashl[x][y];
    while (p!=NULL)
    {
        if (p->x==k)
            return true;
        p=p->next;
    }
    return false;
}
void calthread::addhash(int x,int y,unsigned int k)
{
    hashpc *p;
    p=new hashpc;
    p->x=k;
    p->next=hashl[x][y];
    hashl[x][y]=p;
}

void calthread::reput(int **a,int x,int y,unsigned int k)   //x,y:蛇头位置  k:蛇身形态  函数重现数组
{
    int l,t,x1,y1;
    x1=x;
    y1=y;
    l=lsnake;
    a[x1][y1]=l;
    l--;
    while (l!=0)
    {
        t=(k<<30)>>30;
        k=k>>2;
        if (t==0)
            x1--;
        else if (t==1)
            x1++;
        else if (t==2)
            y1--;
        else if (t==3)
            y1++;
        a[x1][y1]=l;
        l--;
    }
}

void calthread::gosnake(int **a,int x1,int y1,int x,int y)
{
    a[x1][y1]=a[x][y];
    a[x][y]=a[x][y]-1;
    while (a[x][y]!=0)       //从蛇头向蛇尾减少方框
    {
        if (a[x-1][y]==a[x][y])
        {
            x=x-1;
        }
        else if (a[x][y-1]==a[x][y])
        {
            y=y-1;
        }
        else if (a[x+1][y]==a[x][y])
        {
            x=x+1;
        }
        else if (a[x][y+1]==a[x][y])
        {
            y=y+1;
        }
        a[x][y]=a[x][y]-1;
    }
}

void calthread::cleanroad(int **a,int x,int y) //清除蛇走的下一步
{
    if (a[x][y]<=0)
        return;
    if (a[x-1][y]==a[x][y]-1)
        cleanroad(a,x-1,y);
    else if (a[x+1][y]==a[x][y]-1)
        cleanroad(a,x+1,y);
    else if (a[x][y-1]==a[x][y]-1)
        cleanroad(a,x,y-1);
    else if (a[x][y+1]==a[x][y]-1)
        cleanroad(a,x,y+1);
    a[x][y]=0;
}


void calthread::bfs()
{
    int **a;      //记录当前搜索数组  只有-1和0还有要吃的字母
    unsigned long shape=0,k;  //记录当前蛇形态转化为int后的shape 当前k
    int lhead,llast,stepend,stepnum=0,x,y,endx=0,endy=0;   //记录宽搜表头、尾指针位置
    a=new int *[27];     //a复制数组view；
    for(int i=0;i<27;i++)
        a[i]=new int [27];
    for (int i=0;i<=26;i++)
        for (int j=0;j<=26;j++)
        {
           a[i][j]=view[i][j];
           isgo[i][j]=true;
        }
    recordsnake(a, head->x(),head->y(),&shape); //记录蛇身
    addhash(head->x(), head->y(), shape);
    for (int i=1;i<=25;i++)     //不能走的地方都置为-1   清除蛇身位置
        for (int j=1;j<=25;j++)
        {
            if ((a[i][j]>=100)&&(eat!=a[i][j]-100))
                a[i][j]=-1;
            else if (eat==a[i][j]-100)
            {
                endx=i;
                endy=j;
            }
            else if (a[i][j]<100)
                a[i][j]=0;
        }
    lhead=0;
    llast=-1;
    stepend=0;
    o[lhead].k=shape;
    o[lhead].x=head->x();
    o[lhead].y=head->y();
    o[lhead].pre=lhead;
    while ((lhead!=llast)&&(hashl[endx][endy]==NULL))
    {
        stepnum++;
        stepend=lhead;
        while ((llast<stepend)&&(hashl[endx][endy]==NULL))
        {
            llast++;
            x=o[llast].x;
            y=o[llast].y;
            shape=o[llast].k;
            k=shape;
            reput(a, x, y, shape);
            if (((a[x-1][y]==0)||(a[x-1][y]>=100))&&(isgo[x-1][y])) //上可以走
            {
                gosnake(a, x-1, y, x, y);
                shape=0;
                recordsnake(a, x-1, y, &shape);
                if (!isexisted(x-1, y, shape))
                {
                    addhash(x-1, y, shape);
                    lhead++;
                    o[lhead].x=x-1;
                    o[lhead].y=y;
                    o[lhead].k=shape;
                    o[lhead].pre=llast;
                    o[lhead].dir=0;
                    isgo[x-1][y]=false;
                }
                cleanroad(a, x-1, y);
                reput(a, x, y, k);
            }
            if (((a[x+1][y]==0)||(a[x+1][y]>=100))&&(isgo[x+1][y]))   //下可以走
            {
                gosnake(a, x+1, y, x, y);
                shape=0;
                recordsnake(a, x+1, y, &shape);     //记录当前走的路径的shape
                if (!isexisted(x+1, y, shape))      //判重
                {
                    addhash(x+1, y, shape);     //加入判重表
                    lhead++;                //加入队列
                    o[lhead].x=x+1;
                    o[lhead].y=y;
                    o[lhead].k=shape;
                    o[lhead].pre=llast;
                    o[lhead].dir=1;
                    isgo[x+1][y]=false;
                }
                cleanroad(a, x+1, y);       //数组走后清理
                reput(a, x, y, k);      //恢复原状
            }
            if (((a[x][y-1]==0)||(a[x][y-1]>=100))&&(isgo[x][y-1]))   //左可以走
            {
                gosnake(a, x, y-1, x, y);
                shape=0;
                recordsnake(a, x, y-1, &shape);
                if (!isexisted(x, y-1, shape))
                {
                    addhash(x, y-1, shape);
                    lhead++;
                    o[lhead].x=x;
                    o[lhead].y=y-1;
                    o[lhead].k=shape;
                    o[lhead].pre=llast;
                    o[lhead].dir=2;
                    isgo[x][y-1]=false;
                }
                cleanroad(a, x, y-1);
                reput(a, x, y, k);
            }
            if (((a[x][y+1]==0)||(a[x][y+1]>=100))&&(isgo[x][y+1]))   //右可以走
            {
                gosnake(a, x, y+1, x, y);
                shape=0;
                recordsnake(a, x, y+1, &shape);
                if (!isexisted(x, y+1, shape))
                {
                    addhash(x, y+1, shape);
                    lhead++;
                    o[lhead].x=x;
                    o[lhead].y=y+1;
                    o[lhead].k=shape;
                    o[lhead].pre=llast;
                    o[lhead].dir=3;
                    isgo[x][y+1]=false;
                }
                cleanroad(a, x, y+1);
                reput(a, x, y, k);
            }
            cleanroad(a, x, y);
        }
    }
    int endk=0; //最后走的数组标号
    for (int i=lhead;i>0;i--)
        if ((o[i].x==endx)&&(o[i].y==endy))
        {
            endk=i;
        }
    autosn=autoi;
    printload(o,endk);
    for (int i=0;i<=26;i++)     //删除bfs数组a
    {
        delete []a[i];
    }
    delete []a;
    hashpc *pp;
    for (int i=1;i<=25;i++)
            for (int j=1;j<=25;j++)
            {
                while (hashl[i][j]!=NULL)
                {
                    pp=hashl[i][j];
                    hashl[i][j]=hashl[i][j]->next;
                    delete pp;
                }

            }
}

void calthread::printload(bfsl *o,int k)
{
    if (o[k].pre!=k)
    {
        printload(o, o[k].pre);
        autoi++;
        autoroad[autoi]=o[k].dir;
    }
}

void calthread::getauto(bool b)
{
    autogo=b;
}
