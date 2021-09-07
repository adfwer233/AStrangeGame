#ifndef LEVELSELECTION_H
#define LEVELSELECTION_H

#include "statusConstant.h"
#include <QWidget>

namespace Ui {
class LevelSelection;
}

class LevelSelection : public QWidget
{
    Q_OBJECT

public:
    explicit LevelSelection(QWidget *parent = nullptr);
    ~LevelSelection();

signals:
    void levelSelected(int, levelOfAI);

private:
    Ui::LevelSelection *ui;
};

#endif // LEVELSELECTION_H
