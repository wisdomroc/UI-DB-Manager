#ifndef FRAME_H
#define FRAME_H

#include <QtWidgets>

enum FrameType { Rect1, Rect2, Rect3, Horizontal, Vertical };
enum DragType { DragL, DragR, DragT, DragB, DragLT, DragLB, DragRT, DragRB };

class Frame : public QGraphicsRectItem
{
public:
	Frame(FrameType frameType, QMenu *contextMenu);
	~Frame();

public:
	FrameType mFrameType;
	QString mName;
    void setDragType(DragType _dragType);
	void startDraw(QGraphicsSceneMouseEvent * event);
	void drawing(QGraphicsSceneMouseEvent * event);
    void resetChildrenPos();

    void setParentItemS(Frame *frame);
    void setOffset(qreal x_, qreal y_);
protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
	QMenu *myContextMenu;
    DragType dragType;
    Frame *parentFrame;
    qreal x_offset;
    qreal y_offset;
};

#endif // FRAME_H
