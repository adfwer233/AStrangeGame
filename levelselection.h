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
    void closeEvent(QCloseEvent *event) override;
    
signals:
    void levelSelected(int, levelOfAI);
    void levelClosed();
private:
    Ui::LevelSelection *ui;
};

#endif // LEVELSELECTION_H
