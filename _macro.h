#ifndef MACRO_H
#define MACRO_H
#include <QDebug>

typedef long long lli;
typedef double data;
const int MX = 200;

const lli inf = (1LL<<49);
const data DRAW = 0;
const data WIN  = inf;
const data LOSE = -inf;

#define pii pair < int , int >
#define pb push_back
#define rep(i,a) for( i = 0 ; i < (a) ; i++ )
#define SZ( x ) ((int) x.size())

#define _flipAt( x , i ) ( (x) ^ (1LL<<i))
#define _isOn( x , i ) ( (x) & ( 1LL<<i ) )
#define _isEmpty( a ) ( !_isOn( mask[0] , Board->idInM[a] ) && !_isOn( mask[1] , Board->idInM[a] ) )
/// XY in graphics convention
#define _setXY( x , y , tot ) ( x = (tot) % 7 , y = (tot) / 7 )
#define _getTot( x , y ) ( y * 7 + x )

#define out qDebug()

#endif // MACRO_H
