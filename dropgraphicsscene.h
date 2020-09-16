#ifndef DROPGRAPHICSSCENE_H
#define DROPGRAPHICSSCENE_H


#include "fwd.h"
#include "frame.h"

class DropGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DropGraphicsScene(QWidget *parent = nullptr);

public:
    QList<Frame *> getSelectedItems();

	void clearAllItemSelected();
	void afterAddNewFrame(Frame *_frame);
	
	

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void pos(QPointF pointF);
    void itemAdded(Frame *frame);

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
	
	void setChildInfo(Frame *curFrame);

};


#endif // DROPGRAPHICSSCENE_H
