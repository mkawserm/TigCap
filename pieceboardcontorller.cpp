#include "pieceboardcontroller.h"



/*constructor of the PieceBoard Class*/
PieceBoardController::PieceBoardController(QWidget *parent)
    :QGraphicsView(parent), _timer_id(0){


    /*Initilize Game Scene*/
    _game_scene = new QGraphicsScene(this);
    //_game_scene;
    setScene(_game_scene);/*Adding into The PieceBoard QGraphicsView*/

    setViewportUpdateMode(FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    PieceBoard * myBoard = new PieceBoard();
    _game_scene->addItem(myBoard);
    myBoard->play();
    setFixedSize(600,600);




}
