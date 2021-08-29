#include "mainwindow.h"

#include <QApplication>

#include <QGraphicsScene>
#include <battlefieldview.h>

#include "battlefieldWidget.h"
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication       a(argc, argv);
    MainWindow         w;
    BattlefieldWidget* widget = new BattlefieldWidget();
    
    w.setCentralWidget(widget);
    w.show();

    return a.exec();
}
