#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setFixedSize(780, 600);
  setWindowTitle("Classic Snake Game");
  game = new Game();

  // define the space between the UI elements
  const int ROW_SPACE = 40;
  const int COL_SPACE = 50;

  // Label：Game Score Init
  label_score_title = new QLabel(this);
  label_score_title->setText("Game Scores: ");
  label_score_title->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE, START_Y, 120, 20);
  label_score_title->show();
  label_score_content = new QLabel(this);
  label_score_content->setText(QString::number(game->score));
  label_score_content->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 5, START_Y, 40, 20);
  label_score_content->show();

  // Label: Game Speed Init
  label_speed_title = new QLabel(this);
  label_speed_title->setText("Current Speed: ");
  label_speed_title->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE, START_Y + COL_SPACE, 120, 20);
  label_speed_title->show();
  label_speed_content = new QLabel(this);
  label_speed_content->setText("x" + QString::number(game_speed));
  label_speed_content->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 5, START_Y + COL_SPACE, 40, 20);
  label_speed_content->show();

  // Label：Speed Setting Title Init
  label_speed = new QLabel(this);
  label_speed->setText("Speed Setting: ");
  label_speed->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE, START_Y + COL_SPACE * 2, 120, 20);
  label_speed->show();

  // Button: Speed Down Init
  button_speed_sub = new QPushButton(this);
  button_speed_sub->setFocusPolicy(Qt::NoFocus);
  button_speed_sub->setText("-");
  button_speed_sub->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE, START_Y + COL_SPACE * 3, 50, 30);
  button_speed_sub->show();

  // Button: Speed Up Init
  button_speed_add = new QPushButton(this);
  button_speed_add->setFocusPolicy(Qt::NoFocus);
  button_speed_add->setText("+");
  button_speed_add->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 3, START_Y + COL_SPACE * 3, 50, 30);
  button_speed_add->show();

  // Label: Speed Tips Init
  QPalette pe;
  pe.setColor(QPalette::WindowText, Qt::red);
  label_tips = new QLabel(this);
  label_tips->setText("");
  label_tips->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE, START_Y + COL_SPACE * 4, 150, 20);
  label_tips->setPalette(pe);
  label_tips->show();

  // Button: Game Pause Init
  button_pause = new QPushButton(this);
  button_pause->setFocusPolicy(Qt::NoFocus);
  button_pause->setText("Pause");
  button_pause->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 2, START_Y + COL_SPACE * 7, 80, 30);
  button_pause->show();

  // Button: Game Restart Init
  button_start = new QPushButton(this);
  button_start->setFocusPolicy(Qt::NoFocus);
  button_start->setText("Restart");
  button_start->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 2, START_Y + COL_SPACE * 8, 80, 30);
  button_start->show();

  // Button: Exit Init
  button_exit = new QPushButton(this);
  button_exit->setFocusPolicy(Qt::NoFocus);
  button_exit->setText("Exit");
  button_exit->setGeometry(START_X + BOARD_SIZE * SINGLE_SIZE + ROW_SPACE * 2, START_Y + COL_SPACE * 9, 80, 30);
  button_exit->show();

  // Timer Init
  timer = new QTimer(this);
  timer->start(1000);

  // Button clicked event: speed up
  (void)connect(button_speed_sub, &QPushButton::clicked, [=]()
                {
      if (game_speed > 1){
        label_tips->setText("");
        game_speed = game_speed - 1;
        label_speed_content->setText("x" + QString::number(game_speed));
        timer->start(1000 / game_speed);
        if (is_paused == 1) timer->stop();
      }else {
        label_tips->setText("Minimum Speed.");
      } });

  // Button clicked event: speed down
  (void)connect(button_speed_add, &QPushButton::clicked, [=]()
                {
      if (game_speed < 16){
        label_tips->setText("");
        game_speed = game_speed + 1;
        label_speed_content->setText("x" + QString::number(game_speed));
        timer->start(1000 / game_speed);
        if (is_paused == 1) timer->stop();
      } else{
        label_tips->setText("Maximum Speed.");
      } });

  // Button clicked event: game paused
  (void)connect(button_pause, &QPushButton::clicked, [=]()
                {
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
      } });

  // Button clicked event: game restart
  (void)connect(button_start, &QPushButton::clicked, [=]()
                {
      game = new Game;
      game_speed = 1;
      is_paused = 0;
      step_count = 1;
      step_key = 0;
      label_speed_content->setText("x" + QString::number(game_speed));
      label_score_content->setText(QString::number(game->score));
      is_paused = 0;
      button_pause->setText("Paused");
      update();
      timer->start(1000 / game_speed); });

  (void)connect(button_exit, &QPushButton::clicked, [=]()
                { this->close(); });

  (void)connect(timer, &QTimer::timeout, [=]()
                {
        step_count++;
        if (step_count == INT_MAX - 10) step_count = 1;
        // update snake location
        int flag = game->moveSnake();
        // update score
        label_score_content->setText(QString::number(game->score));
        switch (flag){
        case 0:
          // 0 - There was a collision
          timer->stop();
          QMessageBox::information(this, QString::fromLocal8Bit("Game Over"), "Game Over!\nYour score is " + QString::number(game->score) + ".");
          break;
        case 1:
          // 1 - Normal movement
          update();
          break;
        case 2:
          // 2 - Got food
            if(game->gen_food() == std::make_pair(-1,-1)){
                // the max socre reached.
            }
          update();
          break;
        } });
}

MainWindow::~MainWindow()
{
  delete ui;
}

// Draw the game board
void MainWindow::paintEvent(QPaintEvent *)
{
  // draw the green background
  QPainter painterBoard(this);
  painterBoard.setPen(Qt::white);
  painterBoard.setBrush(Qt::white);
  painterBoard.setRenderHint(QPainter::Antialiasing);
  painterBoard.drawRect(QRect(START_X, START_Y, BOARD_SIZE * SINGLE_SIZE, BOARD_SIZE * SINGLE_SIZE));
  // draw the split line
  painterBoard.setPen(Qt::gray);
  painterBoard.setBrush(Qt::gray);
  painterBoard.setRenderHint(QPainter::Antialiasing);
  for (int i = 0; i <= BOARD_SIZE; i++)
  {
    painterBoard.drawLine(QPoint(START_X + i * SINGLE_SIZE, START_Y), QPoint(START_X + i * SINGLE_SIZE, START_Y + BOARD_SIZE * SINGLE_SIZE));
    painterBoard.drawLine(QPoint(START_X, START_Y + i * SINGLE_SIZE), QPoint(START_X + BOARD_SIZE * SINGLE_SIZE, START_Y + i * SINGLE_SIZE));
  }

  // draw the snake body
  QPainter painterSnakeBody(this);
  painterSnakeBody.setPen(Qt::green);
  painterSnakeBody.setBrush(Qt::green);
  painterSnakeBody.setRenderHint(QPainter::Antialiasing);
  int i = 0;
  for (; i < game->snake_body.size() - 1; i++)
  {
    int site_x = START_X + game->snake_body[i][0] * SINGLE_SIZE;
    int site_y = START_Y + game->snake_body[i][1] * SINGLE_SIZE;
    painterSnakeBody.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);
  }

  // draw the snkae head
  QPainter painterSnakeHead(this);
  painterSnakeHead.setPen(Qt::green);
  painterSnakeHead.setBrush(Qt::green);
  painterSnakeHead.setRenderHint(QPainter::Antialiasing);
  int site_x = START_X + game->snake_body[i][0] * SINGLE_SIZE;
  int site_y = START_Y + game->snake_body[i][1] * SINGLE_SIZE;
  painterSnakeHead.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);

  // draw the different eyes baesd on current snake_direction
  QPainter painterSnakeEye(this);
  painterSnakeEye.setPen(Qt::white);
  painterSnakeEye.setBrush(Qt::white);
  painterSnakeEye.setRenderHint(QPainter::Antialiasing);
  if (game->snake_direction == 'U')
  {
    painterSnakeEye.drawEllipse(site_x + 7, site_y + 7, 6, 6);
    painterSnakeEye.drawEllipse(site_x + 17, site_y + 7, 6, 6);
  }
  else if (game->snake_direction == 'D')
  {
    painterSnakeEye.drawEllipse(site_x + 7, site_y + 17, 6, 6);
    painterSnakeEye.drawEllipse(site_x + 17, site_y + 17, 6, 6);
  }
  else if (game->snake_direction == 'L')
  {
    painterSnakeEye.drawEllipse(site_x + 7, site_y + 7, 6, 6);
    painterSnakeEye.drawEllipse(site_x + 7, site_y + 17, 6, 6);
  }
  else
  {
    painterSnakeEye.drawEllipse(site_x + 17, site_y + 7, 6, 6);
    painterSnakeEye.drawEllipse(site_x + 17, site_y + 17, 6, 6);
  }

  // draw the food
  QPainter painterFood(this);
  painterFood.setPen(Qt::red);
  painterFood.setBrush(Qt::red);
  painterFood.setRenderHint(QPainter::Antialiasing);
  site_x = START_X + game->food.first * SINGLE_SIZE;
  site_y = START_Y + game->food.second * SINGLE_SIZE;
  painterFood.drawRoundedRect(site_x + 2, site_y + 2, SINGLE_SIZE - 4, SINGLE_SIZE - 4, 10, 10);

  // draw the number for each box
  QPainter painterNum(this);
  painterNum.setPen(Qt::black);
  painterNum.setBrush(Qt::black);
  painterNum.setRenderHint(QPainter::Antialiasing);
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      int site_x = START_X + i * SINGLE_SIZE;
      int site_y = START_Y + j * SINGLE_SIZE;
      painterNum.drawText(site_x + 5, site_y + 15, QString::number(game->board[j*BOARD_SIZE + i]));
    }
  }
}

// keyboard listener event
void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if (step_key != step_count)
  {
    switch (event->key())
    {
    case Qt::Key_Up:
    case 'w':
    case 'W':
      if (game->snake_direction == 'D')
        break;
      game->snake_direction = 'U';
      step_key = step_count;
      break;
    case Qt::Key_Down:
    case 's':
    case 'S':
      if (game->snake_direction == 'U')
        break;
      game->snake_direction = 'D';
      step_key = step_count;
      break;
    case Qt::Key_Left:
    case 'a':
    case 'A':
      if (game->snake_direction == 'R')
        break;
      game->snake_direction = 'L';
      step_key = step_count;
      break;
    case Qt::Key_Right:
    case 'd':
    case 'D':
      if (game->snake_direction == 'L')
        break;
      game->snake_direction = 'R';
      step_key = step_count;
      break;

    default:
      break;
    }
  }
}
