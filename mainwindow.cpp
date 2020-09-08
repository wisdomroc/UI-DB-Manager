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
}

void MainWindow::zoomIn(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() + level);
}

void MainWindow::zoomOut(int level)
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() - level);
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
