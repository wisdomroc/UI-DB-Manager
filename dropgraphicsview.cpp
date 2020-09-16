#include "dropgraphicsview.h"

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
