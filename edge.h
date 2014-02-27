#ifndef EDGE_H
#define EDGE_H


/* Class Name : Edge
 * Developer : KaWsEr
 * WebSite : http://blog.kawser.org
 *
 *** Purpose :
 ***  The class will be used to Draw Line Between Two Points
 ***
 *** Type = 2
 **/


#include <QDebug>
#include <QtMath>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>


#include "point.h"



class Edge : public QGraphicsItem{
private:
    Point *p1,*p2;
    QColor edge_color;
    int edge_length;
    int edge_thickness;
    QGraphicsItem *parent;

    int x1,y1,x2,y2;


public:
        Edge(Point *p1,Point *p2,QGraphicsItem *parent);
        enum { Type = UserType + 2 };
        int type() const { return Type; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};



#endif // EDGE_H
