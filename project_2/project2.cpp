#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT        4
#endif

// setting the number of nodes:
#ifndef NUMNODES
#define NUMNODES    5000
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTIMES
#define NUMTIMES    10
#endif

#define XMIN   -1.
#define XMAX    1.
#define YMIN   -1.
#define YMAX    1.

const float N = 2.5f;
const float R = 1.2f;

float Height( int, int ); // function prototype

int
main( int argc, char *argv[ ] )
{
	omp_set_num_threads( NUMT );  // set the number of threads to use

	// get ready to record the maximum performance and the volume:
	double megaHeightsComputed = 0.;
	float volume;
	double maxPerformance = 0.;

	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)

	float fullTileArea = ( ( ( XMAX - XMIN )/(float)(NUMNODES-1) ) *
                           ( ( YMAX - YMIN )/(float)(NUMNODES-1) ) );

	// looking for the maximum performance: 
	for( int times = 0; times < NUMTIMES; times++ )
	{
		volume = 0;
        
		double time0 = omp_get_wtime();
			
		#pragma omp parallel for default(none) shared(fullTileArea) reduction(+:volume)
        for( int i = 0; i < NUMNODES * NUMNODES; i++ )
        { 
            int iu = i % NUMNODES;
            int iv = i / NUMNODES;
            float z = Height( iu, iv );
            
            // determine the pin location
            double ratio = 1.;
            if( iu == 0 || iu == NUMNODES-1 || iv == 0 || iv == NUMNODES-1 )
                ratio = 0.5;
            if( (iu ==0 && iv ==0) || (iu == NUMNODES-1 && iv == NUMNODES-1)|| (iu == NUMNODES-1 && iv == 0) || (iu == 0 && iv == NUMNODES-1) )
                ratio = 0.25;
            
            // compute the volume
            volume += 2. * z * ratio * fullTileArea;
        } // for ( parallel computing # of nodes )
		
        double time1 = omp_get_wtime();
        double megaHeightsComputed = NUMNODES * NUMNODES / ( time1 - time0 ) / 1000000.;
        if( megaHeightsComputed > maxPerformance )
            maxPerformance = megaHeightsComputed;

    } // for ( # of timing tries )

	printf("%2d, %8d, %6.2lf, %6.2lf\n", NUMT, NUMNODES, volume, maxPerformance);
	
}

float
Height( int iu, int iv )      // iu, iv = 0 .. NUMNODES-1
{
    float x = -1. + 2.*(float)iu / (float)(NUMNODES-1);
    float y = -1. + 2.*(float)iv / (float)(NUMNODES-1);

    float xn = pow( fabs(x), (double)N );
    float yn = pow( fabs(y), (double)N );
    float rn = pow( fabs(R), (double)N );
    float r = rn - xn - yn;
    if( r <= 0. )
        return 0.;
    float height = pow( r, 1./(double)N );
    return height;
}
