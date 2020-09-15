#include "dropgraphicsview.h"
#include <QtWidgets>
#include "chip.h"

#define INTERVAL		34	//
#define RECT_INTERVAL	12	//
#define RECTWIDTH		22	//
#define RECTHEIGHT		10	//
#define LEFTRECT		4	//
#define SMALL_INTERVAL	5	//鼠标改变形状的那个区域的偏移量
#define FONT_SIZE		8	//
#define FONT_Y_OFFSET	7	//

DropGraphicsView::DropGraphicsView(QWidget *parent) : QGraphicsView(parent), m_zoomFactor(0)
{
    setAcceptDrops(true);
    m_scene = new DropGraphicsScene();
    setScene(m_scene);

    setCacheMode(CacheNone);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking (true);
    setAlignment(Qt::AlignLeft|Qt::AlignTop);
}

void DropGraphicsView::setZoomFactor(int factor)
{
    if(m_zoomFactor > 250)
        m_zoomFactor = 250;
    if(m_zoomFactor < -250)
        m_zoomFactor = -250;

    m_zoomFactor = factor;
    setupMatrix();
}

void DropGraphicsView::resizeEvent(QResizeEvent *event)
{
    setSceneRect(0, 0, width(), height());
}

void DropGraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            zoomIn(5);
        else
            zoomOut(5);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}

void DropGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void DropGraphicsView::setupMatrix()
{
    qreal scale = qPow(qreal(2), (m_zoomFactor) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    setMatrix(matrix);
}

void DropGraphicsView::zoomIn(int level)
{
    m_zoomFactor += level;
    if(m_zoomFactor > 250)
        m_zoomFactor = 250;

    setupMatrix();
    emit zoom(m_zoomFactor);
}

void DropGraphicsView::zoomOut(int level)
{
    m_zoomFactor -= level;
    if(m_zoomFactor < -250)
        m_zoomFactor = -250;

    setupMatrix();
    emit zoom(m_zoomFactor);
}



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
    Frame *frame = new Frame(Rect1, myItemMenu);
    m_tableItems.append(frame);
    QGraphicsTextItem *text = new QGraphicsTextItem(tr("%1%2").arg(QStringLiteral("表格")).arg(m_tableItems.count()), frame);
    text->setDefaultTextColor(Qt::blue);
    text->setFont(QFont("微软雅黑", 24));
    text->setPos(point);
    addItem(frame);
    frame->setRect(point.x(), point.y(), 300, 200);
}

void DropGraphicsScene::setCurrentItemSelected(QGraphicsItem *item)
{
    m_curFrame = qgraphicsitem_cast<Frame *>(item);
    m_curFrame->setOpacity(0.5);
    m_curFrame->setBrush(QColor("lightGreen"));
    m_curFrame->setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void DropGraphicsScene::addToItemSelected()
{
    if(!m_selectedItems.contains(m_curFrame))
    {
        m_selectedItems.append(m_curFrame);
    }
}

void DropGraphicsScene::clearAllItemSelected()
{
    for(int i = 0; i < m_selectedItems.count(); i ++)
    {
        Frame *frame = m_selectedItems.at(i);
        frame->setBrush(QColor("transparent"));
        frame->setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        frame->setOpacity(1);
    }
    m_selectedItems.clear();
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
			qDebug() << "item count: " << itemList.count();
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

            //! 按住Ctrl键
            if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
            {

            }
            else
            {
                clearAllItemSelected();
            }

            //! 设置当前Item选中,并设为m_curFrame
            setCurrentItemSelected(item);

            //! 添加到选中itemList中
            addToItemSelected();

            //! 拖拽
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

            //! 设置children为Frame的一些信息
            QList<QGraphicsItem *> children = m_curFrame->childItems();
            foreach(QGraphicsItem *item, children)
            {
                Frame *frame = qgraphicsitem_cast<Frame *>(item);
                if(frame == nullptr)
                    return;
                QPointF offsetPoint = frame->sceneBoundingRect().topLeft() - m_curFrame->sceneBoundingRect().topLeft() - QPointF(-0.5, -0.5); //! TODO.这里加1进行修正
                qDebug() << "offsetPoint-->" << offsetPoint << endl;
                if(frame != nullptr)
                {
                    frame->setParentItemS(m_curFrame);
                    frame->setOffset(offsetPoint.x(), offsetPoint.y());
                }
            }
        }
        else
        {
            clearAllItemSelected();
        }
    }
    QGraphicsScene::mousePressEvent(event);
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
        qDebug() << x_offset << "," << y_offset << "," << width << "," << height << "," << m_startPos << endl;

        m_curFrame->setRect(m_startPos.x() + x_offset, m_startPos.y() + y_offset, width, height);
        m_curFrame->resetChildrenPos();
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
