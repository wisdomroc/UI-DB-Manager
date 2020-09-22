#include "dropwidget.h"
#include "chip.h"

DropWidget::DropWidget(QWidget *parent) : QWidget(parent)
  ,m_curFrame(nullptr), is_drag(false), is_move(false), m_tableNumber(0)
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
		frame->setSelected(true);
		emit itemSelected(m_selectedItems);
    }
}

void DropWidget::ModifyItem()
{

}

void DropWidget::dragMoveEvent(QDragMoveEvent *event)
{
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

void DropWidget::slot_rightKeySelected()
{
	Frame *frame = qobject_cast<Frame *>(sender());
	clearAllItemSelected();
	addItemToSelected(frame);
	
}

void DropWidget::addTable(const QPointF &point)
{
    Frame *frame = new Frame(Frame::Table, myItemMenu, this);
	connect(frame, SIGNAL(rightKeySelected()), this, SLOT(slot_rightKeySelected()));
    QString name = tr("TableWidget_%1").arg(m_tableNumber ++);
    frame->setText(name);
    frame->setObjectName(name);
    frame->setMinimumSize(200, 200);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->move(point.toPoint());
    frame->setVisible(true);

    emit itemAdded(frame);
}

void DropWidget::clearAllItemSelected()
{
    //! 清除之前的Item
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
	emit clearAllSelected();
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
            clearAllItemSelected();
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

		//! DragL, DragR, DragT, DragB, DragLT, DragLB, DragRT, DragRB
		qDebug() << "dragType: " << dragType << "m_curFrame.geometry: " << m_curFrame->geometry() << endl;
        //! [0]
    }

    QWidget::mousePressEvent(event);
    
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
    }


    if(!is_drag)
    {
        QWidget *childWidget = this->childAt(event->pos());
        if (childWidget == nullptr)
        {
            return;
        }

        Frame *tmpItem = (Frame *)childWidget;
        tmpItem = findRootParent(tmpItem);

        QRectF rect = tmpItem->geometry();
        double mouseX = event->pos().x();
        double mouseY = event->pos().y();

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
