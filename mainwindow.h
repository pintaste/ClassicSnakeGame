#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>     // paintEvent
#include <QWidget>      // keyPressEvent
#include <QKeyEvent>    // keyPressEvent
#include <QTimer>
#include <QLabel>
#include "snake.h"
#include "food.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Snake *m_snake;
    Food *m_food;

    QTimer* timer;
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
    // 按钮：调节速度
    QPushButton* button_speed_add;
    QPushButton* button_speed_sub;
    // 标签：速度上下限提示
    QLabel* label_tips;

    // board settings
    const int SINGLE_SIZE = 30;
    const int START_X = 80;
    const int START_Y = 60;

    // keyboard event
    int step_count = 1;
    int step_key = 0;

    // game settings
    int game_speed = 1;
    // 暂停按钮标志(0：表示游戏正在运行 1: 表示游戏正在暂停)
    int is_paused = 0;

};
#endif // MAINWINDOW_H
