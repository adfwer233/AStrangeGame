#include "mainwindow.h"

#include <QApplication>

#include <QGraphicsScene>
#include "battlefieldview.h"

#include "battlefieldwidget.h"
#include "startscreen.h"
#include "levelselection.h"
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
    auto levelSelection = new LevelSelection(nullptr);
    start->show();
    
    QObject::connect(start, &StartScreen::startButtonClicked, [&]{
        start->hide();
        levelSelection->show();
    });

    QObject::connect(levelSelection, &LevelSelection::levelSelected, [&](int x) {
        levelSelection->hide();
        widget = new BattlefieldWidget(nullptr, x);
        QObject::connect(widget, &BattlefieldWidget::gameover, gameover, &GameoverWidget::setWinner);
        QObject::connect(widget, &BattlefieldWidget::gameover, [&]{
            gameover->show();
            widget->hide();
        });
        QObject::connect(widget, &BattlefieldWidget::exitgame, [&]{
            widget->hide();
            widget->deleteLater();
            start->show();
        });
        widget->show();
    });

    QObject::connect(gameover, &GameoverWidget::goBack, [&]{
        gameover->hide();
        start->show();
        widget->deleteLater();
    });

    return a.exec();
}
