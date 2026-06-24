#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <time.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_timer(new QTimer(this)),
    m_lastShapeIdx(-1),
    m_scoreLabel(new QLabel(this)),
    m_score(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 设置软件的标题 */
    setWindowTitle("俄罗斯方块");

    /* 资料文件 */
#if 0
    const char * filename = "D:\\MyProject\\QTeLuosiProject\\QEluosiPro\\icons\\icon.png";
#else
    const char * filename = ":/icons/icon.png";
#endif
    QIcon icons(filename);
    setWindowIcon(icons);

    /* 设置固定大小 */
    setFixedSize(680, 680);

    /* 添加随机数的种子 */
    srand(time(NULL));

    /* 清空活动区域 */
    clearBoard();


    /* 信号和槽 */
    connect(m_timer, &QTimer::timeout, this, &MainWindow::handleUpdateDown);

    /* 舒适化按钮和标签 */
    initButtonLabel();
}

int SHAPES[7][4][4] =
{
    // I型
    {
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // O型
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // S型
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // Z型
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // L型
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // J型
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // T型
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

/* 显示分数 */
void MainWindow::displayScore()
{
    m_scoreLabel->setText(QString("分数:%1").arg(m_score));
}


/* 处理开始游戏按钮 */
void MainWindow::handleStartSlot()
{
    if (m_isGameStarted == false)
    {
        /* 启动标志位 */
        m_isGameStarted = true;
        /* 暂停标志位 */
        m_isPaused = false;
        /* 清空活动区域 */
        clearBoard();
        /* 创建砖块 */
        createBlock();
        /* 启动定时器 */
        m_timer->start(1000);

        /* 手动更新 */
        update();
        this->setFocus();
    }

}
/* 处理重新开始按钮 */
void MainWindow::handleReStartSlot()
{
    /* 启动标志位 */
    m_isGameStarted = true;
    /* 暂停标志位 */
    m_isPaused = false;
    /* 清空活动区域 */
    clearBoard();
    /* 创建砖块 */
    createBlock();
    /* 启动定时器 */
    m_timer->start(1000);

    /* 手动更新 */
    update();
    this->setFocus();
}

/* 处理暂停游戏按钮 */
void MainWindow::handlePauseSlot()
{
    if (m_isGameStarted == false)
    {
        return;
    }

    m_isPaused = !m_isPaused;

    if (m_isPaused == true)
    {
        m_pauseBtn->setText("继续游戏");
        m_timer->stop();
    }
    else
    {
        m_pauseBtn->setText("暂停游戏");
        m_timer->start();
    }

    update();
    this->setFocus();
}


/* 初始化按钮和标签 */
void MainWindow::initButtonLabel()
{
    m_scoreLabel->move(530, 50);
    m_scoreLabel->resize(120, 60);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet(
                "QLabel {"
                "    /* 设置背景颜色为淡蓝色 */"
                "    background-color: #ADD8E6;"
                "    /* 设置文字颜色为深灰色 */"
                "    color: #333333;"
                "    /* 设置边框，宽度为 2px，颜色为灰色，样式为实线 */"
                "    border: 2px solid #808080;"
                "    /* 设置边框圆角为 10px */"
                "    border-radius: 10px;"
                "    /* 设置内边距为 10px */"
                "    padding: 10px;"
                "    /* 设置字体为 Arial，大小为 16px，加粗 */"
                "    font-family: Arial;"
                "    font-size: 16px;"
                "    font-weight: bold;"
                "}"
            );
    displayScore();



    /* 开始游戏 */
    m_startBtn = new QPushButton("开始游戏", this);
    m_startBtn->move(530, 150);
    m_startBtn->resize(120, 60);
    m_startBtn->setStyleSheet("QPushButton {"
                              "   background-color: rgb(238, 154, 73);" // 按钮的背景颜色
                              "   border-radius: 10px;"                 // 圆角半径
                              "   font-size: 16px;"                     // 字体大小
                              "   font-weight: bold;"                   // 字体加粗
                              "   font-family: Arial;"                  // 字体类型
                              "   color: white;"                        // 字体颜色
                              "}"
                              "QPushButton:hover {"
                              "   background-color: rgb(255, 200, 100);" // 鼠标悬浮时的背景颜色
                              "}"
                              "QPushButton:pressed {"
                                      "   background-color: rgb(200, 100, 50);" // 鼠标按下时的背景颜色
                                      "}"
                              );



    m_reStartBtn = new QPushButton("重新开始", this);
    m_reStartBtn->move(530, 250);
    m_reStartBtn->resize(120, 60);
    m_reStartBtn->setStyleSheet("QPushButton {"
                              "   background-color: rgb(238, 154, 73);" // 按钮的背景颜色
                              "   border-radius: 10px;"                 // 圆角半径
                              "   font-size: 16px;"                     // 字体大小
                              "   font-weight: bold;"                   // 字体加粗
                              "   font-family: Arial;"                  // 字体类型
                              "   color: white;"                        // 字体颜色
                              "}"
                              "QPushButton:hover {"
                              "   background-color: rgb(255, 200, 100);" // 鼠标悬浮时的背景颜色
                              "}"
                              "QPushButton:pressed {"
                                      "   background-color: rgb(200, 100, 50);" // 鼠标按下时的背景颜色
                                      "}"
                              );



    m_pauseBtn = new QPushButton("暂停游戏", this);
    m_pauseBtn->move(530, 350);
    m_pauseBtn->resize(120, 60);
    m_pauseBtn->setStyleSheet("QPushButton {"
                              "   background-color: rgb(238, 154, 73);" // 按钮的背景颜色
                              "   border-radius: 10px;"                 // 圆角半径
                              "   font-size: 16px;"                     // 字体大小
                              "   font-weight: bold;"                   // 字体加粗
                              "   font-family: Arial;"                  // 字体类型
                              "   color: white;"                        // 字体颜色
                              "}"
                              "QPushButton:hover {"
                              "   background-color: rgb(255, 200, 100);" // 鼠标悬浮时的背景颜色
                              "}"
                              "QPushButton:pressed {"
                                      "   background-color: rgb(200, 100, 50);" // 鼠标按下时的背景颜色
                                      "}"
                              );

    /* 信号和槽 */
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::handleStartSlot);
    connect(m_reStartBtn, &QPushButton::clicked, this, &MainWindow::handleReStartSlot);
    connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::handlePauseSlot);
}

/* 放置区域的方格 */
void MainWindow::placeBlock()
{
    for (int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if (m_currentShape[y][x] == 1)
            {
                m_board[m_currentY + y][m_currentX + x] = 1;
            }
        }
    }
}

/* 绘制固定的方块 */
void MainWindow::drawFixedBlock(QPainter & painter)
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for(int x = 0; x < BOARD_WIDTH; x++)
        {
            if (m_board[y][x] == 1)
            {
                painter.fillRect(m_margin + x * 32, m_margin +  y * 32, 32, 32, Qt::blue);
            }
        }
    }
}


/* 消除整行 */
void MainWindow::removeCompleteRows()
{
    int lineClearCnt = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0;y--)
    {
        bool isLineFull = true;

        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (m_board[y][x] == 0)
            {
                isLineFull = false;
                break;
            }
        }


        if (isLineFull == true)
        {
            for (int aboveY = y; aboveY > 0; aboveY--)
            {
                for (int x = 0; x < BOARD_WIDTH;x++)
                {
                    m_board[aboveY][x] = m_board[aboveY - 1][x];
                }
            }


            lineClearCnt++;
            /* 需要再次检查当前行 */
            ++y;
        }

    }

    /* 根据消除的行数更新分数 */
    if (lineClearCnt > 0)
    {
        m_score += lineClearCnt * 100;
        displayScore();
    }
}



/* 处理自然下落 */
void MainWindow::handleUpdateDown()
{
    if (isCanMove(m_currentX, m_currentY + 1))
    {
        m_currentY++;
    }
    else
    {
        /* 放置方块 */
        placeBlock();
        /* 消除整行 */
        removeCompleteRows();
        /* 创建新的方块 */
        createBlock();
    }

    /* 手动触发绘图事件 */
    update();
}


/* 创建俄罗斯方块 */
void MainWindow::createBlock()
{
    /* 随机数 */
    int shapeIdx = 0;
    do
    {
        shapeIdx = rand() % 7;
    } while(m_lastShapeIdx == shapeIdx);
    /* 更新上一次索引标记 */
    m_lastShapeIdx = shapeIdx;

    for (int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            m_currentShape[y][x] = SHAPES[shapeIdx][y][x];
        }
    }


    m_currentX = 6;
    m_currentY = 0;
}

/* 绘制移动方块 */
void MainWindow::drawActiveBlock(QPainter & painter)
{
    for (int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if (m_currentShape[y][x] == 1)
            {
                painter.fillRect(m_margin + (m_currentX + x) * 32, m_margin + (m_currentY + y) * 32, 32,32, Qt::red);
            }
        }
    }
}


/* 绘制活动区域 */
void MainWindow::drawRangeRegion(QPainter &painter)
{
    /* 创建一个画笔 */
    QPen pen(Qt::black);
    /* 设置画笔的宽度 */
    pen.setWidth(3);
    /* 设置画笔风格为虚线 */
    pen.setStyle(Qt::DashLine);

    /* 画笔应用到QPainter */
    painter.setPen(pen);

    /* 绘制矩形 */
    painter.drawRect(m_margin, m_margin, 480, 640);
}

/* 绘制网格线 */
void MainWindow::drawGridRegion(QPainter & painter)
{
    /* 创建一个画笔 */
    QPen pen(Qt::gray);
    /* 设置画笔的宽度 */
    pen.setWidth(2);
    /* 设置画笔风格为虚线 */
    pen.setStyle(Qt::DashLine);

    /* 画笔应用到QPainter */
    painter.setPen(pen);

    /* 绘制垂直的网格线 */
    for (int x = 0; x <= BOARD_WIDTH; x++)
    {
        int lineX = m_margin + x * 32;
        painter.drawLine(lineX, m_margin, lineX, m_margin + BOARD_HEIGHT * 32);
    }

    /* 绘制水平的网格线 */
    for (int y = 0; y <= BOARD_HEIGHT; y++)
    {
        int lineY = m_margin + y * 32;
        painter.drawLine(m_margin, lineY, m_margin + BOARD_WIDTH * 32, lineY);
    }

}

/* 绘画事件  */
void MainWindow::paintEvent(QPaintEvent *e)
{
    qDebug() << "paintEvent" << endl;

    QPainter painter(this);

    /* 绘制活动区域 */
    drawRangeRegion(painter);
    /* 绘制俄罗斯方块 */
    drawActiveBlock(painter);
    /* 绘制固定方块 */
    drawFixedBlock(painter);
    /* 绘制网格线 */
    drawGridRegion(painter);

}

/* 清空活动区域所有的方格 */
void MainWindow::clearBoard()
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            m_board[y][x] = 0;
        }
    }
}

/* 判断是否可以移动 */
bool MainWindow::isCanMove(int newX, int newY)
{
    for (int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if (m_currentShape[y][x] == 1)
            {
                int boardX = newX + x;
                int boardY = newY + y;

                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT ||
                        (boardY >= 0) && (m_board[boardY][boardX] == 1))
                {
                    return false;
                }
            }
        }
    }


    return true;
}


/* 旋转方块 */
void MainWindow::rotateBlock()
{
    /* 定义数组 */
    int rotateBlock[4][4] = { 0 };

    /* 顺时针旋转90度 */
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            rotateBlock[x][3 - y] = m_currentShape[y][x];
        }
    }

    /* 备份原数组 */
    int tempBlock[4][4] = { 0 };
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            tempBlock[y][x] = m_currentShape[y][x];
        }
    }

    /* 把旋转之后的数组覆盖当前的数组 */
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            m_currentShape[y][x] = rotateBlock[y][x];
        }
    }


    /* 碰撞检测 */
    if (isCanMove(m_currentX, m_currentY) == false)
    {
        /* 回滚 */
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                m_currentShape[y][x] = tempBlock[y][x];
            }
        }
    }
}


/* 键盘按压事件 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (m_isGameStarted == false || m_isPaused == true)
    {
        return;
    }

    switch (e->key())
    {
    case Qt::Key_Left:
        if (isCanMove(m_currentX - 1, m_currentY))
        {
            m_currentX--;
            /* 手动刷新绘图事件 */
            update();
        }
        break;
    case Qt::Key_Right:
        if (isCanMove(m_currentX + 1, m_currentY))
        {
            m_currentX++;
            /* 手动刷新绘图事件 */
            update();
        }
        break;
    case Qt::Key_Up:
        {
            rotateBlock();
            update();
        }
        break;
    case Qt::Key_Down:
        if (isCanMove(m_currentX, m_currentY + 1))
        {
            m_currentY++;
            /* 手动刷新绘图事件 */
            update();
        }
        break;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
