#include "QCalculatorUI.h"
#include <QDebug>

QStack<QString> historstack;    //记录历史记录
QStack<QString> historstack_tmp;//记录导出的历史记录


QCalculatorUI::QCalculatorUI() : QWidget(NULL,Qt::WindowCloseButtonHint)
{
}

bool  QCalculatorUI::construct()
{
    int ret;
    const char* butnText[34]=
    {
        "<-","CE","!","^","lg",
        "sin","cos","tan","asin","acos",
        "7","8","9","+","(",
        "4","5","6","-",")",
        "1","2","3","*","=",
        "0",".", "/","<<", ">>",
        "cal","y=","x","draw"
    };

    const int butnPos[34][4]=       //存放 x y w h
    {
      {10,50,40,40},{60,50,40,40},{110,50,40,40},{160,50,40,40},{210,50,40,40},                                                   ///< CE
      {10,100,40,40},{60,100,40,40},{110,100,40,40},{160,100,40,40},{210,100,40,40},
      {10,150,40,40},{60,150,40,40},{110,150,40,40},{160,150,40,40},{210,150,40,40},    //7 8 9 + （
      {10,200,40,40},{60,200,40,40},{110,200,40,40},{160,200,40,40},{210,200,40,40},    //4 5 6 - ）
      {10,250,40,40},{60,250,40,40},{110,250,40,40},{160,250,40,40},{210,250,40,40},    //1 2 3 * =
      {10,300,40,40},{60,300,40,40},{110,300,40,40},{160,300,40,40},{210,300,40,40},    //0 . / < >
      {10,350,40,40},{60,350,40,40},{110,350,40,40},{160,350,90,40}                     //cal y= x draw
    };
    // 文本输入框初始化
    mline  =new QLineEdit(this);
    if(mline==NULL)
        return false;
    mline->resize(240,30);
    mline->move(10,10);
    mline->setAlignment(Qt::AlignRight);
    mline->setReadOnly(1);
    /* mline->setFont(QFont(0,10));        //设置字体
    //this->setWindowTitle("计算器");*/
    this->setWindowTitle("Calculator v1.0");

    // 给按钮初始化
    for(int i=0;i<34;i++) {
         mbuton[i]= new  QPushButton(butnText[i],this);
         if(mbuton[i]==NULL)
             return false;

         mbuton[i]->resize(butnPos[i][2],butnPos[i][3]);
         mbuton[i]->move(butnPos[i][0],butnPos[i][1]);

         // 信号槽，将点击事件和按键处理函数绑定在一起
         QObject::connect(mbuton[i],SIGNAL(clicked()),this,SLOT(handler_clicked()));

   }
   return true;
}

// 单例模式
QCalculatorUI* QCalculatorUI::NewIntance()
{
    QCalculatorUI* ret = new QCalculatorUI();
    if(ret==NULL || !ret->construct())
    {
        delete ret;
        return NULL;
    }
    return ret;
}

//反向匹配str1和str2
int  QCalculatorUI::LastMatchingBoth(QString& str1,const char* str2)
{
    for(int i=str1.length();i>=0;i--)
    {
        for(unsigned int j=0;j<strlen(str2);j++)
            if(str1[i]==str2[j])
                 return i;
    }
    // 输出-1就是没有检测到
    return -1;
}

//匹配str1和str2,判断str1是否有str2的字符
int  QCalculatorUI::MatchingBoth(QString& str1,const char* str2)
{
    for(int i=0;i<str1.length();i++)
    {
        for(unsigned int j=0;j<strlen(str2);j++)
            if(str1[i]==str2[j])
                 return i;
    }
   return -1;
}

//处理按键消息
void QCalculatorUI::handler_clicked()
{
    // 得到当前算式的结果值
    QString result_tmp = mline->text();
    //清理信号
    static int ClearLine=0;
    //圆括号计数
    static int bracket_cnt=0;
    //响应信号，然后就从中取出是哪个按钮（对象）发的信号
    QPushButton *btn =dynamic_cast<QPushButton* >(sender());
    //取得当前输入框中的数据赋给line这个字符串
    QString line = mline->text();
    // 把文本框的数据占存于此，查看历史记录的时候，比较用
    QString line_tmp = mline->text();
    //获取对象的值，也就是要输入的消息
    QString text = btn->text();

    // 如果是要清零
    if(ClearLine)
    {
        mline->setText("");
        line.clear();
        ClearLine=0;
    }
    //数字
    if(text>="0"&&text<="9") {
        // line.right(1)就是说当前输入值的最右边的第一个字符
        QString tmp= line.right(1);  
        //数字前面不能为右括号,或者直接为sin,cos,tan等运算符，除了^
        if(tmp.length() && MatchingBoth(tmp,")sincostanasinacoslg!") != -1)
        {
            // 什么都不操作
            return;
        }
        line+=text;
    }
    //小数点
    else if(text=="." ) {
        QString tmp= line.right(1);
        // 如果小数点前不为空
        if(tmp.length()) {
            //没找到数字
            if(MatchingBoth(tmp,"0123456789") == -1) {
                return;
            }
        }
        //小数点前面为空
        else {
                return ;
        }
        // 看看已经输入的算式里面的情况
        // 反向查找，从最右边往前面走，找到一个符号就退出
        int pos= LastMatchingBoth(line,"+-*/.()");
        // 如果检测到当前数字字符内已经有一个小数点
        if(pos!= -1 && line[pos]=='.' )
        {
            return ;
        }
        line+=text;
    }
    //加减号
    else if(text=="+"||text=="-")
    {
        QString tmp= line.right(1);
       if(tmp.length()&& MatchingBoth(tmp,"+-*/.^sincostanasinacoslg") != -1)     //前面不能为:小数点
       {
          return ;
       }
       /*
       tmp= line.right(2);
       if(tmp.length()==2)          //前面不能连续有两次加减乘除
       {
           if(tmp[0]=='+'||tmp[0]=='-'||tmp[0]=='*'||tmp[0]=='/'||tmp[0]=='(')
                if(tmp[1]=='+'||tmp[1]=='-'||tmp[1]=='*'||tmp[1]=='/')
                            return ;
       }*/
        line+=text;
    }
    //乘除号
    else if(text=="*"||text=="/")
    {
         QString tmp= line.right(1);
         if(tmp.length())       //前面不能为:左括号,小数点,加减乘除,
         {
             if(MatchingBoth(tmp,"(.+-*/sincostanasinacos^lg")!= -1) //查找左括号,小数点,加减乘除
             {
                 return;
             }
         }
         else                   //乘除号前面不能为空
              return;

        line+=text;
    }
    //左括号
    else if(text=="(")
    {
        QString tmp= line.right(1);
        if(tmp.length())
        {
            if(MatchingBoth(tmp,")0123456789.!x")!= -1)
            {
                return;
            }
        }

        tmp= line.right(2);
        if(tmp.length()==2)          //前面不能连续有两次加减乘除
        {
             if(tmp[0]=='+'||tmp[0]=='-'||tmp[0]=='*'||tmp[0]=='/')
                 if(tmp[1]=='+'||tmp[1]=='-'||tmp[1]=='*'||tmp[1]=='/')
                             return ;
        }
         line+=text;
         bracket_cnt++;
    }
    //右括号
    else if(text==")")
    {
        QString tmp= line.right(1);
        if(bracket_cnt==0)  //前面没有左括号
           return;

        if(tmp.length())             //前面不能为:加减乘除,小数点,左括号
        {
           if(MatchingBoth(tmp,"+-*/.(sincostanasinacos^lg!")!= -1) //查找加减乘除,小数点,左括号
           {
               return;
           }
        }
        else                    //右括号前面不能为空
           return;

        line+=text;
        bracket_cnt--;
    }
    //<-
    else if(text=="<-")
    {
        if(line.length())
        line.chop(1);
    }
    //sin
    else if(text=="sin")
    {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-*/(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    //asin
    else if(text=="asin") {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-*/(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    //acos
    else if(text=="acos") {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-*/(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    //tan
    else if(text=="tan")
    {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-*/(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    //cos
    else if(text=="cos")
    {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-*/(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    //乘方
    else if(text=="^")
    {
        QString tmp= line.right(1);
        //前面不能为:左括号,小数点,加减乘除,
        if(tmp.length()) {
            //查找左括号,小数点,加减乘除
            if(MatchingBoth(tmp,")0123456789x") == -1)
            {
                return;
            }
        }
        // 如果前面为空
        else
             return;

       line+=text;
    }
    //阶乘
    else if(text=="!")
    {

        QString tmp= line.right(1);
        // 如果这个不是一个数
        if(MatchingBoth(tmp,")0123456789x") == -1) {
            return;
        }
        int pos= LastMatchingBoth(line,"+-*/.()");
        // 如果检测到当前数字字符内已经有一个小数点,表示是一个小数，阶乘不能是一个小数
        if(pos!= -1 && line[pos]=='.' ) {
            return ;
        } else {
            line+=text;
        }
    }
    //对数
    else if(text=="lg")
    {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 前面不能为.)或者直接是数字
           if(MatchingBoth(tmp,"+-/*(^=") == -1)
           {
               return;
           }
        }
        line+=text;
    }
    // 往前面翻历史记录
    else if(text == "<<"){
        // 先判断前面有没有
        if (historstack.isEmpty() && historstack_tmp.isEmpty()){
            return;
        }
        if(historstack.isEmpty()){
            // 为了不出现空白状态，读取historstack_tmp的top
            line.clear();
            bracket_cnt = 0;
            line += historstack_tmp.top();
        } else {
            // 读取栈顶元素
            QString tmp = historstack.top();
            // 把历史记录导入前置栈里面去
            historstack_tmp.push(historstack.pop());
            if (tmp == line_tmp) {
                if (!historstack.isEmpty()){
                    tmp = historstack.top();
                    historstack_tmp.push(historstack.pop());
                    line.clear();
                    bracket_cnt = 0;
                    line += tmp;
                } else {
                    // 只有一个算式的情况
                    line.clear();
                    bracket_cnt = 0;
                    line += tmp;
                }
            } else {
                line.clear();
                bracket_cnt = 0;
                line += tmp;
            }
        }
        // 发送下一次点击清屏指令
        ClearLine=1;
    }
    // 在看了前面的历史记录之后往现在翻
    else if(text == ">>"){
        // 先判断里面有没有
        if (historstack.isEmpty() && historstack_tmp.isEmpty()){
            return;
        }
        if(historstack_tmp.isEmpty()) {
            // 为了不出现空白状态，读取historstack的top
            line.clear();
            bracket_cnt = 0;
            line += historstack.top();
        } else {
            // 读取栈顶元素
            QString tmp = historstack_tmp.top();
            // 把历史记录导入前置栈里面去
            historstack.push(historstack_tmp.pop());
            if (tmp == line_tmp) {
                if (!historstack_tmp.isEmpty()){
                    tmp = historstack_tmp.top();
                    historstack.push(historstack_tmp.pop());
                    line.clear();
                    bracket_cnt = 0;
                    line += tmp;
                } else {
                    // 只有一个算式的情况
                    line.clear();
                    bracket_cnt = 0;
                    line += tmp;
                }
            } else {
                line.clear();
                bracket_cnt = 0;
                line += tmp;
            }
        }
        ClearLine=1;
    }
    // 翻历史记录的时候进行运算的
    else if(text == "cal") {
        if(MatchingBoth(result_tmp, "y=x") != -1) {
            return;
        }
        // 拿到结果
        int pos= LastMatchingBoth(result_tmp,"=");
        if (pos != 0 || pos != -1){
            result_tmp = result_tmp.mid((++pos), result_tmp.length());
        }

        line.clear();
        bracket_cnt = 0;
        // 拿到当前的结果值
        line += result_tmp;
    }
    // y=
    else if(text == "y=") {
        QString tmp= line.right(1);
        if(tmp.length())
        {
           // 必须要居于第一个
           if(MatchingBoth(tmp,"+-*/.()sincostanasinacos^lg!0123456789x") != -1)
           {
               return;
           }
        }
        line+=text;
    }
    // x
    else if(text == "x") {
        QString tmp = line;
        QString tmp_right = tmp.right(1);
        if(tmp.isNull() || tmp.isEmpty()){
            return;
        }
        if(tmp.length()){
            // 如果输入的x前面
            if(MatchingBoth(tmp_right, "0123456789)xsincostanasinacoslg!") != -1 && MatchingBoth(tmp, "y=") != -1) {
                return;
            }
        }
        line += text;
    }
    // 跳转画图
    else if(text == "draw") {
        // 对当前表达式进行判断，如果是一个正确的表达式，就进行替换计算
        QString tmp= line.left(2);
        // 如果不是以y=开端，就不是真的函数表达式
        if (tmp.length() && MatchingBoth(tmp,"y=") == -1){
            return;
        }
        // 做字符串替换处理
        // 取到x的表达式
        QString functmp = line.mid(2,line.length());
        // 做数据处理
        double x;
        bool yisOK = false;
        QString ret, result;
        QVector<double> orx(201),y(201);
        // 将表达式中的x全部替换掉,并进行计算
        for(int i=0;i<201;i++) {
          // 给出x的范围
          orx[i] = i/5.0-20;
          // 替换所有的为这个数
          // 把string变成一个字符串
          QByteArray ba(functmp.toLatin1());
          // 把x替换成数字
          ba.replace(QByteArray("x"),QByteArray(ret.sprintf("%f",orx[i]).toLatin1()));

          // 计算结果
          result = mDec.Result(ba);
          // x == 0 处理
          //除数为0
          /*
          if(result.isNull()) {
              line += " : ";
              line += "Divisor cannot be 0";
              break;
          }
          else*/
          if(result == "Error") {
              line += ":";
              line +="Format error";
              break;
          }
          else {
               y[i] = result.toDouble();
          }
          // 跑到最后一步，我们都OK了，可以打打印了
          if (i == 100) {
              yisOK = true;
          }
        }
        if (yisOK){
            Form *pic = new Form();
            for (int i = 0; i < 101; i++){
                  qDebug() << y[i];
            }
            // 将y的vector传入后台打印。
            //pic->getDate(orx,y);
            pic->setUI(orx, y);
            pic->show();
        }
    }
    //清空
    else if(text=="CE")
    {
        line.clear();
        bracket_cnt=0;
        // 将history_tmp中的数据全部压入history中
        while(!historstack_tmp.isEmpty()) {
            QString tans = historstack_tmp.pop();
            historstack.push(tans);
        }
    }
    // 如果是按了 =，设置历史记录转换变量
    else if(text=="="&& line.length()&&MatchingBoth(line,"y=x") == -1) {
        // 这里调用Result()接口进行运算，把line送入，后台进行计算
        QString ret=mDec.Result(line);
        qDebug() << "this is a result test:";
        qDebug() << ret;
        //除数为0
        if(ret.isNull()) {
            line += " : ";
            line +="Divisor cannot be 0";
        }
        else if(ret=="Error") {

            line += ":";
            line +="Format error";
         }
         else {
             // 如果是一个正确的表达式
             line += "=";
             line += ret;

             // 一定要入历史栈的最后一个
             // 将history_tmp中的数据全部压入history中
             while(!historstack_tmp.isEmpty()) {
                 QString tans = historstack_tmp.pop();
                 historstack.push(tans);
             }

             historstack.push(line);
         }
         ClearLine =1;
    }
    mline->setText(line);
}

void QCalculatorUI::show()              //显示窗口
{
    QWidget::show();
    this->setFixedSize(this->width(),this->height());
}
