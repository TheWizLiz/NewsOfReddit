#include "calendardialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Calendar w;

    QString temp = w.loadTable();
    w.loadTree(temp);

    w.show();

    return a.exec();
}
