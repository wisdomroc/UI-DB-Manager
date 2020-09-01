#include "mainwindow.h"
#include "ui_mainwindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initLeftControlsList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLeftControlsList()
{
    QStringList controls;
    controls << "**********图形**********" << "列表" << "树" << "表格" << "柱状图" << "饼图";
    controls << "**********容器**********" << "Group Box" << "Scroll Area" << "Tab Widget" << "Stacked Widget" << "Frame";
    controls << "**********按钮**********" << "Push Button" << "Radio Button" << "Check Box";
    controls << "**********输入**********" << "Combo Box" << "Line Edit" << "Text Edit" << "Spin Box" << "Date/Time Edit";
    controls << "**********显示**********" << "Label" << "LCD Number" << "Progress Bar" << "Horizontal Line" << "Vertical Line";

    ui->listWidget_controls->clear();
    for(int i = 0 ; i < controls.count(); i ++)
    {
        QString info = controls.at(i);
        QListWidgetItem *item;
        item = new QListWidgetItem();
        QPixmap pixmap;
        pixmap.load(":/image/list.png");
        if(info.contains("*"))
        {
            item->setFlags(Qt::NoItemFlags);
        }
        else
        {
            item->setIcon(QIcon(pixmap));
            item->setFlags(Qt::ItemIsEnabled);
        }
        item->setText(info);
        ui->listWidget_controls->addItem(item);
    }


}
