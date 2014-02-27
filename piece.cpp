#include <QtWidgets>

#include "piece.h"


Piece::Piece(Point *arg){
    color = QColor(0,0,0);
    this->p = arg;
    this->moveBy(p->x(),p->y());
    this->setParentItem( this->p->parentItem() );
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);


    //setFlag(QGraphicsItem::ItemIsSelectable);
    //setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void Piece::setColor(int r,int g,int b){
    this->color = QColor(r,g,b);
}


QRectF Piece::boundingRect() const
{
    return QRectF(-0.5,-0.5,12,12);
}


QPainterPath Piece::shape() const
{
    QPainterPath path;
    path.addEllipse(0,0,10,10);
    return path;
}




void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(this->color);

    //painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(0,0,10,10);
}






void Piece::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::ClosedHandCursor);
}




void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "Mouse Move Event";


    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    mime->setColorData(color);
    mime->setText(QString("#%1%2%3")
                  .arg(color.red(), 2, 16, QLatin1Char('0'))
                  .arg(color.green(), 2, 16, QLatin1Char('0'))
                  .arg(color.blue(), 2, 16, QLatin1Char('0')));

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




void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}

