#include "pieceboard.h"

using namespace std;

void PieceBoard:: debugPrintGraph()
{
    int i,j;
    out << "nVertexMeshal in Board: " << nVertexBrd;
    /*/
    out << "Checking Vertex Id In Board";
    rep(i,nVertexBrd)
    {
        out<< i << idInB[ i ] ;
    }
    out << "Check is Ok :) ";
    out << "Checking Mask Id";
    rep(i,nVertexMesh)
    {
        if( idInM[ i ] != -1 )
            out<< idInM[i] << i ;
    }
    out << "Check is ok :) ";
    out << "Checking xy position in Id System";
    rep(i,nVertexMesh)
    {
        if( idInM[i] != -1 )
            out << idInM[i] << idInB[idInM[i]] << "->"<< xxInMesh[i] << yyInMesh[i] <<
                   xxAtM[ idInM[i] ] << yyAtM[ idInM[i] ];
    }
    out << " finish - all ok" ;
    //*/
    //*/ Set the Jumps manually and Check All
    out << "Checking Graphs :";
    rep(i,nVertexMesh)
    {
        if( idInM[i] == -1 ) continue;
        out << i << SZ( adjMove[i] ) << SZ( adjJump[i] );
        rep(j,SZ( adjMove[i] ))
                out << "ad: " << adjMove[i][j];

        rep(j,SZ( adjJump[i] ))
                out << "Jump:" << adjJump[i][j];
        out<< "End Node";
    }
    //*/
}



void PieceBoard:: make4( int u , int v )
{
    adjMove[u].push_back(v);
    adjMove[v].push_back(u);
    if(deb)this->ConnectTwoPoints(P[u],P[v]);

    int a = xxInMesh[u], b = yyInMesh[u] , c = xxInMesh[v] , d = yyInMesh[v];
    u = idInMeshXY[a][5-b+5]; // Rotation Geoms :D
    v = idInMeshXY[c][5-d+5];
    adjMove[u].push_back(v);
    adjMove[v].push_back(u);
    if(deb)this->ConnectTwoPoints(P[u],P[v]);
}



void PieceBoard:: make5( int u , int v )
{
    adjMove[u].push_back(v);
    adjMove[v].push_back(u);
    if(deb)this->ConnectTwoPoints(P[u],P[v]);
    return ;
}



bool PieceBoard:: drawGraph()
{
    int i,j;
    bool vis[MX][MX];
    rep(i,colLine+1) rep(j,rowLine+1) vis[i][j] = false;
    //qDebug()<<nVertexMesh<<endl;
    rep(i,nVertexMesh)
    {
        rep(j,SZ(adjMove[i]))
        {
            if( ! vis[ i ][ adjMove[i][j] ] )
            {
                if( ! vis[ i ][ adjMove[i][j] ] && ! vis[ adjMove[i][j] ][ i ] )
                    this->ConnectTwoPoints( P[ i ] , P[ adjMove[i][j] ]); // single Line Draw
                vis[ i ][ adjMove[i][j] ] = true;

            }
            else return false;//qDebug() << "Duplicate";
        }
    }
    return true;
}

PieceBoard::PieceBoard(){
    /*Defining PieceBoard Size*/
    //setAcceptDrops(true);
    int i,j,k;
    scaleF  = 52;
    deb     = false;
    rowLine = 11;
    colLine = 7;
    nVertexMesh     = 0;
    rep(i,100) adjMove[i].clear();

    //Numbering + Creating Vertices Start
        // See Drawing For Details
    nVertexMesh = 0;
    rep(j,rowLine) rep(i,colLine)
    {
        idInMeshXY[ i ][ j ] = nVertexMesh;
        xxInMesh[ nVertexMesh ] = i;
        yyInMesh[ nVertexMesh ] = j;
        P[nVertexMesh] = new Point(i*scaleF , j*scaleF , this); // Scaled(x,y) in graphics convention
        // printf("%d %d %d = %d %d %d\n",i,j,nVertexMesh,nVertexMesh%7,nVertexMesh/7,j*7+i);
        nVertexMesh++;
    }
    //Numbering + Creating Vertices End

    //Set Board Size Start
    h = ( rowLine - 1 ) * scaleF ;
    w = ( colLine - 1 ) * scaleF;

    piece_board_size.setX(0);
    piece_board_size.setY(0);
    piece_board_size.setWidth(w);
    piece_board_size.setHeight(h);
    //Set Board Size End

    //Board Boundary Start
    Point *p1 = P[idInMeshXY[0][0]];
    Point *p2 = P[idInMeshXY[0][10]];
    Point *p3 = P[idInMeshXY[6][10]];
    Point *p4 = P[idInMeshXY[6][0]];

    //qDebug() << "ID: " << P[ idInMeshXY[6][10] ]->x() << P[ idInMeshXY[6][10] ]->y() ;

    p1->setBoardVertex();
    p2->setRED();




    this->ConnectTwoPoints(p1,p2);
    this->ConnectTwoPoints(p2,p3);
    this->ConnectTwoPoints(p3,p4);
    this->ConnectTwoPoints(p4,p1);
    //Board Boundary End

    //Build Triangle Start
    make4(8,10);  make4(10,12);
    make4(16,17); make4(17,18);
    make4(24,16); make4(24,17); make4(24,18);
    make4(16,8);  make4(17,10); make4(18,12);
    //Build Triangle End

    //Build Middle Board Start
        // Horizontals, leaving the unchanged middle line
    rep(i,2)rep(j,4) make4(22+j+7*i,22+j+1+7*i);
    rep(j,4) make5(36+j,36+j+1);
        // Verticals
    rep(i,5)
    {
        make4(22+i,22+i+7);
        make4(22+i+7,22+i+14);
    }
        //Diagonals
    make4(36,30); make4(30,24);
    make4(38,32); make4(32,26);

    make4(22,30); make4(30,38);
    make4(24,32); make4(32,40);
    //Build Middle Board End

    /// Setting Board and Mask ID Start
    nVertexBrd = 0;
    rep(i,nVertexMesh)
    {
        if( SZ( adjMove[i] ) )
        {
            idInB[ nVertexBrd ] = i;
            idInM[ i ] = nVertexBrd;
            xxAtM[ nVertexBrd ] = xxInMesh[ i ];
            yyAtM[ nVertexBrd ] = yyInMesh[ i ];
            nVertexBrd++;
        } else idInM[ i ] = -1 ;
    }
    /// Setting Board and Mask ID END

    /// Calculating Jump Start
    rep(i,nVertexBrd)
    {
        int ii = idInB[ i ];
        adjJump[ ii ].clear();
        rep(j,SZ( adjMove[ ii ] ))
        {
            int xm = xxInMesh[ adjMove[ii][j] ];
            int ym = yyInMesh[ adjMove[ii][j] ];
            rep(k,nVertexBrd)
            {
                if( i == k ) continue;
                int kk = idInB[ k ];
                if( xxAtM[i] + xxAtM[k] == 2*xm )
                    if( yyAtM[i] + yyAtM[k] == 2*ym )
                        adjJump[ ii ].pb( kk );
            }
        }
    }
    /// Calculating Jump End

    qDebug() << drawGraph();
    debugPrintGraph();




}



void PieceBoard::ConnectTwoPoints(Point *p1,Point *p2){
    new Edge(p1,p2,this);
    //p1->enableDrops();
    //p2->enableDrops();
    //p1->setBLACK();
    //p1->enableGrab();
    //p2->enableGrab();

}



QRectF PieceBoard::boundingRect() const
{
    return this->piece_board_size;
}


QPainterPath PieceBoard::shape() const
{
    QPainterPath path;
    path.addRect(this->piece_board_size);
    return path;
}




void PieceBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_UNUSED(painter);
}

