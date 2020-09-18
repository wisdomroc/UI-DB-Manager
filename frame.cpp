#include "frame.h"
#include <QPen>
#include <QWidget>

Frame::Frame(FrameType frameType, QMenu *contextMenu, QWidget *parent):QLabel(parent), m_parentFrame(nullptr),
            m_offsetX(0), m_offsetY(0), m_offsetFlag(false)
{
    m_frameType = frameType;
    m_contextMenu = contextMenu;

	//! 初始化设置为透明
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QColor("transparent")));
    setPalette(palette);
    setAutoFillBackground(true);
	setMouseTracking(true);
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


void Frame::drawing(QMouseEvent * event)
{
    QRect r;
    QRect re = geometry();
    qDebug() << "pre rect: " << re << endl;
    if ((this->cursor().shape() == Qt::SizeHorCursor) && (m_dragType == DragL))
    {
        r = QRect(QPoint(event->pos().x(), re.top()), QSize(re.right() - event->pos().x(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeHorCursor) && (m_dragType == DragR))
    {
        r = QRect(re.topLeft(), QSize(event->pos().x() - re.left(), re.bottom() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (m_dragType == DragT))
    {
        r = QRect(QPoint(re.left(), event->pos().y()), QSize(re.right() - re.left(), re.bottom() - event->pos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeVerCursor) && (m_dragType == DragB))
    {
        r = QRect(re.topLeft(), QSize(re.right() - re.left(), event->pos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (m_dragType == DragLT))
    {
        r = QRect(QPoint(event->pos().x(), event->pos().y()), QSize(re.right() - event->pos().x(), re.bottom() - event->pos().y()));
    }
    else if ((this->cursor().shape() == Qt::SizeFDiagCursor) && (m_dragType == DragRB))
    {
        r = QRect(re.topLeft(), QSize(event->pos().x() - re.left(), event->pos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (m_dragType == DragLB))
    {
        r = QRect(QPoint(event->pos().x(), re.top()), QSize(re.right() - event->pos().x(), event->pos().y() - re.top()));
    }
    else if ((this->cursor().shape() == Qt::SizeBDiagCursor) && (m_dragType == DragRT))
    {
        r = QRect(QPoint(re.left(), event->pos().y()), QSize(event->pos().x() - re.left(), re.bottom() - event->pos().y()));
    }

	qDebug() << "geometry-->" << r << endl;
    setGeometry(r);
    //resetChildrenPos(false);
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


void Frame::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawText(0, 100, this->text());
	switch (m_frameType)
	{
	case Table:
	case List:
	case Tree:
	{
		QPen pen;
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(1);
		pen.setBrush(Qt::black);
		pen.setCapStyle(Qt::RoundCap);
		pen.setJoinStyle(Qt::RoundJoin);
		painter.setPen(pen);
	}
	break;
	case Horizontal:
	case Vertical:
	{
		QPen pen;
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(1);
		pen.setBrush(Qt::red);
		pen.setCapStyle(Qt::RoundCap);
		pen.setJoinStyle(Qt::RoundJoin);
		painter.setPen(pen);
	}
	break;
	}
	painter.drawRect(0, 0, this->width()-1, this->height()-1);
}
