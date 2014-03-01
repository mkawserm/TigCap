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
    lli mask[2]; /// black , red
    int nb,nr,moveAfter11;

    PieceBoardInfo *Board;
public:
    GameState *temp , *bestJump , *bestJump1 , *bestJump2;
    vector < GameState > child;
    vector < int > lastMoveSeq; /// board id
    GameState(data aa ,data bb , PieceBoardInfo *x)
    {
        mask[0] = aa;
        mask[1] = bb;
        nb = nr = 16;
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
    int midBoardPos( int posA , int posB )
    {
        int ax,ay,bx,by,mx,my,mid,pos;
        _setXY( ax , ay , posA );
        _setXY( bx , by , posB );
        mx = ( ax + bx ) >> 1 , my = ( ay + by ) >> 1;
        mid = _getTot( mx , my );
        pos = Board->idInM[ mid ];
        return pos;
    }
    inline bool _redInMid( int posA , int posB )
    {
        return _isOn( mask[1] , midBoardPos( posA , posB ) );
    }
    inline bool _blackInMid( int posA , int posB )
    {
        return _isOn( mask[0] , midBoardPos( posA , posB ) );
    }
    void setChildState( lli maskB , lli maskR , int u , int v , int b , int r ) /// set everything necessary , red+black
    {
        if( SZ( lastMoveSeq ) == 0 )    lastMoveSeq.pb( u ) , lastMoveSeq.pb( v );
        else    lastMoveSeq.pb( v );
        int flag = 0;
        if( _isOn( maskB , u ) )
        {
            mask[0] = maskB ;
            u = Board->idInM[ u ] ;

            mask[0] = _flipAt( mask[0] , u );
            v = Board->idInM[ v ] ;
            mask[0] = _flipAt( mask[0] , v );
            flag++;
        }
        if( _isOn( maskR , u ) )
        {
            mask[1] = maskR ;
            u = Board->idInM[ u ] ;
            mask[1] = _flipAt( mask[1] , u );
            v = Board->idInM[ v ] ;
            mask[1] = _flipAt( mask[1] , v );
            flag++;
        }
        if( flag == 2 ) { printf("Error in setChildState\n");}
        nb = b , nr = r;
    }
    void setElim( lli maskB , lli maskR , int u , int v , int b , int r )       /// set everything necessary , red+black
    {
        if( SZ( lastMoveSeq ) == 0 ) lastMoveSeq.pb( u ) , lastMoveSeq.pb( v );
        else lastMoveSeq.pb( v );
        int flag = 0;
        nb = b , nr = r;
        if( _isOn( maskB , u ) ) /// Elim red
        {
            int m = Board->idInM[ midBoardPos( u , v ) ];
            mask[0] = maskB ;
            u = Board->idInM[ u ] ;
            mask[0] = _flipAt( mask[0] , u );
            v = Board->idInM[ v ] ;
            mask[0] = _flipAt( mask[0] , v );
            mask[1] = _flipAt( mask[1] , m );
            nr--;
            flag++;
        }
        if( _isOn( maskR , u ) ) /// Elim black
        {
            int m = Board->idInM[ midBoardPos( u , v ) ];
            mask[1] = maskB ;
            u = Board->idInM[ u ] ;
            mask[1] = _flipAt( mask[1] , u );
            v = Board->idInM[ v ] ;
            mask[1] = _flipAt( mask[1] , v );
            mask[0] = _flipAt( mask[0] , m );
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

    void moveSearchBlack()
    {
        int i,j,I,J,uPos,uX,uY,vPos,vX,vY, sz;
        temp = new GameState(0,0,Board);
        bestJump = new GameState(0,0,Board);
        bestJump1 = new GameState(0,0,Board);
        bestJump2 = new GameState(0,0,Board);
        rep(I,38)
        {
            //GameState *temp = new GameState(0,0,Board) , *bestJump =new GameState(0,0,Board) ,
              //      *bestJump1 = new GameState(0,0,Board) , *bestJump2 = new GameState(0,0,Board);
            if( _isOn( mask[0] , I ) )
            {
                uPos = Board->idInB[ I ] ;
                _setXY( uX , uY , uPos ) ;
                sz = SZ( Board->adjMove[ uPos ] );
                rep(i,sz) /// Traversing Neighbour
                {
                    vPos = Board->adjMove[ uPos ][ i ];
                    _setXY( vX , vY , vPos );
                    if( _isEmpty( vPos ) )
                    {
                        //if( uPos == 29 ) out << "here" << vPos;
                        temp->flushPointer();
                        temp->setChildState( mask[0] , mask[1] , uPos , vPos , nb , nr );
                        child.pb( *temp );
                    }
                }
                //*
                sz = SZ( Board->adjJump[ uPos ] );
                bestJump->flushPointer();
                data Hval = -inf;
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uPos ][ i ];
                    _setXY( vX , vY , vPos );
                    if( _isEmpty( vPos ) && _redInMid( uPos , vPos ) )
                    {
                        temp->flushPointer();
                        temp->setElim( mask[0] , mask[1] , uPos , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val > Hval ) Hval = val , bestJump = temp;
                    }
                }
                if( ! bestJump->mask[0] )
                {
                    continue;
                }
                int uuPos = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ uuPos ] );
                bestJump1->flushPointer();
                Hval = -inf;
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uuPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) && _redInMid( uuPos , vPos ) )
                    {
                        temp->flushPointer();
                        temp->setElim( mask[0] , mask[1] , uuPos , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val > Hval ) Hval = val , bestJump1 = temp;
                    }
                }
                if( ! bestJump1->mask[0] )
                {
                    child.pb( *bestJump );
                    continue;
                }
                bestJump = bestJump1;

                int uuuPos = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ uuuPos ] );
                bestJump2->flushPointer();
                Hval = -inf;
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uuuPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) && _redInMid( uuuPos , vPos ) )
                    {
                        temp->flushPointer();
                        temp->setElim( mask[0] , mask[1] , uuuPos , vPos , nb , nr );
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
        GameState *temp , *bestJump , *bestJump1 , *bestJump2;
        int i,j,I,J,uPos,uX,uY,vPos,vX,vY, sz;
        rep(I,38)
        {
            if( _isOn( mask[1] , I ) ) // differ
            {
                uPos = Board->idInB[ I ] ;
                _setXY( uX , uY , uPos ) ;
                sz = SZ( Board->adjMove[ uPos ] );
                rep(i,sz) /// Traversing Neighbour
                {
                    vPos = Board->adjMove[ uPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) )
                    {
                        temp->setChildState( mask[0] , mask[1] , uPos , vPos , nb , nr );
                        child.pb( *temp );
                    }
                }

                sz = SZ( Board->adjJump[ uPos ] );
                bestJump;
                data Hval = inf; // differ
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) && _blackInMid( uPos , vPos ) ) // differ
                    {
                        temp->setElim( mask[0] , mask[1] , uPos , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val  < Hval ) Hval = val , bestJump = temp;
                    }
                }
                if( ! bestJump->mask[0] ) return;
                int uuPos = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ uuPos ] );
                bestJump1;
                Hval = inf; //differ
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uuPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) && _blackInMid( uuPos , vPos ) ) // differ
                    {
                        temp->setElim( mask[0] , mask[1] , uuPos , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val < Hval ) Hval = val , bestJump1 = temp;
                    }
                }
                if( ! bestJump1->mask[0] )
                {
                    child.pb( *bestJump );
                    return;
                }
                bestJump = bestJump1;

                int uuuPos = bestJump->lastMoveSeq.back(); /// It's for sure, there is a last move
                sz = SZ( Board->adjJump[ uuuPos ] );
                bestJump2;
                Hval = inf; // differ
                rep(i,sz) /// Traversing Eliminations
                {
                    vPos = Board->adjJump[ uuuPos ][ i ];
                    _setXY( vX , vY , vPos );

                    if( _isEmpty( vPos ) && _blackInMid( uuuPos , vPos ) ) //differ
                    {
                        temp->setElim( mask[0] , mask[1] , uuuPos , vPos , nb , nr );
                        data val = temp->Heuristic();
                        if( val < Hval ) Hval = val , bestJump2 = temp;
                    }
                }

                if( !bestJump2->mask[0] )
                {
                    child.pb( *bestJump ) ;
                    return;
                }
                bestJump = bestJump2;
                child.pb( *bestJump );
                return;
            }
        }
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
