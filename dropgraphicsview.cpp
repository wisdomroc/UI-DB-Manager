﻿#include "dropgraphicsview.h"
#include <QtWidgets>

DropGraphicsView::DropGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    m_scene = new DropGraphicsScene();
    setScene(m_scene);
    setSceneRect(0,0,this->width(),this->height());
}

void DropGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

DropGraphicsScene::DropGraphicsScene(QWidget *parent) : QGraphicsScene(parent)
{

}

void DropGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void DropGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "scenePos: " << event->scenePos() << ", pos: " << event->pos() << endl;
    event->accept();
}

void DropGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
    {
        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        dataStream >> piece.type >> piece.pixmap;
        QPointF point = event->scenePos();

        if(piece.type == "list")
        {

        }
        else if(piece.type == "tree")
        {

        }
        else if(piece.type == "table")
        {
            addTable(point, 300, 120);
        }
        else if(piece.type == "columnchart")
        {

        }
        else if(piece.type == "piechart")
        {

        }
        else if(piece.type == "tupu")
        {

        }



        event->setDropAction(Qt::MoveAction);
        event->accept();


    }
}

void DropGraphicsScene::addTable(const QPointF &point, int width, int height)
{
    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(3);
    pen.setBrush(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    QRadialGradient gradient(50, 50, 50, 50, 50);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    QBrush brush(gradient);
    QGraphicsRectItem *item = addRect(point.x(), point.y(), width, height, pen, brush);
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    m_tableItems.append(item);
}

