#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "QCalculatorUI.h"
#include "qcustomplot.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    // 从计算器的文本框里面去拿到函数表达式字符串
    void getDate(QVector<double> &orx, QVector<double> &ory);
    void getDate(QVector<double> &orx, QVector<double> &ory, QVector<double> &orx1, QVector<double> &ory1);
    void setUI(QVector<double> &orx, QVector<double> &ory);
private:
    Ui::Form *ui;
};

#endif // FORM_H
