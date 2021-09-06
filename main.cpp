#include "mainwindow.h"

#include <QApplication>

#include <QGraphicsScene>
#include "battlefieldview.h"

#include "battlefieldwidget.h"
#include "startscreen.h"
#include "gameoverwidget.h"
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication       a(argc, argv);
    MainWindow         w;
    BattlefieldWidget* widget = nullptr;

    auto start = new StartScreen(nullptr);
    auto gameover = new GameoverWidget(nullptr);
    start->show();
    
    QObject::connect(start, &StartScreen::startButtonClicked, [&]{ 
        widget = new BattlefieldWidget; 
        QObject::connect(widget, &BattlefieldWidget::gameover, gameover, &GameoverWidget::setWinner);
        QObject::connect(widget, &BattlefieldWidget::gameover, [&]{ 
            gameover->show();
            widget->hide();
            qDebug() << "gameover running";
        });
        widget->show();
        start->hide();
    });

    QObject::connect(gameover, &GameoverWidget::goBack, [&]{
        gameover->hide();
        start->show();
        widget->deleteLater();
    });

    return a.exec();
}
