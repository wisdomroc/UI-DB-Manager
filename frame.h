#ifndef FRAME_H
#define FRAME_H

#include <QtWidgets>

enum FrameType { Table, List, Tree, Horizontal, Vertical,Null };
enum DragType { DragL, DragR, DragT, DragB, DragLT, DragLB, DragRT, DragRB };

class Frame : public QGraphicsRectItem
{
public:
	Frame(FrameType frameType, QMenu *contextMenu);
	~Frame();

public:

    FrameType getType() const;
    void setDragType(DragType _dragType);
    void resetChildrenPos();
    void addChildItem(Frame *_frame);

    void setParentItemS(Frame *_frame);
    void setOriginalWidthAndHeight(int _width, int _height);
    void setOffset(qreal x_, qreal y_);

    void drawing(QGraphicsSceneMouseEvent * event);

private:

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    Frame *m_parentFrame;
    QString m_name;
    QList<Frame *> m_childrenFrame;


    DragType m_dragType;

    FrameType m_frameType;
    QMenu *m_contextMenu;

	int m_width;
	int m_height;

    bool m_offsetFlag;
    qreal m_offsetX;
    qreal m_offsetY;
};

#endif // FRAME_H
