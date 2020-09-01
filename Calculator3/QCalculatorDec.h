#ifndef QCALCULATORDEC_H
#define QCALCULATORDEC_H
#include <QString>
#include <QStack>
#include <QQueue>
#include <QDebug>


class QCalculatorDec
{
private:
    QQueue<QString> Split(const QString& exp);          //分离前缀
    QQueue<QString> Transfer(QQueue<QString>& exp);     //将中缀队列转换为后缀队列
    QString Calculate(QQueue<QString>& exp);            //将后缀队列计算出结果

    QString Calculate(QString& l,QString& op,QString& r );
    QString Calculate2(QString& op,QString& r );
    QString ValidNum(QString str);

public:
    QCalculatorDec();
    QString Result(const QString& exp);
};

#endif // QCALCULATORDEC_H
