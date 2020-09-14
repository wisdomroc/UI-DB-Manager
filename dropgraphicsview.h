#ifndef DROPGRAPHICSVIEW_H
#define DROPGRAPHICSVIEW_H

#include "mainwindow.h"
#include "fwd.h"
#include "frame.h"

class DropGraphicsScene;
class DropGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DropGraphicsView(QWidget *parent = nullptr);
    void setZoomFactor(int factor);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

signals:
    void zoom(int factor);

private:
    void zoomIn(int level);
    void zoomOut(int level);

    DropGraphicsScene *m_scene;
    int m_zoomFactor;
    void setupMatrix();
};

class DropGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DropGraphicsScene(QWidget *parent = nullptr);

public:
    QList<Frame *> getSelectedItems();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void pos(QPointF pointF);

private slots:
    void slot_deleteItem();
    void slot_modifyItem();
private:
    Frame *         m_curFrame;
    QPointF         m_shiftOrg;
    QPointF         m_startPos;
    bool            is_drag;
    bool            is_move;
    DragType        dragType;
private:
    struct Piece {
        QPixmap pixmap;
        QString type;
    };

    QList<Frame *> m_tableItems;
    QList<Frame *> m_selectedItems;
    QMenu *myItemMenu;


    void addTable(const QPointF &point);
    void SetMouseMenu();

    void DeleteItem();
    void ModifyItem();
    void clearAllItemSelected();
    void setCurrentItemSelected(QGraphicsItem *item);
    void addToItemSelected();
};


#endif // DROPGRAPHICSVIEW_H
