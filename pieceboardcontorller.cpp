#include "pieceboardcontroller.h"



/*constructor of the PieceBoard Class*/
PieceBoardController::PieceBoardController(QWidget *parent)
    :QGraphicsView(parent), _timer_id(0){


    /*Initilize Game Scene*/
    _game_scene = new QGraphicsScene(this);
    //_game_scene;
    setScene(_game_scene);/*Adding into The PieceBoard QGraphicsView*/

    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    PieceBoard * _piece_board = new PieceBoard();
    _game_scene->addItem(_piece_board);

    setFixedSize(600,600);




}
