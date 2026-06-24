#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <time.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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

    /* 创建砖块 */
    createBlock();

    /* 清空活动区域 */
    clearBoard();
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




/* 创建俄罗斯方块 */
void MainWindow::createBlock()
{
    /* 随机数 */
    int shapeIdx = rand() % 7;

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


/* 键盘按压事件 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
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
