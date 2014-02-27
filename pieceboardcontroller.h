#ifndef PIECEBOARDCONTROLLER_H
#define PIECEBOARDCONTROLLER_H


/* Class Name : PieceBoardController
 * Developer : KaWsEr
 * WebSite : http://blog.kawser.org
 *
 ***Purpose :
 ***  The Purpose of this Class is to manage PieceBoard and Game Logic
 **/




#include <QGraphicsView>
#include "pieceboard.h"



class PieceBoardController : public QGraphicsView{
    Q_OBJECT

    public:
        PieceBoardController(QWidget *parent = 0);

    private:
        int _timer_id; /*It will be used as a animation timer*/
        QGraphicsScene * _game_scene;/*Board and Everything Will be Drawn in that Scene*/

};





#endif // PIECEBOARDCONTROLLER_H
