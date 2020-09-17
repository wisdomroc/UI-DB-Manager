#include "dropgraphicsview.h"
#include <QtWidgets>
#include "chip.h"

DropGraphicsScene::DropGraphicsScene(QWidget *parent) : QGraphicsScene(parent),
    m_curFrame(nullptr), is_drag(false), is_move(false)
{
    SetMouseMenu();
}

QList<Frame *> DropGraphicsScene::getSelectedItems()
{
    return m_selectedItems;
}

void DropGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void DropGraphicsScene::SetMouseMenu()
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

void DropGraphicsScene::slot_deleteItem()
{
    //    QList<QGraphicsItem *> items = selectedItems();
    //    QPointF point;
    //    int count = items.count();
    //    Frame *frame = (Frame *)items.at(count - 1);
    //    emit deleteFrame(frame);
    //    for (int i = 0; i < count; i++)
    //    {
    //        QGraphicsItem *item = items.at(i);
    //        point = item->scenePos();
    //        removeItem(item);
    //    }
}

void DropGraphicsScene::DeleteItem()
{

}

void DropGraphicsScene::slot_modifyItem()
{

}

void DropGraphicsScene::ModifyItem()
{

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
            Chip *chip = new Chip(QColor("steelBlue"), 100, 100);
            addItem(chip);
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

void DropGraphicsScene::addTable(const QPointF &point)
{
    Frame *frame = new Frame(Table, myItemMenu);
    m_tableItems.append(frame);
    QGraphicsTextItem *text = new QGraphicsTextItem(tr("%1%2").arg(QStringLiteral("表格")).arg(m_tableItems.count()), frame);
    text->setDefaultTextColor(Qt::blue);
    text->setFont(QFont("微软雅黑", 8));
    text->setPos(point);
    addItem(frame);
	frame->setOriginalWidthAndHeight(300, 200);
    frame->setRect(point.x(), point.y(), 300, 200);
    emit itemAdded(frame);
}

void DropGraphicsScene::clearAllItemSelected()
{
	//! 清除之前的Item
	for (int i = 0; i < m_selectedItems.count(); i++)
	{
		Frame *frame = m_selectedItems.at(i);
		frame->setBrush(QColor("transparent"));
		frame->setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		frame->setOpacity(1);
	}
	m_selectedItems.clear();
}

void DropGraphicsScene::afterAddNewFrame(Frame *_frame)
{
	//! 添加当前的Item
	if (!m_selectedItems.contains(_frame))
	{
		m_selectedItems.append(_frame);
	}

	//! 设置被选中
	
	_frame->setOpacity(0.5);
	_frame->setBrush(QColor("lightGreen"));
	_frame->setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	setChildInfo(_frame);
}

QGraphicsItem *DropGraphicsScene::findRootParent(QGraphicsItem *_item)
{
	QGraphicsItem *parentItem = _item->parentItem();
	if (parentItem == nullptr)
	{
		return _item;
	}
	else
	{
		return findRootParent(_item);
	}
}

// 鼠标按下获取当前单选中的QGraphicsProxyWidget图元对象
void DropGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
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
    }
    QGraphicsScene::mousePressEvent(event);
}

void DropGraphicsScene::setChildInfo(Frame *curFrame)
{
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
}

// 鼠标移动过程中跟随位置改变
void DropGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(is_move)
    {
        qreal x_offset = event->scenePos().x() - m_shiftOrg.x();
        qreal y_offset = event->scenePos().y() - m_shiftOrg.y();
        qreal width = m_curFrame->rect().width();
        qreal height = m_curFrame->rect().height();
        qDebug() << "x_offset:" << x_offset << ", y_offset:" << y_offset << ", width:" << width << ", height:" << height << ", m_startPos:" << m_startPos << endl;

        m_curFrame->setRect(m_startPos.x() + x_offset, m_startPos.y() + y_offset, width, height);
        m_curFrame->resetChildrenPos(false);
    }

    if(!is_drag)
    {
        QList<QGraphicsItem *>itemList = items(event->scenePos(), Qt::IntersectsItemShape);
        if(itemList.count() != 0)
        {
            //! [0]查找根Item即Frame
            QGraphicsItem *item;
            QGraphicsItem *baseItem = itemList.at(itemList.count() - 1);
            QGraphicsItem *parentItem = baseItem->parentItem();
            if (parentItem == nullptr)
            {
                item = baseItem;
            }
            else
            {
                item = parentItem;
            }
            //! [0]
            QGraphicsItem *tmpItem = qgraphicsitem_cast<Frame *>(item);
            if(tmpItem == nullptr)
                return;
            QRectF rect = tmpItem->sceneBoundingRect();
            double mouseX = event->scenePos().x();
            double mouseY = event->scenePos().y();
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
    }
    else
    {
        //! 拖动改变Item大小
        m_curFrame->drawing(event);
        update();
    }

    emit pos(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}
// 鼠标释放后作为最后的位置
void DropGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(is_move)
    {

    }
    is_drag = false;
    is_move = false;
    QGraphicsScene::mouseReleaseEvent(event);
}
