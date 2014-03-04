#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <vector>
#include "_macro.h"
using namespace std;

class PieceBoardInfo
{
public:
    vector < int > adjMove[MX+2];// Built on Node BoardId
    vector < int > adjJump[MX+2];// Built on Node BoardId
    int idInB[40]; /// 0 based id in Board
    int idInM[100];/// 0 based
};
class GameState
{
private:

    int nb,nr,moveAfter11;

    PieceBoardInfo *Board;
public:
    lli mask[2]; /// black , red
    GameState *temp , *bestJump , *bestJump1 , *bestJump2;
    vector < GameState > child;
    vector < int > lastMoveSeq; /// board id
    GameState(data aa ,data bb , PieceBoardInfo *x,int BB = 16,int RR = 16)
    {
        mask[0] = aa;
        mask[1] = bb;
        nb = BB , nr = RR;
        moveAfter11 = 0;
        child.clear() , lastMoveSeq.clear();
        Board = x;
    }
    void gotIt()
    {
        int i;
        rep(i,40) out<< i << Board->idInB[i] ;
    }
    void Delete()
    {
        delete temp; delete bestJump ; delete bestJump1 ; delete bestJump2;
    }
    void flushPointer()
    {
        mask[0] = mask[1] = nb = nr = moveAfter11 = 0;
        child.clear() , lastMoveSeq.clear();
    }
    /// parameter    : Board ID
    /// return value : Board ID
    int midBoardPos( int posA , int posB )
    {
        int ax,ay,bx,by,mx,my,mid;
        _setXY( ax , ay , posA );
        _setXY( bx , by , posB );
        mx = ( ax + bx ) >> 1 , my = ( ay + by ) >> 1;
        mid = _getTot( mx , my );
        return mid;
    }
    inline bool _redInMid( int posA , int posB , lli masK)
    {
        return _isOn( masK , Board->idInM[ midBoardPos( posA , posB ) ] );
    }
    inline bool _blackInMid( int posA , int posB , lli masK )
    {
        return _isOn( masK , Board->idInM[ midBoardPos( posA , posB ) ] );
    }
    /// set everything necessary , red+black [ Only Adjacent Move ]
    void setChildState( lli maskB , lli maskR , int posU , int posV , int b , int r )
    {
        if( SZ( lastMoveSeq ) == 0 )    lastMoveSeq.pb( posU ) , lastMoveSeq.pb( posV );
        else    lastMoveSeq.pb( posV );
        int flag = 0;
        int u = Board->idInM[ posU ] ;
        int v = Board->idInM[ posV ] ;
        if( _isOn( maskB , u ) )
        {
            mask[0] = maskB ;
            mask[0] = _flipAt( mask[0] , u );
            mask[0] = _flipAt( mask[0] , v );
            mask[1] = maskR; /// ERROR FIX : Implementation
            flag++;
        }
        if( _isOn( maskR , u ) )
        {
            mask[1] = maskR ;
            mask[1] = _flipAt( mask[1] , u );
            mask[1] = _flipAt( mask[1] , v );
            mask[0] = maskB; /// ERROR FIX : Implementation
            flag++;
        }
        if( flag == 2 ) { printf("Error in setChildState\n");}
        nb = b , nr = r;
    }
    /// set everything necessary , red+black [ Only Elimination Move ]
    void setElim( lli maskB , lli maskR , int posU , int posV , int b , int r )
    {
        if( SZ( lastMoveSeq ) == 0 ) lastMoveSeq.pb( posU ) , lastMoveSeq.pb( posV );
        else lastMoveSeq.pb( posV );
        int flag = 0;
        nb = b , nr = r;
        int m = Board->idInM[ midBoardPos( posU , posV ) ];
        int u = Board->idInM[ posU ] ;
        int v = Board->idInM[ posV ] ;
        if( _isOn( maskB , u ) ) /// Elim Red
        {
            mask[0] = maskB ;
            mask[0] = _flipAt( mask[0] , u );
            mask[0] = _flipAt( mask[0] , v );
            mask[1] = _flipAt( maskR , m );
            nr--;
            flag++;
        }
        if( _isOn( maskR , u ) ) /// Elim black
        {
            mask[1] = maskB ;
            mask[1] = _flipAt( mask[1] , u );
            mask[1] = _flipAt( mask[1] , v );
            mask[0] = _flipAt( maskB , m );
            nb--;
            flag++;
        }
        if( flag == 2 ) { printf("Error in setElim\n");}
    }

    bool isTerminal()
    {
        if( !nb || !nr ) return true;
        if( nb == 1 && nr == 1 ) moveAfter11++;
        return false;
    }
    data Heuristic()
    {
        if( moveAfter11 == 40 ) return DRAW;
        if( !nr ) return WIN;
        if( !nb ) return LOSE;
        return 1;
        //return rand() * ( nb - nr ) ; //:P
    }
    /// To Fix: take care about the opposite mask, remains unchanged
    void moveSearchBlack()
    {
        int i,I,posU,vPos,sz;
        //int j,J,uX,uY,vX,vY;
        temp = new GameState(0,0,Board);
        bestJump = new GameState(0,0,Board);
        bestJump1 = new GameState(0,0,Board);
        bestJump2 = new GameState(0,0,Board);
        rep(I,38)
        {
            if( _isOn( mask[0] , I ) )
            {
                posU = Board->idInB[ I ] ;
                //_setXY( uX , uY , posU ) ;
                sz = SZ( Board->adjMove[ posU ] );
                rep(i,sz) /// Traversing Neighbour
                {
                    vPos = Board->adjMove[ posU ][ i ];
                    //_setXY( vX , vY , vPos );
                    if( _isEmpty( vPos ) )
                    {
                        temp->flushPointer();
                        temp->setChildState( mask[0] , mask[1] , posU , vPos , nb , nr );
                        child.pb( *temp );
                    }
                }
                //* Search Jumps
                /// Jump 1
                sz = SZ( Board->adjJump[ posU ] );
                bestJump->flushPointer();
                data Hval = -inf;
                rep(i,sz) /// Traversing Eliminations 1
                {
                    vPos = Board->adjJump[ posU ][ i ];
                    //_setXY( vX , vY , vPos );
                    if( _isEmpty( vPos ) && _redInMid( posU , vPos , mask[1] ) )
                    {
                        temp->flushPointer();
                        temp->setElim( mask[0] , mask[1] , posU , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val > Hval ) Hval = val , bestJump = temp;
                    }
                }
                if( ! bestJump->mask[0] )
                {
                    continue;
                }
                /// Jump 2
                lli curMaskB = bestJump->mask[0];
                lli curMaskR = bestJump->mask[1];
                int posUU = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ posUU ] );
                bestJump1->flushPointer();
                Hval = -inf;
                rep(i,sz) /// Traversing Eliminations 2
                {
                    vPos = Board->adjJump[ posUU ][ i ];
                    //_setXY( vX , vY , vPos );
                    if( vPos == posU ) continue;
                    if( _isEmptyCur( vPos ) && _redInMid( posUU , vPos , curMaskR ) )
                    {
                        temp->flushPointer();
                        temp->lastMoveSeq.push_back(posU); /// New Edit
                        temp->lastMoveSeq.push_back(posUU);
                        //temp->setElim( mask[0] , mask[1] , posUU , vPos , nb , nr );
                        temp->setElim( curMaskB , curMaskR , posUU , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val > Hval ) Hval = val , bestJump1 = temp;
                    }
                }
                //if( posU == 33 ) out<<bestJump1->lastMoveSeq.back() << bestJump1->mask[0];
                // THIS WAS A VERY TRICKY RUN TIME ERROR
                if( ! bestJump1->mask[0] )
                {
                    child.pb( *bestJump );
                    continue;
                }
                bestJump = bestJump1;
                /// Jump 3

                curMaskB = bestJump->mask[0];
                curMaskR = bestJump->mask[1];

                int posUUU = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ posUUU ] );
                bestJump2->flushPointer();
                Hval = -inf;
                rep(i,sz) /// Traversing Eliminations 3
                {
                    vPos = Board->adjJump[ posUUU ][ i ];
                    //_setXY( vX , vY , vPos );
                    if( vPos == posUU ) continue;
                    if( _isEmptyCur( vPos ) && _redInMid( posUUU , vPos ,curMaskR ) )
                    {
                        temp->flushPointer();
                        temp->lastMoveSeq.push_back(posU); /// new edit
                        temp->lastMoveSeq.push_back(posUU);
                        temp->lastMoveSeq.push_back(posUUU);
                        //temp->setElim( mask[0] , mask[1] , posUUU , vPos , nb , nr );
                        temp->setElim( curMaskB , curMaskR , posUUU , vPos , nb , nr );

                        data val = temp->Heuristic();
                        if( val > Hval ) Hval = val , bestJump2 = temp;
                    }
                }
                if( !bestJump2->mask[0] )
                {
                    child.pb( *bestJump ) ;
                    continue ;
                }
                bestJump = bestJump2;
                child.pb( *bestJump );
                //*/
            }
        }
        Delete();
    }

    void moveSearchRed()
    {
    }

    void setAdj( bool blackMove )
    {

        if( blackMove )
        {
            moveSearchBlack();
        }
        else
        {
            moveSearchRed();
        }
    }
    data abSearch( GameState Node , data alpha , data beta , int deep , bool maxBlack )
    {
        int i;
        if( !deep || Node.isTerminal() )
        {
            return Node.Heuristic();
        }
        Node.setAdj( maxBlack ); int sz = SZ( Node.child );
        rep(i,sz)
        {
            if( maxBlack )
            {
                alpha = max ( alpha , abSearch( Node.child[i] , alpha , beta , deep - 1 , !maxBlack ) ) ;
                if( alpha >= beta ) return alpha; /// Beta Cut
            }
            else
            {
                beta = min ( beta , abSearch( Node.child[i] , alpha , beta , deep - 1, !maxBlack ) ) ;
                if( alpha >= beta ) return beta; /// Alpha Cut
            }
        }
        return maxBlack ? alpha : beta ;
    }
};

#endif // GAMESTATE_H
