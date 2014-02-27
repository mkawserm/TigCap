#include <QtWidgets>
#include "point.h"




Point::Point(int x, int y,QGraphicsItem *parent) : QGraphicsObject(parent)
{

    point_x = x;
    point_y = y;
    point_color = QColor(255,255,255); /*Making The Point Invisible . The Bangladeshi Style :D */
    point_size = 10; /*5 pixel Point Size*/
    this->setPos(point_x,point_y);
    parent = parent;
    this->setParentItem(parent);
    this->disableDrops();

    this->is_black = false;
    this->is_red = false;
    this->is_visible = false;
    this->is_board_vertex = false;
}



void Point::enableGrab(){
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

}

void Point::disableGrab(){
    setCursor(Qt::ArrowCursor);
    //setAcceptedMouseButtons(Qt::MouseButton);
}



void Point::setBLACK(){
    this->is_visible = true;
    this->is_black = true;
    this->is_red = false;

    point_color = QColor(0,0,0);

    this->enableDrops();
    this->enableGrab();

    this->update();
}



void Point::setRED(){
    this->is_visible = true;
    this->is_red = true;
    this->is_black = false;

    point_color = QColor(255,0,0);

    this->enableDrops();
    this->enableGrab();

    this->update();
}


void Point::setINVISIBLE(){
    this->is_visible = false;
    this->is_red = false;
    this->is_black = false;
    this->is_board_vertex = false;

    point_color = QColor(255,255,255);

    this->disableDrops();
    this->disableGrab();

    this->update();

}



void Point::setBoardVertex(){
    this->setINVISIBLE();

    this->is_board_vertex = true;
    this->enableDrops(); /*Board Vertex and Red and Black Can Contain Point*/
    this->disableGrab();
}



void Point::disableDrops(){
    this->setAcceptDrops(false);
}

void Point::enableDrops(){
    this->setAcceptDrops(true);
}



bool Point::isBlack(){
    return this->is_black;
}


bool Point::isRed(){
    return this->is_red;
}

bool Point::isVisible(){
    return this->is_visible;
}

bool Point::isBoardVertex(){
    return this->is_board_vertex;
}



QRectF Point::boundingRect() const
{
    return QRectF(0,0,point_size,point_size);
}


QPainterPath Point::shape() const
{
    QPainterPath path;
    path.addEllipse(0,0,point_size,point_size);
    return path;
}




void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(point_color);

    //painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(0,0,point_size,point_size);
}




void Point::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (this->isBlack() || this->isRed() || this->isBoardVertex() )
    {
        event->setAccepted(true);
        qDebug() << "Drag Enter into point";

        if (event->mimeData()->hasColor()) {
            event->setAccepted(true);
            dragOver = true;
            update();
        }


    this->scene()->update();
    }

}




void Point::dropEvent(QGraphicsSceneDragDropEvent *event)
{

    if (this->isBlack() || this->isRed() || this->isBoardVertex() )
    {
        qDebug() << "Drop Event";
        if (event->mimeData()->hasColor()) {
            dragOver = false;
            point_color = qvariant_cast<QColor>(event->mimeData()->colorData());
            if ( point_color == QColor(255,0,0) ) this->setRED();
            else if( point_color == QColor(0,0,0)  ) this->setBLACK();

            QPointF p_pos = qvariant_cast<QPointF> ( event->mimeData()->property("pos") );
            /*Use This pos and make this point as Board Vertex*/
            qDebug() << "I Have come From " << p_pos;

            update();
        }
        this->scene()->update();
    }

}



void Point::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (this->isBlack() || this->isRed() || this->isBoardVertex() )
    {
    qDebug() << "Drag Leave point";
    Q_UNUSED(event);
    dragOver = false;
    update();


    this->scene()->update();
    }
}










void Point::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if( this->isBlack() || this->isRed() ) setCursor(Qt::ClosedHandCursor);
}




void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "Mouse Move Event";

    if( this->isBlack() || this->isRed() )
    {

    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);
    mime->setProperty("pos",QPointF( this->x(),this->y() ));
    mime->setColorData(point_color);
    mime->setText(QString("#%1%2%3")
                  .arg(point_color.red(), 2, 16, QLatin1Char('0'))
                  .arg(point_color.green(), 2, 16, QLatin1Char('0'))
                  .arg(point_color.blue(), 2, 16, QLatin1Char('0')));

    QPixmap pixmap(34, 34);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    painter.translate(15, 15);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter, 0, 0);
    painter.end();

    pixmap.setMask(pixmap.createHeuristicMask());

    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(15, 20));
    drag->exec();
    setCursor(Qt::OpenHandCursor);
    }

}




void Point::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if( this->isBlack() || this->isRed() ) setCursor(Qt::OpenHandCursor);
}

