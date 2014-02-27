#ifndef PIECE_H
#define PIECE_H


/* Class Name : Piece
 * Developer : KaWsEr
 * WebSite : http://blog.kawser.org
 *
 *** Purpose :
 ***  The class will create piece which will be operated by the USER or AI
 ***
 *** Type = 3
 **/

#include <QDebug>
#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QPixmap>
#include <QApplication>


#include "point.h"



class Piece : public QGraphicsItem{

private:
    QColor color;
    Point *p;

public:
    Piece(Point *arg);
    void setColor(int r,int g,int b);
    enum { Type = UserType + 3 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};




#endif // PIECE_H
