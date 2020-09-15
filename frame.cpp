#include "frame.h"
#include <QPen>

Frame::Frame(FrameType frameType, QMenu *contextMenu):parentFrame(nullptr),
            x_offset(0), y_offset(0)
{
    mFrameType = frameType;
    myContextMenu = contextMenu;
    setAcceptHoverEvents(true);
    switch (frameType) {
    case Rect1:
    case Rect2:
    case Rect3:
        setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        break;
    case Horizontal:
    case Vertical:
        setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        break;
    }

}

Frame::~Frame()
{

}

void Frame::setParentItemS(Frame *frame)
{
    parentFrame = frame;
}

void Frame::setOffset(qreal x_, qreal y_)
{
    x_offset = x_;
    y_offset = y_;
}

void Frame::setDragType(DragType _dragType)
{
    dragType = _dragType;
}

void Frame::startDraw(QGraphicsSceneMouseEvent * event)
{
    setRect(QRectF(event->scenePos(), QSizeF(0, 0)));
}

void Frame::resetChildrenPos()
{
    QList<QGraphicsItem *> children = this->childItems();
    QPointF topLeft = this->sceneBoundingRect().topLeft();
    foreach(QGraphicsItem *item, children)
    {
        Frame *frame = qgraphicsitem_cast<Frame *>(item);
        if(frame != nullptr)
        {
            QPointF offsetPoint = QPointF(frame->x_offset, frame->y_offset);
            qDebug() << "offsetPoint: " << offsetPoint << endl;
			frame->setRect(topLeft.x() + offsetPoint.x(), topLeft.y() + offsetPoint.y(), frame->rect().width(), frame->rect().height());
			//! Frame的标题child也随之移动
			QGraphicsItem *childItem = frame->childItems().first();
			QPointF standardPoint = frame->sceneBoundingRect().topLeft();
			childItem->setPos(standardPoint.x(), standardPoint.y());
		}
        else
        {
            QPointF standardPoint = topLeft;
            item->setPos(standardPoint.x(), standardPoint.y());
        }
    }
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
    resetChildrenPos();
    qDebug() << "after rect: " << r << endl;
}

void Frame::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
