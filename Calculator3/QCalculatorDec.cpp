#include "QCalculatorDec.h"
#include <math.h>
#include <iostream>
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <qalgorithms.h>

QCalculatorDec::QCalculatorDec() {
}

//变成中缀表达式,进行数据预处理
QQueue<QString> QCalculatorDec::Split(const QString& exp_source) {
    // 把string变成一个字符串
    QByteArray ba(exp_source.toLatin1());
    // 替换成左右值运算符，使之符合逆波兰算法
    ba.replace(QByteArray("sin"),QByteArray("s"));
    ba.replace(QByteArray("cos"),QByteArray("c"));
    ba.replace(QByteArray("tan"),QByteArray("t"));
    ba.replace(QByteArray("lg"),QByteArray("l"));
    ba.replace(QByteArray("asin"),QByteArray("a"));
    ba.replace(QByteArray("acos"),QByteArray("o"));
    const QString exp(ba);
    qDebug()<<"ss = " << ba;
    // 中缀表达式容器
    QQueue<QString> ret;
    // 数字容器
    QString num="";
    // 遍历表达式字符串
    for(int i=0;i<exp.length();i++) {
        // 分离数字，判断小数点和数字
        if( (exp[i]=='.') || ( (exp[i]>='0') && (exp[i]<='9') )) {
            num += exp[i];
        }
        // 分离非一级符号，这里将s,c,t,a,n,!,^,l改变变成一个左右值运算符
        else if(exp[i] == '(' || exp[i] == ')' || exp[i] == '*' || exp[i] == '/' ||\
                exp[i] == 's' || exp[i] == 'c' || exp[i] == 't' || exp[i] == 'a' || exp[i] == 'o' ||\
                exp[i] == '!' || exp[i] == '^' || exp[i] == 'l') {
            // 如果符号前面有数字
            if(!num.isEmpty()) {
                // 把符号前面的那个数字字符串入队，因为有的数字可能是多位数
                ret.enqueue(num);
                // 清空数字容器
                num.clear();
            }
            // 如果是三角函数和对数函数就在前面多个1使其变成一个左右值的符号
            if ( exp[i]=='s' || exp[i]=='c' || exp[i]=='t' || exp[i] == 'a' || exp[i] == 'o'|| \
                 exp[i]=='l') {
                ret.enqueue("1");
            }
            // 把操作符入队
            ret.enqueue(exp[i]);
            // 单独对阶乘在后面进行加1操作
            if (exp[i] == '!') {
                ret.enqueue("1");
            }
        }

        // 对+-单独操作，主要是判断正负号
        else if(exp[i]== '+' || exp[i]== '-') {
            // 如果是在算式开头，表达式开头,说明是正负号
            if(i==0) {
             // 把这个符号算入数字里面，因为表示数字的正负
             num+= exp[i];
            }
            // 如果这个符号前面还是一个符号，说明这个+-还是修饰数字的正负
            else if(exp[i-1] == '(' || exp[i-1] == '+' || exp[i-1] == '-' || exp[i-1] == '*' || exp[i-1] == '/'||\
                    exp[i-1] == '^') {
             num+= exp[i];
            }
            // 如果不是修饰数字正负的，就是加减运算符，将数字入队列
            else {
                if(!num.isEmpty()) {
                    ret.enqueue(num);
                    num.clear();
                }
             ret.enqueue(exp[i]);
            }
        }
    }
    //遍历完成,判断是否还有数字
    if(!num.isEmpty())
    {
        ret.enqueue(num);
        num.clear();
    }
    qDebug() << ret;
    return ret;
}

 //将中缀队列转换为后缀队列,优先级，+- < * / <　sct a o l < ^ ! < ()
QQueue<QString> QCalculatorDec::Transfer(QQueue<QString>& exp)
{
    QStack<QString> stack;
    QQueue<QString> ret;
    bool num_ok;
    QString symbol;

    while(!exp.isEmpty()) {
      //出队列
      symbol = exp.dequeue();
      //字符串变成数字
      symbol.toDouble(&num_ok);
      //数字优先级最低
      if(num_ok==true) {
           stack.push(symbol);
      }
      //+-为第二级别优先级
      else if(symbol=="+"||symbol=="-") {
          while(!stack.isEmpty() &&(stack.top()!="(")) {
              //取出栈顶运算符并入队列
              ret.enqueue(stack.pop());
          }
          stack.push(symbol);
      }
      //第三级的符号
      else if(symbol=="*" || symbol=="/") {
          while(!stack.isEmpty() && (stack.top()!="(") && (stack.top()!="+") && (stack.top()!="-")) {
              // 直接加入到后缀表达式中
              ret.enqueue(stack.pop());
          }
          stack.push(symbol);
      }
      //第四级运算符等级
      else if(symbol=="s"|| symbol=="c"||symbol=="t"||\
              symbol=="a"|| symbol=="o"||symbol=="l"){
          while(!stack.isEmpty() && (stack.top()!="(") && (stack.top()!="+") && (stack.top()!="-")\
                && (stack.top()!="*") && (stack.top()!="/")) {
              // 直接加入到后缀表达式中
              ret.enqueue(stack.pop());
          }
          stack.push(symbol);

      }
      //第五级别优先级
      else if(symbol=="^" || symbol=="!") {
          while(!stack.isEmpty() && (stack.top()!="(") && (stack.top()!="+") && (stack.top()!="-") &&\
               (stack.top()!="*") && (stack.top()!="/") && (stack.top()!="s") && (stack.top()!="c")&&\
               (stack.top()!="t") && (stack.top()!="a") && (stack.top()!="o") && (stack.top()!="l")) {
              // 直接加入到后缀表达式中
              ret.enqueue(stack.pop());
          }
          stack.push(symbol);

      }
      else if(symbol == "(") {
         stack.push(symbol);
      }

      else if(symbol ==")") {
          while(!stack.isEmpty() && (stack.top()!="("))
          {
              ret.enqueue(stack.pop());     //取出栈顶运算符并入队列
          }
          if(stack.top()=="(")
            stack.pop();
      }
    }

    //遍历完成,判断栈里是否为空
    while(!stack.isEmpty()&& (stack.top()!="(")) {
       //取出栈顶运算符并入队列
       ret.enqueue(stack.pop());
    }
    qDebug() << ret;
    return ret;
}

// 这个接口没有看
QString QCalculatorDec::ValidNum(QString str) {
    QString num;
    //判断是否小数
    if(str.indexOf(".") == -1)
        return str;
    //避免0被去掉
    while(str.length()>1) {
         num=str.right(1);
         if(num=="."||num=="0") {
             str.chop(1);
             if(num==".")
                 return  str;
         }
         else
             return str;
    }
    return str;
}

// 左右值运算
QString QCalculatorDec::Calculate(QString& l,QString& op,QString& r ) {
    double left,right,res=0.000000;
    QString ret="";
    qDebug() << "8888888888888888888888888";
    left = l.toDouble();
    qDebug() << left;
    right = r.toDouble();
    qDebug() << right;
    if(op == "+") {
        res = left + right;
    }

    else if(op == "-") {
        res = left - right;
    }

    else if(op == "*") {
        res = left * right;
    }

    else if(op == "/") {
        if( (right>(-0.000000000000001)) && (right<(0.000000000000001)) )   //判断除数为0
            return NULL;
        else
            res = left/right;
    }
    else if(op == "s") {
        //res =left * sin(right/180.0*3.14159265358979323846);
        res =left * sin(right);
    }
    else if(op == "c") {
        //res =left * cos(right/180.0*3.14159265358979323846);
        res =left * cos(right);
    }
    else if(op == "t") {
        //res =left * tan(right/180.0*3.14159265358979323846);
        res =left * tan(right);
        qDebug() << "this is a tan test:";
        qDebug() << res;
    }
    // 如果是asin
    else if(op == "a") {
        //res =left * asin(right/180.0*3.14159265358979323846);
        res =left * asin(right);
    }
    else if(op == "o") {
        //res =left * acos(right/180.0*3.14159265358979323846);
        res =left * acos(right);
    }
    // 阶乘
    else if(op == "!") {
        unsigned long n = 1;
        for(int i = left; i >= 1; i--)
            n *= i;
        res = n * right;
    }
    // 求幂
    else if(op == "^") {
        res = pow(left, right);
    }
    // 对数
    else if(op == "l") {
        res = left * log10(right);
    }
    qDebug() << "this is a res test";
    qDebug() << res;
    // 要返回为一个字符串
    ret.sprintf("%f",res);
    qDebug() << ret;
    return ret;
}

// 逆波兰计算
QString QCalculatorDec::Calculate(QQueue<QString>& exp) {
    // 逆波兰栈
    QStack<QString> stack;
    QString symbol,L,R,op,ret;
    bool num_ok;

    while(!exp.isEmpty()) {
      qDebug() << "?????????????????????????????????";
      // 从后缀表达式里面拿字符串
      symbol = exp.dequeue();
      // 将当前字符串变成数字
      symbol.toDouble(&num_ok);

      //如果当前字符串是数字，入栈
      if(num_ok==true) {
        stack.push(symbol);
      }
      //如果当前字符串是运算符
      else {
        qDebug() << "/////////////////////////////";
        // 逆波兰算式
        if(stack.size()<2)
              return "Error";

        R= stack.pop();
        L= stack.pop();
        // 逆波兰算法只能计算左右值计算
        ret = Calculate(L,symbol,R );
        if(ret==NULL) {
             // 报错
             return ret;
        }
        // 将计算结果压栈
        stack.push(ret);
      }
    }
    //遍历完成,拿到结果
    if(stack.size()==1) {
         // 字符串转换成数字
         return ValidNum(stack.pop());
    }
    else {
        return "Error";
    }
}

QString QCalculatorDec::Result(const QString& exp) {
      QQueue<QString> q=Split(exp); //分离中缀
      q=Transfer(q);                //转换为后缀
      return Calculate(q);          //返回结果
}
