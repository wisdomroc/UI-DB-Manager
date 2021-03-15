#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fwd.h"
#include "frame.h"
#include "databasemanager.h"
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

    enum DatabaseType {
        SSK,        //实时库
        DMK,        //达蒙库
        MySQL,
        SQLITE,
        SqlServer,
        Oracle
    };

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

    void saveIni();
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

    void on_pushButton_connect_clicked();

    void on_comboBox_tableNames_activated(const QString &tableName);

    void on_pushButton_add_clicked();

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void slot_setGroup();

private:
    Ui::MainWindow		*ui;
    QTreeWidgetItem     *m_topLevelItem;
    DatabaseManager     *m_databaseManager;

    int					m_horizontalNumber;
    int					m_verticalNumber;

    int                 m_indexDatabaseType;
    DatabaseType        m_databaseType;
    QString             m_ip;
    QString             m_port;
    QString             m_dbName;
    QString             m_username;
    QString             m_password;
    QString             m_dbInfos;
    QStringList         m_groupNames;
};

#endif // MAINWINDOW_H
