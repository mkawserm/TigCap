#include "edge.h"

Edge::Edge(Point *p1, Point *p2,QGraphicsItem *parent)
{
    //this->setZValue(0.9);
    p1 = p1;
    p2 = p2;
    edge_color = QColor(0,0,0);
    edge_thickness = 1; /*Edge Thickness*/



    x1 = p1->pos().x();
    y1 = p1->pos().y();

    x2 = p2->pos().x();
    y2 = p2->pos().y();

    edge_length = sqrt( pow( (x1-x2),2) + pow( (y1-y2),2)  );
    parent = parent;
    this->setParentItem(parent);
}



QRectF Edge::boundingRect() const{
    return QRectF(0,0,edge_length,edge_thickness);
}

QPainterPath Edge::shape() const{
    QPainterPath path;
    path.addRect(x1,y1,edge_thickness,edge_length);
    return path;
}


void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    //painter->setPen(Qt::NoPen);
    painter->setBrush(edge_color);
    painter->drawLine( x1+5,y1+5 , x2 +5,y2+5 );
    //p1->update();
    //p2->update();

}
