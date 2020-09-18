#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fwd.h"
#include "frame.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

using namespace std;
extern QString g_controls[];
extern map<QString, QString> g_controlsPngMap;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void initUI();
    void initLeftControlsList();
    void initGraphicsView();

    void zoomIn(int level);
    void zoomOut(int level);
	Frame *findRootParent(Frame *_item);

private slots:
    void slot_pos(QPointF pointF);
    void slot_itemAdded();
    void slot_zoom(int factor);
    void slot_setZoomFactor(int factor);
    void on_zoomInIcon_clicked();
    void on_zoomOutIcon_clicked();
    void on_horizontalLay_clicked();
    void on_verticalLay_clicked();

private:
    Ui::MainWindow *ui;
    Frame *m_rootFrame;
};

#endif // MAINWINDOW_H
