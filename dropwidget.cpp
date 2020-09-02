#include "dropwidget.h"
#include <QtWidgets>

DropWidget::DropWidget(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);
}

void DropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void DropWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
    {

        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        dataStream >> piece.type >> piece.pixmap;

        if(piece.type == "表格")
        {

        }




        event->setDropAction(Qt::MoveAction);
        event->accept();


    }
}
