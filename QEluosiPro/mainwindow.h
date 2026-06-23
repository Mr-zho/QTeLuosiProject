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

private:
    Ui::MainWindow *ui;

    /* 边缘 */
    static const int m_margin = 20;
};

#endif // MAINWINDOW_H
