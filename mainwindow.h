﻿#ifndef MAINWINDOW_H
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
	void initMenu();
    void initUserPanelFromFile(const QString &fileName);
    void saveUserPanelToFile(const QString &fileName);
    void initLeftControlsList();
    void initConnections();
    QList<Frame *> findRootFrames();
    void initTreeAccordingRootFrames();
	void initUserPanelAccordingTreeWidget();

    void initOneTreeItem(Frame *frame, QTreeWidgetItem *item, int level);
	void selectTreeWidget(QList<Frame *> frameList);
	void selectOneTreeWidgetItem(QTreeWidgetItem *item, QString name);
	void selectOneFrame(Frame *frame, QString name);
	void preInitTreeWidget();

    void zoomIn(int level);
    void zoomOut(int level);
	Frame *findRootParent(Frame *_item);

private slots:
    void slot_open();
    void slot_save();
	void slot_itemPressed(QTreeWidgetItem *item, int column);
    void slot_pos(QPointF pointF);
    void slot_itemAdded(Frame *frame);
	void slot_itemSelected(QList<Frame *> frameList);
	void slot_clearAllSelected();
    void slot_zoom(int factor);
    void slot_setZoomFactor(int factor);
    void on_zoomInIcon_clicked();
    void on_zoomOutIcon_clicked();
    void on_horizontalLay_clicked();
    void on_verticalLay_clicked();

private:
    Ui::MainWindow		*ui;

	QTreeWidgetItem		*m_topLevelItem;
    int					m_horizontalNumber;
    int					m_verticalNumber;
};

#endif // MAINWINDOW_H
