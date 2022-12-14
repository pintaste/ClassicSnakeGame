#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include "game.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Game *game;
    QTimer *timer;
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;
    QPainter *paint;
    QPushButton *button_start;
    QPushButton *button_pause;
    QPushButton *button_exit;

    QLabel* label_score_title;
    QLabel* label_score_content;
    QLabel* label_speed_title;
    QLabel* label_speed_content;
    QLabel* label_speed;
    QPushButton* button_speed_add;
    QPushButton* button_speed_sub;
    QLabel* label_tips;

    // board settings
    const int SINGLE_SIZE = 30;
    const int START_X = 40;
    const int START_Y = 40;

    // keyboard event
    int step_count = 1;
    int step_key = 0;

    // game settings
    int game_speed = 1;
    int is_paused = 0;

};
#endif // MAINWINDOW_H
