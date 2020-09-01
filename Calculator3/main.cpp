#include <QtWidgets>
#include "QCalculatorUI.h"
#include "QCalculatorDec.h"
int main(int argc, char* argv[])
{
    /*设置字体为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
*/
    QApplication app(argc,argv);
    QCalculatorUI* ui = QCalculatorUI::NewIntance();
    if(ui==NULL)
        return 0;

    ui->show();
    return app.exec();
}
