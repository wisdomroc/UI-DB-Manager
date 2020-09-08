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

    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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
    m_pItemSelected(nullptr), m_pItemHovered(nullptr), is_drag(false)
{
    SetMouseMenu();
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
            QGraphicsItem *item = new Chip("yellow", 50, 50);
            item->setPos(point);
            addItem(item);
        }
        else if(piece.type == "tree")
        {
            QListWidget *listWidget = new QListWidget();
            QGraphicsProxyWidget *proxyWidget = addWidget(listWidget);
            proxyWidget->setPos(point);
        }
        else if(piece.type == "table")
        {
            Frame *item = new Frame(Rect1, myItemMenu);
            item->setRect(point.x(), point.y(), 300, 200);
            QGradient gradient;
            gradient.setColorAt(0, QColor("lightBlue"));
            gradient.setColorAt(1, QColor("white"));
            item->setBrush(QBrush(QColor("steelBlue")));
            item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
            addItem(item);
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

// 鼠标按下获取当前单选中的QGraphicsProxyWidget图元对象
void DropGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "scene mouse press";
    QGraphicsScene::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
    {
        // 检测光标下是否有 item
        m_pItemSelected = nullptr;
        m_pItemHovered = nullptr;
        foreach (QGraphicsItem *item, items(event->scenePos()))
        {
//            if (item->type() == QGraphicsProxyWidget::Type)   // 代理Widget
//            {
//                QGraphicsProxyWidget *proxyWidget = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
//                proxyWidget->setSelected(true);
//                QPointF point = proxyWidget->mapToScene(QPointF(0.0, 0.0));
                // 图元左上角在scene中的位置
//                qDebug() << point.x();
//                qDebug() << point.y();
                // 当前点击图元点在scene中的位置
                qDebug() << event->scenePos().x();
                qDebug() << event->scenePos().y();
//                m_shiftOrg.setX(event->scenePos().x() - point.x());
//                m_shiftOrg.setY(event->scenePos().y() - point.y());
                m_pItemSelected = item;
                m_pItemHovered = item;


                //! [2] 拖拽
                double mouseX = event->scenePos().x();
                double mouseY = event->scenePos().y();
                QRectF rect = m_pItemHovered->boundingRect();
                double myTop = rect.top();
                double myLeft = rect.left();
                double myBottom = rect.bottom();
                double myRight = rect.right();

                if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragLT;
                }
                else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragRB;
                }
                else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragLB;
                }
                else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragRT;
                }
                else if ((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragL;
                }
                else if ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragR;
                }
                else if ((myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragT;
                }
                else if ((myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL))
                {
                    is_drag = true;
                    dragType = DragB;
                }
                else
                {
                    is_drag = false;
                }
                //! [2]


                break;
//            }
        }
    }

}
// 鼠标移动过程中跟随位置改变
void DropGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
//    if(m_pItemSelected != nullptr)
//    {
//        //! 移动选中的Item
//        m_pItemSelected->setPos(event->scenePos().x() - m_shiftOrg.x(), event->scenePos().y() - m_shiftOrg.y());
//    }

    qDebug() << "move...1";
    if(m_pItemHovered == nullptr)
        return;

    qDebug() << "move...2";
    QRectF currentBoundingRect = m_pItemHovered->sceneBoundingRect();
    double mouseX = event->scenePos().x();
    double mouseY = event->scenePos().y();
    double myTop = currentBoundingRect.top();
    double myLeft = currentBoundingRect.left();
    double myBottom = currentBoundingRect.bottom();
    double myRight = currentBoundingRect.right();
    qDebug() << mouseX << "," << mouseY << "," << myTop << "," << myLeft << "," << myBottom << "," << myRight << "," << is_drag << endl;

    if (is_drag)
    {
        //! 拖动改变Item大小
        Frame *frame = (Frame *)m_pItemHovered;
        frame->drawing(event);
        update();
    }
    else
    {
        //改变鼠标形状
        if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)))
        {
            m_pItemHovered->setCursor(Qt::SizeFDiagCursor);
        }
        else if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL) && (myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL) && (myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)))
        {
            m_pItemHovered->setCursor(Qt::SizeBDiagCursor);
        }
        else if (((myLeft - SMALL_INTERVAL < mouseX) && (mouseX < myLeft + SMALL_INTERVAL)) || ((myRight - SMALL_INTERVAL < mouseX) && (mouseX < myRight + SMALL_INTERVAL)))
        {
            m_pItemHovered->setCursor(Qt::SizeHorCursor);
        }
        else if (((myTop - SMALL_INTERVAL < mouseY) && (mouseY < myTop + SMALL_INTERVAL)) || ((myBottom - SMALL_INTERVAL < mouseY) && (mouseY < myBottom + SMALL_INTERVAL)))
        {
            m_pItemHovered->setCursor(Qt::SizeVerCursor);
        }
        else
        {
            m_pItemHovered->setCursor(Qt::ArrowCursor);
        }
    }

}
// 鼠标释放后作为最后的位置
void DropGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "scene mouse release";
    QGraphicsScene::mouseReleaseEvent(event);
    is_drag = false;

//    if(m_pItemSelected != nullptr)
//    {
//        m_pItemSelected->setPos(event->scenePos().x() - m_shiftOrg.x(), event->scenePos().y() - m_shiftOrg.y());
//        m_pItemSelected = nullptr;
//    }
}
