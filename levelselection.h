#ifndef LEVELSELECTION_H
#define LEVELSELECTION_H

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
    void levelSelected(int);

private:
    Ui::LevelSelection *ui;
};

#endif // LEVELSELECTION_H
