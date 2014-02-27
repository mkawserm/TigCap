#ifndef TIGCAP_H
#define TIGCAP_H


#include <QMainWindow>
#include "pieceboardcontroller.h"


class TigCap : public QMainWindow{
    Q_OBJECT

    private:
        PieceBoardController *_piece_board_contorller;

    public:
        TigCap(QWidget *parent = 0);
        ~TigCap();
};

#endif // TIGCAP_H
