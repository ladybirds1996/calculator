#include "form.h"
#include "ui_form.h"
#include <QVector>


/*
void Form::getDate(QVector<double> &orx, QVector<double> &ory){
    x.swap(orx);
    y.swap(ory);
    return;
}

void Form::getDate(QVector<double> &orx, QVector<double> &ory, QVector<double> &orx1, QVector<double> &ory1){
    orx.swap(orx1);
    ory.swap(ory1);
    return;
}
*/

void Form::setUI(QVector<double> &orx, QVector<double> &ory) {
    ui->setupUi(this);
    //设定背景为黑色
    //ui->setBackground(QBrush(Qt::black));
    //设定右上角图形标注可见
    ui->widget->setVisible(true);
    //设定右上角图形标注的字体
    ui->widget->legend->setFont(QFont("Helvetica", 9));
    //添加图形
    ui->widget->addGraph();
    // 存储x,y的值
    static QVector<double> x(201), y(201);
    x.swap(orx);
    y.swap(ory);
    for (int i = 0; i < 201; i++){
          qDebug() << "this is a test";
          qDebug() << y[i];
    }

    //设置画笔
    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    //设置画刷,曲线和X轴围成面积的颜色
    ui->widget->graph(0)->setBrush(QBrush(QColor(255,255,0)));
    //设置右上角图形标注名称
    ui->widget->graph(0)->setName("曲线");
    //传入数据，setData的两个参数类型为double
    ui->widget->graph(0)->setData(x,y);

    /*
    //================================================================
    QVector<double> temp(20);
    QVector<double> temp1(20);
    //图形为y = 100*x;
    for(int i=0;i<20;i++){
       temp[i] = i;
       temp1[i] = 10*i+10;
    }
    //添加图形
    ui->widget->addGraph();
    //设置画笔
    ui->widget->graph(1)->setPen(QPen(Qt::red));
    //设置画刷,曲线和X轴围成面积的颜色
    //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));
    //传入数据
    ui->widget->graph(1)->setData(temp,temp1);


    //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
    //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
    //setRange ( double  position, double  size, Qt::AlignmentFlag  alignment  )
    //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询


    //设置右上角图形标注名称
    //ui->widget->graph(1)->setName("直线");
    */
    //设置X轴文字标注
    ui->widget->xAxis->setLabel("x");
    //设置Y轴文字标注
    ui->widget->yAxis->setLabel("y");
    //设置X轴坐标范围
    ui->widget->xAxis->setRange(-20,20);
    //设置Y轴坐标范围
    //ui->widget->yAxis->setRange(-1100,1100);
    ui->widget->yAxis->setRange(-20,20);
    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->widget->axisRect()->setupFullAxesBox();

}


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form){
}



Form::~Form()
{
    delete ui;
}
