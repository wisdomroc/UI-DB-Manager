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

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QGraphicsItem   *m_pItemSelected;                   // 鼠标单选item
    QGraphicsItem   *m_pItemHovered;
    QPoint          m_shiftOrg;                         // 鼠标点选item中位置相对于item原点的偏移量
    bool is_drag;
    DragType dragType;
private:
    struct Piece {
        QPixmap pixmap;
        QString type;
    };
    QList<QGraphicsItem *> m_tableItems;

    QMenu *myItemMenu;
    void addTable(const QPointF &point, int width, int height);

    void SetMouseMenu();
private slots:

};


#endif // DROPGRAPHICSVIEW_H
