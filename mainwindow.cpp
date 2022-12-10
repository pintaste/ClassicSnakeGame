#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_snake = new Snake();
    m_food = new Food(m_snake);

    // set the font
    QFont font("Helvetica [Cronyx]", 14);
    // Label：Game Score
    label_score_title = new QLabel(this);
    label_score_title->setFont(font);
    label_score_title->setText("Game Scores: ");
    label_score_title->setGeometry(610, 100, 85, 20);
    label_score_title->show();
    label_score_content = new QLabel(this);
    label_score_content->setText(QString::number(m_snake->score));
    label_score_content->setFont(font);
    label_score_content->setGeometry(720, 100, 40, 20);
    label_score_content->show();

    // Label: Game Speed
    label_speed_title = new QLabel(this);
    label_speed_title->setFont(font);
    label_speed_title->setText("Current Speed: ");
    label_speed_title->setGeometry(610, 140, 85, 20);
    label_speed_title->show();
    label_speed_content = new QLabel(this);
    label_speed_content->setFont(font);
    label_speed_content->setText("x" + QString::number(game_speed));
    label_speed_content->setGeometry(720, 140, 40, 20);
    label_speed_content->show();

    // Label：speed display
    label_speed = new QLabel(this);
    label_speed->setFont(font);
    label_speed->setText("Setup Speed: ");
    label_speed->setGeometry(610, 230, 85, 20);
    label_speed->show();

    // Button: speed down
    button_speed_sub = new QPushButton(this);
    button_speed_sub->setFocusPolicy(Qt::NoFocus);
    button_speed_sub->setText("-");
    button_speed_sub->setGeometry(620, 270, 50, 25);
    button_speed_sub->show();
    // Button: speed up
    button_speed_add = new QPushButton(this);
    button_speed_add->setFocusPolicy(Qt::NoFocus);
    button_speed_add->setText("+");
    button_speed_add->setGeometry(700, 270, 50, 25);
    button_speed_add->show();

    // Label: max min speed
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    label_tips = new QLabel(this);
    label_tips->setFont(font);
    label_tips->setText("");
    label_tips->setGeometry(620, 200, 150, 15);
    label_tips->setPalette(pe);
    label_tips->show();

    // Button: game pause
    button_pause = new QPushButton(this);
    button_pause->setFocusPolicy(Qt::NoFocus);
    button_pause->setText("Pause");
    button_pause->setGeometry(640, 360, 80, 30);
    button_pause->show();

    // Button: game restart
    button_start = new QPushButton(this);
    button_start->setFocusPolicy(Qt::NoFocus);
    button_start->setText("Restart");
    button_start->setGeometry(640, 410, 80, 30);
    button_start->show();

    // Button: exit
    button_exit = new QPushButton(this);
    button_exit->setFocusPolicy(Qt::NoFocus);
    button_exit->setText("Exit");
    button_exit->setGeometry(640, 460, 80, 30);
    button_exit->show();

    timer = new QTimer(this);
    timer->start(1000);

    // Button clicked event: speed up
    (void)connect(button_speed_sub, &QPushButton::clicked, [=]() {
      if (game_speed > 1){
        label_tips->setText("");
        game_speed = game_speed / 2;
        label_speed_content->setText("x" + QString::number(game_speed));
        timer->start(1000 / game_speed);
        if (is_paused == 1) timer->stop();
      }else {
        label_tips->setText("Minimum Speed.");
      }
      });

    // Button clicked event: speed down
    (void)connect(button_speed_add, &QPushButton::clicked, [=]() {
      if (game_speed < 8){
        label_tips->setText("");
        game_speed = game_speed * 2;
        label_speed_content->setText("x" + QString::number(game_speed));
        timer->start(1000 / game_speed);
        if (is_paused == 1) timer->stop();
      } else{
        label_tips->setText("Maximum Speed.");
      }
      });

    // Button clicked event: game paused
    (void)connect(button_pause, &QPushButton::clicked, [=]() {
      switch (is_paused){
      case 0:
        timer->stop();
        is_paused = 1;
        button_pause->setText("Continue");
        break;
      case 1:
        timer->start(1000 / game_speed);
        is_paused = 0;
        button_pause->setText("Paused");
        break;
      default:
        break;
      }
      });

    // Button clicked event: game restart
    (void)connect(button_start, &QPushButton::clicked, [=]() {
      m_snake = new Snake;
      game_speed = 1;
      is_paused = 0;
      step_count = 1;
      step_key = 0;
      label_speed_content->setText("x" + QString::number(game_speed));
      label_score_content->setText(QString::number(m_snake->score));
      is_paused = 0;
      button_pause->setText("Paused");
      update();
      timer->start(1000 / game_speed);
      });

    (void)connect(button_exit, &QPushButton::clicked, [=]() {
      this->close();
      });


    (void)connect(timer, &QTimer::timeout, [=](){
        step_count++;
        if (step_count == INT_MAX - 10) step_count = 1;
        // update snake location
        int flag = m_snake->moveSnake();
        // update score
        label_score_content->setText(QString::number(m_snake->score));
        switch (flag){
        case 0:
          // 0 - There was a collision
          timer->stop();
          QMessageBox::information(this, QString::fromLocal8Bit("Game Over"), "Game Over!\nYour score is " + QString::number(m_snake->score) + ".");
          break;
        case 1:
          // 1 - Normal movement
          update();
          break;
        case 2:
          // 2 - Got food
          m_food->new_food(m_snake);
          update();
          break;
        default:
          break;
        }
      });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Draw the game board
void MainWindow::paintEvent(QPaintEvent *){
    // draw the green background
    QPainter painterBoard(this);
    painterBoard.setPen(Qt::black);
    painterBoard.setBrush(Qt::black);
    painterBoard.setRenderHint(QPainter::Antialiasing);
    int x = this->width();
    int y = this->height();
    painterBoard.drawRect(QRect(x*0.1 , y*0.1, y*0.8, y*0.8));
    // draw the split line
    painterBoard.setPen(Qt::blue);
    painterBoard.setBrush(Qt::blue);
    painterBoard.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < 16; i++){
      painterBoard.drawLine(QPoint(START_X + i * SINGLE_SIZE, START_Y), QPoint(START_X + i * SINGLE_SIZE, START_Y + 16 * SINGLE_SIZE));
      painterBoard.drawLine(QPoint(START_X, START_Y + i * SINGLE_SIZE), QPoint(START_X + 16 * SINGLE_SIZE, START_Y + i * SINGLE_SIZE));
    }

    // draw the snake body
    QPainter painterSnakeBody(this);
    painterSnakeBody.setPen(Qt::green);
    painterSnakeBody.setBrush(Qt::green);
    painterSnakeBody.setRenderHint(QPainter::Antialiasing);
    int i = 0;
    for (; i < m_snake->snake_body.size() - 1; i++){
      int site_x = START_X + m_snake->snake_body[i][0] * SINGLE_SIZE;
      int site_y = START_Y + m_snake->snake_body[i][1] * SINGLE_SIZE;
      painterSnakeBody.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);
    }

    // draw the snkae head
    QPainter painterSnakeHead(this);
    painterSnakeHead.setPen(Qt::green);
    painterSnakeHead.setBrush(Qt::green);
    painterSnakeHead.setRenderHint(QPainter::Antialiasing);
    int site_x = START_X + m_snake->snake_body[i][0] * SINGLE_SIZE;
    int site_y = START_Y + m_snake->snake_body[i][1] * SINGLE_SIZE;
    painterSnakeHead.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);

    // draw the different eyes baesd on current direction
    QPainter painterSnakeEye(this);
    painterSnakeEye.setPen(Qt::white);
    painterSnakeEye.setBrush(Qt::white);
    painterSnakeEye.setRenderHint(QPainter::Antialiasing);
    if (m_snake->direction == 'U') {
      painterSnakeEye.drawEllipse(site_x + 7, site_y + 7, 6, 6);
      painterSnakeEye.drawEllipse(site_x + 17, site_y + 7, 6, 6);
    }
    else if (m_snake->direction == 'D') {
      painterSnakeEye.drawEllipse(site_x + 7, site_y + 17, 6, 6);
      painterSnakeEye.drawEllipse(site_x + 17, site_y + 17, 6, 6);
    }
    else if (m_snake->direction == 'L') {
      painterSnakeEye.drawEllipse(site_x + 7, site_y + 7, 6, 6);
      painterSnakeEye.drawEllipse(site_x + 7, site_y + 17, 6, 6);
    }
    else {
      painterSnakeEye.drawEllipse(site_x + 17, site_y + 7, 6, 6);
      painterSnakeEye.drawEllipse(site_x + 17, site_y + 17, 6, 6);
    }

    // draw the food
    QPainter painterFood(this);
    painterFood.setPen(Qt::red);
    painterFood.setBrush(Qt::red);
    painterFood.setRenderHint(QPainter::Antialiasing);
    site_x= START_X + m_food->x * SINGLE_SIZE;
    site_y= START_Y + m_food->y * SINGLE_SIZE;
    painterFood.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);
}


// keyboard listener event
void MainWindow::keyPressEvent(QKeyEvent* event){
    // qDebug() << "keyPressed: " << event->key() ;
    if (step_key != step_count){
        switch (event->key()){
        case Qt::Key_Up:
        case 'w':
        case 'W':
          if (m_snake->direction == 'D') break;
          m_snake->direction = 'U';
          step_key = step_count;
          break;
        case Qt::Key_Down:
        case 's':
        case 'S':
          if (m_snake->direction == 'U') break;
          m_snake->direction = 'D';
          step_key = step_count;
          break;
        case Qt::Key_Left:
        case 'a':
        case 'A':
          if (m_snake->direction == 'R') break;
          m_snake->direction = 'L';
          step_key= step_count;
          break;
        case Qt::Key_Right:
        case 'd':
        case 'D':
          if (m_snake->direction == 'L') break;
          m_snake->direction = 'R';
          step_key= step_count;
          break;

        default:
          break;
        }
      }

}
