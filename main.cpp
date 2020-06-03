#include <QApplication>

#include "ApplicationWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://images/Icon.png"));
    ApplicationWindow w;
    w.show();
    return a.exec();
}
