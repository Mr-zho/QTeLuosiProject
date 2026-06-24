#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /* 处理自然下落 */
    void handleUpdateDown();

    /* 处理开始游戏按钮 */
    void handleStartSlot();
    /* 处理重新开始按钮 */
    void handleReStartSlot();
    /* 处理暂停游戏按钮 */
    void handlePauseSlot();
protected:
    /* 绘画事件  */
    void paintEvent(QPaintEvent *e) override;
    /* 键盘按压事件 */
    void keyPressEvent(QKeyEvent *e) override;
private:
    /* 绘制活动区域 */
    void drawRangeRegion(QPainter & painter);
    /* 绘制网格线 */
    void drawGridRegion(QPainter & painter);

    /* 创建俄罗斯方块 */
    void createBlock();
    /* 绘制移动方块 */
    void drawActiveBlock(QPainter & painter);
    /* 判断是否可以移动 */
    bool isCanMove(int newX, int newY);
    /* 清空活动区域所有的方格 */
    void clearBoard();
    /* 放置区域的方格 */
    void placeBlock();

    /* 绘制固定的方块 */
    void drawFixedBlock(QPainter & painter);

    /* 旋转方块 */
    void rotateBlock();

    /* 消除整行 */
    void removeCompleteRows();

    /* 初始化按钮和标签 */
    void initButtonLabel();
    /* 显示分数 */
    void displayScore();
private:
    Ui::MainWindow *ui;

    /* 边缘 */
    static const int m_margin = 20;

    static const int BOARD_WIDTH = 480 / 32;
    static const int BOARD_HEIGHT = 640 / 32;

    /* 当前正在下落砖块的形状 */
    int m_currentShape[4][4];
    /* 当前下落方块的X坐标 */
    int m_currentX;
    /* 当前下落方块的Y坐标 */
    int m_currentY;

    /* 维护区域里面所有的方块 */
    int m_board[20][20];
    /* 定时器 */
    QTimer * m_timer;

    int m_lastShapeIdx;

    /* 得分 */
    int m_score;
    /* 分数标签 */
    QLabel * m_scoreLabel;

    /* 游戏开始的标志位 */
    bool m_isGameStarted;
    /* 游戏结束的标志位 */
    bool m_isGameOver;
    /* 暂停标志 */
    bool m_isPaused;
    /* 开始游戏 */
    QPushButton * m_startBtn;
    /* 重新开始游戏 */
    QPushButton * m_reStartBtn;
    /* 暂停游戏 */
    QPushButton * m_pauseBtn;
};

#endif // MAINWINDOW_H
