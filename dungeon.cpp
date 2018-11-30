
#define USE_PATHER

#include <ctype.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>

#include <vector>
#include <iostream>

#include "micropather.h"
#include "singlemap.h"
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace micropather;
using namespace cv;
using namespace std;


class Dungeon
#ifdef USE_PATHER
  : public Graph
#endif
{
  private:
	Dungeon( const Dungeon& );
	void operator=( const Dungeon& );
  
	int playerX, playerY;
	MPVector<void*> path;
	bool doorsOpen;
	bool showConsidered;

	MicroPather* pather;

  public:
	Dungeon() : playerX( 0 ), playerY( 0 ), doorsOpen( false ), showConsidered( false ), pather( 0 )
	{
		pather = new MicroPather( this, 20 );	// Use a very small memory block to stress the pather
	}

    void SetStart(int x, int y) {
        playerX = x;
        playerY = y;
        cout << "Start value: (" << x << "," << y << ") " << (int)SingleMapMat::get().GetSingleMap().at<uchar>(y, x) << endl;
    }

	virtual ~Dungeon() {
		delete pather;
	}

	int X()	{ return playerX; }
	int Y() { return playerY; }

	void ClearPath()
	{
		#ifdef USE_PATHER
		path.resize( 0 );
		#endif
	}

	int Passable( int nx, int ny ) 
	{
        Mat world_map = SingleMapMat::get().GetSingleMap();
		if (    nx >= 0 && nx < world_map.cols
			 && ny >= 0 && ny < world_map.rows )
		{
			if (world_map.at<uchar>(ny, nx) == 0xFF)
				return 1;
		}		
		return 0;
	}

	int SetPos( int nx, int ny ) 
	{
        cout << "Goal value: " << (int)SingleMapMat::get().GetSingleMap().at<uchar>(ny, nx) << endl;

        int result = 0;
		if ( Passable( nx, ny ) == 1 )
		{
			#ifdef USE_PATHER
				float totalCost;
				if ( showConsidered )
					pather->Reset();
					
				result = pather->Solve( XYToNode( playerX, playerY ), XYToNode( nx, ny ), &path, &totalCost );

				if ( result == MicroPather::SOLVED ) {
					playerX = nx;
					playerY = ny;
				}
				printf( "Pather returned %d\n", result );

			#else
				playerX = nx;
				playerY = ny;
			#endif
		}
		return result;
	}

#ifdef USE_PATHER

	void NodeToXY( void* node, int* x, int* y ) 
	{
		intptr_t index = (intptr_t)node;
		*y = index / SingleMapMat::get().GetSingleMap().cols;
		*x = index - *y * (SingleMapMat::get().GetSingleMap().rows);
	}

	void* XYToNode( int x, int y )
	{
		return (void*) ( y*(SingleMapMat::get().GetSingleMap().cols) + x );
	}
		
	
	virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd ) 
	{
		int xStart, yStart, xEnd, yEnd;
		NodeToXY( nodeStart, &xStart, &yStart );
		NodeToXY( nodeEnd, &xEnd, &yEnd );

		/* Compute the minimum path cost using distance measurement. It is possible
		   to compute the exact minimum path using the fact that you can move only 
		   on a straight line or on a diagonal, and this will yield a better result.
		*/
		int dx = xStart - xEnd;
		int dy = yStart - yEnd;
		return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
	}

	virtual void AdjacentCost( void* node, micropather::MPVector< StateCost > *neighbors ) 
	{
		int x, y;
		const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

		NodeToXY( node, &x, &y );

		for( int i=0; i<8; ++i ) {
			int nx = x + dx[i];
			int ny = y + dy[i];

			int pass = Passable( nx, ny );
			if ( pass > 0 ) {
				if ( pass == 1 || doorsOpen ) 
				{
					// Normal floor
					StateCost nodeCost = { XYToNode( nx, ny ), cost[i] };
					neighbors->push_back( nodeCost );
				}
				else 
				{
					// Normal floor
					StateCost nodeCost = { XYToNode( nx, ny ), FLT_MAX };
					neighbors->push_back( nodeCost );
				}
			}
		}
	}

	virtual void PrintStateInfo( void* node ) 
	{
		int x, y;
		NodeToXY( node, &x, &y );
		printf( "(%d,%d)", x, y );
	}

#endif

    void ShowPath() {
        Mat world_map = SingleMapMat::get().GetSingleMap();
        cvtColor(world_map, world_map, CV_GRAY2BGR);
        unsigned size = path.size();
        for( int k=0; k<size; ++k ) {
            int x, y;
            NodeToXY(path[k], &x, &y);
//            world_map.at<uchar>(y, x) = 128;
            circle(world_map, Point(x,y), 1, Scalar(0, 255, 0), -1);
        }
        imshow("path", world_map);
        waitKey();
    }
};

int main()
{
    Dungeon dungeon;
    dungeon.SetStart(330, 630);
    clock_t start, finish;
    double totaltime;
    start = clock();
    dungeon.SetPos(50, 50); //end
    finish    = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "use " << totaltime << " seconds to find a path." << endl;

    dungeon.ShowPath();
	return 0;
}
