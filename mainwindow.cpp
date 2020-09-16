#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initLeftControlsList();
    initGraphicsView();
    initUI();

    m_rootFrame = new Frame(Null, new QMenu());
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

void MainWindow::initGraphicsView()
{
    connect(ui->graphicsView, SIGNAL(zoom(int)), this, SLOT(slot_zoom(int)));
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_setZoomFactor(int)));
    connect(ui->graphicsView->scene(), SIGNAL(pos(QPointF)), this, SLOT(slot_pos(QPointF)));
    connect(ui->graphicsView->scene(), SIGNAL(itemAdded(Frame *)), this, SLOT(slot_itemAdded(Frame *)));
}

void MainWindow::zoomIn(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() + level);
}

void MainWindow::zoomOut(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() - level);
}

void MainWindow::slot_pos(QPointF pointF)
{
    ui->label_position->setText(tr("Position:（%1, %2）").arg((int)pointF.x()).arg((int)pointF.y()));
}

void MainWindow::slot_itemAdded(Frame *_frame)
{
    m_rootFrame->addChildItem(_frame);
}

void MainWindow::slot_zoom(int factor)
{
    ui->zoomSlider->setValue(250 + factor);
}

void MainWindow::slot_setZoomFactor(int factor)
{
    ui->graphicsView->setZoomFactor(factor - 250);
}

void MainWindow::on_zoomInIcon_clicked()
{
    zoomIn(5);
}

void MainWindow::on_zoomOutIcon_clicked()
{
    zoomOut(5);
}

void MainWindow::on_horizontalLay_clicked()
{
    DropGraphicsScene *graphicsScene = (DropGraphicsScene *)(ui->graphicsView->scene());
    QList<Frame *> selectedItems = graphicsScene->getSelectedItems();
    Frame *frame_new = new Frame(Horizontal, new QMenu());
    qDebug() << "selected item count: " << selectedItems.count();
    QPointF topLeft = QPointF(10000,10000);
    QPointF bottomRight = QPointF(0, 0);
    for(int i = 0; i < selectedItems.count(); i ++)
    {
        Frame *frame = selectedItems.at(i);
        QRectF rect = frame->sceneBoundingRect();
        if(rect.x() < topLeft.x())
            topLeft.rx() = rect.x();
        if(rect.y() < topLeft.y())
            topLeft.ry() = rect.y();
        if(rect.x() + rect.width() > bottomRight.x())
            bottomRight.rx() = rect.x() + rect.width();
        if(rect.y() + rect.height() > bottomRight.y())
            bottomRight.ry() = rect.y() + rect.height();
        frame->setParentItem(frame_new);
    }

    qreal width = bottomRight.x() - topLeft.x();
    qreal height = bottomRight.y() - topLeft.y();
    qreal splitWidth = width/selectedItems.size();
    qDebug() << "topLeft: " << topLeft << ", width: " << width << ", height: " << height << endl;
    for(int i = 0; i < selectedItems.count(); i ++)
    {
        QRectF splitRect(topLeft.x() + splitWidth*i, topLeft.y(), splitWidth, height);
        Frame *frame = selectedItems.at(i);
        frame->setRect(splitRect);
		frame->setOriginalWidthAndHeight(splitWidth, height);
		frame->resetChildrenPos();
    }
    frame_new->setRect(topLeft.x(), topLeft.y(), bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
	DropGraphicsScene *scene = (DropGraphicsScene *)(ui->graphicsView->scene());
	scene->addItem(frame_new);
    m_rootFrame->addChildItem(frame_new);
    //TODO.
	scene->clearAllItemSelected();
	scene->afterAddNewFrame(frame_new);
}

void MainWindow::on_verticalLay_clicked()
{
    DropGraphicsScene *graphicsScene = (DropGraphicsScene *)(ui->graphicsView->scene());
    QList<Frame *> selectedItems = graphicsScene->getSelectedItems();
    Frame *frame_new = new Frame(Vertical, new QMenu());
    qDebug() << "selected item count: " << selectedItems.count();
    QPointF topLeft = QPointF(10000,10000);
    QPointF bottomRight = QPointF(0, 0);
    for(int i = 0; i < selectedItems.count(); i ++)
    {
        Frame *frame = selectedItems.at(i);
        QRectF rect = frame->sceneBoundingRect();
        if(rect.x() < topLeft.x())
            topLeft.rx() = rect.x();
        if(rect.y() < topLeft.y())
            topLeft.ry() = rect.y();
        if(rect.x() + rect.width() > bottomRight.x())
            bottomRight.rx() = rect.x() + rect.width();
        if(rect.y() + rect.height() > bottomRight.y())
            bottomRight.ry() = rect.y() + rect.height();
        frame->setParentItem(frame_new);
    }

    qreal width = bottomRight.x() - topLeft.x();
    qreal height = bottomRight.y() - topLeft.y();
    qreal splitHeight = height/selectedItems.size();
    qDebug() << "topLeft: " << topLeft << ", width: " << width << ", height: " << height << endl;
    for(int i = 0; i < selectedItems.count(); i ++)
    {
        QRectF splitRect(topLeft.x(), topLeft.y() + splitHeight*i, width, splitHeight);
        Frame *frame = selectedItems.at(i);
        frame->setRect(splitRect);
		frame->setOriginalWidthAndHeight(width, splitHeight);
        frame->resetChildrenPos();
    }
    frame_new->setRect(topLeft.x(), topLeft.y(), bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
	DropGraphicsScene *scene = (DropGraphicsScene *)(ui->graphicsView->scene());
    scene->addItem(frame_new);
    m_rootFrame->addChildItem(frame_new);
    //TODO.
    scene->clearAllItemSelected();
	scene->afterAddNewFrame(frame_new);
}
