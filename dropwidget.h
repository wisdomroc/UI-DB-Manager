#ifndef DROPWIDGET_H
#define DROPWIDGET_H

#include "mainwindow.h"
#include <QWidget>
QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropWidget(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    struct Piece {
        QPixmap pixmap;
        QString type;
    };
};

#endif // DROPWIDGET_H
