#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include "statusConstant.h"
#include <QWidget>

namespace Ui {
class GameoverWidget;
}

class GameoverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameoverWidget(QWidget *parent = nullptr);
    ~GameoverWidget();

public slots:
    void setWinner(coordinateStatus t_team);

signals:
    void goBack();

private:
    Ui::GameoverWidget *ui;
};

#endif // GAMEOVERWIDGET_H
