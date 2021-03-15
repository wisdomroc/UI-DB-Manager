#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gwriter.h"
#include "greader.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

using namespace std;
QString g_controls[] = {"**********图形**********", "列表", "树", "表格", "柱状图", "饼图", "图谱",
                        "**********容器**********", "Group Box", "Scroll Area", "Tab Widget", "Stacked Widget", "Frame",
                        "**********按钮**********", "Push Button", "Radio Button", "Check Box",
                        "**********输入**********", "Combo Box", "Line Edit", "Text Edit", "Spin Box", "Date/Time Edit",
                        "**********显示**********", "Label", "LCD Number", "Progress Bar", "Horizontal Line", "Vertical Line"};
map<QString, QString> g_controlsPngMap = {map<QString, QString>::value_type("列表", "list"),
                                          map<QString, QString>::value_type("树", "tree"),
                                          map<QString, QString>::value_type("表格", "table"),
                                          map<QString, QString>::value_type("柱状图", "columnchart"),
                                          map<QString, QString>::value_type("饼图", "piechart"),
                                          map<QString, QString>::value_type("图谱", "tupu")};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),m_horizontalNumber(0), m_verticalNumber(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initLeftControlsList();
    initConnections();
    initUI();
    initMenu();

    m_databaseManager = new DatabaseManager();
    ui->comboBox_databaseType->setCurrentIndex(2);//! 这里测试，使用已有的MySQL库
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);
    ui->zoomSlider->setMinimum(0);
    ui->zoomSlider->setMaximum(500);
    ui->zoomSlider->setValue(250);
    ui->zoomSlider->setTickInterval(5);
    ui->zoomSlider->setTickPosition(QSlider::TicksRight);
    ui->zoomInIcon->setAutoRepeat(true);
    ui->zoomInIcon->setAutoRepeatInterval(100);
    ui->zoomInIcon->setAutoRepeatDelay(1000);
    ui->zoomInIcon->setIcon(QPixmap(":/image/zoomin.png"));
    ui->zoomInIcon->setIconSize(iconSize);
    ui->zoomOutIcon->setAutoRepeat(true);
    ui->zoomOutIcon->setAutoRepeatInterval(100);
    ui->zoomOutIcon->setAutoRepeatDelay(1000);
    ui->zoomOutIcon->setIcon(QPixmap(":/image/zoomout.png"));
    ui->zoomOutIcon->setIconSize(iconSize);
    ui->verticalLay->setAutoRepeat(true);
    ui->verticalLay->setAutoRepeatInterval(100);
    ui->verticalLay->setAutoRepeatDelay(1000);
    ui->verticalLay->setIcon(QPixmap(":/image/vertical.png"));
    ui->verticalLay->setIconSize(iconSize);
    ui->horizontalLay->setAutoRepeat(true);
    ui->horizontalLay->setAutoRepeatInterval(100);
    ui->horizontalLay->setAutoRepeatDelay(1000);
    ui->horizontalLay->setIcon(QPixmap(":/image/horizontal.png"));
    ui->horizontalLay->setIconSize(iconSize);

    ui->zoomInIcon->setVisible(false);
    ui->zoomOutIcon->setVisible(false);
    ui->zoomSlider->setVisible(false);

    ui->lineEdit_connectResult->setEnabled(false);

    QSettings settings(tr("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
    qDebug() << "ApplicationPath: " << QCoreApplication::applicationDirPath();
    settings.beginGroup("DatabaseConfig");
    m_ip = settings.value("IP").toString();
    m_port = settings.value("PORT").toString();
    m_dbName = settings.value("DATABASE").toString();
    m_username = settings.value("USERNAME").toString();
    m_password = settings.value("PASSWORD").toString();
    settings.endGroup();

    ui->lineEdit_ip->setText(m_ip);
    ui->lineEdit_port->setText(m_port);
    ui->lineEdit_databaseName->setText(m_dbName);
    ui->lineEdit_username->setText(m_username);
    ui->lineEdit_password->setText(m_password);
}

void MainWindow::saveIni()
{
    QSettings settings(tr("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
    settings.beginGroup("DatabaseConfig");
    settings.setValue("IP", m_ip);
    settings.setValue("PORT", m_port);
    settings.setValue("DATABASE", m_dbName);
    settings.setValue("USERNAME", m_username);
    settings.setValue("PASSWORD", m_password);
    settings.endGroup();
}

void MainWindow::initMenu()
{
    QMenu *menu1 = ui->menuBar->addMenu(tr("文件"));
    QAction *action_open = menu1->addAction(tr("打开"));
    QAction *action_save = menu1->addAction(tr("保存"));
    QAction *action_close = menu1->addAction(tr("关闭"));
    connect(action_open, &QAction::triggered, this, &MainWindow::slot_open);
    connect(action_save, &QAction::triggered, this, &MainWindow::slot_save);
    connect(action_close, &QAction::triggered, [=](){ qApp->quit(); });
}

void MainWindow::initLeftControlsList()
{
    ui->listWidget_controls->clear();
    for(int i = 0 ; i < g_controls->size(); i ++)
    {
        QString control_name = g_controls[i];
        QString control_png_name = g_controlsPngMap[control_name];
        QPixmap pixmap(tr(":/image/%1.png").arg(control_png_name));
        ui->listWidget_controls->addPiece(pixmap, control_png_name);
    }
}

void MainWindow::initConnections()
{
    connect(ui->userpanel, SIGNAL(itemAdded(Frame *)), this, SLOT(slot_itemAdded(Frame *)));
    connect(ui->userpanel, SIGNAL(itemSelected(QList<Frame *>)), this, SLOT(slot_itemSelected(QList<Frame *>)));
    connect(ui->userpanel, SIGNAL(clearAllSelected()), this, SLOT(slot_clearAllSelected()));
    connect(ui->userpanel, SIGNAL(pos(QPointF)), this, SLOT(slot_pos(QPointF)));
    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int )), this, SLOT(slot_itemPressed(QTreeWidgetItem *, int )));
}

QList<Frame *> MainWindow::findRootFrames()
{
    QList<Frame *> rootFrameList;

    QWidget *userpanel = ui->userpanel;
    QObjectList objList = userpanel->children();
    foreach(QObject *obj, objList)
    {
        Frame *frame = qobject_cast<Frame *>(obj);
        if(frame != nullptr)
        {
            Frame *oneRootFrame = findRootParent(frame);
            if(!rootFrameList.contains(oneRootFrame))
            {
                rootFrameList.append(oneRootFrame);
            }
        }
    }

    return rootFrameList;
}

void MainWindow::initOneTreeItem(Frame *frame, QTreeWidgetItem *item, int level)
{
    QStringList names;
    Frame::FrameType frameType = frame->getType();
    switch (frameType) {
    case Frame::Horizontal:
    {
        QHBoxLayout *hBoxLayout = (QHBoxLayout *)(frame->layout());
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(item, QStringList() << hBoxLayout->objectName());
        treeWidgetItem->setData(0, Qt::UserRole, tr("LEVEL_%1").arg(level));
        treeWidgetItem->setData(0, Qt::UserRole + 1, tr("%1,%2").arg(frame->pos().x()).arg(frame->pos().y()));
        treeWidgetItem->setData(0, Qt::UserRole + 2, tr("%1,%2").arg(frame->size().width()).arg(frame->size().height()));
        treeWidgetItem->setExpanded(true);
        item->addChild(treeWidgetItem);
        int childCount = hBoxLayout->count();

        ++level;
        for(int i = 0; i < childCount; i ++)
        {
            QLayoutItem *layoutItem = hBoxLayout->itemAt(i);
            QWidget *widget = layoutItem->widget();
            Frame *frame = (Frame *)(widget);
            initOneTreeItem(frame, treeWidgetItem, level);
        }
    }
        break;
    case Frame::Vertical:
    {
        QVBoxLayout *vBoxLayout = (QVBoxLayout *)(frame->layout());
        
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(item, QStringList() << vBoxLayout->objectName());
        treeWidgetItem->setData(0, Qt::UserRole, tr("LEVEL_%1").arg(level));
        treeWidgetItem->setData(0, Qt::UserRole + 1, tr("%1,%2").arg(frame->pos().x()).arg(frame->pos().y()));
        treeWidgetItem->setData(0, Qt::UserRole + 2, tr("%1,%2").arg(frame->size().width()).arg(frame->size().height()));
        treeWidgetItem->setExpanded(true);
        item->addChild(treeWidgetItem);
        int childCount = vBoxLayout->count();

        ++level;
        for(int i = 0; i < childCount; i ++)
        {
            QLayoutItem *layoutItem = vBoxLayout->itemAt(i);
            QWidget *widget = layoutItem->widget();
            Frame *frame = (Frame *)(widget);
            initOneTreeItem(frame, treeWidgetItem, level);
        }
    }
        break;
    case Frame::Table:
    case Frame::List:
    case Frame::Tree:
    {
        QString name = frame->objectName();
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(item, QStringList() << name);
        treeWidgetItem->setData(0, Qt::UserRole, tr("LEVEL_%1").arg(level));
        treeWidgetItem->setExpanded(true);
        item->addChild(treeWidgetItem);

    }
        break;
    case Frame::Null:

        break;
    }
}

void MainWindow::preInitTreeWidget()
{
    ui->treeWidget->clear();
    m_topLevelItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << "userpanel");
    m_topLevelItem->setData(0, Qt::UserRole, tr("LEVEL_0"));
    m_topLevelItem->setExpanded(true);
    ui->treeWidget->addTopLevelItem(m_topLevelItem);
}

void MainWindow::initTreeAccordingRootFrames()
{
    preInitTreeWidget();

    QList<Frame *> rootFrames = findRootFrames();
    for(int i = 0; i < rootFrames.count(); i ++)
    {
        QStringList names;
        Frame *frame = rootFrames.at(i);
        initOneTreeItem(frame, m_topLevelItem, 1);
    }
}

void MainWindow::initUserPanelAccordingTreeWidget()
{
    int count = ui->treeWidget->topLevelItemCount();
    if (count != 1)
    {
        QMessageBox::information(this, tr("注意"), tr("根节点不止一个"));
        return;
    }
    else
    {
        m_topLevelItem = ui->treeWidget->topLevelItem(0);
        if (m_topLevelItem->text(0) == "userpanel")
        {
            ui->userpanel->initUserPanelAccordindOneTreeWidgetItem(m_topLevelItem, NULL);
        }
    }
}

void MainWindow::zoomIn(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() + level);
}

void MainWindow::zoomOut(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() - level);
}

void MainWindow::slot_itemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    QString name = item->text(0);
    QList<Frame *> frameList = findRootFrames();

    ui->userpanel->clearAllItemSelected();
    for (int i = 0; i < frameList.count(); i++)
    {
        selectOneFrame(frameList.at(i), name);
    }
}

void MainWindow::slot_pos(QPointF pointF)
{
    ui->label_position->setText(tr("Position:（%1, %2）").arg((int)pointF.x()).arg((int)pointF.y()));
}

void MainWindow::slot_itemAdded(Frame *frame)
{
    /*
    preInitTreeWidget();

    QList<Frame *> rootFrames = findRootFrames();
    for (int i = 0; i < rootFrames.count(); i++)
    {
        QStringList names;
        Frame *frame = rootFrames.at(i);
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(m_topLevelItem, QStringList() << frame->objectName());
        treeWidgetItem->setData(0, Qt::UserRole, 1);
        treeWidgetItem->setExpanded(true);
        m_topLevelItem->addChild(treeWidgetItem);
    }
    */

    initTreeAccordingRootFrames();
}

void MainWindow::slot_itemSelected(QList<Frame *> frameList)
{	//! TODO.
    selectTreeWidget(frameList);
}

void MainWindow::slot_clearAllSelected()
{
    ui->treeWidget->clearSelection();
}

void MainWindow::slot_zoom(int factor)
{
    ui->zoomSlider->setValue(250 + factor);
}

void MainWindow::slot_setZoomFactor(int factor)
{
    //ui->graphicsView->setZoomFactor(factor - 250);
}

void MainWindow::on_zoomInIcon_clicked()
{
    zoomIn(5);
}

void MainWindow::on_zoomOutIcon_clicked()
{
    zoomOut(5);
}

Frame *MainWindow::findRootParent(Frame *_item)
{
    QObject *parentItem = _item->parent();
    Frame *frame = qobject_cast<Frame *>(parentItem);
    if (frame == nullptr)
    {
        return _item;
    }
    else
    {
        return findRootParent(frame);
    }
}

void MainWindow::slot_open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开"), "", tr("XML (*.xml);;TET (*.txt)"));
    if(fileName.isEmpty())
        return;

    initUserPanelFromFile(fileName);
}

void MainWindow::slot_save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "", tr("XML (*.xml);;TXT (*.txt)"));
    if(fileName.isEmpty())
        return;

    saveUserPanelToFile(fileName);
}

void MainWindow::initUserPanelFromFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }

    GReader reader(ui->treeWidget);
    if (!reader.read(&file))
    {
        QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                             tr("Parse error in file %1:\n\n%2")
                             .arg(QDir::toNativeSeparators(fileName),
                                  reader.errorString()));
    }
    else
    {
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    initUserPanelAccordingTreeWidget();
}

void MainWindow::saveUserPanelToFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QXmlStream G"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }

    GWriter writer(ui->treeWidget);
    if (writer.writeFile(&file))
    {
        QMessageBox::information(this, tr("提示"), tr("保存文件成功！"));
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void MainWindow::on_horizontalLay_clicked()
{
    QList<Frame *> selectedItems = ui->userpanel->getSelectedItems();
    if (selectedItems.count() == 0)
    {
        QHBoxLayout *hBoxLayout = new QHBoxLayout(ui->userpanel);
        hBoxLayout->setObjectName(tr("horizontal_%1").arg(m_horizontalNumber++));
        QList<Frame *> rootFrames = findRootFrames();
        for (int i = 0; i < rootFrames.count(); i++)
        {
            QStringList names;
            Frame *frame = rootFrames.at(i);
            hBoxLayout->addWidget(frame);
        }
        ui->userpanel->setLayout(hBoxLayout);
        return;
    }


    Frame *hFrame = new Frame(Frame::Horizontal, new QMenu(), ui->userpanel);
    hFrame->setVisible(true);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(hFrame);
    hBoxLayout->setObjectName(tr("horizontal_%1").arg(m_horizontalNumber ++));
    hBoxLayout->setMargin(0);
    hBoxLayout->setSpacing(6);
    qDebug() << "selected item count: " << selectedItems.count();
    QPointF topLeft = QPointF(10000,10000);
    QPointF bottomRight = QPointF(0, 0);
    int widthCount = 0;
    int heightCount = 0;
    for(int i = 0; i < selectedItems.count(); i ++)
    {
        Frame *frame = selectedItems.at(i);
        QRect rect = frame->geometry();
        qDebug() << "number: " << i << ", geometry:" << rect << endl;
        QPoint topLeft_ = rect.topLeft();
        int width = rect.width();
        int height = rect.height();
        widthCount += width;
        if(heightCount < height)
            heightCount = height;


        if(topLeft_.x() < topLeft.x())
            topLeft.rx() = topLeft_.x();
        if(topLeft_.y() < topLeft.y())
            topLeft.ry() = topLeft_.y();
        if(topLeft_.x() + width > bottomRight.x())
            bottomRight.rx() = topLeft_.x() + width;
        if(topLeft_.y() + width > bottomRight.y())
            bottomRight.ry() = topLeft_.y() + height;
        hBoxLayout->addWidget(frame);
        if (frame->getType() == Frame::Horizontal)
            frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        else
            frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    }
    widthCount += 6 * (selectedItems.count() - 1);

    ui->userpanel->clearAllItemSelected();
    ui->userpanel->addItemToSelected(hFrame);
    qDebug() << topLeft << "," << widthCount << "," << heightCount << endl;
    hFrame->setGeometry(QRect(topLeft.x(), topLeft.y(), widthCount, heightCount));


    initTreeAccordingRootFrames();
}

void MainWindow::on_verticalLay_clicked()
{
    QList<Frame *> selectedItems = ui->userpanel->getSelectedItems();
    if (selectedItems.count() == 0)
    {
        QVBoxLayout *vBoxLayout = new QVBoxLayout(ui->userpanel);
        vBoxLayout->setObjectName(tr("vertical_%1").arg(m_verticalNumber++));
        QList<Frame *> rootFrames = findRootFrames();
        for (int i = 0; i < rootFrames.count(); i++)
        {
            QStringList names;
            Frame *frame = rootFrames.at(i);
            vBoxLayout->addWidget(frame);
        }
        ui->userpanel->setLayout(vBoxLayout);
        return;
    }



    Frame *hFrame = new Frame(Frame::Vertical, new QMenu(), ui->userpanel);
    hFrame->setVisible(true);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(hFrame);
    vBoxLayout->setObjectName(tr("vertical_%1").arg(m_verticalNumber ++));
    vBoxLayout->setMargin(0);
    vBoxLayout->setSpacing(6);
    qDebug() << "selected item count: " << selectedItems.count();
    QPointF topLeft = QPointF(10000, 10000);
    QPointF bottomRight = QPointF(0, 0);
    int widthCount = 0;
    int heightCount = 0;
    for (int i = 0; i < selectedItems.count(); i++)
    {
        Frame *frame = selectedItems.at(i);
        QRect rect = frame->geometry();
        qDebug() << "number: " << i << ", geometry:" << rect << endl;
        QPoint topLeft_ = rect.topLeft();
        int width = rect.width();
        int height = rect.height();
        if(widthCount < width)
            widthCount = width;
        heightCount += height;


        if (topLeft_.x() < topLeft.x())
            topLeft.rx() = topLeft_.x();
        if (topLeft_.y() < topLeft.y())
            topLeft.ry() = topLeft_.y();
        if (topLeft_.x() + width > bottomRight.x())
            bottomRight.rx() = topLeft_.x() + width;
        if (topLeft_.y() + width > bottomRight.y())
            bottomRight.ry() = topLeft_.y() + height;
        vBoxLayout->addWidget(frame);
        if (frame->getType() == Frame::Vertical)
            frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        else
            frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }
    heightCount += 6 * (selectedItems.count() - 1);

    ui->userpanel->clearAllItemSelected();
    ui->userpanel->addItemToSelected(hFrame);

    hFrame->setGeometry(QRect(topLeft.x(), topLeft.y(), widthCount, heightCount));
    initTreeAccordingRootFrames();
}

void MainWindow::selectTreeWidget(QList<Frame *> frameList)
{
    ui->treeWidget->clearSelection();

    QString name;
    Frame::FrameType frameType;
    Frame *frame;
    for (int i = 0; i < frameList.count(); i++)
    {
        frame = frameList.at(i);
        frameType = frameList.at(i)->getType();
        if (frameType == Frame::Horizontal || frameType == Frame::Vertical)
        {
            name = frame->layout()->objectName();
        }
        else
        {
            name = frame->objectName();
        }
        selectOneTreeWidgetItem(m_topLevelItem, name);
    }
}

void MainWindow::selectOneTreeWidgetItem(QTreeWidgetItem *item, QString name)
{
    int count = item->childCount();
    for (int i = 0; i < count; i++)
    {
        QTreeWidgetItem *_item = item->child(i);
        if (_item->text(0) == name)
        {
            _item->setSelected(true);
            return;
        }
        int _count = _item->childCount();
        if (_count != 0)
        {
            selectOneTreeWidgetItem(_item, name);
        }
    }
}

void MainWindow::selectOneFrame(Frame *frame, QString name)
{
    Frame::FrameType frameType = frame->getType();
    if (frameType == Frame::Vertical || frameType == Frame::Horizontal)
    {
        QString layoutName = frame->layout()->objectName();
        if (layoutName == name)
        {
            ui->userpanel->addItemToSelected(frame);
            return;
        }
        else
        {
            int count = frame->layout()->count();
            for (int i = 0; i < count; i++)
            {
                QWidget *widget = frame->layout()->itemAt(i)->widget();
                Frame *_frame = qobject_cast<Frame *>(widget);
                selectOneFrame(_frame, name);
            }
        }
    }
    else
    {
        QString _name = frame->objectName();
        if (_name == name)
        {
            ui->userpanel->addItemToSelected(frame);
            return;
        }
        else
        {
            QObjectList objList = frame->children();
            for (int i = 0; i < objList.count(); i++)
            {
                Frame *_frame = qobject_cast<Frame *>(objList.at(i));
                selectOneFrame(_frame, name);
            }
        }
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    m_indexDatabaseType = ui->comboBox_databaseType->currentIndex();
    switch (m_indexDatabaseType) {
    case 0:
        m_databaseType = SSK;
        break;
    case 1:
        m_databaseType = DMK;
        break;
    case 2:
        m_databaseType = MySQL;
        break;
    case 3:
        m_databaseType = SQLITE;
        break;
    case 4:
        m_databaseType = SqlServer;
        break;
    case 5:
        m_databaseType = Oracle;
        break;
    default:
        m_databaseType = MySQL;
        break;
    }

    m_ip = ui->lineEdit_ip->text();
    m_port = ui->lineEdit_port->text();
    m_dbName = ui->lineEdit_databaseName->text();
    m_username = ui->lineEdit_username->text();
    m_password = ui->lineEdit_password->text();
    m_dbInfos = tr("%1;%2;%3;%4;%5;%6").arg(m_indexDatabaseType).arg(m_ip).arg(m_port).arg(m_dbName).arg(m_username).arg(m_password);

    saveIni();
    bool ok = m_databaseManager->initDatabase(m_ip, m_port.toInt(), m_dbName, m_username, m_password);
    if(ok)
    {
        ui->lineEdit_connectResult->setText(tr("连接成功！"));
        ui->lineEdit_connectResult->setStyleSheet("color: \"green\"");
    }
    else
    {
        ui->lineEdit_connectResult->setText(tr("连接失败！"));
        ui->lineEdit_connectResult->setStyleSheet("color: \"red\"");
        return;
    }

    QStringList tableNames = m_databaseManager->getTables(ui->lineEdit_databaseName->text());
    qDebug() << "tableNames-->" << tableNames;
    ui->comboBox_tableNames->clear();
    ui->comboBox_tableNames->addItems(tableNames);

    m_groupNames = m_databaseManager->getGroupNames(m_dbInfos);
    ui->comboBox_show->clear();
    ui->comboBox_show->addItems(m_groupNames);
}

void MainWindow::on_comboBox_tableNames_activated(const QString &tableName)
{
    QStringList columnNames = m_databaseManager->getTableColumns(ui->lineEdit_databaseName->text(), tableName);
    //! 先删除已有的项目
    int child_count = ui->gridLayout_map->count();
    for(int i = child_count - 1; i >= 0; i --)
    {
        QLayoutItem *item = ui->gridLayout_map->itemAt(i);
        QWidget *widget = item->widget();
        delete widget;
        widget = NULL;
        ui->gridLayout_map->removeItem(item);
    }
    //! 再添加新的项目
    for(int i = 0; i < columnNames.size(); i ++)
    {
        QLabel *label = new QLabel();
        label->setText(columnNames.at(i));
        QLineEdit *lineEdit = new QLineEdit();
        ui->gridLayout_map->addWidget(label,i,0);
        ui->gridLayout_map->addWidget(lineEdit,i,1);
    }
}

void MainWindow::on_pushButton_add_clicked()
{
    QString table_name = ui->comboBox_tableNames->currentText();
    QStringList table_columns_list;
    QStringList field_names_list;
    QString table_columns;
    QString field_names;
    int row_count = ui->gridLayout_map->rowCount();
    for(int i = 0; i < row_count; i ++)
    {
        QLabel *label = qobject_cast<QLabel *>(ui->gridLayout_map->itemAtPosition(i, 0)->widget());
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(ui->gridLayout_map->itemAtPosition(i, 1)->widget());
        QString column_name = label->text();
        QString field_name = lineEdit->text();
        if(field_name == "")
        {
            continue;
        }
        else
        {
            table_columns_list.append(column_name);
            field_names_list.append(field_name);
        }
    }
    table_columns = table_columns_list.join(";");
    field_names = field_names_list.join(";");
    QString title_name = ui->lineEdit_title->text();

    int ret = m_databaseManager->saveGroupInfo(m_dbInfos, table_name, table_columns, field_names, title_name);
    switch (ret) {
    case 0:
    {
        QMessageBox::information(this, tr("提示"), tr("添加成功！"));
        m_groupNames = m_databaseManager->getGroupNames(m_dbInfos);
        ui->comboBox_show->clear();
        ui->comboBox_show->addItems(m_groupNames);
    }
        break;
    case -2:
        QMessageBox::information(this, tr("提示"), tr("名称重复！"));
        break;
    default:
        QMessageBox::information(this, tr("提示"), tr("添加失败！"));
        break;
    }


}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);
    if(curItem == NULL)
    {
        return;
    }
    QVariant var = curItem->data(0,Qt::UserRole);
    if(0 != var)
    {
        QMenu *popMenu =new QMenu(this);
        QMenu *root = new QMenu(tr("设置group"), popMenu);
        for(int i = 0; i < m_groupNames.size(); i ++)
        {
            QAction *action = new QAction(m_groupNames.at(i), root);
            connect(action, SIGNAL(triggered()), this, SLOT(slot_setGroup()));
            root->addAction(action);
        }
        popMenu->addMenu(root);
        popMenu->exec(QCursor::pos());
    }
}

void MainWindow::slot_setGroup()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString title_name = action->text();
    qDebug() << "title_name: " << title_name;

    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    QPixmap pixmap(":/image/ui_db_manager.ico");
    pixmap = pixmap.scaled(20, 20);

    curItem->setData(0, Qt::DecorationRole, pixmap);
    curItem->setData(0, Qt::UserRole + 3, title_name);
}
