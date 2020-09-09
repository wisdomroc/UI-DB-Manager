#include "frame.h"
#include <QPen>

Frame::Frame(FrameType frameType, QMenu *contextMenu)
{
	setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	mFrameType = frameType;
	myContextMenu = contextMenu;
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
    if ((this->cursor().shape() == Qt::SizeHorCursor) && (dragType == DragL))
	{
		r = QRectF(QPointF(event->scenePos().x(), rect().top()), QSizeF(rect().right() - event->scenePos().x(), rect().bottom() - rect().top()));
	}
    else if ((this->cursor().shape() == Qt::SizeHorCursor) && (dragType == DragR))
	{
		r = QRectF(rect().topLeft(), QSizeF(event->scenePos().x() - rect().left(), rect().bottom() - rect().top()));
	}
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (dragType == DragT))
	{
		r = QRectF(QPointF(rect().left(), event->scenePos().y()), QSizeF(rect().right() - rect().left(), rect().bottom() - event->scenePos().y()));
	}
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (dragType == DragB))
	{
		r = QRectF(rect().topLeft(), QSizeF(rect().right() - rect().left(), event->scenePos().y() - rect().top()));
	}
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (dragType == DragLT))
	{
		r = QRectF(QPointF(event->scenePos().x(), event->scenePos().y()), QSizeF(rect().right() - event->scenePos().x(), rect().bottom() - event->scenePos().y()));
	}
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (dragType == DragRB))
	{
		r = QRectF(rect().topLeft(), QSizeF(event->scenePos().x() - rect().left(), event->scenePos().y() - rect().top()));
	}
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (dragType == DragLB))
	{
		r = QRectF(QPointF(event->scenePos().x(), rect().top()), QSizeF(rect().right() - event->scenePos().x(), event->scenePos().y() - rect().top()));
	}
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (dragType == DragRT))
	{
		r = QRectF(QPointF(rect().left(), event->scenePos().y()), QSizeF(event->scenePos().x() - rect().left(), rect().bottom() - event->scenePos().y()));
    }
	
	setRect(r);
}

void Frame::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);

	myContextMenu->exec(event->screenPos());	
}
