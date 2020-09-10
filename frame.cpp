#include "frame.h"
#include <QPen>

Frame::Frame(FrameType frameType, QMenu *contextMenu)
{
    mFrameType = frameType;
    myContextMenu = contextMenu;
    setAcceptHoverEvents(true);

    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
}

Frame::~Frame()
{

}

void Frame::setDragType(DragType _dragType)
{
    dragType = _dragType;
}

void Frame::startDraw(QGraphicsSceneMouseEvent * event)
{
    setRect(QRectF(event->scenePos(), QSizeF(0, 0)));
}

void Frame::drawing(QGraphicsSceneMouseEvent * event)
{
    QRectF r;
    QRectF re = rect();
    qDebug() << "pre rect: " << re << endl;
    if ((this->cursor().shape() == Qt::SizeHorCursor) && (dragType == DragL))
    {
        r = QRectF(QPointF(event->scenePos().x(), re.top()), QSizeF(re.right() - event->scenePos().x(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeHorCursor) && (dragType == DragR))
    {
        r = QRectF(re.topLeft(), QSizeF(event->scenePos().x() - re.left(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (dragType == DragT))
    {
        r = QRectF(QPointF(re.left(), event->scenePos().y()), QSizeF(re.right() - re.left(), re.bottom() - event->scenePos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (dragType == DragB))
    {
        r = QRectF(re.topLeft(), QSizeF(re.right() - re.left(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (dragType == DragLT))
    {
        r = QRectF(QPointF(event->scenePos().x(), event->scenePos().y()), QSizeF(re.right() - event->scenePos().x(), re.bottom() - event->scenePos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (dragType == DragRB))
    {
        r = QRectF(re.topLeft(), QSizeF(event->scenePos().x() - re.left(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (dragType == DragLB))
    {
        r = QRectF(QPointF(event->scenePos().x(), re.top()), QSizeF(re.right() - event->scenePos().x(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (dragType == DragRT))
    {
        r = QRectF(QPointF(re.left(), event->scenePos().y()), QSizeF(event->scenePos().x() - re.left(), re.bottom() - event->scenePos().y()));
    }

    setRect(r);
    qDebug() << "after rect: " << r << endl;
}

void Frame::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
