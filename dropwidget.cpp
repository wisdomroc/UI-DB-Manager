#include "dropwidget.h"
#include "chip.h"

DropWidget::DropWidget(QWidget *parent) : QWidget(parent)
            ,m_curFrame(nullptr), is_drag(false), is_move(false)
{
    SetMouseMenu();
    setAcceptDrops(true);
	setMouseTracking(true);
}

QList<Frame *> DropWidget::getSelectedItems()
{
    return m_selectedItems;
}

void DropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void DropWidget::SetMouseMenu()
{
    QAction *deleteAction = new QAction(QIcon(":/Resources/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    QAction *modifyAction = new QAction(QIcon(":/Resources/modify.png"), tr("&Modify"), this);
    modifyAction->setShortcut(tr("Modify"));
    modifyAction->setStatusTip(tr("Modify item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(slot_deleteItem()));
    connect(modifyAction, SIGNAL(triggered()), this, SLOT(slot_modifyItem()));
    myItemMenu = new QMenu("&Item");
    myItemMenu->addAction(deleteAction);
    myItemMenu->addAction(modifyAction);
}

void DropWidget::slot_deleteItem()
{

}

void DropWidget::DeleteItem()
{

}

void DropWidget::slot_modifyItem()
{

}

void DropWidget::addItemToSelected(Frame *frame)
{
    if(!m_selectedItems.contains(frame))
    {
        m_selectedItems.append(frame);
    }
}

void DropWidget::ModifyItem()
{

}

void DropWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "pos: " << event->pos() << endl;
    event->accept();
}

void DropWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
    {
        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        dataStream >> piece.type >> piece.pixmap;
        QPointF point = (event->pos());

        if(piece.type == "list")
        {
//            Chip *chip = new Chip(QColor("steelBlue"), 100, 100);
//            addItem(chip);
        }
        else if(piece.type == "tree")
        {

        }
        else if(piece.type == "table")
        {
            addTable(point);
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

void DropWidget::addTable(const QPointF &point)
{
    Frame *label = new Frame(Frame::Table, new QMenu(), this);
	label->setText("TableWidget");
    label->setMinimumSize(200, 200);
	label->setFrameShape(QFrame::Box);
	label->setFrameShadow(QFrame::Plain);
	label->move(point.toPoint());
	label->setVisible(true);

    emit itemAdded();
}

void DropWidget::clearAllItemSelected()
{
    //! 清除之前的Item
    for (int i = 0; i < m_selectedItems.count(); i++)
    {
        Frame *frame = m_selectedItems.at(i);
		QPalette palette;
		palette.setBrush(QPalette::Background, QBrush(QColor("transparent")));
		frame->setPalette(palette);
    }
    m_selectedItems.clear();
}

void DropWidget::afterAddNewFrame(Frame *_frame)
{
    //! 添加当前的Item
    if (!m_selectedItems.contains(_frame))
    {
        m_selectedItems.append(_frame);
    }

    //! 设置被选中

	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QColor("transparent")));
	_frame->setPalette(palette);

    setChildInfo(_frame);
}

Frame *DropWidget::findRootParent(Frame *_item)
{
    QObject *parentItem = _item->parent();
	Frame *frame = qobject_cast<Frame *>(parentItem);
    if (frame == nullptr)
    {
        return _item;
    }
    else
    {
        return findRootParent(frame);
    }
}

// 鼠标按下获取当前单选中的QGraphicsProxyWidget图元对象
void DropWidget::mousePressEvent(QMouseEvent *event)
{
    
    if (event->button() == Qt::LeftButton)
    {
		
		QWidget *childWidget = this->childAt(event->pos());
        //! 按住Ctrl键
        if (QApplication::keyboardModifiers() != Qt::ControlModifier)
        {
            if(m_selectedItems.count() != 0)
            {
                foreach(Frame *frame, m_selectedItems)
                {
                    frame->setSelected(false);
                    frame = nullptr;
                }
				m_selectedItems.clear();
                if(m_curFrame != nullptr)
                {
                    m_curFrame->setSelected(false);
                    m_curFrame = nullptr;
                }
            }
        }
        if (childWidget == nullptr)
        {
			return;
        }
		
		m_curFrame = (Frame *)childWidget;
		m_curFrame = findRootParent(m_curFrame);
        m_curFrame->setSelected(true);
        addItemToSelected(m_curFrame);
		qDebug() << "m_curFrame.geometry: " << m_curFrame->geometry();

		
		is_move = true;
		m_startPos = mapToParent(event->pos());
		m_originalPos = m_curFrame->geometry().topLeft();


		//! [0] 拖拽
		int mouseX = event->pos().x();
		int mouseY = event->pos().y();
		QRect rect = m_curFrame->geometry();
		int myTop = rect.top();
		int myLeft = rect.left();
		int myBottom = rect.bottom();
		int myRight = rect.right();

		if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragLT;
		}
		else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragRB;
		}
		else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragLB;
		}
		else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragRT;
		}
		else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragL;
		}
		else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragR;
		}
		else if ((myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragT;
		}
		else if ((myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
		{
			is_drag = true;
			is_move = false;
			dragType = Frame::DragB;
		}
		else
		{
			is_drag = false;
			is_move = true;
		}
		m_curFrame->setDragType(dragType);
		//! [0]


		/*
        QList<QGraphicsItem *>itemList = items(event->scenePos());
        if(itemList.count() != 0)
        {
            //! [0]查找根Item即Frame
            qDebug() << "mouse press, item count: " << itemList.count();
            QGraphicsItem *item;
            item = findRootParent(itemList.last());

            //! [0]

            //! 按住Ctrl键
            if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
            {

            }
            else
            {
                clearAllItemSelected();
            }
            m_curFrame = qgraphicsitem_cast<Frame *>(item);
            qDebug() << "rootFrame.type: " << m_curFrame->getType() << endl;

            afterAddNewFrame(m_curFrame);
            setChildInfo(m_curFrame);

            //! [0] 拖拽
            double mouseX = event->scenePos().x();
            double mouseY = event->scenePos().y();
            QRectF rect = m_curFrame->sceneBoundingRect();
            double myTop = rect.top();
            double myLeft = rect.left();
            double myBottom = rect.bottom();
            double myRight = rect.right();

            if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragLT;
            }
            else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragRB;
            }
            else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragLB;
            }
            else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragRT;
            }
            else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragL;
            }
            else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragR;
            }
            else if ((myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragT;
            }
            else if ((myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
            {
                is_drag = true;
                is_move = false;
                dragType = DragB;
            }
            else
            {
                is_drag = false;
                is_move = true;
                m_startPos = m_curFrame->boundingRect().topLeft();
                m_shiftOrg.setX(event->scenePos().x() - m_startPos.x());
                m_shiftOrg.setY(event->scenePos().y() - m_startPos.y());
                m_shiftOrg = event->scenePos();
            }
            m_curFrame->setDragType(dragType);
            //! [0]
        }
        else
        {
            clearAllItemSelected();
        }
		*/
    }
	
    QWidget::mousePressEvent(event);
    
}

void DropWidget::setChildInfo(Frame *curFrame)
{
    /*
    QList<QGraphicsItem *> children = curFrame->childItems();
    int i = 1;
    foreach(QGraphicsItem *item, children)
    {
        Frame *frame = qgraphicsitem_cast<Frame *>(item);
        if (frame == nullptr)
            return;
        QPointF offsetPoint = frame->sceneBoundingRect().topLeft() - curFrame->sceneBoundingRect().topLeft(); //! TODO.这里加1进行修正
        qDebug() << i << ", " << "offsetPoint-->" << offsetPoint << endl;
        i++;
        if (frame != nullptr)
        {
            frame->setParentItemS(curFrame);
            frame->setOffset(offsetPoint.x(), offsetPoint.y());
        }
    }
    */
}

// 鼠标移动过程中跟随位置改变
void DropWidget::mouseMoveEvent(QMouseEvent *event)
{
	emit pos((event->pos()));
    if(is_move)
    {
		QPointF curPos = mapToParent(event->pos());
		m_offset = curPos.toPoint() - m_startPos;
		if (m_curFrame == nullptr)
			return;
		m_curFrame->move(m_originalPos + m_offset);
		return;

		/*
        qreal x_offset = event->scenePos().x() - m_shiftOrg.x();
        qreal y_offset = event->scenePos().y() - m_shiftOrg.y();
        qreal width = m_curFrame->rect().width();
        qreal height = m_curFrame->rect().height();
        qDebug() << "x_offset:" << x_offset << ", y_offset:" << y_offset << ", width:" << width << ", height:" << height << ", m_startPos:" << m_startPos << endl;

        m_curFrame->setRect(m_startPos.x() + x_offset, m_startPos.y() + y_offset, width, height);
        m_curFrame->resetChildrenPos(false);
		*/
    }
	

    if(!is_drag)
    {
		QWidget *childWidget = this->childAt(event->pos());
		//! 按住Ctrl键
			if (m_selectedItems.count() != 0)
			{
				foreach(Frame *frame, m_selectedItems)
				{
					frame->setSelected(false);
					frame = nullptr;
				}
				m_selectedItems.clear();
				if (m_curFrame != nullptr)
				{
					m_curFrame->setSelected(false);
					m_curFrame = nullptr;
				}
			}
		
		if (childWidget == nullptr)
		{
			return;
		}

		Frame *tmpItem = (Frame *)childWidget;
		tmpItem = findRootParent(tmpItem);


            QRectF rect = tmpItem->geometry();
            double mouseX = event->pos().x();
            double mouseY = event->pos().y();
            //qDebug() << "mouse move, sceneBoundingRect-->" << rect;
            double myTop = rect.top();
            double myLeft = rect.left();
            double myBottom = rect.bottom();
            double myRight = rect.right();

            //改变鼠标形状
            if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)))
            {
                tmpItem->setCursor(Qt::SizeFDiagCursor);
            }
            else if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)))
            {
                tmpItem->setCursor(Qt::SizeBDiagCursor);
            }
            else if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL)))
            {
                tmpItem->setCursor(Qt::SizeHorCursor);
            }
            else if (((myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)) || ((myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)))
            {
                tmpItem->setCursor(Qt::SizeVerCursor);
            }
            else
            {
                tmpItem->setCursor(Qt::ArrowCursor);
            }
        
    }
    else
    {
        //! 拖动改变Item大小
        m_curFrame->drawing(event);
        update();
    }

    
    


    //QWidget::mouseMoveEvent(event);
}
// 鼠标释放后作为最后的位置
void DropWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(is_move)
    {
    }
    is_drag = false;
    is_move = false;
    QWidget::mouseReleaseEvent(event);
}
