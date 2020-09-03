#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fwd.h"
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
    Ui::MainWindow *ui;

    void initLeftControlsList();
    void initGraphicsView();

private slots:
    void slot_focusItemChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason);
};

#endif // MAINWINDOW_H
