#ifndef DROPGRAPHICSVIEW_H
#define DROPGRAPHICSVIEW_H


#include "dropgraphicsscene.h"

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

#endif // DROPGRAPHICSVIEW_H
