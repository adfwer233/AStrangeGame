#include "musicpanel.h"
#include <QGroupBox>
#include <QLayout>

MusicPanel::MusicPanel(QWidget *parent) : QWidget(parent)
{
    m_slider = new QSlider(this);
    m_playButton = new QPushButton(this);
    m_pauseButton = new QPushButton(this);

    QGroupBox* groupbox = new QGroupBox(this);
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(m_slider);
    layout->addWidget(m_playButton);
    layout->addWidget(m_pauseButton);

    groupbox->setLayout(layout);

    QVBoxLayout* m_layout = new QVBoxLayout(this);
    m_layout->addWidget(groupbox);
    this->setLayout(m_layout);   

    m_slider->setMaximum(100);
    m_slider->setMinimum(0);
    m_slider->setValue(30);
    m_slider->setOrientation(Qt::Horizontal);

    m_playButton->setText(tr("播放背景音乐"));
    m_pauseButton->setText(tr("暂停背景音乐"));
    connect(m_slider, &QSlider::valueChanged, this, changeVolume);
    connect(m_playButton, &QPushButton::clicked, this, &MusicPanel::musicPlay);
    connect(m_pauseButton, &QPushButton::clicked, this, &MusicPanel::musicPause);
}
