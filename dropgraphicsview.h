#ifndef DROPGRAPHICSVIEW_H
#define DROPGRAPHICSVIEW_H

#include "mainwindow.h"
#include "fwd.h"

class DropGraphicsScene;
class DropGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DropGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private:
    void zoomIn(int level);
    void zoomOut(int level);

    DropGraphicsScene *m_scene;
    int m_zoomFactor;
};

class DropGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DropGraphicsScene(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    struct Piece {
        QPixmap pixmap;
        QString type;
    };
    QList<QGraphicsItem *> m_tableItems;


    void addTable(const QPointF &point, int width, int height);

private slots:

};


#endif // DROPGRAPHICSVIEW_H
