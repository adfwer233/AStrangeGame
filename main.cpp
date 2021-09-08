#include "mainwindow.h"

#include <QApplication>

#include <QGraphicsScene>
#include "battlefieldview.h"
#include "statusConstant.h"
#include "battlefieldwidget.h"
#include "startscreen.h"
#include "levelselection.h"
#include "gameoverwidget.h"
#include "coursewidget.h"
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication       a(argc, argv);
    BattlefieldWidget* widget = nullptr;

    auto start = new StartScreen(nullptr);
    auto gameover = new GameoverWidget(nullptr);
    auto levelSelection = new LevelSelection(nullptr);
    auto course = new CourseWidget(nullptr);

    start->setWindowTitle("开始");
    gameover->setWindowTitle("游戏结束");
    levelSelection->setWindowTitle("选择关卡和难度");
    course->setWindowTitle("游戏教程");

    start->setWindowIcon(QIcon(":/new/roles/src/GameBackground.png"));
    gameover->setWindowIcon(QIcon(":/new/roles/src/GameBackground.png"));
    levelSelection->setWindowIcon(QIcon(":/new/roles/src/GameBackground.png"));
    course->setWindowIcon(QIcon(":/new/roles/src/GameBackground.png"));

    start->show();
    
    QObject::connect(start, &StartScreen::startButtonClicked, [&]{
        start->hide();
        levelSelection->show();
    });

    QObject::connect(start, &StartScreen::courseButtonClicked, [&]{
        start->hide();
        course->show();
    });

    QObject::connect(course, &CourseWidget::courseClosed, [&]{
        start->show();
    });

    QObject::connect(levelSelection, &LevelSelection::levelSelected, [&](int x, levelOfAI t_level) {
        levelSelection->hide();
        widget = new BattlefieldWidget(nullptr, x, t_level);
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
