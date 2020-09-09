#ifndef FRAME_H
#define FRAME_H

#include <QtWidgets>

enum FrameType { Rect1, Rect2, Rect3 };
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

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
	QMenu *myContextMenu;
    DragType dragType;
};

#endif // FRAME_H
