#ifndef PIECEBOARD_H
#define PIECEBOARD_H


/* Class Name : PieceBoard
 * Developer : KaWsEr
 * WebSite : http://blog.kawser.org
 *
 *** Purpose :
 ***  The class will be used to draw Piece Board Using Point Class and Edge Class
 ***
 *** Type = 4
 **/


/*Qt's Header*/
#include <QMap>
#include <QPair>
#include <QList>
#include <QPoint>
#include <QRectF>
#include <QGraphicsItem>
#include <QGraphicsObject>

/*C++ Standard Header*/
#include <vector>
#include <algorithm>
using namespace std;

/*TigCap Own Header*/
#include "edge.h"
#include "point.h"
#include "_macro.h"
#include "_Tester.h"
#include "gamestate.h"


class PieceBoard : public QGraphicsObject{

    Q_OBJECT
    private:
        QRectF piece_board_size;
        int scaleF;
        bool deb;
        int h,w,rowLine,colLine;
        int nVertexMesh; // total screen
        int nVertexBrd;
        int idInMeshXY[MX+2][MX+2]; // Full Board / Mesh Vertex Mappings
        int xxInMesh[MX+2];
        int yyInMesh[MX+2];
        int xxAtM[40]; // x and y of Mesh in Board, at Mask position
        int yyAtM[40];
        lli maskBlack , maskRed;
        bool movedHuman,movedCPU;
    public:
        Point *P[ MX + 10 ];
        vector < int > adjMove[MX+2];// Built on Node BoardId
        vector < int > adjJump[MX+2];// Built on Node BoardId
        int idInB[40]; /// 0 based id in Board
        int idInM[100];/// 0 based
        PieceBoardInfo pInfo;

        PieceBoard();
        enum { Type = UserType + 4 };
        int type() const { return Type; }

        void ConnectTwoPoints(Point *p1,Point *p2);

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void make4( int u , int v );
        void make5( int u , int v );
        bool drawGraph(); // check duplicity of edge
        void showP( int x );
        void debugPrintGraph();
        void debugMoveInMask(int a,int b);
        void placePieceInit();
        void play();

        void MovePoint(int src,int dest);// Move a point from source to destination
        ///Warnig: this moves only valid moves, will not work properly for all arbitrary moves
    private:
        Point *source;
        Point *destination;
        Point *animobject;

    private slots:
        void animation_finished();

    private:
        /// WARNING: Must call after void placePieceInit();
        /* Here is the idea how will animate Computer
        * move sequence one after another.
        * We will cretae a QList< QPair<int,int> > where we will
        * store all the moves Point index.
        * A method startComputerMove will initiate
        * the move sequence.
        */
    QList < QPair<int,int> > computer_moves;
    enum Turn {HumanTurn,ComputerTurn};
    Turn whose_turn;
    void pushMove( int a , int b );
    void clearMove();
    void startMove();
    public:
        void startComputerMove();
};

#endif // PIECEBOARD_H
