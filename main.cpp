#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CanConnector* sAGVController=new CanConnector();
    w.registerControler(sAGVController);

    CustomAlgorithm* algo=new CustomAlgorithm();
    sAGVController->registerAlgo(algo);

    sAGVController->start();
    algo->start();
    return a.exec();
}
