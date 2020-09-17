#ifndef DROPWIDGET_H
#define DROPWIDGET_H

#include <QWidget>
#include "fwd.h"
#include "frame.h"

class DropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropWidget(QWidget *parent = nullptr);
public:
    QList<Frame *> getSelectedItems();

    void clearAllItemSelected();
    void afterAddNewFrame(Frame *_frame);

    void addItemToSelected(Frame *frame);


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void pos(QPointF pointF);
    void itemAdded();

private slots:
    void slot_deleteItem();
    void slot_modifyItem();
private:
    Frame *         m_curFrame;
    //QPointF         m_shiftOrg;
    QPoint          m_startPos;
	QPoint			m_offset;
	QPoint			m_originalPos;

    bool            is_drag;
    bool            is_move;
//    DragType        dragType;
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
    QGraphicsItem *findRootParent(QGraphicsItem *_item);
};

#endif // DROPWIDGET_H
