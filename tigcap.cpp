#include "tigcap.h"









TigCap::TigCap(QWidget *parent)
    : QMainWindow(parent)
{
    _piece_board_contorller = new PieceBoardController;
    this->setCentralWidget(_piece_board_contorller);
    this->setFixedSize(600,600);
}





TigCap::~TigCap(){
    delete _piece_board_contorller;/*Rescue Memory*/

}
