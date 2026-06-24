#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /* 绘画事件  */
    void paintEvent(QPaintEvent *e) override;


private:
    /* 绘制活动区域 */
    void drawRangeRegion(QPainter & painter);
    /* 绘制网格线 */
    void drawGridRegion(QPainter & painter);

    /* 创建俄罗斯方块 */
    void createBlock();
    /* 绘制移动方块 */
    void drawActiveBlock(QPainter & painter);
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
};

#endif // MAINWINDOW_H
