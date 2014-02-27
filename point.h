#ifndef POINT_H
#define POINT_H

/* Class Name : Point
 * Developer : KaWsEr
 * WebSite : http://blog.kawser.org
 *
 ***Purpose :
 ***  create a point in (x,y) co-ordinate
 ***
 ***Type = 1
 **/


#include <QDebug>
#include <QMimeData>
#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>

#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QPixmap>
#include <QApplication>




/* Red Point CPU
 * Black Point User
 */




class Point : public QGraphicsObject{

private:
    int point_x,point_y;
    QColor point_color;
    int point_size;
    QGraphicsItem *parent;

    bool is_black;
    bool is_red;
    bool is_visible;
    bool is_board_vertex;

public:
    bool color; /// empty , black , red = 0 , 1 , 2
    Point(int x ,int y , QGraphicsItem *parent );
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setBLACK();
    void setRED();

    void setINVISIBLE();

    void setBoardVertex();

    bool isBlack();
    bool isRed();
    bool isVisible();
    bool isBoardVertex();


    void disableDrops();
    void enableDrops();

    void enableGrab();
    void disableGrab();


protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    bool dragOver;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};



#endif // POINT_H
