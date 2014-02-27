#ifndef MACRO_H
#define MACRO_H
#include <QDebug>

typedef long long lli;
#define pii pair < int , int >
#define pb push_back
#define rep(i,a) for( i = 0 ; i < (a) ; i++ )
#define SZ( x ) ((int) x.size())

#define _flipAt( x , i ) ( (x) ^ (1LL<<i))
#define _isOn( x , i ) ( (x) & ( 1LL<<i ) )
#define out qDebug()

#endif // MACRO_H
