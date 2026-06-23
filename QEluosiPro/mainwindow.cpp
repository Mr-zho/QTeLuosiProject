#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>


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
    /* 绘制网格线 */
    drawGridRegion(painter);
}


MainWindow::~MainWindow()
{
    delete ui;
}
