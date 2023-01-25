#include "viewer.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC, "C");
    viewer w;
    w.show();
    return a.exec();
}
