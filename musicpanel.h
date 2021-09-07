#ifndef MUSICPANEL_H
#define MUSICPANEL_H

#include <QPushButton>
#include <QSlider>
#include <QWidget>
class MusicPanel : public QWidget {
    Q_OBJECT
public:
    explicit MusicPanel(QWidget* parent = nullptr);

signals:
    void musicPlay();
    void musicPause();
    void changeVolume(int);
private:
    QSlider*     m_slider;
    QPushButton* m_playButton;
    QPushButton* m_pauseButton;
};

#endif  // MUSICPANEL_H
