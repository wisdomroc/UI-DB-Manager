#include "frame.h"
#include <QPen>
#include <QWidget>

Frame::Frame(FrameType frameType, QMenu *contextMenu, QWidget *parent):QLabel(parent), m_parentFrame(nullptr),
            m_offsetX(0), m_offsetY(0), m_offsetFlag(false)
{
    m_frameType = frameType;
    m_contextMenu = contextMenu;
    switch (frameType) {
    case Table:
    case List:
    case Tree:
        //setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        break;
    case Horizontal:
    case Vertical:
        //setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        break;
    }

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor("transparent")));
    setPalette(palette);
    setAutoFillBackground(true);
}

Frame::~Frame()
{
}

Frame::FrameType Frame::getType() const
{
    return m_frameType;
}

void Frame::setParentItemS(Frame *_frame)
{
    m_parentFrame = _frame;
    m_parentFrame->addChildItem(_frame);
}

void Frame::setOriginalWidthAndHeight(int _width, int _height)
{
    m_width = _width;
    m_height = _height;
}

void Frame::setOffset(qreal x_, qreal y_)
{
    if (!m_offsetFlag)
	{
        m_offsetX = x_;
        m_offsetY = y_;
        m_offsetFlag = true;
    }
}

void Frame::setSelected(bool selected)
{
    QPalette palette;
    if(selected)
    {
        palette.setBrush(QPalette::Background, QBrush(QColor("lightGreen")));
        setPalette(palette);
    }
    else
    {
        palette.setBrush(QPalette::Background, QBrush(QColor("transparent")));
        setPalette(palette);
    }
}

void Frame::setDragType(DragType _dragType)
{
    m_dragType = _dragType;
}

void Frame::resetChildrenPos(bool firstAdjust)
{
    /*
    QList<QGraphicsItem *> children = this->childItems();
    QPointF topLeft = this->sceneBoundingRect().topLeft();
    QPointF bottomRight = this->sceneBoundingRect().bottomRight();
    FrameType frameType = this->getType();
    foreach(QGraphicsItem *item, children)
    {
        Frame *frame = qgraphicsitem_cast<Frame *>(item);
        if(frame != nullptr)
        {	//! child是Frame的情况
            QPointF offsetPoint = QPointF(frame->m_offsetX, frame->m_offsetY);
            qreal new_width = frame->m_width;
            qreal new_height = frame->m_height;
            qreal width_standard = bottomRight.x() - frame->sceneBoundingRect().x();
            qreal height_standard = bottomRight.y() - frame->sceneBoundingRect().y();
			if (firstAdjust)
			{
				if (new_height < height_standard)
				{
					new_height = height_standard;
				}
				if (new_width < width_standard)
				{
					new_width = width_standard;
				}
				frame->setOriginalWidthAndHeight(new_width, new_height);
			}

            qDebug() << "offsetPoint: " << offsetPoint << endl;
            frame->setRect(topLeft.x() + offsetPoint.x(), topLeft.y() + offsetPoint.y(), new_width, new_height);

			//! 递归查找子Item，并同样设置位置信息
			frame->resetChildrenPos(false);

		}
        else
        {	//! child是Frame的情况
            QPointF standardPoint = topLeft;
            item->setPos(standardPoint.x(), standardPoint.y());
        }
    }
    */
}

void Frame::drawing(QGraphicsSceneMouseEvent * event)
{
    QRectF r;
    QRectF re = rect();
    qDebug() << "pre rect: " << re << endl;
    if ((this->cursor().shape() == Qt::SizeHorCursor) && (m_dragType == DragL))
    {
        r = QRectF(QPointF(event->scenePos().x(), re.top()), QSizeF(re.right() - event->scenePos().x(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeHorCursor) && (m_dragType == DragR))
    {
        r = QRectF(re.topLeft(), QSizeF(event->scenePos().x() - re.left(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (m_dragType == DragT))
    {
        r = QRectF(QPointF(re.left(), event->scenePos().y()), QSizeF(re.right() - re.left(), re.bottom() - event->scenePos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (m_dragType == DragB))
    {
        r = QRectF(re.topLeft(), QSizeF(re.right() - re.left(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (m_dragType == DragLT))
    {
        r = QRectF(QPointF(event->scenePos().x(), event->scenePos().y()), QSizeF(re.right() - event->scenePos().x(), re.bottom() - event->scenePos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (m_dragType == DragRB))
    {
        r = QRectF(re.topLeft(), QSizeF(event->scenePos().x() - re.left(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (m_dragType == DragLB))
    {
        r = QRectF(QPointF(event->scenePos().x(), re.top()), QSizeF(re.right() - event->scenePos().x(), event->scenePos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (m_dragType == DragRT))
    {
        r = QRectF(QPointF(re.left(), event->scenePos().y()), QSizeF(event->scenePos().x() - re.left(), re.bottom() - event->scenePos().y()));
    }

    //setRec(r);
    resetChildrenPos(false);
    qDebug() << "after rect: " << r << endl;
}

void Frame::addChildItem(Frame *_frame)
{
    if(!m_childrenFrame.contains(_frame))
        m_childrenFrame.append(_frame);
}

void Frame::contextMenuEvent(QContextMenuEvent *event)
{
    m_contextMenu->exec(event->pos());
}
