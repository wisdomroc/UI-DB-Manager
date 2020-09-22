#ifndef FRAME_H
#define FRAME_H

#include <QtWidgets>




class Frame : public QLabel
{
    Q_OBJECT
    Q_ENUMS(FrameType)
    Q_ENUMS(DragType)
public:
    enum FrameType { Table, List, Tree, Horizontal, Vertical,Null };
    enum DragType { DragL, DragR, DragT, DragB, DragLT, DragLB, DragRT, DragRB };
    explicit Frame(FrameType frameType, QMenu *contextMenu, QWidget *parent = nullptr);
    ~Frame();

    FrameType getType() const;
    void setDragType(DragType _dragType);
    void resetChildrenPos(bool firstAdjust);
    void addChildItem(Frame *_frame);

    void setParentItemS(Frame *_frame);
    void setOriginalWidthAndHeight(int _width, int _height);
    void setOffset(qreal x_, qreal y_);

    void setSelected(bool selected);

    void drawing(QMouseEvent * event);

signals:
	void rightKeySelected();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
	void paintEvent(QPaintEvent *event);


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
